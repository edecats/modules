/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#include <mutex>
#include <initHooks.h>
#include <epicsThread.h>
#include <epicsTime.h>
#include "err.h"
#include "type.h"
#include "ADSAddress.h"
#include "ADSPortDriver.h"
#include "ads_port_driver_version.h"

#include <epicsExit.h>

void exit_func(void *args);
static void ads_comm_task(void *);
static void epicsHookState(initHookState state);

typedef std::pair<ADSPortDriverPtr, ADSPVInfoPtr> ADSPortPair;

static const long default_ads_function_timeout = 5;

// Masks grouped according to different interrupt callback types
static const uint16_t interruptStandardMask = asynInt32Mask | asynFloat64Mask | asynOctetMask;
static const uint16_t interruptDigitalMask = asynUInt32DigitalMask;
static const uint16_t interruptArrayMask = asynInt8ArrayMask | asynInt16ArrayMask |
                         asynInt32ArrayMask | asynFloat32ArrayMask | asynFloat64ArrayMask;

// Map between notifications handles and pair of ADSPortDriver instances and ADSPVInfo instances
static std::map<uint32_t, ADSPortPair> notify_adsPort_map;

ADSPortDriver::ADSPortDriver(const char *port_name,
                             const char *ip_addr,
                             const char *ams_net_id,
                             epicsUInt16 sum_buffer_nelem,
                             epicsUInt32 ads_function_timeout_ms,
                             int auto_connect,
                             int priority) :
                                 DynamicAsynPortDriver(
                                     port_name,
                                     1, // maxAddr
                                     asynCommonMask
                                     | asynDrvUserMask
                                     | asynOptionMask
                                     | asynInt32Mask
                                     | asynUInt32DigitalMask
                                     | asynFloat64Mask
                                     | asynOctetMask
                                     | asynInt8ArrayMask
                                     | asynInt16ArrayMask
                                     | asynInt32ArrayMask
                                     | asynFloat32ArrayMask
                                     | asynFloat64ArrayMask
                                     | asynGenericPointerMask, // Interface mask
                                     asynInt32Mask
                                     | asynUInt32DigitalMask
                                     | asynInt32Mask
                                     | asynFloat64Mask
                                     | asynOctetMask
                                     | asynInt8ArrayMask
                                     | asynInt16ArrayMask
                                     | asynInt32ArrayMask
                                     | asynFloat32ArrayMask
                                     | asynFloat64ArrayMask
                                     | asynGenericPointerMask, // Interrupt mask
                                     ASYN_CANBLOCK | ASYN_MULTIDEVICE, // asynFlags
                                     auto_connect, // autoConnect
                                     priority, // priority
                                     0 , // Default stack size
                                     pvkeeper = new PVKeeper<ADSPVInfo>( [](const ADSPVInfoConstPtr& i)-> std::string { return std::to_string(i->global_index); } )),
                                    port_name(port_name),
                                    ip(ip_addr),
                                    ams_remote(ams_net_id),
                                    sum_buffer_nelem(sum_buffer_nelem),
                                    ads_function_timeout_ms(ads_function_timeout_ms),
                                    ads_conn(new Connection()),
                                    sum_read_req(sum_buffer_nelem, ads_conn)


{
    if (this->sum_buffer_nelem == 0) {
        this->sum_buffer_nelem = ADSPortDriver::DEFAULT_SUM_BUFFER_NELEM;
    }

    if (this->ads_function_timeout_ms == 0) {
        this->ads_function_timeout_ms = ADSPortDriver::DEFAULT_ADS_CALL_TIMEOUT_MS;
    }

    adsInterface = new HWInterface<ADSPortDriver>;
    registerADSHandlers();

    int status = initHookRegister(epicsHookState);
    if (status) {
        errlogPrintf("Error: failed to register initHookFunction,\n"
                     " call ADSFindIOIntrRecord manually for notifications to work\n");
    }

    epicsAtExit(exit_func, this);

    this->ads_comm_tid = epicsThreadCreate(
            (this->port_name + "_ads_comm").c_str(),
            epicsThreadPriorityMedium,
            epicsThreadGetStackSize(epicsThreadStackMedium),
            (EPICSTHREADFUNC)::ads_comm_task,
            this);
}

ADSPortDriver::~ADSPortDriver() {
    LOG_WARN_ASYN(this->pasynUserSelf, "shutting down");

    /* Indicate that the port driver instance is shutting down */
    this->port_driver_shutting_down = true;

    /* Signal communication thread that it should exit */
    this->exit_ads_comm_thread.trigger();

    LOG_TRACE_ASYN(this->pasynUserSelf, "waiting for ADS communication thread to exit");
    if (this->exit_ads_comm_thread_done.wait(5) == true) {
        LOG_TRACE_ASYN(this->pasynUserSelf, "ADS communication thread signaled exit");
    } else {
        LOG_WARN_ASYN(this->pasynUserSelf, "ADS communication thread exit timed out");
    }

    delete adsInterface;

    LOG_WARN_ASYN(this->pasynUserSelf, "shutdown complete");
}

int ADSPortDriver::initialize() {
    LOG_TRACE_ASYN(this->pasynUserSelf, "entering");

    /* Initialize can be called multiple times */
    if (this->ioc_initialized == true) {
        LOG_TRACE_ASYN(this->pasynUserSelf, "%s already initialized", this->portName.c_str());
        return 0;
    }

    std::lock_guard<epicsMutex> lock(this->ads_conn_mtx);

    /* Allocate ADS sum-read buffers */
    int rc = this->sum_read_req.allocate(this->ads_read_vars);
    switch (rc) {
        case 0: {
            LOG_TRACE_ASYN(this->pasynUserSelf, "successfully allocated sum-read request buffers for %lu variables", this->ads_read_vars.size());
            break;
        }
        default: {
            LOG_ERR_ASYN(this->pasynUserSelf, "error allocating sum-read request buffers (%i): %s", rc, ads_errors[rc].c_str());
            return 1;
        }
    }

    this->ioc_initialized = true;

    /* Connect to ADS device */
    this->connect(this->pasynUserSelf);

    return 0;
}

asynStatus ADSPortDriver::connect(asynUser *asyn_user) {
    LOG_TRACE_ASYN(asyn_user, "entering");

    /* Exclusive use of ads_conn for this method */
    std::lock_guard<epicsMutex> lock(this->ads_conn_mtx);

    if (this->ioc_initialized == false) {
        LOG_WARN_ASYN(asyn_user, "IOC not initialized");
        return asynError;
    }

    if (this->ads_conn->is_connected() == true) {
        LOG_WARN_ASYN(asyn_user, "already connected to ADS device");
        return asynSuccess;
    }

    /* Don't touch ADS if IOC is shutting down */
    if (this->is_exiting() == true) {
        return asynSuccess;
    }

    /* Connect to ADS device */
    int rc = this->ads_conn->connect(this->ams_remote, this->ip);
    if (rc != 0) {
        LOG_ERR_ASYN(asyn_user, "could not connect to ADS device (%i): %s", rc, ads_errors[rc].c_str());
        return asynSuccess;
    }
    LOG_WARN_ASYN(asyn_user, "connected to ADS device (IP: %s)", this->ip.c_str());

    /* Resolve ADS read variables */
    if (this->ads_read_vars.size() > 0) {
        LOG_WARN_ASYN(asyn_user, "resolving %lu ADS read variable names..", this->ads_read_vars.size());
        rc = this->ads_conn->resolve_variables(this->ads_read_vars);
        if (rc != 0) {
            LOG_ERR_ASYN(asyn_user, "could not resolve ADS read variable names (%i): %s", rc, ads_errors[rc].c_str());
            return asynSuccess;
        }
    }
    LOG_WARN_ASYN(asyn_user, "resolved %lu ADS read variable names", this->ads_read_vars.size());

    /* Resolve ADS write variables */
    if (this->ads_write_vars.size() > 0) {
        rc = this->ads_conn->resolve_variables(this->ads_write_vars);
        if (rc != 0) {
            LOG_ERR_ASYN(asyn_user, "could not resolve ADS write variable names(%i): %s", rc, ads_errors[rc].c_str());
            return asynSuccess;
        }
    }
    LOG_WARN_ASYN(asyn_user, "resolved %lu ADS write variable names", this->ads_write_vars.size());

    /* Initialize ADS sum-read buffers */
    rc = this->sum_read_req.initialize();
    if (rc != 0) {
        LOG_ERR_ASYN(asyn_user, "error initializing sum-read request buffers (%i): %s", rc, ads_errors[rc].c_str());
        return asynSuccess;
    }
    LOG_WARN_ASYN(asyn_user, "initialized sum-read request buffers");

    rc = this->sum_read();
    LOG_WARN_ASYN(asyn_user, "inital sum-read status (%i): %s", rc, ads_errors[rc].c_str());

    return DynamicAsynPortDriver::connect(asyn_user);
}

asynStatus ADSPortDriver::disconnect(asynUser *asyn_user) {
    LOG_TRACE_ASYN(asyn_user, "entering");

    /* Don't leave ADS sum-read buffers in a dirty state */
    this->sum_read_req.deinitialize();

    /* A clean disconnect can be performed if the ADS port driver is not shutting
     * down, i.e. the destructor isn't being called. See Connection::set_disconnect()
     * for more information. */
    if (this->exit_called == false) {
        std::lock_guard<epicsMutex> lock(this->ads_conn_mtx);
        if (this->ads_conn->is_connected() == false) {
            LOG_WARN_ASYN(asyn_user, "already disconnected");
            return DynamicAsynPortDriver::disconnect(asyn_user);
        }

        /* Release read variable handles */
        LOG_TRACE_ASYN(asyn_user, "unresolving ADS read variables");
        int rc = this->ads_conn->unresolve_variables(this->ads_read_vars);
        if (rc != 0) {
            LOG_ERR_ASYN(asyn_user, "failed to unresolve ADS read variables (%i): %s", rc, ads_errors[rc].c_str());
        }

        /* Release write variable handles */
        LOG_TRACE_ASYN(asyn_user, "unresolving ADS write variables");
        rc = this->ads_conn->unresolve_variables(this->ads_write_vars);
        if (rc != 0) {
            LOG_ERR_ASYN(asyn_user, "failed to unresolve ADS write variables (%i): %s", rc, ads_errors[rc].c_str());
        }

        /* Finally disconnect from ADS device */
        LOG_TRACE_ASYN(asyn_user, "disconnecting from ADS device");
        this->ads_conn->disconnect();
    }

    /* ADS connection is set to a disconnected state, regardless if a clean
     * disconnect was performed or not. */
    {
        std::lock_guard<epicsMutex> lock(this->ads_conn_mtx);
        this->ads_conn->set_disconnected();
    }

    return DynamicAsynPortDriver::disconnect(asyn_user);
}

asynStatus ADSPortDriver::drvUserCreate(asynUser *asyn_user, const char *reason, const char **type_name, size_t *size) {
    LOG_TRACE_ASYN(asyn_user, "entering; reason: '%s'", reason);

    std::shared_ptr<ADSAddress> ads_addr = nullptr;
    try {
        ads_addr.reset(new ADSAddress(reason));
    }
    catch (std::invalid_argument &ex) {
        LOG_ERR_ASYN(asyn_user, "%s", ex.what());
        return asynError;
    }

    try {
        auto pv_info = pvkeeper->registerPV(reason);
        asyn_user->reason = pv_info->getId();

        auto *h = new SharedPointerHolder<ADSPVInfo>(pv_info);
        asyn_user->userData = (void*) (h);
    }
    catch(...) {
        LOG_ERR_ASYN(asyn_user, "could not register PV for asyn reason '%s'", reason);
    	return asynError;
    }

    PVInfoHolder *pv_info_holder = (PVInfoHolder *)asyn_user->userData;
    auto pv_info = pv_info_holder->getPointer();

    /* Return if PV was already initalized */
    if (pv_info->first_init == false) {
        return asynSuccess;
    }

    pv_info->asyn_user = asyn_user;

    pv_info->first_init = false;

    std::shared_ptr<Variable> ads_var = std::make_shared<Variable>(ads_addr);
    ads_var->set_connection(this->ads_conn);

    this->ads_vars_by_asyn_reason[asyn_user->reason] = std::shared_ptr<Variable>(ads_var);

    if (ads_var->addr->get_operation() == Operation::Read) {
        this->ads_read_vars.push_back(std::shared_ptr<Variable>(ads_var));
    } else {
        this->ads_write_vars.push_back(std::shared_ptr<Variable>(ads_var));
    }

    pv_info_by_ads_var[ads_var] = pv_info;

    pv_info->set_ads_variable(std::shared_ptr<Variable>(ads_var));
    LOG_TRACE_ASYN(asyn_user, "ADS variable: '%s', asyn reason: %i", pv_info->get_ads_variable()->addr->info().c_str(), pv_info->asyn_user->reason);

    return asynSuccess;
}

// WORK IN PROGRESS
asynStatus ADSPortDriver::drvUserDestroy(asynUser *asyn_user) {
//    return DynamicAsynPortDriver::drvUserDestroy(asyn_user);
    return asynSuccess;
}

void ADSPortDriver::listIOScanPVs() {
    DynamicAsynPortDriver::listIOScanPVs();
}

void ADSPortDriver::addPVToIOScan(const ADSPVInfoPtr& pv_info, int interfaceMask) {
    LOG_TRACE_ASYN(pv_info->asyn_user, "interrupt for record '%s'", pv_info->getPVName().c_str());

    pv_info->interruptMask = interfaceMask;
    pv_info->get_ads_variable()->set_notify_on_update(true);
}

void ADSPortDriver::report(FILE *fp, int details) {
    asynPortDriver::report(fp, details);

    if (details >= 1) {
        reportSummary(fp);
    }

    this->sum_read_req.print_info(fp, details);

    if (details >= 2) {
        reportDetails(fp);
    }
}

void ADSPortDriver::reportSummary(FILE *fp) {
    fprintf(fp, "ADS summary report:\n");
    fprintf(fp, " Port driver version: %s\n", ADS_PORT_DRIVER_VERSION);
    fprintf(fp, " General:\n");
    fprintf(fp, "   - Connected to ADS device: %s\n", (this->ads_conn->is_connected() ? "yes" : "no"));
    fprintf(fp, "   - Remote IP: %s\n", ip.c_str());
    fprintf(fp, "   - Remote AMS Net ID: %u.%u.%u.%u.%u.%u\n", ams_remote.b[0],
                         ams_remote.b[1], ams_remote.b[2], ams_remote.b[3], ams_remote.b[4], ams_remote.b[5]);
    if (ads_function_timeout_ms < 0) {
        fprintf(fp, "   - ADS timeout: %lu [s]\n", default_ads_function_timeout);
    }

    fprintf(fp, "ADS device information: %s\n", this->ads_device_info.c_str());
    fprintf(fp, "ADS information\n");
    fprintf(fp, "  Version: %i\n", (int)this->ads_version.version);
    fprintf(fp, "  Revision: %i\n", (int)this->ads_version.revision);
    fprintf(fp, "  Build: %i\n", (int)this->ads_version.build);

    fprintf(fp, "ADS device state: (%i)\n", this->ads_device_state);
    fprintf(fp, "ADS state: %s (%i)\n", ads_states[this->ads_state].c_str(), this->ads_state);
}

void ADSPortDriver::reportDetails(FILE *fp) {
    fprintf(fp, "ADS detailed report:\n");

    using pv_map = std::map<std::string, ADSPVInfoPtr>;
    pv_map PVInfoMap = pvkeeper->getList();

    for (pv_map::const_iterator itr = PVInfoMap.begin(); itr != PVInfoMap.end(); ++itr) {
        auto pv_info = itr->second;
        fprintf(fp, "ADS Address Info: Variable = '%s'\n", pv_info->get_ads_variable()->addr->info().c_str());
        fprintf(fp, "   - Asyn Reason ID: %d\n", pv_info->getId());
        fprintf(fp, "   - PLC Data Type: %s\n", pv_info->getFunctionName().c_str());
    }
}

void ADSPortDriver::registerADSHandlers() {
    // asynInt32 Interface
    asynInt32Handlers.reg("BOOL",
            &HWInterface<ADSPortDriver>::readInt8,
            &HWInterface<ADSPortDriver>::writeBool, adsInterface);

    asynInt32Handlers.reg("SINT",
            &HWInterface<ADSPortDriver>::readInt8,
            &HWInterface<ADSPortDriver>::writeInt8, adsInterface);

    asynInt32Handlers.reg("BYTE",
            &HWInterface<ADSPortDriver>::readUInt8,
            &HWInterface<ADSPortDriver>::writeUInt8, adsInterface);

    asynInt32Handlers.reg("USINT",
            &HWInterface<ADSPortDriver>::readUInt8,
            &HWInterface<ADSPortDriver>::writeUInt8, adsInterface);

    asynInt32Handlers.reg("INT",
            &HWInterface<ADSPortDriver>::readInt16,
            &HWInterface<ADSPortDriver>::writeInt16, adsInterface);

    asynInt32Handlers.reg("WORD",
            &HWInterface<ADSPortDriver>::readUInt16,
            &HWInterface<ADSPortDriver>::writeUInt16, adsInterface);

    asynInt32Handlers.reg("UINT",
            &HWInterface<ADSPortDriver>::readUInt16,
            &HWInterface<ADSPortDriver>::writeUInt16, adsInterface);

    asynInt32Handlers.reg("DINT",
            &HWInterface<ADSPortDriver>::readInt32,
            &HWInterface<ADSPortDriver>::writeInt32, adsInterface);

    asynInt32Handlers.reg("DWORD",
            &HWInterface<ADSPortDriver>::readUInt32,
            &HWInterface<ADSPortDriver>::writeUInt32, adsInterface);

    asynInt32Handlers.reg("UDINT",
            &HWInterface<ADSPortDriver>::readUInt32,
            &HWInterface<ADSPortDriver>::writeUInt32, adsInterface);

    // asynFloat64 Interface
    asynFloat64Handlers.reg("REAL",
            &HWInterface<ADSPortDriver>::readFloat32,
            &HWInterface<ADSPortDriver>::writeFloat32, adsInterface);

    asynFloat64Handlers.reg("LREAL",
            &HWInterface<ADSPortDriver>::readFloat64,
            &HWInterface<ADSPortDriver>::writeFloat64, adsInterface);

    // asynUInt32Digital Interface
    asynUInt32DigitalHandlers.reg("BOOL",
            &HWInterface<ADSPortDriver>::readUInt8Digital,
            &HWInterface<ADSPortDriver>::writeBoolDigital, adsInterface);

    asynUInt32DigitalHandlers.reg("BYTE",
            &HWInterface<ADSPortDriver>::readUInt8Digital,
            &HWInterface<ADSPortDriver>::writeUInt8Digital, adsInterface);

    asynUInt32DigitalHandlers.reg("USINT",
            &HWInterface<ADSPortDriver>::readUInt8Digital,
            &HWInterface<ADSPortDriver>::writeUInt8Digital, adsInterface);

    asynUInt32DigitalHandlers.reg("WORD",
            &HWInterface<ADSPortDriver>::readUInt16Digital,
            &HWInterface<ADSPortDriver>::writeUInt16Digital, adsInterface);

    asynUInt32DigitalHandlers.reg("UINT",
            &HWInterface<ADSPortDriver>::readUInt16Digital,
            &HWInterface<ADSPortDriver>::writeUInt16Digital, adsInterface);

    asynUInt32DigitalHandlers.reg("DWORD",
            &HWInterface<ADSPortDriver>::readUInt32Digital,
            &HWInterface<ADSPortDriver>::writeUInt32Digital, adsInterface);

    asynUInt32DigitalHandlers.reg("UDINT",
            &HWInterface<ADSPortDriver>::readUInt32Digital,
            &HWInterface<ADSPortDriver>::writeUInt32Digital, adsInterface);

    // asynOctet Interface
    asynOctetHandlers.reg("STRING",
            &HWInterface<ADSPortDriver>::readString,
            &HWInterface<ADSPortDriver>::writeString, adsInterface);

    // asynIntXXXArray Interface
    asynArrayInt8Handlers.reg("BOOL",
            &HWInterface<ADSPortDriver>::readInt8Array,
            &HWInterface<ADSPortDriver>::writeInt8Array, adsInterface);

    asynArrayInt8Handlers.reg("SINT",
            &HWInterface<ADSPortDriver>::readInt8Array,
            &HWInterface<ADSPortDriver>::writeInt8Array, adsInterface);

    asynArrayInt8Handlers.reg("BYTE",
            &HWInterface<ADSPortDriver>::readInt8Array,
            &HWInterface<ADSPortDriver>::writeInt8Array, adsInterface);

    asynArrayInt8Handlers.reg("USINT",
            &HWInterface<ADSPortDriver>::readInt8Array,
            &HWInterface<ADSPortDriver>::writeInt8Array, adsInterface);


    asynArrayInt16Handlers.reg("INT",
            &HWInterface<ADSPortDriver>::readInt16Array,
            &HWInterface<ADSPortDriver>::writeInt16Array, adsInterface);

    asynArrayInt16Handlers.reg("WORD",
            &HWInterface<ADSPortDriver>::readInt16Array,
            &HWInterface<ADSPortDriver>::writeInt16Array, adsInterface);

    asynArrayInt16Handlers.reg("UINT",
            &HWInterface<ADSPortDriver>::readInt16Array,
            &HWInterface<ADSPortDriver>::writeInt16Array, adsInterface);


    asynArrayInt32Handlers.reg("DINT",
            &HWInterface<ADSPortDriver>::readInt32Array,
            &HWInterface<ADSPortDriver>::writeInt32Array, adsInterface);

    asynArrayInt32Handlers.reg("DWORD",
            &HWInterface<ADSPortDriver>::readInt32Array,
            &HWInterface<ADSPortDriver>::writeInt32Array, adsInterface);

    asynArrayInt32Handlers.reg("UDINT",
            &HWInterface<ADSPortDriver>::readInt32Array,
            &HWInterface<ADSPortDriver>::writeInt32Array, adsInterface);


    asynArrayFloat32Handlers.reg("REAL",
            &HWInterface<ADSPortDriver>::readFloat32Array,
            &HWInterface<ADSPortDriver>::writeFloat32Array, adsInterface);


    asynArrayFloat64Handlers.reg("LREAL",
            &HWInterface<ADSPortDriver>::readFloat64Array,
            &HWInterface<ADSPortDriver>::writeFloat64Array, adsInterface);

    // Interrupt Interfaces
    asynInterruptHandlers.reg("BOOL",
                    &HWInterface<ADSPortDriver>::readInt8Intr, adsInterface);

    asynInterruptHandlers.reg("SINT",
                    &HWInterface<ADSPortDriver>::readInt8Intr, adsInterface);

    asynInterruptHandlers.reg("BYTE",
                    &HWInterface<ADSPortDriver>::readUInt8Intr, adsInterface);

    asynInterruptHandlers.reg("USINT",
                    &HWInterface<ADSPortDriver>::readUInt8Intr, adsInterface);

    asynInterruptHandlers.reg("INT",
                    &HWInterface<ADSPortDriver>::readInt16Intr, adsInterface);

    asynInterruptHandlers.reg("WORD",
                    &HWInterface<ADSPortDriver>::readUInt16Intr, adsInterface);

    asynInterruptHandlers.reg("UINT",
                    &HWInterface<ADSPortDriver>::readUInt16Intr, adsInterface);

    asynInterruptHandlers.reg("DINT",
                    &HWInterface<ADSPortDriver>::readInt32Intr, adsInterface);

    asynInterruptHandlers.reg("DWORD",
                    &HWInterface<ADSPortDriver>::readUInt32Intr, adsInterface);

    asynInterruptHandlers.reg("UDINT",
                    &HWInterface<ADSPortDriver>::readUInt32Intr, adsInterface);

    asynInterruptHandlers.reg("REAL",
                    &HWInterface<ADSPortDriver>::readFloat32Intr, adsInterface);

    asynInterruptHandlers.reg("LREAL",
                    &HWInterface<ADSPortDriver>::readFloat64Intr, adsInterface);

    asynInterruptHandlers.reg("STRING",
                    &HWInterface<ADSPortDriver>::readOctetIntr, adsInterface);

    asynDigitalInterruptHandlers.reg("BOOL",
                    &HWInterface<ADSPortDriver>::readUInt8DigitalIntr, adsInterface);

    asynDigitalInterruptHandlers.reg("BYTE",
                    &HWInterface<ADSPortDriver>::readUInt8DigitalIntr, adsInterface);

    asynDigitalInterruptHandlers.reg("USINT",
                    &HWInterface<ADSPortDriver>::readUInt8DigitalIntr, adsInterface);

    asynDigitalInterruptHandlers.reg("WORD",
                    &HWInterface<ADSPortDriver>::readUInt16DigitalIntr, adsInterface);

    asynDigitalInterruptHandlers.reg("UINT",
                    &HWInterface<ADSPortDriver>::readUInt16DigitalIntr, adsInterface);

    asynDigitalInterruptHandlers.reg("DWORD",
                    &HWInterface<ADSPortDriver>::readUInt32DigitalIntr, adsInterface);

    asynDigitalInterruptHandlers.reg("UDINT",
                    &HWInterface<ADSPortDriver>::readUInt32DigitalIntr, adsInterface);

    asynArrayInterruptHandlers.reg("BOOL",
                    &HWInterface<ADSPortDriver>::readInt8ArrayIntr, adsInterface);

    asynArrayInterruptHandlers.reg("SINT",
                    &HWInterface<ADSPortDriver>::readInt8ArrayIntr, adsInterface);

    asynArrayInterruptHandlers.reg("BYTE",
                    &HWInterface<ADSPortDriver>::readInt8ArrayIntr, adsInterface);

    asynArrayInterruptHandlers.reg("USINT",
                    &HWInterface<ADSPortDriver>::readInt8ArrayIntr, adsInterface);

    asynArrayInterruptHandlers.reg("INT",
                    &HWInterface<ADSPortDriver>::readInt16ArrayIntr, adsInterface);

    asynArrayInterruptHandlers.reg("WORD",
                    &HWInterface<ADSPortDriver>::readInt16ArrayIntr, adsInterface);

    asynArrayInterruptHandlers.reg("UINT",
                    &HWInterface<ADSPortDriver>::readInt16ArrayIntr, adsInterface);

    asynArrayInterruptHandlers.reg("DINT",
                    &HWInterface<ADSPortDriver>::readInt32ArrayIntr, adsInterface);

    asynArrayInterruptHandlers.reg("DWORD",
                    &HWInterface<ADSPortDriver>::readInt32ArrayIntr, adsInterface);

    asynArrayInterruptHandlers.reg("UDINT",
                    &HWInterface<ADSPortDriver>::readInt32ArrayIntr, adsInterface);

    asynArrayInterruptHandlers.reg("REAL",
                    &HWInterface<ADSPortDriver>::readFloat32ArrayIntr, adsInterface);

    asynArrayInterruptHandlers.reg("LREAL",
                    &HWInterface<ADSPortDriver>::readFloat64ArrayIntr, adsInterface);
}

void ADSPortDriver::set_exit_called() {
    LOG_WARN_ASYN(this->pasynUserSelf, "received shutdown signal");

    this->exit_called = true;
}

bool ADSPortDriver::is_exiting() {
    return this->exit_called;
}

void ADSPortDriver::ads_comm_task() {
    LOG_TRACE_ASYN(this->pasynUserSelf, "ADS communication thread starting");

    /* Used for chekcing if periodic tasks should be performed */
    epicsTimeStamp time_empty = {0, 0};
    epicsTimeStamp time_last_get_ads_info = time_empty;
    epicsTimeStamp time_last_reconnect_attempt = time_empty;

    /* Loop until thread exit event is signaled */
    while (this->exit_ads_comm_thread.tryWait() == false) {
        /* Skip execution while IOC and port driver is not yet initialized */
        if (this->ioc_initialized == false) {
            epicsThreadSleep(sleep_while_not_connected_sec);
            continue;
        }

        /* Get current time, used to check if periodic tasks (get device info,
         * reconnect) should be done */
        epicsTimeStamp time_now;
        int rc = epicsTimeGetCurrent(&time_now);
        if (rc != 0) {
            LOG_ERR_ASYN(this->pasynUserSelf, "could not get current time (%i)", rc);
            this->disconnect(this->pasynUserSelf);
            continue;
        }

        /* Get ADS connection status */
        bool is_connected;
        {
            std::lock_guard<epicsMutex> lock(this->ads_conn_mtx);
            is_connected = this->ads_conn->is_connected();
        }

        /* ADS not connected? */
        if (is_connected == false) {
            /* Set last reconnect timer to sensible value if it is empty */
            if (epicsTimeEqual(&time_last_reconnect_attempt, &time_empty)) {
                time_last_reconnect_attempt = time_now;
            }

            /* Should ADS reconnect be performed? */
            double time_diff = epicsTimeDiffInSeconds(&time_now, &time_last_reconnect_attempt);
            if (time_diff > reconnect_timer) {
                LOG_WARN_ASYN(this->pasynUserSelf, "attempting to reconnect to ADS device");
                this->connect(this->pasynUserSelf);
                time_last_reconnect_attempt = time_now;
            } else {
                LOG_TRACE_ASYN(this->pasynUserSelf, "waiting before attempting to reconnect to ADS device");
                epicsThreadSleep(sleep_while_not_connected_sec);
            }

            /* In any case, skip one loop iteration */
            continue;
        }

        /* Check if it's time to read ADS device and protocol information  */
        double time_diff = epicsTimeDiffInSeconds(&time_now, &time_last_get_ads_info);
        if (time_diff > get_device_info_interval_sec) {
            int rc = epicsTimeGetCurrent(&time_last_get_ads_info);
            if (rc != 0) {
                LOG_ERR_ASYN(this->pasynUserSelf, "could not get current time (%i)", rc);
                this->disconnect(this->pasynUserSelf);
                continue;
            }

            /* Don't touch ADS if IOC is shutting down */
            if (this->is_exiting() == true) {
                break;
            }

            /* Read device information */
            rc = this->read_ads_device_information();
            if (rc != 0) {
                continue;
            }

            /* Don't touch ADS if IOC is shutting down */
            if (this->is_exiting() == true) {
                break;
            }

            /* Read device state */
            rc = this->read_ads_device_state();
            if (rc != 0) {
                continue;
            }
        }

        /* Don't touch ADS if IOC is shutting down */
        if (this->is_exiting() == true) {
            break;
        }

        /* Sum-read ADS input variables */
        rc = this->sum_read();
        if (rc != 0) {
            continue;
        }

        /* Trigger callbacks for I/O Intr records */
        auto updated_vars = this->sum_read_req.get_updated_variables();
        this->trigger_callback_variables(updated_vars);

        epicsThreadSleep(sleep_after_sum_reads_sec);
    }

    LOG_TRACE_ASYN(this->pasynUserSelf, "ADS communication thread exiting");
    this->exit_ads_comm_thread_done.trigger();
}

int ADSPortDriver::read_ads_device_state() {
    uint16_t device_state;
    ADSState state;
    int rc;
    {
        std::lock_guard<epicsMutex> lock(this->ads_conn_mtx);
        rc = this->ads_conn->read_device_state(&device_state, &state);
    }

    switch (rc) {
        case EPICSADS_OK:
            this->ads_device_state = device_state;
            this->ads_state = state;
            break;

        case EPICSADS_DISCONNECTED:
            LOG_WARN_ASYN(this->pasynUserSelf, "cannot read ADS device state, no connection to ADS device");
            this->disconnect(this->pasynUserSelf);
            return rc;

        default:
            LOG_ERR_ASYN(this->pasynUserSelf, "error reading ADS device state (%i): %s", rc, ads_errors[rc].c_str());
            this->disconnect(this->pasynUserSelf);
            return rc;
    }

    return 0;
}

int ADSPortDriver::read_ads_device_information() {
    char device_info[16];
    AdsVersion version;
    int rc;
    {
        std::lock_guard<epicsMutex> lock(this->ads_conn_mtx);
        rc = this->ads_conn->read_device_info(device_info, sizeof(device_info), &version);
    }

    switch (rc) {
        case EPICSADS_OK:
            this->ads_device_info = device_info;
            this->ads_version = version;
            break;

        case EPICSADS_DISCONNECTED:
            LOG_WARN_ASYN(this->pasynUserSelf, "cannot read ADS device info; no connection to ADS device");
            this->disconnect(this->pasynUserSelf);
            return rc;

        default:
            LOG_ERR_ASYN(this->pasynUserSelf, "error reading ADS device info (%i): %s", rc, ads_errors[rc].c_str());
            this->disconnect(this->pasynUserSelf);
            return rc;
    }

    return 0;
}

int ADSPortDriver::sum_read() {
    int rc;
    {
        std::lock_guard<epicsMutex> lock(this->ads_conn_mtx);
        rc = this->sum_read_req.read();
    }

    switch (rc) {
        case EPICSADS_OK: break;

        case EPICSADS_DISCONNECTED:
            LOG_WARN_ASYN(this->pasynUserSelf, "sum-read failed due to no connection to ADS device");
            this->disconnect(this->pasynUserSelf);
            return rc;

        default:
            LOG_ERR_ASYN(this->pasynUserSelf, "error reading ADS data (%i): %s", rc, ads_errors[rc].c_str());
            this->disconnect(this->pasynUserSelf);
            return rc;
    }

    return 0;
}

void ADSPortDriver::trigger_callback_variables(std::vector<std::shared_ptr<Variable>> updated_variables) {
    for (auto it = updated_variables.begin(); it != updated_variables.end(); it++) {
        auto variable = *it;

        auto pv_info = this->pv_info_by_ads_var[variable];
        if (pv_info == nullptr) {
            LOG_ERR_ASYN(this->pasynUserSelf, "could not find PV info for ADS variable '%s'", variable->addr->get_var_name().c_str());
            continue;
        }

        /* Fire callbacks */
        if (pv_info->interruptMask & interruptStandardMask) {
            this->asynInterruptHandlers.invokeCallback(pv_info, this, nullptr);
        } else if (pv_info->interruptMask & interruptDigitalMask) {
            this->asynDigitalInterruptHandlers.invokeCallback(pv_info, this, nullptr);
        } else if (pv_info->interruptMask & interruptArrayMask) {
            this->asynArrayInterruptHandlers.invokeCallback(pv_info, this, nullptr);
        } else {
            LOG_ERR_ASYN(this->pasynUserSelf, "unknown interrupt mask %x for '%s'", pv_info->interruptMask, pv_info->get_ads_variable()->addr->info().c_str());
        }
    }
}

void exit_func(void *args) {
    ADSPortDriver *ads_port_driver = (ADSPortDriver *)args;
    ads_port_driver->set_exit_called();
}

static void ads_comm_task(void *param) {
    ADSPortDriver *ads_port_driver = (ADSPortDriver *)param;
    ads_port_driver->ads_comm_task();
}

static void epicsHookState(initHookState state) {
    switch(state) {
        case initHookAfterScanInit:
            using ADSPortDriverMap = std::map<std::string, ADSPortDriverPtr>;
            for (ADSPortDriverMap::iterator itr=registered_port_drivers.begin();
                       itr!=registered_port_drivers.end(); ++itr) {
                itr->second->listIOScanPVs();
                itr->second->initialize();
            }
            break;

        default:
            break;
    }
}

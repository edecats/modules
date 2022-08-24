/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#ifndef ADSPORTDRIVER_H
#define ADSPORTDRIVER_H

#include <memory>
#include <map>
#include <DynamicAsynPortDriver.h>
#include <AdsLib.h>
#include <epicsEvent.h>
#include <epicsThread.h>
#include "epics-ads/Connection.h"
#include "epics-ads/SumReadRequest.h"
#include "epics-ads/Variable.h"
#include "HWInterface.h"
#include "AdsInterruptHandlers.h"

class ADSPortDriver;

/* How often does the ADS communication thread query the ADS device and protocol
 * information, as well as device and ADS state. */
const int get_device_info_interval_sec = 5.0;

/* How long to wait between reconnection attempts */
const int reconnect_timer = 10.0;

/* Sleep duration for the ADS communication thread while waiting for port driver
 * to etablish ADS connection. */
const double sleep_while_not_connected_sec = 0.5;

/* Default sleep duration for the ADS communication thread between PLC variables
 * sum-reads. */
const double sleep_after_sum_reads_sec = 0.001;

typedef std::shared_ptr<ADSPortDriver> ADSPortDriverPtr;

typedef struct db {
        std::string name;
        std::string interface;
} db;

class ADSPortDriver : public DynamicAsynPortDriver<ADSPVInfo>,
                             public std::enable_shared_from_this<ADSPortDriver> {
protected:
    std::string port_name;
    std::string ip;
    AmsNetId ams_remote;
    epicsUInt16 sum_buffer_nelem; /* Max number of elements for ADS sum buffers */
    epicsUInt32 ads_function_timeout_ms; /* Timeout for ADS functions [ms] */

    epicsMutex ads_conn_mtx;
    const std::shared_ptr<Connection> ads_conn;

    SumReadRequest sum_read_req;
    std::map<int, std::shared_ptr<Variable>> ads_vars_by_asyn_reason;
    std::vector<std::shared_ptr<Variable>> ads_read_vars;
    std::vector<std::shared_ptr<Variable>> ads_write_vars;
    std::map<std::shared_ptr<Variable>, ADSPVInfoPtr> pv_info_by_ads_var;

    PVKeeper<ADSPVInfo> *pvkeeper;
    HWInterface<ADSPortDriver> *adsInterface;

    epicsThreadId ads_comm_tid = 0; /* ADS communication thread ID  */
    epicsEvent exit_ads_comm_thread;
    epicsEvent exit_ads_comm_thread_done;

    std::string ads_device_info;
    AdsVersion ads_version = {0, 0, 0};
    uint16_t ads_device_state = 0;
    ADSState ads_state;

    bool ioc_initialized = false;
    bool port_driver_shutting_down = false;
    bool exit_called = false;

    // Functions
    void registerADSHandlers();

    /* Read ADS device state */
    int read_ads_device_state();

    /* Read ADS device information */
    int read_ads_device_information();

    /* Sum-read all input ADS variables */
    int sum_read();

    /* Trigger I/O Intr callbacks for updated_variables */
    void trigger_callback_variables(std::vector<std::shared_ptr<Variable>> updated_variables);

    // Handlers for interrupt
    AsynInterruptTypeHandlerKeeper<ADSPortDriver, ADSPVInfo>   asynInterruptHandlers;
    AsynInterruptTypeHandlerKeeper<ADSPortDriver, ADSPVInfo>   asynDigitalInterruptHandlers;
    AsynInterruptTypeHandlerKeeper<ADSPortDriver, ADSPVInfo>   asynArrayInterruptHandlers;

public:
    static const epicsUInt32 DEFAULT_SUM_BUFFER_NELEM = 500;
    static const epicsUInt32 DEFAULT_ADS_CALL_TIMEOUT_MS = 5000;

    ADSPortDriver(const char *port_name,
                  const char *ip_addr,
                  const char *ams_net_id,
                  epicsUInt16 sum_buffer_nelem,
                  epicsUInt32 ads_function_timeout,
                  int auto_connect,
                  int priority);

    virtual ~ADSPortDriver();

    virtual asynStatus connect(asynUser *asyn_user);
    virtual asynStatus disconnect(asynUser *asyn_user);
    virtual asynStatus drvUserCreate(asynUser *asyn_user, const char *reason, const char **type_name, size_t *size);
    virtual asynStatus drvUserDestroy(asynUser *asyn_user);
    virtual void report(FILE *fp, int details);
    virtual void reportSummary(FILE *fp);
    virtual void reportDetails(FILE *fp);
    virtual void ads_comm_task();
    virtual int initialize();
    virtual void set_exit_called();
    virtual bool is_exiting();

    virtual void listIOScanPVs();
    virtual void addPVToIOScan(const ADSPVInfoPtr& pv_info, int interfaceMask);

    virtual void callCallbacksInt32(epicsInt32 value, int reason, int address, asynStatus status) {
        doCallbacksInt32(value, reason, address, asynSuccess); }

    virtual void callCallbacksFloat64(epicsFloat64 value, int reason, int address, asynStatus status) {
        doCallbacksFloat64(value, reason, address, asynSuccess); }

    virtual void callCallbacksUInt32Digital(epicsUInt32 value, int reason, int address, asynStatus status) {
        doCallbacksUInt32Digital(value, reason, address, asynSuccess); }

    virtual void callCallbacksOctet(const char *value, int reason, int address, asynStatus status) {
        doCallbacksOctet((char *) value, reason, address, asynSuccess); }

    virtual void callCallbacksInt8Array(const epicsInt8 *value, size_t nElements, int reason, int address, asynStatus status) {
        doCallbacksInt8Array((epicsInt8 *) value, nElements, reason, address, asynSuccess); }

    virtual void callCallbacksInt16Array(const epicsInt16 *value, size_t nElements, int reason, int address, asynStatus status) {
        doCallbacksInt16Array((epicsInt16 *) value, nElements, reason, address, asynSuccess); }

    virtual void callCallbacksInt32Array(const epicsInt32 *value, size_t nElements, int reason, int address, asynStatus status) {
        doCallbacksInt32Array((epicsInt32 *) value, nElements, reason, address, asynSuccess); }

    virtual void callCallbacksFloat32Array(const epicsFloat32 *value, size_t nElements, int reason, int address, asynStatus status) {
        doCallbacksFloat32Array((epicsFloat32 *) value, nElements, reason, address, asynSuccess); }

    virtual void callCallbacksFloat64Array(const epicsFloat64 *value, size_t nElements, int reason, int address, asynStatus status) {
        doCallbacksFloat64Array((epicsFloat64 *) value, nElements, reason, address, asynSuccess); }
};

extern std::map<std::string, std::shared_ptr<ADSPortDriver>> registered_port_drivers;

#endif /* ADSPORTDRIVER_H */

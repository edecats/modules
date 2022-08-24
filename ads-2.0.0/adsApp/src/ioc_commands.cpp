/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#include <cstring>
#include <map>
#include <memory>
#include <epicsExport.h>
#include <iocsh.h>
#include <errlog.h>
#include "ADSPortDriver.h"

#include "PVKeeper.h"

std::map<std::string, std::shared_ptr<ADSPortDriver>> registered_port_drivers;

static const iocshArg ads_open_arg = {"parameters", iocshArgArgv};
static const iocshArg *ads_open_args[1] = {&ads_open_arg};
static const iocshFuncDef ads_open_func_def = {"AdsOpen", 1, ads_open_args};

static const iocshArg  ads_find_io_arg = {"port_name", iocshArgString};
static const iocshArg *ads_find_io_args[] = {&ads_find_io_arg};
static const iocshFuncDef ads_find_io_func_def = {"AdsFindIOIntrRecord", 1, ads_find_io_args};

static const iocshArg ads_set_ams_arg = {"ams_net_id", iocshArgString};
static const iocshArg *ads_set_ams_args[] = {&ads_set_ams_arg};
static const iocshFuncDef ads_set_local_amsNetID_func_def = {"AdsSetLocalAMSNetID", 1, ads_set_ams_args};

epicsShareFunc int ads_open(int argc, const char * const * argv) {
    std::string port_name;
    std::string ip_addr;
    std::string ams_net_id;
    int sum_buffer_nelem = ADSPortDriver::DEFAULT_SUM_BUFFER_NELEM;
    int ads_function_timeout_ms = -1;
    int no_auto_connect = 0;
    int priority = 0;


    if (argc < 4 || argc > 8) {
        errlogPrintf("AdsOpen <port_name> <ip_addr> <ams_net_id>"
   " | optional: <sum_buffer_nelem (default: %u)> <ads_timeout (default: %u) [ms]> <no_auto_connect> <priority>\n", ADSPortDriver::DEFAULT_SUM_BUFFER_NELEM, ADSPortDriver::DEFAULT_ADS_CALL_TIMEOUT_MS);
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        switch (i) {
            case 1:
                port_name = argv[i];
                break;
            case 2:
                ip_addr = argv[i];
                break;
            case 3:
                ams_net_id = argv[i];
                break;
            case 4:
                sum_buffer_nelem = strtol(argv[i], nullptr, 10);
                if (sum_buffer_nelem < 0) {
                    errlogPrintf("Error: sum_buffer_nelem must be a positive integer (%i)\n", sum_buffer_nelem);
                    return -1;
                }
                break;
            case 5:
                ads_function_timeout_ms = strtol(argv[i], nullptr, 10);
                if (ads_function_timeout_ms < 0) {
                    errlogPrintf("ads_function_timeout must be positive integer!\n");
                    return -1;
                }
                break;
            case 6:
                no_auto_connect = atoi(argv[i]);
                if (no_auto_connect < 0) {
                    errlogPrintf("no_auto_connect must be positive integer!\n");
                    return -1;
                }
                break;
            case 7:
                priority = atoi(argv[i]);
                if (priority < 0) {
                    errlogPrintf("priority must be positive integer!\n");
                    return -1;
                }
                break;
            default:
                break;
        }
    }

    /* The ADS port driver should connect automatically if the user only
     * specifies mandatory parameters */
    int auto_connect = (no_auto_connect == 0 ? 1 : 0);

    if (registered_port_drivers[port_name] != nullptr) {
        errlogPrintf("ADS port driver is already registered: '%s'\n", port_name.c_str());
        return -1;
    }

    /* Create a new instance of ADS port driver and add it to the
     * bookkeeping map */
    std::shared_ptr<ADSPortDriver> port_driver(new ADSPortDriver(port_name.c_str(), ip_addr.c_str(), ams_net_id.c_str(), sum_buffer_nelem, ads_function_timeout_ms, auto_connect, priority));
    registered_port_drivers[port_name] = port_driver;

    return 0;
}

epicsShareFunc void ads_find_io(const char *port_name) {
    /* Mandatory parameters */
    if (port_name == NULL) {
        for (auto itr = registered_port_drivers.begin(); itr != registered_port_drivers.end(); itr++) {
            registered_port_drivers[itr->first]->listIOScanPVs();
        }
    } else {
        std::map<std::string, std::shared_ptr<ADSPortDriver>>::iterator reg_port_itr;
        reg_port_itr = registered_port_drivers.find(port_name);
        if (reg_port_itr != registered_port_drivers.end())
            registered_port_drivers[port_name]->listIOScanPVs();
        else
            printf("ADS port driver does not exist\n");
    }
    return ;
}

epicsShareFunc void ads_set_local_amsNetID(const char* ams) {
    if (ams == NULL)
        errlogPrintf("Local Ams Net ID must be specified\n");
    else
        AdsSetLocalAddress((std::string)ams);

    return ;
}

static void ads_open_call_func(const iocshArgBuf *args) {
    ads_open(args[0].aval.ac, args[0].aval.av);
}

static void ads_find_io_call_func(const iocshArgBuf *args) {
    ads_find_io(args[0].sval);
}

static void ads_set_local_amsNetID_call_func(const iocshArgBuf *args) {
    ads_set_local_amsNetID(args[0].sval);
}

static void ads_open_register_command(void) {
        static int already_registered = 0;

            if (already_registered == 0) {
                iocshRegister(&ads_open_func_def, ads_open_call_func);
                already_registered = 1;
            }
}

static void ads_find_io_register_command(void) {
    static int already_registered = 0;

    if (already_registered == 0) {
        iocshRegister(&ads_find_io_func_def, ads_find_io_call_func);
        already_registered = -1;
    }
}

static void ads_set_local_amsNetID_register_command(void) {
    static int already_registered = 0;

    if (already_registered == 0) {
        iocshRegister(&ads_set_local_amsNetID_func_def, ads_set_local_amsNetID_call_func);
        already_registered = -1;
    }
}

epicsExportRegistrar(ads_open_register_command);
epicsExportRegistrar(ads_find_io_register_command);
epicsExportRegistrar(ads_set_local_amsNetID_register_command);

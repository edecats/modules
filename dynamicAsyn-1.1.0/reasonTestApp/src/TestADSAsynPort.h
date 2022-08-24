/*
 * TestDevice.h
 *
 *  Created on: 2018-04-15
 *      Author: Viacheslav Isaev
 */

#ifndef TESTDEVICE_H_
#define TESTDEVICE_H_

#include <string>
#include <stdexcept>

#include <vector>

#include <epicsEvent.h>
#include <epicsMutex.h>
#include <asynPortDriver.h>

#include <errlog.h> /* errlogPrintf */

#include "consts.h"
#include "PVKeeper.h"

#include "ADSAsynPort.h"
#include "HWInterface.h"

/* These are the drvInfo strings that are used to identify the parameters.
 * They are used by asyn clients, including standard asyn device support */
#define P_IDString              "ID"      /* asynFloat64,    r */
#define P_ErrorString           "Error"      /* asynFloat64,    r */
#define P_DIPollingString       "DIPolling"

#define P_DigInputString        "DigInput_%d"      /* asynInt32,    r */
#define P_DigOutputString       "DigOutput_%d"     /* asynInt32,    r */

typedef unsigned char uint8;


class TestADSAsynPort : public ADSAsynPort
{
public:

	typedef std::function<void(int, int)> my_func;

    // parent constructor
    //asynPortDriver(const char *portName, int maxAddr, int paramTableSize, int interfaceMask, int interruptMask,
    //               int asynFlags, int autoConnect, int priority, int stackSize);

    TestADSAsynPort(const char *portName, const char* deviceDescription, int autoConnect, HWInterface * hw);

    virtual ~TestADSAsynPort();

    // Asyn functions
    //virtual void report(FILE *fp, int details);

    virtual asynStatus connect(asynUser *pasynUser);
    virtual asynStatus disconnect(asynUser *pasynUser);

    // thread task
    void readbackPollingTask();
    void readDIStateTask();
    void setDigitalInputValue(int idx, int value);

protected:

    // definition of parameter addresses
    int P_ID;
    #define FIRST_SCOPE_COMMAND P_ID
    int P_DigOutputValue[MAX_OUTPUTS];
    int P_DigInputValue[MAX_INPUTS];
    int P_DIPolling;
    int P_Error;
    #define LAST_SCOPE_COMMAND  P_Error

protected:
    std::string _portName;
    std::string _deviceDescription;

    epicsEvent    _evntDigOutputReadBack;
    epicsEvent    _evntDigInpRead;
    volatile bool _doneWithThreads;

    int counter;

    std::vector<ADSPVInfoConstPtr> pvInt32List;
    epicsMutex  pvInt32ListMutex;

	void createParameters();
	void createThread();
	void createDIReadThread();

	void onIOScanStart() override;
	void addPVToIOScan(const std::shared_ptr<ADSPVInfo>&, const int interfaceMask) override;
	void onIOScanFinish() override;

	void regFunc(const my_func& f);
};

#define NUM_SCOPE_PARAMS (&LAST_SCOPE_COMMAND - &FIRST_SCOPE_COMMAND + 1)

#endif /* TESTDEVICE_H_ */

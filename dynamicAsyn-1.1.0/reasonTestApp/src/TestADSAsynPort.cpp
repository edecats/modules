/*
 * TestDevice.cpp
 *
 *  Created on: 18. maj 2017
 *      Author: Viacheslav Isaev
 */

#include <algorithm>
#include <epicsThread.h>
#include <time.h>
#include <stdlib.h>

#include <boost/bind.hpp>

//EPICS

#include "TestADSAsynPort.h"
#include "PVInfo.h"

void readDOStateTask(void *drvPvt);
void readDIStateTask(void *drvPvt);


#define DO_READOUT_TIMEOUT 0.5
#define DI_READ_TIMEOUT 1
#define DO_READOUT_DELAY 0.20

void TestADSAsynPort::setDigitalInputValue(int idx, int value)
{
	setIntegerParam(P_DigInputValue[idx], value );
}

void TestADSAsynPort::createParameters()
{
	createParam(P_ErrorString, asynParamInt32, &P_Error);

	createParam(P_IDString, asynParamOctet, &P_ID);
	createParam(P_DIPollingString, asynParamInt32, &P_DIPolling);

    for(int i=0; i<MAX_OUTPUTS; ++i)
    {
    	char str[40];
    	snprintf(str, 40, P_DigOutputString, i+1);
        createParam(str,  asynParamInt32,  &P_DigOutputValue[i]);
    }

    for(int i=0; i<MAX_INPUTS; ++i)
    {
    	char str[40];
    	snprintf(str, 40, P_DigInputString, i+1);
        createParam(str,  asynParamInt32,  &P_DigInputValue[i]);
    }

	/* Set the initial values of some parameters */
	setIntegerParam(P_Error, 0);
	setStringParam(P_ID, _deviceDescription.c_str());
}

void TestADSAsynPort::createThread()
{
	std::string taskName = _portName + "ReadBackPollingTask";

	asynStatus status = (asynStatus)(
			epicsThreadCreate(taskName.c_str(), epicsThreadPriorityMedium,
					epicsThreadGetStackSize(epicsThreadStackMedium),
					(EPICSTHREADFUNC)(::readDOStateTask), this) == NULL);
	if (status)
	{
		char str[80];
		snprintf(str, 80, "%s:%s: epicsThreadCreate failure.\n",
				_portName.c_str(), __FUNCTION__);

		errlogPrintf(str);

		throw std::runtime_error(str);
	}
	else
	{
		errlogPrintf("%s:%s: Success.\n", _portName.c_str(), __FUNCTION__);
	}
}

void TestADSAsynPort::createDIReadThread()
{
	std::string taskName = _portName + "DIReadTask";

	asynStatus status = (asynStatus)(
			epicsThreadCreate(taskName.c_str(), epicsThreadPriorityMedium,
					epicsThreadGetStackSize(epicsThreadStackMedium),
					(EPICSTHREADFUNC)(::readDIStateTask), this) == NULL);
	if (status)
	{
		char str[80];
		snprintf(str, 80, "%s:%s: epicsThreadCreate failure.\n",
				_portName.c_str(), __FUNCTION__);

		errlogPrintf(str);

		throw std::runtime_error(str);
	}
	else
	{
		errlogPrintf("%s:%s: Success.\n", _portName.c_str(), __FUNCTION__);
	}
}

static void my_func_test_1(int, int)
{
	errlogPrintf("FUNC: %s\n", __FUNCTION__);
}

TestADSAsynPort::TestADSAsynPort(const char* portName, const char* deviceDescription,
		int autoConnect,
		HWInterface * hw):
        ADSAsynPort(portName,
                           1, /* maxAddr */
                           (int)NUM_SCOPE_PARAMS+10,
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
        | asynGenericPointerMask  , /* Interface mask */

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
        | asynGenericPointerMask  , /* Interface mask */
               ASYN_CANBLOCK || ASYN_MULTIDEVICE, /* asynFlags.
               // This driver does not block and it is not multi-device, so flag is 0 */
                           autoConnect, /* Autoconnect */
                           0, /* Default priority */
                           0,
                           hw) /* Default stack size*/
	,_portName(portName)
	,_deviceDescription(deviceDescription)
	,_doneWithThreads(false)
 	,counter(0)
{
	std::string strTemp;

	createParameters();

	createThread();
	createDIReadThread();

	int a = -1;
	unsigned char  c = a;
	unsigned int d  = a;
	errlogPrintf("%s:%s: a=%016X %d d=%u c=%#010X\n", _portName.c_str(), __FUNCTION__, a, a, d, c);

	regFunc(&my_func_test_1);

}

TestADSAsynPort::~TestADSAsynPort()
{
	_doneWithThreads = true;
}

void readDOStateTask(void *drvPvt)
{
	TestADSAsynPort *pPvt = (TestADSAsynPort *)drvPvt;

    pPvt->readbackPollingTask();
}

void readDIStateTask(void *drvPvt)
{
	TestADSAsynPort *pPvt = (TestADSAsynPort *)drvPvt;

    pPvt->readDIStateTask();
}

void TestADSAsynPort::readbackPollingTask()
{
    asynStatus ret;
    uint8 inpDigOutputPorts[MAX_OUTPUTS];

    /* Loop forever */
    while (!_doneWithThreads)
    {
        _evntDigOutputReadBack.wait( DO_READOUT_TIMEOUT );

        if (_doneWithThreads)
        	break;

        // wait till relay out put is settled down.
        epicsThreadSleep(DO_READOUT_DELAY);


        //memset(inpDigOutputPorts, 0, MAX_INPUTS);
        ret = asynSuccess;

		lock();
        if( ret == asynSuccess )
        {
        	int mask = 1;
			for(int pin=0; pin < MAX_OUTPUTS; ++pin)
			{
				setIntegerParam(P_DigOutputValue[pin], inpDigOutputPorts[0] & mask);
				mask = mask << 1;
			}
        }else
        {
			setIntegerParam(P_Error, 1);
        }
		callParamCallbacks();
		unlock();
    }
}

asynStatus TestADSAsynPort::connect(asynUser *pasynUser)
{
	errlogPrintf("%s:%s: Connecting.\n",
			_portName.c_str(), __FUNCTION__);

	if ( true )
	{
		errlogPrintf("%s:%s: Connection successful.\n", _portName.c_str(), __FUNCTION__);
		ADSAsynPort::connect(pasynUser);
		return asynSuccess;
	}

	return asynError;
}

asynStatus TestADSAsynPort::disconnect(asynUser *pasynUser)
{
	errlogPrintf("%s:%s: Disconnecting.\n", _portName.c_str(), __FUNCTION__);
	asynPortDriver::disconnect(pasynUser);

	return 	asynSuccess;
}

void TestADSAsynPort::readDIStateTask()
{
    /* Loop forever */
    while (!_doneWithThreads)
    {
        // wait till relay out put is settled down.
        epicsThreadSleep(DI_READ_TIMEOUT);
    	counter++;

        if ( pvInt32ListMutex.tryLock() )
        {

            for_each(pvInt32List.begin(),
            		pvInt32List.end(),
            		  boost::bind(&TestADSAsynPort::doCallbacksInt32, this, counter,
            				  boost::bind(&PVInfo::getId, _1), 0, asynSuccess)
                    );

        	pvInt32ListMutex.unlock();
        }
    }
}

void TestADSAsynPort::addPVToIOScan(const std::shared_ptr<ADSPVInfo>& info, const int interfaceMask)
{
	errlogPrintf( "%s:%s: TestADSAsynPort: Interrupt for %s. Reason: %d \n",
			portName.c_str(), __FUNCTION__,
			info->getFullReason().c_str(),
			info->getId() );

	if (interfaceMask == asynInt32Mask)
	{
		// Extract ADS type from PVInfo
		// and
		// register PV for polling data
		// from ADS in type relevant list (?)
		pvInt32List.push_back(info);
	}else if (interfaceMask == asynFloat64Mask)
	{
	}else if (interfaceMask ==  asynOctetMask)
	{
	}else if (interfaceMask == asynInt8ArrayMask)
	{
	}else if (interfaceMask == asynInt16ArrayMask)
	{
	}else if (interfaceMask == asynInt32ArrayMask)
	{
	}else if (interfaceMask == asynFloat32ArrayMask)
	{
	}else if (interfaceMask == asynFloat64ArrayMask)
	{
	}
}


void TestADSAsynPort::onIOScanStart()
{
	pvInt32ListMutex.lock();
	pvInt32List.clear();
}

void TestADSAsynPort::onIOScanFinish()
{
	pvInt32ListMutex.unlock();
}

void TestADSAsynPort::regFunc(const my_func& f)
{
	f(1, 2);
}



/*
 * iocconfig.c
 *
 *  Created on: 18. maj 2017
 *      Author: sisaev
 */

/* Configuration routine.  Called directly, or from the iocsh function below */

#include <iocsh.h>
#include <epicsExport.h>

#include "HWStub.h"

#include "TestADSAsynPort.h"

/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxPoints The maximum  number of points in the volt and time arrays */
int ADSConfigure(const char *portName, const char* deviceDescription, int autoConnect)
{
    new TestADSAsynPort(portName, deviceDescription, autoConnect, new HWStub() );
    return(asynSuccess);
}


/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName", iocshArgString};
static const iocshArg initArg1 = { "deviceDescription", iocshArgString};
static const iocshArg initArg2 = { "autoConnect", iocshArgInt};

static const iocshArg * const initArgs[] = {&initArg0, &initArg1, &initArg2};
static const iocshFuncDef initFuncDef = {"ADSConfigure", 3, initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    ADSConfigure(args[0].sval, args[1].sval, args[2].ival);
}


extern "C"
{

///// IOC specific function to handle initialization signals
//epicsShareFunc void epicsInitHookFunction(initHookState state)
//{
//
//}

void ADSConfigureRegister(void)
{
    iocshRegister(&initFuncDef, initCallFunc);
}

epicsExportRegistrar(ADSConfigureRegister);

}

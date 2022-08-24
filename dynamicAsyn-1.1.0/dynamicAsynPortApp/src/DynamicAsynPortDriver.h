/*
 * SmartAsynPortDriver.h
 *
 *  Created on: 2018-05-10
 *      Author: Viacheslav Isaev
 */

#ifndef SMARTASYNPORTDRIVER_H_
#define SMARTASYNPORTDRIVER_H_


#include <asynPortDriver.h>

#include <errlog.h> /* errlogPrintf */

#include "PVKeeper.h"
#include "AsynReadWriteHandlers.h"

template<typename T>
class DynamicAsynPortDriver : public asynPortDriver
{
public:

	typedef SharedPointerHolder<T> THolder;

	DynamicAsynPortDriver(const char *portName, int maxAddr, int interfaceMask, int interruptMask,
                   int asynFlags, int autoConnect, int priority, int stackSize);

	DynamicAsynPortDriver(const char *portName, int maxAddr, int interfaceMask, int interruptMask,
                   int asynFlags, int autoConnect, int priority, int stackSize, PVKeeper<T> *pvk);

	virtual ~DynamicAsynPortDriver();


    virtual asynStatus drvUserCreate(asynUser *pasynUser, const char *drvInfo,
                                     const char **pptypeName, size_t *psize);

    virtual asynStatus drvUserDestroy(asynUser *pasynUser);

    // Int32
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus readInt32 (asynUser *pasynUser, epicsInt32 *value);

    // Float64
    virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
    virtual asynStatus readFloat64(asynUser *pasynUser, epicsFloat64 *value);

    // UInt32Digital
    virtual asynStatus writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);
    virtual asynStatus readUInt32Digital(asynUser *pasynUser, epicsUInt32 *value, epicsUInt32 mask);

    // Octet
    virtual asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t maxChars,size_t *nActual);
    virtual asynStatus readOctet (asynUser *pasynUser, char *value, size_t maxChars,size_t *nActual, int *eomReason);

    // Arrays
    virtual asynStatus readInt8Array(asynUser *pasynUser, epicsInt8 *value,
                                        size_t nElements, size_t *nIn);
    virtual asynStatus writeInt8Array(asynUser *pasynUser, epicsInt8 *value,
                                        size_t nElements);

    virtual asynStatus writeInt16Array(asynUser *pasynUser, epicsInt16 *value, size_t nelements);
    virtual asynStatus readInt16Array(asynUser *pasynUser,  epicsInt16 *value, size_t nelements, size_t *nIn);

    virtual asynStatus readInt32Array(asynUser *pasynUser, epicsInt32 *value,
                                        size_t nElements, size_t *nIn);
    virtual asynStatus writeInt32Array(asynUser *pasynUser, epicsInt32 *value,
                                        size_t nElements);

    virtual asynStatus readFloat32Array(asynUser *pasynUser, epicsFloat32 *value,
                                        size_t nElements, size_t *nIn);
    virtual asynStatus writeFloat32Array(asynUser *pasynUser, epicsFloat32 *value,
                                        size_t nElements);

    virtual asynStatus readFloat64Array(asynUser *pasynUser, epicsFloat64 *value,
                                        size_t nElements, size_t *nIn);
    virtual asynStatus writeFloat64Array(asynUser *pasynUser, epicsFloat64 *value,
                                        size_t nElements);

    virtual asynStatus connect(asynUser *pasynUser);


protected:
    std::string portName;
    std::unique_ptr<PVKeeper<T>> pvkeeper;


    AsynSimpleTypeHandlerKeeper<T, epicsInt32>   asynInt32Handlers;
    AsynSimpleTypeHandlerKeeper<T, epicsFloat64> asynFloat64Handlers;

    AsynDigitalTypeHandlerKeeper<T, epicsUInt32> asynUInt32DigitalHandlers;

    AsynOctetTypeHandlerKeeper<T, char> asynOctetHandlers;

    AsynArrayTypeHandlerKeeper<T, epicsInt8>    asynArrayInt8Handlers;
    AsynArrayTypeHandlerKeeper<T, epicsInt16>   asynArrayInt16Handlers;
    AsynArrayTypeHandlerKeeper<T, epicsInt32>   asynArrayInt32Handlers;
    AsynArrayTypeHandlerKeeper<T, epicsFloat32> asynArrayFloat32Handlers;
    AsynArrayTypeHandlerKeeper<T, epicsFloat64> asynArrayFloat64Handlers;

    int pvIdListIOScans;
    #define DAP_FIRST_SCOPE_COMMAND pvIdListIOScans
    #define DAP_LAST_SCOPE_COMMAND  pvIdListIOScans

    /**
     * List all PVs with I/O Intr
     */
	void listIOScanPVs();

	template <typename interruptType>
	void listIOScanPVsForType(void *interruptPvt, int interfaceMask);


	/**
	 * @defgroup IOScan The IOScan listing interface.
	 * Functions which provides listing of IO Intr PVs.
	 *
	 * \addtogroup <IOScan>
	 *  @{
	 * Part of the IO Scan listing interface.
	 * It is called when ever new cycle of IOScan listing requested.
	 */
	virtual void onIOScanStart(){}

	/**
	 * Called for each new PV with SCAN set to I/O Intr.
	 * @param info - PV information.
	 * @param interfaceMask - mask of relevant interface, to know the asyn PV type.
	 */
	virtual void addPVToIOScan(const std::shared_ptr<T>& info, const int interfaceMask);

	/**
	 * Part of IO Scan listing  interface.
	 * It is called when listing finished.
	 *
	 */
	virtual void onIOScanFinish(){}
	/**
	 * @}
	 * */

	/**
	 * Does single value asynchronous update.
	 */
    template <typename epicsType, typename interruptType>
    asynStatus doCallbacksOneValue(epicsType value, int reason, int address,
            epicsTimeStamp timestamp, asynStatus status, void *interruptPvt);

    asynStatus doCallbacksUInt32(epicsUInt32 value, int reason, int address,
            epicsTimeStamp timestamp, asynStatus status, void *interruptPvt);

    asynStatus doCallbacksChar(char* value, int reason, int address,
            epicsTimeStamp timestamp, asynStatus status, void *interruptPvt);

    template <typename epicsType, typename interruptType>
    asynStatus doCallbacksArray(epicsType *value, size_t nElements, int reason, int address, epicsTimeStamp timestamp, asynStatus status, void *interruptPvt);

    asynStatus doCallbacksInt32(epicsInt32 value, int reason, int addr, asynStatus status);
    asynStatus doCallbacksInt32(epicsInt32 value, int reason, int addr, asynStatus status, epicsTimeStamp timestamp);

    asynStatus doCallbacksFloat64(epicsFloat64 value, int reason, int addr, asynStatus status);
    asynStatus doCallbacksFloat64(epicsFloat64 value, int reason, int addr, asynStatus status, epicsTimeStamp timestamp);

    asynStatus doCallbacksUInt32Digital(epicsUInt32 value, int reason, int addr, asynStatus status);
    asynStatus doCallbacksUInt32Digital(epicsUInt32 value, int reason, int addr, asynStatus status, epicsTimeStamp timestamp);

    asynStatus doCallbacksOctet(char* value, int reason, int addr, asynStatus status);
    asynStatus doCallbacksOctet(char* value, int reason, int addr, asynStatus status, epicsTimeStamp timestamp);

    asynStatus doCallbacksInt8Array(epicsInt8 *value, size_t nElements, int reason, int addr, asynStatus status);  
    asynStatus doCallbacksInt8Array(epicsInt8 *value, size_t nElements, int reason, int addr, asynStatus status, epicsTimeStamp timestamp);

    asynStatus doCallbacksInt16Array(epicsInt16 *value, size_t nElements, int reason, int addr, asynStatus status);  
    asynStatus doCallbacksInt16Array(epicsInt16 *value, size_t nElements, int reason, int addr, asynStatus status, epicsTimeStamp timestamp);

    asynStatus doCallbacksInt32Array(epicsInt32 *value, size_t nElements, int reason, int addr, asynStatus status);  
    asynStatus doCallbacksInt32Array(epicsInt32 *value, size_t nElements, int reason, int addr, asynStatus status, epicsTimeStamp timestamp);

    asynStatus doCallbacksFloat32Array(epicsFloat32 *value, size_t nElements, int reason, int addr, asynStatus status);  
    asynStatus doCallbacksFloat32Array(epicsFloat32 *value, size_t nElements, int reason, int addr, asynStatus status, epicsTimeStamp timestamp);

    asynStatus doCallbacksFloat64Array(epicsFloat64 *value, size_t nElements, int reason, int addr, asynStatus status);  
    asynStatus doCallbacksFloat64Array(epicsFloat64 *value, size_t nElements, int reason, int addr, asynStatus status, epicsTimeStamp timestamp);
};

#define DAP_NUM_SCOPE_PARAMS (&DAP_LAST_SCOPE_COMMAND - &DAP_FIRST_SCOPE_COMMAND + 1)

template<typename T>
DynamicAsynPortDriver<T>::DynamicAsynPortDriver(const char *pn, int maxAddr, int interfaceMask, int interruptMask,
        int asynFlags, int autoConnect, int priority, int stackSize)
:asynPortDriver(pn, maxAddr, interfaceMask, interruptMask,
        asynFlags, autoConnect, priority, stackSize),
        portName(pn),
        pvkeeper(new PVKeeper<T>())
{
	pvkeeper->setStartCounter(DAP_NUM_SCOPE_PARAMS);
}

template<typename T>
DynamicAsynPortDriver<T>::DynamicAsynPortDriver(const char *pn, int maxAddr, int interfaceMask, int interruptMask,
        int asynFlags, int autoConnect, int priority, int stackSize, PVKeeper<T> *pvk)
:asynPortDriver(pn, maxAddr, interfaceMask, interruptMask,
        asynFlags, autoConnect, priority, stackSize),
        portName(pn),
        pvkeeper(pvk)
{
	pvkeeper->setStartCounter(DAP_NUM_SCOPE_PARAMS);
}

template<typename T>
DynamicAsynPortDriver<T>::~DynamicAsynPortDriver()
{
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::connect(asynUser *pasynUser)
{
	asynPortDriver::connect(pasynUser);

	return asynSuccess;
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::drvUserCreate(asynUser *pasynUser, const char *reason, const char **pptypeName, size_t *psize)
{
    asynStatus status;
    int index;
    int addr;

    try
    {
        status = getAddress(pasynUser, &addr);
        if (status != asynSuccess) return(status);

        asynPrint(pasynUser, ASYN_TRACE_FLOW,
        		"%s:%s: drvInfo=%s",
                portName.c_str(), __FUNCTION__, reason);

        status = this->findParam(addr, reason, &index);
        if (status)
        {
            auto info = pvkeeper->registerPV (reason);

            pasynUser->reason = info->getId();

            //Durty
            auto *h = new SharedPointerHolder<T>(info);
            pasynUser->userData = (void*) (h);

            asynPrint(pasynUser, ASYN_TRACE_FLOW,
            		"%s:%s: drvInfo=%s, index=%d\n",
                    portName.c_str(), __FUNCTION__, reason, pasynUser->reason);

            return(asynSuccess);
        }else
        {
        	pasynUser->reason = index;
//        	errlogPrintf(
//                  "%s:%s: drvInfo=%s, index=%d\n",
//                  portName.c_str(), __FUNCTION__, reason, index);
            return(asynSuccess);
        }
    }
    catch(...)
    {
    	errlogPrintf(
    	"%s:%s: drvInfo=%s Throws execption!\n",
    		portName.c_str(), __FUNCTION__, reason);
    }
	return asynError;
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::drvUserDestroy(asynUser *pasynUser)
{

	if (pasynUser->userData != 0)
	{
		SharedPointerHolder<T>* i = (THolder*)pasynUser->userData;
				errlogPrintf(
						"%s: drvInfo=%s\n",
						__FUNCTION__, i->getPointer()->getFullReason().c_str());
	}

	if (pasynUser->userData != 0)
		delete((SharedPointerHolder<T>*)pasynUser->userData);

	asynPortDriver::drvUserDestroy(pasynUser);

	return asynSuccess;
}

template<typename T>
void DynamicAsynPortDriver<T>::listIOScanPVs()
{
	onIOScanStart();

	listIOScanPVsForType<asynInt32Interrupt>(this->asynStdInterfaces.int32InterruptPvt, asynInt32Mask);
	listIOScanPVsForType<asynFloat64Interrupt>(this->asynStdInterfaces.float64InterruptPvt, asynFloat64Mask);

        listIOScanPVsForType<asynUInt32DigitalInterrupt>(this->asynStdInterfaces.uInt32DigitalInterruptPvt, asynUInt32DigitalMask);

        listIOScanPVsForType<asynOctetInterrupt>(this->asynStdInterfaces.octetInterruptPvt, asynOctetMask);

	listIOScanPVsForType<asynInt8ArrayInterrupt>(this->asynStdInterfaces.int8ArrayInterruptPvt, asynInt8ArrayMask);
	listIOScanPVsForType<asynInt16ArrayInterrupt>(this->asynStdInterfaces.int16ArrayInterruptPvt, asynInt16ArrayMask);
	listIOScanPVsForType<asynInt32ArrayInterrupt>(this->asynStdInterfaces.int32ArrayInterruptPvt, asynInt32ArrayMask);
	listIOScanPVsForType<asynFloat32ArrayInterrupt>(this->asynStdInterfaces.float32ArrayInterruptPvt, asynFloat32ArrayMask);
	listIOScanPVsForType<asynFloat64ArrayInterrupt>(this->asynStdInterfaces.float64ArrayInterruptPvt, asynFloat64ArrayMask);

	onIOScanFinish();
}

template<typename T>
template <typename interruptType>
void DynamicAsynPortDriver<T>::listIOScanPVsForType(void *interruptPvt, int interfaceMask)
{
    ELLLIST *pclientList;
    interruptNode *pnode;

    pasynManager->interruptStart(interruptPvt, &pclientList);
    pnode = (interruptNode *)ellFirst(pclientList);
    while (pnode) {
        interruptType *pInterrupt = (interruptType *)pnode->drvPvt;

        auto pinfo = (THolder*)pInterrupt->pasynUser->userData;

    	addPVToIOScan(pinfo->getPointer(), interfaceMask);

        pnode = (interruptNode *)ellNext(&pnode->node);
    }
    pasynManager->interruptEnd(interruptPvt);
}

template<typename T>
void DynamicAsynPortDriver<T>::addPVToIOScan(const std::shared_ptr<T>& info, int interfaceMask)
{
	errlogPrintf( "%s:%s: Interrupt for %s. Reason: %d \n",
			portName.c_str(), __FUNCTION__,
			info->getFullReason().c_str(),
			info->getId());
}

// Additional interrupt dispatchers
template<typename T>
template <typename epicsType, typename interruptType>
asynStatus DynamicAsynPortDriver<T>::doCallbacksOneValue(epicsType value,
	int reason,
	int address,
    epicsTimeStamp timestamp,
	asynStatus status,
	void *interruptPvt)
{
    ELLLIST *pclientList;
    interruptNode *pnode;
    int addr;

    pasynManager->interruptStart(interruptPvt, &pclientList);
    pnode = (interruptNode *)ellFirst(pclientList);
    while (pnode) {
        interruptType *pInterrupt = (interruptType *)pnode->drvPvt;
        pasynManager->getAddr(pInterrupt->pasynUser, &addr);
        /* If this is not a multi-device then address is -1, change to 0 */
        if (addr == -1) addr = 0;

        if ((pInterrupt->pasynUser->reason == reason) &&
            (address == addr)) {
            pInterrupt->pasynUser->timestamp = timestamp;
            pInterrupt->pasynUser->auxStatus = (asynStatus)status;

            pInterrupt->callback(pInterrupt->userPvt,
                                 pInterrupt->pasynUser,
                                 value);
        }
        pnode = (interruptNode *)ellNext(&pnode->node);
    }
    pasynManager->interruptEnd(interruptPvt);
    return(asynSuccess);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksUInt32(epicsUInt32 value,
	int reason,
	int address,
    epicsTimeStamp timestamp,
	asynStatus status,
	void *interruptPvt)
{
    ELLLIST *pclientList;
    interruptNode *pnode;
    int addr;

    pasynManager->interruptStart(interruptPvt, &pclientList);
    pnode = (interruptNode *)ellFirst(pclientList);
    while (pnode) {
        asynUInt32DigitalInterrupt *pInterrupt = (asynUInt32DigitalInterrupt *)pnode->drvPvt;
        pasynManager->getAddr(pInterrupt->pasynUser, &addr);
        /* If this is not a multi-device then address is -1, change to 0 */
        if (addr == -1) addr = 0;

        if ((pInterrupt->pasynUser->reason == reason) &&
            (address == addr)) {
            pInterrupt->pasynUser->timestamp = timestamp;
            pInterrupt->pasynUser->auxStatus = (asynStatus)status;

            pInterrupt->callback(pInterrupt->userPvt,
                                 pInterrupt->pasynUser,
                                 pInterrupt->mask & value);
        }
        pnode = (interruptNode *)ellNext(&pnode->node);
    }
    pasynManager->interruptEnd(interruptPvt);
    return(asynSuccess);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksChar(char* value,
	int reason,
	int address,
    epicsTimeStamp timestamp,
	asynStatus status,
	void *interruptPvt)
{
    ELLLIST *pclientList;
    interruptNode *pnode;
    int addr;

    pasynManager->interruptStart(interruptPvt, &pclientList);
    pnode = (interruptNode *)ellFirst(pclientList);
    while (pnode) {
        asynOctetInterrupt *pInterrupt = (asynOctetInterrupt *)pnode->drvPvt;
        pasynManager->getAddr(pInterrupt->pasynUser, &addr);
        /* If this is not a multi-device then address is -1, change to 0 */
        if (addr == -1) addr = 0;

        if ((pInterrupt->pasynUser->reason == reason) &&
            (address == addr)) {
            pInterrupt->pasynUser->timestamp = timestamp;
            pInterrupt->pasynUser->auxStatus = (asynStatus)status;
    
            pInterrupt->callback(pInterrupt->userPvt,
                                 pInterrupt->pasynUser,
                                 value, strlen(value)+1, ASYN_EOM_END);
        }
        pnode = (interruptNode *)ellNext(&pnode->node);
    }
    pasynManager->interruptEnd(interruptPvt);
    return(asynSuccess);
}

template<typename T>
template <typename epicsType, typename interruptType>
asynStatus DynamicAsynPortDriver<T>::doCallbacksArray(epicsType *value,
        size_t nElements,
	int reason,
	int address,
    epicsTimeStamp timestamp,
	asynStatus status,
	void *interruptPvt)
{
    ELLLIST *pclientList;
    interruptNode *pnode;
    int addr;

    pasynManager->interruptStart(interruptPvt, &pclientList);
    pnode = (interruptNode *)ellFirst(pclientList);
    while (pnode) {
        interruptType *pInterrupt = (interruptType *)pnode->drvPvt;
        pasynManager->getAddr(pInterrupt->pasynUser, &addr);
        /* If this is not a multi-device then address is -1, change to 0 */
        if (addr == -1) addr = 0;

        if ((pInterrupt->pasynUser->reason == reason) &&
            (address == addr)) {
            pInterrupt->pasynUser->timestamp = timestamp;
            pInterrupt->pasynUser->auxStatus = (asynStatus)status;

            pInterrupt->callback(pInterrupt->userPvt,
                                 pInterrupt->pasynUser,
                                 value, nElements);
        }
        pnode = (interruptNode *)ellNext(&pnode->node);
    }
    pasynManager->interruptEnd(interruptPvt);
    return(asynSuccess);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksInt32(epicsInt32 value, int reason, int addr, asynStatus status)
{
	epicsTime time;
	return doCallbacksOneValue<epicsInt32, asynInt32Interrupt>(value, reason, addr, time, status,
			this->asynStdInterfaces.int32InterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksInt32(epicsInt32 value, int reason, int addr, asynStatus status, epicsTimeStamp timestamp)
{
	return doCallbacksOneValue<epicsInt32, asynInt32Interrupt>(value, reason, addr, timestamp, status,
			this->asynStdInterfaces.int32InterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksFloat64(epicsFloat64 value, int reason, int addr, asynStatus status)
{
	epicsTime time;
	return doCallbacksOneValue<epicsFloat64, asynFloat64Interrupt>(value, reason, addr, time, status,
			this->asynStdInterfaces.float64InterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksFloat64(epicsFloat64 value, int reason, int addr, asynStatus status, epicsTimeStamp timestamp)
{
	return doCallbacksOneValue<epicsFloat64, asynFloat64Interrupt>(value, reason, addr, timestamp, status,
			this->asynStdInterfaces.float64InterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksUInt32Digital(epicsUInt32 value, int reason, int addr, asynStatus status)
{
	epicsTime time;
	return doCallbacksUInt32(value, reason, addr, time, status,
			this->asynStdInterfaces.uInt32DigitalInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksUInt32Digital(epicsUInt32 value, int reason, int addr, asynStatus status, epicsTimeStamp timestamp)
{
	return doCallbacksUInt32(value, reason, addr, timestamp, status,
			this->asynStdInterfaces.uInt32DigitalInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksOctet(char *value, int reason, int addr, asynStatus status)
{
	epicsTime time;
	return doCallbacksChar(value, reason, addr, time, status,
			this->asynStdInterfaces.octetInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksOctet(char *value, int reason, int addr, asynStatus status, epicsTimeStamp timestamp)
{
	return doCallbacksChar(value, reason, addr, timestamp, status,
			this->asynStdInterfaces.octetInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksInt8Array(epicsInt8 *value, size_t nElements, int reason, int addr, asynStatus status)
{
	epicsTime time;
	return doCallbacksArray<epicsInt8, asynInt8ArrayInterrupt>(value, nElements, reason, addr, time, status, this->asynStdInterfaces.int8ArrayInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksInt8Array(epicsInt8 *value, size_t nElements, int reason, int addr, asynStatus status, epicsTimeStamp timestamp)
{
	return doCallbacksArray<epicsInt8, asynInt8ArrayInterrupt>(value, nElements, reason, addr, timestamp, status, this->asynStdInterfaces.int8ArrayInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksInt16Array(epicsInt16 *value, size_t nElements, int reason, int addr, asynStatus status)
{
	epicsTime time;
	return doCallbacksArray<epicsInt16, asynInt16ArrayInterrupt>(value, nElements, reason, addr, time, status, this->asynStdInterfaces.int16ArrayInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksInt16Array(epicsInt16 *value, size_t nElements, int reason, int addr, asynStatus status, epicsTimeStamp timestamp)
{
	return doCallbacksArray<epicsInt16, asynInt16ArrayInterrupt>(value, nElements, reason, addr, timestamp, status, this->asynStdInterfaces.int16ArrayInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksInt32Array(epicsInt32 *value, size_t nElements, int reason, int addr, asynStatus status)
{
	epicsTime time;
	return doCallbacksArray<epicsInt32, asynInt32ArrayInterrupt>(value, nElements, reason, addr, time, status, this->asynStdInterfaces.int32ArrayInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksInt32Array(epicsInt32 *value, size_t nElements, int reason, int addr, asynStatus status, epicsTimeStamp timestamp)
{
	return doCallbacksArray<epicsInt32, asynInt32ArrayInterrupt>(value, nElements, reason, addr, timestamp, status, this->asynStdInterfaces.int32ArrayInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksFloat32Array(epicsFloat32 *value, size_t nElements, int reason, int addr, asynStatus status)
{
	epicsTime time;
	return doCallbacksArray<epicsFloat32, asynFloat32ArrayInterrupt>(value, nElements, reason, addr, time, status, this->asynStdInterfaces.float32ArrayInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksFloat32Array(epicsFloat32 *value, size_t nElements, int reason, int addr, asynStatus status, epicsTimeStamp timestamp)
{
	return doCallbacksArray<epicsFloat32, asynFloat32ArrayInterrupt>(value, nElements, reason, addr, timestamp, status, this->asynStdInterfaces.float32ArrayInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksFloat64Array(epicsFloat64 *value, size_t nElements, int reason, int addr, asynStatus status)
{
	epicsTime time;
	return doCallbacksArray<epicsFloat64, asynFloat64ArrayInterrupt>(value, nElements, reason, addr, time, status, this->asynStdInterfaces.float64ArrayInterruptPvt);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::doCallbacksFloat64Array(epicsFloat64 *value, size_t nElements, int reason, int addr, asynStatus status, epicsTimeStamp timestamp)
{
	return doCallbacksArray<epicsFloat64, asynFloat64ArrayInterrupt>(value, nElements, reason, addr, timestamp, status, this->asynStdInterfaces.float64ArrayInterruptPvt);
}

// Int32
template<typename T>
asynStatus DynamicAsynPortDriver<T>::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    const int function = pasynUser->reason;
	asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
		portName.c_str(), __FUNCTION__, function);


    if (function == pvIdListIOScans)
    {
    	asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: ScanIO\n",
    		portName.c_str(), __FUNCTION__);

    	listIOScanPVs();

    	return asynSuccess;
    }else
    {
    	if (!pasynUser->userData) return asynError;
    	auto *info = (THolder*) pasynUser->userData;
    	return asynInt32Handlers.invokeWrite(info->getPointer(), value);
    }
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::readInt32 (asynUser *pasynUser, epicsInt32 *value)
{
	if (!pasynUser->userData) return asynError;
    const int function = pasynUser->reason;
	asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
		portName.c_str(), __FUNCTION__, function);

    auto *info = (THolder*) pasynUser->userData;

	return asynInt32Handlers.invokeRead(info->getPointer(), value);
}

// Float64
template<typename T>
asynStatus DynamicAsynPortDriver<T>::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
	if (!pasynUser->userData) return asynError;
	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);

	return asynFloat64Handlers.invokeWrite(info->getPointer(), value);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
{
	if (!pasynUser->userData) return asynError;

	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);

	return asynFloat64Handlers.invokeRead(info->getPointer(), value);
}

// UInt32Digital
template<typename T>
asynStatus DynamicAsynPortDriver<T>::writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask)
{
	if (!pasynUser->userData) return asynError;
	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);

	return asynUInt32DigitalHandlers.invokeWrite(info->getPointer(), value, mask);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::readUInt32Digital(asynUser *pasynUser, epicsUInt32 *value, epicsUInt32 mask)
{
	if (!pasynUser->userData) return asynError;
    const int function = pasynUser->reason;
	asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
		portName.c_str(), __FUNCTION__, function);

    auto *info = (THolder*) pasynUser->userData;

	return asynUInt32DigitalHandlers.invokeRead(info->getPointer(), value, mask);
}

// Octet
template<typename T>
asynStatus DynamicAsynPortDriver<T>::writeOctet(asynUser *pasynUser, const char *value, size_t maxChars,size_t *nActual)
{
	if (!pasynUser->userData) return asynError;
    const int function = pasynUser->reason;
	asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
		portName.c_str(), __FUNCTION__, function);

    auto *info = (THolder*) pasynUser->userData;

	return asynOctetHandlers.invokeWrite(info->getPointer(), value, maxChars, nActual);    
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::readOctet(asynUser *pasynUser, char *value, size_t maxChars,size_t *nActual, int *eomReason)
{
	if (!pasynUser->userData) return asynError;
    const int function = pasynUser->reason;
	asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
		portName.c_str(), __FUNCTION__, function);

    auto *info = (THolder*) pasynUser->userData;

	return asynOctetHandlers.invokeRead(info->getPointer(), value, maxChars, nActual, eomReason);
}

// Arrays
template<typename T>
asynStatus DynamicAsynPortDriver<T>::readInt8Array(asynUser *pasynUser, epicsInt8 *value,
                                    size_t nElements, size_t *nIn)
{
	if (!pasynUser->userData) return asynError;
	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);

	return asynArrayInt8Handlers.invokeRead(info->getPointer(), value, nElements, nIn);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::writeInt8Array(asynUser *pasynUser, epicsInt8 *value,
                                    size_t nElements)
{
	if (!pasynUser->userData) return asynError;
	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);
	return asynArrayInt8Handlers.invokeWrite(info->getPointer(), value, nElements);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::writeInt16Array(asynUser *pasynUser, epicsInt16 *value, size_t nElements)
{
	if (!pasynUser->userData) return asynError;
	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);

	return asynArrayInt16Handlers.invokeWrite(info->getPointer(), value, nElements);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::readInt16Array(asynUser *pasynUser,  epicsInt16 *value, size_t nElements, size_t *nIn)
{
	if (!pasynUser->userData) return asynError;
	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);

	return asynArrayInt16Handlers.invokeRead(info->getPointer(), value, nElements, nIn);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::readInt32Array(asynUser *pasynUser, epicsInt32 *value,
                                    size_t nElements, size_t *nIn)
{
	if (!pasynUser->userData) return asynError;
	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);

	return asynArrayInt32Handlers.invokeRead(info->getPointer(), value, nElements, nIn);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::writeInt32Array(asynUser *pasynUser, epicsInt32 *value,
                                    size_t nElements)
{
	if (!pasynUser->userData) return asynError;
	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);

	return asynArrayInt32Handlers.invokeWrite(info->getPointer(), value, nElements);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::readFloat32Array(asynUser *pasynUser, epicsFloat32 *value,
                                    size_t nElements, size_t *nIn)
{
	if (!pasynUser->userData) return asynError;
	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);

	return asynArrayFloat32Handlers.invokeRead(info->getPointer(), value, nElements, nIn);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::writeFloat32Array(asynUser *pasynUser, epicsFloat32 *value,
                                    size_t nElements)
{
	if (!pasynUser->userData) return asynError;
	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);

	return asynArrayFloat32Handlers.invokeWrite( info->getPointer(), value, nElements);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::readFloat64Array(asynUser *pasynUser, epicsFloat64 *value,
                                    size_t nElements, size_t *nIn)
{
	if (!pasynUser->userData) return asynError;
	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);

	return asynArrayFloat64Handlers.invokeRead(info->getPointer(), value, nElements, nIn);
}

template<typename T>
asynStatus DynamicAsynPortDriver<T>::writeFloat64Array(asynUser *pasynUser, epicsFloat64 *value,
                                    size_t nElements)
{
	if (!pasynUser->userData) return asynError;
	auto *info = (THolder*) pasynUser->userData;

    const int reason = info->getPointer()->getId();

    asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s:%s: Reason: %d\n",
			portName.c_str(), __FUNCTION__, reason);

	return asynArrayFloat64Handlers.invokeWrite(info->getPointer(), value, nElements);
}

#endif /* SMARTASYNPORTDRIVER_H_ */

/*
 * ADSAsynPort.h
 *
 *  Created on: 2018-05-14
 *      Author: Viacheslav Isaev
 */

#ifndef ADSASYNPORT_H_
#define ADSASYNPORT_H_

#include "HWInterface.h"

#include "DynamicAsynPortDriver.h"

#include "ADSPVInfo.h"
/**
 *  Possible implementation of ADS asynPort.
 *  It provides registration of all possible type handlers for PLC types.
 *
 *	Class has a dependency injection HWInterface.
 *
 */

class ADSAsynPort : public DynamicAsynPortDriver<ADSPVInfo>
{
public:
	ADSAsynPort(const char *portName, int maxAddr, int paramTableSize, int interfaceMask, int interruptMask,
            int asynFlags, int autoConnect, int priority, int stackSize, HWInterface *);

	virtual ~ADSAsynPort();

protected:
	HWInterface *adsInterface;

	//FunctionsKeeper<HWInterface> keeper;

	void registerADSHandlers();

};

#endif /* ADSASYNPORT_H_ */

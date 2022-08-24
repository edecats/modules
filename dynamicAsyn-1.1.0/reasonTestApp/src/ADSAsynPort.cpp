/*
 * ADSAsynPort.cpp
 *
 *  Created on: 14. maj 2018
 *      Author: sisaev
 */

#include <boost/bind.hpp>

#include "HWInterface.h"
#include "HWStub.h"

#include "ADSAsynPort.h"


ADSAsynPort::ADSAsynPort(const char *portName, int maxAddr, int paramTableSize, int interfaceMask, int interruptMask,
        int asynFlags, int autoConnect, int priority, int stackSize, HWInterface *hw):
   DynamicAsynPortDriver(portName, maxAddr, interfaceMask, interruptMask,
                asynFlags, autoConnect, priority, stackSize,
				new PVKeeper<ADSPVInfo>( [](const ADSPVInfoConstPtr& i)-> std::string {return (i->getArgsNum()>0)?i->getStrArg(0):"???";} )
   ),
   adsInterface(hw)
{
	registerADSHandlers();
}

ADSAsynPort::~ADSAsynPort()
{

}

void ADSAsynPort::registerADSHandlers()
{
	//	BOOL	8
//	asynInt32Handlers.insert("BOOL",
//			&HWInterface::readInt8
//			);

//	keeper.setOwner(adsInterface);
//	keeper.insert("BOOL",
//			&HWInterface::writeInt8, adsInterface);

//	asynHandlers.reg("BOOL",
//			&HWInterface::readInt8,
//			adsInterface );
//
//	asynHandlers.reg("BOOL",
//			&HWInterface::writeInt8,
//			adsInterface );


//	asynInt32Handlers.reg("BOOL",
//			&HWInterface::readInt8,
//			&HWInterface::writeInt8,
//			adsInterface );

    asynUInt32DigitalHandlers.reg("BOOL",
        &HWInterface::readBool,
        &HWInterface::writeBool, adsInterface);

	//	BYTE	8
	asynInt32Handlers.reg("BYTE",
		&HWInterface::readInt8,
		&HWInterface::writeInt8, adsInterface);

	//	WORD	16
	asynInt32Handlers.reg("WORD",
		&HWInterface::readInt16,
		&HWInterface::writeInt16, adsInterface);

	//	DWORD	32
	asynInt32Handlers.reg("DWORD",
		&HWInterface::readInt32,
		&HWInterface::writeInt32, adsInterface);

	//	SINT	8
	asynInt32Handlers.reg("SINT",
		&HWInterface::readInt8,
		&HWInterface::writeInt8, adsInterface);

	//	USINT	8
	asynInt32Handlers.reg("USINT",
		&HWInterface::readInt8,
		&HWInterface::writeInt8, adsInterface);

	//	INT	16
	asynInt32Handlers.reg("INT16",
		&HWInterface::readInt16,
		&HWInterface::writeInt16, adsInterface);

	//	UINT16	16
	asynInt32Handlers.reg("UINT16",
		&HWInterface::readInt16,
		&HWInterface::writeInt16, adsInterface);

	//	DINT	32
	asynInt32Handlers.reg("INT32",
		&HWInterface::readInt32,
		&HWInterface::writeInt32, adsInterface);

	asynInt32Handlers.reg("DINT",
		&HWInterface::readInt32,
		&HWInterface::writeInt32, adsInterface);

	//	UDINT	32
	asynInt32Handlers.reg("UDINT",
		&HWInterface::readInt32,
		&HWInterface::writeInt32, adsInterface);

	//	LINT	64
	asynInt32Handlers.reg("LINT",
		&HWInterface::readInt64,
		&HWInterface::writeInt64, adsInterface);

	//	ULINT	64
	asynInt32Handlers.reg("UDINT",
		&HWInterface::readInt64,
		&HWInterface::writeInt64, adsInterface);

	//	TIME	32
	asynInt32Handlers.reg("TIME",
		&HWInterface::readInt32,
		&HWInterface::writeInt32, adsInterface);

	//	TIME_OF_DAY	32
	asynInt32Handlers.reg("TIME_OF_DAY",
		&HWInterface::readInt32,
		&HWInterface::writeInt32, adsInterface);

	//	DATE	32
	asynInt32Handlers.reg("DATE",
		&HWInterface::readInt32,
		&HWInterface::writeInt32, adsInterface);

	//	DATE_AND_TIME	32
	asynInt32Handlers.reg("DATE_AND_TIME",
		&HWInterface::readInt32,
		&HWInterface::writeInt32, adsInterface);

	//	POINTER	32
	asynInt32Handlers.reg("POINTER",
		&HWInterface::readInt32,
		&HWInterface::writeInt32, adsInterface);

	//	ENUM	16
	asynInt32Handlers.reg("ENUM",
		&HWInterface::readInt32,
		&HWInterface::writeInt32, adsInterface);

	//	REAL	32
	asynFloat64Handlers.reg("REAL",
		&HWInterface::readFloat32,
		&HWInterface::writeFloat32, adsInterface);

	//	LREAL	64
	asynFloat64Handlers.reg("LREAL",
		&HWInterface::readFloat64,
		&HWInterface::writeFloat64, adsInterface);

	asynArrayInt8Handlers.reg("INT8ARR",
			&HWInterface::readInt8Array,
			&HWInterface::writeInt8Array, adsInterface);

	asynArrayInt16Handlers.reg("INT16ARR",
			&HWInterface::readInt16Array,
			&HWInterface::writeInt16Array, adsInterface);

	asynArrayInt32Handlers.reg("INT32ARR",
			&HWInterface::readInt32Array,
			&HWInterface::writeInt32Array, adsInterface);

	asynArrayFloat32Handlers.reg("REALARR",
			&HWInterface::readFloat32Array,
			&HWInterface::writeFloat32Array, adsInterface);

	asynArrayFloat64Handlers.reg("LREALARR",
			&HWInterface::readFloat64Array,
			&HWInterface::writeFloat64Array, adsInterface);
}

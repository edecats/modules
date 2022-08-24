/*
 * ADSSim.cpp
 *
 *  Created on: 14. maj 2018
 *      Author: sisaev
 */

#include <errlog.h> /* errlogPrintf */

#include "HWStub.h"

HWStub::HWStub():
	_portName("HWStub")
{
}

HWStub::~HWStub()
{
}

asynStatus HWStub::writeBool(const ADSPVInfoConstPtr &info, epicsUInt32 value, epicsUInt32 mask)
{
	errlogPrintf("%s:%s: Reason:%s val: %d\n",  _portName.c_str(), __FUNCTION__, info->getFullReason().c_str(), value);

	return asynSuccess;
}

asynStatus HWStub::readBool(const ADSPVInfoConstPtr &info, epicsUInt32 *value, epicsUInt32 mask)
{
	errlogPrintf("%s:%s: Reason:%s val: %d\n",  _portName.c_str(), __FUNCTION__, info->getFullReason().c_str(), *value);

	return asynSuccess;
}

asynStatus HWStub::writeInt8(const ADSPVInfoConstPtr &info, epicsInt32 value)
{
	errlogPrintf("%s:%s: Reason:%s val: %d\n",  _portName.c_str(), __FUNCTION__, info->getFullReason().c_str(), value);

	return asynSuccess;
}

asynStatus HWStub::readInt8(const ADSPVInfoConstPtr &info, epicsInt32 *value)
{
	errlogPrintf("%s:%s: Reason:%s \n",  _portName.c_str(), __FUNCTION__, info->getFullReason().c_str());
	return asynSuccess;
}

asynStatus HWStub::writeInt16(const ADSPVInfoConstPtr &info, epicsInt32 value)
{
	errlogPrintf("%s:%s: Reason:%s val: %d\n",  _portName.c_str(), __FUNCTION__, info->getFullReason().c_str(), value);

	return asynSuccess;
}

asynStatus HWStub::readInt16(const ADSPVInfoConstPtr &info, epicsInt32 *value)
{
	errlogPrintf("%s:%s: Reason:%s \n",  _portName.c_str(), __FUNCTION__, info->getFullReason().c_str());
	return asynSuccess;
}

// Int32
asynStatus HWStub::writeInt32(const ADSPVInfoConstPtr &info, epicsInt32 value)
{
	errlogPrintf("%s:%s: Reason:%s val: %d\n",  _portName.c_str(), __FUNCTION__, info->getFullReason().c_str(), value);
	return asynSuccess;
}

asynStatus HWStub::readInt32 (const ADSPVInfoConstPtr &info, epicsInt32 *value)
{
	errlogPrintf("%s:%s: Reason:%s \n",  _portName.c_str(), __FUNCTION__, info->getFullReason().c_str() );
	return asynSuccess;
}

asynStatus HWStub::writeInt64(const ADSPVInfoConstPtr &, epicsInt32 value)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}

asynStatus HWStub::readInt64(const ADSPVInfoConstPtr &, epicsInt32 *value)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}

asynStatus HWStub::writeFloat32(const ADSPVInfoConstPtr &, epicsFloat64 value)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}

asynStatus HWStub::readFloat32(const ADSPVInfoConstPtr &, epicsFloat64 *value)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}


asynStatus HWStub::writeFloat64(const ADSPVInfoConstPtr &info, epicsFloat64 value)
{
	errlogPrintf("%s:%s: %s %f \n",  _portName.c_str(), __FUNCTION__, info->getFullReason().c_str(), value);
	return asynSuccess;
}

asynStatus HWStub::readFloat64(const ADSPVInfoConstPtr &, epicsFloat64 *value)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}

asynStatus HWStub::readInt8Array(const ADSPVInfoConstPtr &, epicsInt8 *value,
                            size_t nElements, size_t *nIn)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}

asynStatus HWStub::writeInt8Array(const ADSPVInfoConstPtr &, epicsInt8 *value,
                            size_t nElements)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}

asynStatus HWStub::writeInt16Array(const ADSPVInfoConstPtr &, epicsInt16 *value, size_t nelements)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}
asynStatus HWStub::readInt16Array(const ADSPVInfoConstPtr &,  epicsInt16 *value, size_t nelements, size_t *nIn)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}

asynStatus HWStub::readInt32Array(const ADSPVInfoConstPtr &, epicsInt32 *value,
                            size_t nElements, size_t *nIn)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}
asynStatus HWStub::writeInt32Array(const ADSPVInfoConstPtr &, epicsInt32 *value,
                            size_t nElements)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}

asynStatus HWStub::readFloat32Array(const ADSPVInfoConstPtr &, epicsFloat32 *value,
                            size_t nElements, size_t *nIn)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}
asynStatus HWStub::writeFloat32Array(const ADSPVInfoConstPtr &, epicsFloat32 *value,
                            size_t nElements)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}

asynStatus HWStub::readFloat64Array(const ADSPVInfoConstPtr &, epicsFloat64 *value,
                            size_t nElements, size_t *nIn)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}
asynStatus HWStub::writeFloat64Array(const ADSPVInfoConstPtr &, epicsFloat64 *value,
                            size_t nElements)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}

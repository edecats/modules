/*
 * HWBase.cpp
 *
 *  Created on: 14. maj 2018
 *      Author: sisaev
 */

#include <errlog.h> /* errlogPrintf */

#include "HWBase.h"

HWBase::HWBase():
 _portName("HWBase")
{

}

HWBase::~HWBase()
{
}

asynStatus HWBase::writeInt8(const ADSPVInfoConstPtr &info, epicsInt32 value)
{
	errlogPrintf("%s:%s: Not implemented! Reason: %s\n",  _portName.c_str(), __FUNCTION__, info->getFullReason().c_str());
	return asynError;
}

asynStatus HWBase::readInt8(const ADSPVInfoConstPtr &, epicsInt32 *value)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::writeInt16(const ADSPVInfoConstPtr &, epicsInt32 value)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::readInt16(const ADSPVInfoConstPtr &, epicsInt32 *value)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

// Int32
asynStatus HWBase::writeInt32(const ADSPVInfoConstPtr &, epicsInt32 value)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::readInt32 (const ADSPVInfoConstPtr &, epicsInt32 *value)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::writeInt64(const ADSPVInfoConstPtr &, epicsInt32 value)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::readInt64(const ADSPVInfoConstPtr &, epicsInt32 *value)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::writeFloat32(const ADSPVInfoConstPtr &, epicsFloat64 value)
{
	errlogPrintf("%s:%s: \n",  _portName.c_str(), __FUNCTION__);
	return asynSuccess;
}

asynStatus HWBase::readFloat32(const ADSPVInfoConstPtr &, epicsFloat64 *value)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::writeFloat64(const ADSPVInfoConstPtr &, epicsFloat64 value)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::readFloat64(const ADSPVInfoConstPtr &, epicsFloat64 *value)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::readInt8Array(const ADSPVInfoConstPtr &, epicsInt8 *value,
                            size_t nElements, size_t *nIn)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::writeInt8Array(const ADSPVInfoConstPtr &, epicsInt8 *value,
                            size_t nElements)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::writeInt16Array(const ADSPVInfoConstPtr &, epicsInt16 *value, size_t nelements)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}
asynStatus HWBase::readInt16Array(const ADSPVInfoConstPtr &,  epicsInt16 *value, size_t nelements, size_t *nIn)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::readInt32Array(const ADSPVInfoConstPtr &, epicsInt32 *value,
                            size_t nElements, size_t *nIn)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}
asynStatus HWBase::writeInt32Array(const ADSPVInfoConstPtr &, epicsInt32 *value,
                            size_t nElements)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::readFloat32Array(const ADSPVInfoConstPtr &, epicsFloat32 *value,
                            size_t nElements, size_t *nIn)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}
asynStatus HWBase::writeFloat32Array(const ADSPVInfoConstPtr &, epicsFloat32 *value,
                            size_t nElements)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

asynStatus HWBase::readFloat64Array(const ADSPVInfoConstPtr &, epicsFloat64 *value,
                            size_t nElements, size_t *nIn)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}
asynStatus HWBase::writeFloat64Array(const ADSPVInfoConstPtr &, epicsFloat64 *value,
                            size_t nElements)
{
	errlogPrintf("%s:%s: Not implemented!\n",  _portName.c_str(), __FUNCTION__);
	return asynError;
}

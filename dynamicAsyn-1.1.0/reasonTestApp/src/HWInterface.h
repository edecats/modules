/*
 * ADSInterface.h
 *
 *  Created on: 14. maj 2018
 *      Author: sisaev
 */

#ifndef ADSINTERFACE_H_
#define ADSINTERFACE_H_

#include <asynPortDriver.h>

#include "ADSPVInfo.h"

/**
 *  Interfaec describing possible implementation of communication layer with ADS.
 *  Goal is to define interface and have ability to separate simulation and different HW implementation later on.
 */
class HWInterface
{
public:
	HWInterface(){}
	virtual ~HWInterface(){}

    virtual asynStatus writeBool(const ADSPVInfoConstPtr &, epicsUInt32 value, epicsUInt32 mask)=0;
    virtual asynStatus readBool (const ADSPVInfoConstPtr &, epicsUInt32 *value, epicsUInt32 mask)=0;

    virtual asynStatus writeInt8(const ADSPVInfoConstPtr &, epicsInt32 value)=0;
    virtual asynStatus readInt8 (const ADSPVInfoConstPtr &, epicsInt32 *value)=0;

    virtual asynStatus writeInt16(const ADSPVInfoConstPtr &, epicsInt32 value)=0;
    virtual asynStatus readInt16 (const ADSPVInfoConstPtr &, epicsInt32 *value)=0;

    // Int32
    virtual asynStatus writeInt32(const ADSPVInfoConstPtr &, epicsInt32 value)=0;
    virtual asynStatus readInt32 (const ADSPVInfoConstPtr &, epicsInt32 *value)=0;

    virtual asynStatus writeInt64(const ADSPVInfoConstPtr &, epicsInt32 value)=0;
    virtual asynStatus readInt64 (const ADSPVInfoConstPtr &, epicsInt32 *value)=0;

    virtual asynStatus writeFloat32(const ADSPVInfoConstPtr &, epicsFloat64 value)=0;
    virtual asynStatus readFloat32 (const ADSPVInfoConstPtr &, epicsFloat64 *value)=0;

    virtual asynStatus writeFloat64(const ADSPVInfoConstPtr &, epicsFloat64 value)=0;
    virtual asynStatus readFloat64 (const ADSPVInfoConstPtr &, epicsFloat64 *value)=0;


    virtual asynStatus readInt8Array(const ADSPVInfoConstPtr &, epicsInt8 *value,
                                        size_t nElements, size_t *nIn)=0;
    virtual asynStatus writeInt8Array(const ADSPVInfoConstPtr &, epicsInt8 *value,
                                        size_t nElements)=0;

    virtual asynStatus writeInt16Array(const ADSPVInfoConstPtr &, epicsInt16 *value, size_t nelements)=0;
    virtual asynStatus readInt16Array(const ADSPVInfoConstPtr &,  epicsInt16 *value, size_t nelements, size_t *nIn)=0;

    virtual asynStatus readInt32Array(const ADSPVInfoConstPtr &, epicsInt32 *value,
                                        size_t nElements, size_t *nIn)=0;
    virtual asynStatus writeInt32Array(const ADSPVInfoConstPtr &, epicsInt32 *value,
                                        size_t nElements)=0;

    virtual asynStatus readFloat32Array(const ADSPVInfoConstPtr &, epicsFloat32 *value,
                                        size_t nElements, size_t *nIn)=0;
    virtual asynStatus writeFloat32Array(const ADSPVInfoConstPtr &, epicsFloat32 *value,
                                        size_t nElements)=0;

    virtual asynStatus readFloat64Array(const ADSPVInfoConstPtr &, epicsFloat64 *value,
                                        size_t nElements, size_t *nIn)=0;
    virtual asynStatus writeFloat64Array(const ADSPVInfoConstPtr &, epicsFloat64 *value,
                                        size_t nElements)=0;
};

#endif /* ADSINTERFACE_H_ */

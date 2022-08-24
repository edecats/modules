/*
 * ADSSim.h
 *
 *  Created on: 14. maj 2018
 *      Author: sisaev
 */

#ifndef ADSSIM_H_
#define ADSSIM_H_

#include <string>

#include "HWInterface.h"

class HWStub: public HWInterface
{
public:
	HWStub();
	virtual ~HWStub();

    asynStatus writeBool(const ADSPVInfoConstPtr &, epicsUInt32 value, epicsUInt32 mask) override;
    asynStatus readBool (const ADSPVInfoConstPtr &, epicsUInt32 *value, epicsUInt32 mask) override;

    asynStatus writeInt8(const ADSPVInfoConstPtr &, epicsInt32 value) override;
    asynStatus readInt8 (const ADSPVInfoConstPtr &, epicsInt32 *value) override;

    asynStatus writeInt16(const ADSPVInfoConstPtr &, epicsInt32 value) override;
    asynStatus readInt16 (const ADSPVInfoConstPtr &, epicsInt32 *value) override;


    asynStatus writeInt32(const ADSPVInfoConstPtr &, epicsInt32 value) override;
    asynStatus readInt32 (const ADSPVInfoConstPtr &, epicsInt32 *value) override;

    asynStatus writeInt64(const ADSPVInfoConstPtr &, epicsInt32 value) override;
    asynStatus readInt64 (const ADSPVInfoConstPtr &, epicsInt32 *value) override;

    asynStatus writeFloat32(const ADSPVInfoConstPtr &, epicsFloat64 value) override;
    asynStatus readFloat32 (const ADSPVInfoConstPtr &, epicsFloat64 *value) override;

    asynStatus writeFloat64(const ADSPVInfoConstPtr &, epicsFloat64 value) override;
    asynStatus readFloat64 (const ADSPVInfoConstPtr &, epicsFloat64 *value) override;


    asynStatus readInt8Array(const ADSPVInfoConstPtr &, epicsInt8 *value,
                                size_t nElements, size_t *nIn) override;
    asynStatus writeInt8Array(const ADSPVInfoConstPtr &, epicsInt8 *value,
                                size_t nElements) override;

    asynStatus writeInt16Array(const ADSPVInfoConstPtr &, epicsInt16 *value, size_t nelements) override;
    asynStatus readInt16Array(const ADSPVInfoConstPtr &,  epicsInt16 *value, size_t nelements, size_t *nIn) override;

    asynStatus readInt32Array(const ADSPVInfoConstPtr &, epicsInt32 *value,
                                size_t nElements, size_t *nIn) override;
    asynStatus writeInt32Array(const ADSPVInfoConstPtr &, epicsInt32 *value,
                                size_t nElements) override;

    asynStatus readFloat32Array(const ADSPVInfoConstPtr &, epicsFloat32 *value,
                                size_t nElements, size_t *nIn) override;
    asynStatus writeFloat32Array(const ADSPVInfoConstPtr &, epicsFloat32 *value,
                                size_t nElements) override;

    asynStatus readFloat64Array(const ADSPVInfoConstPtr &, epicsFloat64 *value,
                                size_t nElements, size_t *nIn) override;
    asynStatus writeFloat64Array(const ADSPVInfoConstPtr &, epicsFloat64 *value,
                                size_t nElements) override;

protected:
    std::string _portName;
};

#endif /* ADSSIM_H_ */

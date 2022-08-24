/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#ifndef HWINTERFACE_H
#define HWINTERFACE_H

#include "ADSPVInfo.h"
#include "err.h"

template<typename P>
class HWInterface {
  public:
    HWInterface(){}
    virtual ~HWInterface(){}

    // asynInt32 Interface
    virtual asynStatus writeBool(const ADSPVInfoPtr& pv_info, epicsInt32 value);
    virtual asynStatus writeInt8(const ADSPVInfoPtr& pv_info, epicsInt32 value);
    virtual asynStatus readInt8(const ADSPVInfoPtr& pv_info, epicsInt32 *value);

    virtual asynStatus writeUInt8(const ADSPVInfoPtr& pv_info, epicsInt32 value);
    virtual asynStatus readUInt8(const ADSPVInfoPtr& pv_info, epicsInt32 *value);

    virtual asynStatus writeInt16(const ADSPVInfoPtr& pv_info, epicsInt32 value);
    virtual asynStatus readInt16(const ADSPVInfoPtr& pv_info, epicsInt32 *value);

    virtual asynStatus writeUInt16(const ADSPVInfoPtr& pv_info, epicsInt32 value);
    virtual asynStatus readUInt16(const ADSPVInfoPtr& pv_info, epicsInt32 *value);

    virtual asynStatus writeInt32(const ADSPVInfoPtr& pv_info, epicsInt32 value);
    virtual asynStatus readInt32(const ADSPVInfoPtr& pv_info, epicsInt32 *value);

    virtual asynStatus writeUInt32(const ADSPVInfoPtr& pv_info, epicsInt32 value);
    virtual asynStatus readUInt32(const ADSPVInfoPtr& pv_info, epicsInt32 *value);

    // asynFloat64 Interface
    virtual asynStatus writeFloat32(const ADSPVInfoPtr& pv_info, epicsFloat64 value);
    virtual asynStatus readFloat32(const ADSPVInfoPtr& pv_info, epicsFloat64 *value);

    virtual asynStatus writeFloat64(const ADSPVInfoPtr& pv_info, epicsFloat64 value);
    virtual asynStatus readFloat64(const ADSPVInfoPtr& pv_info, epicsFloat64 *value);

  
    // asynInt32Digital Interface
    virtual asynStatus writeBoolDigital(const ADSPVInfoPtr& pv_info, epicsUInt32 value, epicsUInt32 mask);
    virtual asynStatus writeUInt8Digital(const ADSPVInfoPtr& pv_info, epicsUInt32 value, epicsUInt32 mask);
    virtual asynStatus readUInt8Digital(const ADSPVInfoPtr& pv_info, epicsUInt32 *value, epicsUInt32 mask);

    virtual asynStatus writeUInt16Digital(const ADSPVInfoPtr& pv_info, epicsUInt32 value, epicsUInt32 mask);
    virtual asynStatus readUInt16Digital(const ADSPVInfoPtr& pv_info, epicsUInt32 *value, epicsUInt32 mask);

    virtual asynStatus writeUInt32Digital(const ADSPVInfoPtr& pv_info, epicsUInt32 value, epicsUInt32 mask);
    virtual asynStatus readUInt32Digital(const ADSPVInfoPtr& pv_info, epicsUInt32 *value, epicsUInt32 mask);

    // asynOctet Interface
    virtual asynStatus writeString(const ADSPVInfoPtr& pv_info, const char *value, size_t maxChars,size_t *nActual);
    virtual asynStatus readString(const ADSPVInfoPtr& pv_info, char *value, size_t maxChars,size_t *nActual, int *eomReason);

 
    // asynIntXXXArray Interface
    virtual asynStatus writeInt8Array(const ADSPVInfoPtr& pv_info, epicsInt8 *value,
                                                                    size_t nElements);
    virtual asynStatus readInt8Array(const ADSPVInfoPtr& pv_info, epicsInt8 *value,
                                                       size_t nElements, size_t *nIn);

    virtual asynStatus writeInt16Array(const ADSPVInfoPtr& pv_info, epicsInt16 *value,
                                                                   size_t nElements);
    virtual asynStatus readInt16Array(const ADSPVInfoPtr& pv_info, epicsInt16 *value, 
                                                      size_t nElements, size_t *nIn);

    virtual asynStatus writeInt32Array(const ADSPVInfoPtr& pv_info, epicsInt32 *value,
                                                                    size_t nElements);
    virtual asynStatus readInt32Array(const ADSPVInfoPtr& pv_info, epicsInt32 *value,
                                                       size_t nElements, size_t *nIn);

    // asynFloatXXXArray Interface
    virtual asynStatus writeFloat32Array(const ADSPVInfoPtr& pv_info, epicsFloat32 *value,
                                                                    size_t nElements);
    virtual asynStatus readFloat32Array(const ADSPVInfoPtr& pv_info, epicsFloat32 *value,
                                                       size_t nElements, size_t *nIn);

    virtual asynStatus writeFloat64Array(const ADSPVInfoPtr& pv_info, epicsFloat64 *value,
                                                                    size_t nElements);
    virtual asynStatus readFloat64Array(const ADSPVInfoPtr& pv_info, epicsFloat64 *value,
                                                       size_t nElements, size_t *nIn);

    // Interrupt interfaces
    // asynInt32Intr Interface
    virtual void readInt8Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readUInt8Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readInt16Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readUInt16Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readInt32Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readUInt32Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readFloat32Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readFloat64Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readUInt8DigitalIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readUInt16DigitalIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readUInt32DigitalIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readOctetIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readInt8ArrayIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readInt16ArrayIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readInt32ArrayIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readFloat32ArrayIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);

    virtual void readFloat64ArrayIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data);
};

#include "HWInterface.tpp"

#endif /* HWINTERFACE_H */

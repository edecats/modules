/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

template <typename T>
static asynStatus write(const ADSPVInfoPtr& pv_info, T *value, size_t nelements) {
    uint32_t write_bytes = nelements * sizeof(T);
    auto ads_var = pv_info->get_ads_variable();

    int rc = ads_var->write((uint8_t *)value, write_bytes);
    switch (rc) {
        case EPICSADS_OK: break;

        case EPICSADS_DISCONNECTED:
            LOG_WARN_ASYN(pv_info->getAsynUsrPtr(), "no connection to ADS device");
            return asynDisconnected;

        default:
            LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "error writing to ADS variable '%s' (%i): %s", ads_var->addr->info().c_str(), rc, ads_errors[rc].c_str());
            return asynError;
    }

    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "successfuly written %u bytes", write_bytes);

    return asynSuccess;
}

template <typename T>
static asynStatus read(const ADSPVInfoPtr& pv_info, T *value, size_t nelements, size_t *nread) {
    auto ads_var = pv_info->get_ads_variable();
    if (ads_var == nullptr) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "ADS variable is not set");
        return asynError;
    }

    size_t read_bytes = sizeof(T) * nelements;
    int rc = ads_var->read_from_buffer(read_bytes, (uint8_t *)value);
    if (rc != 0) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "error reading ADS variable data (%i): %s", rc, ads_errors[rc].c_str());
        return asynError;
    }

    if (nread != nullptr) {
        *nread = read_bytes;
    }

    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "successfuly read %zu bytes", read_bytes);

    return asynSuccess;
}

// asynInt32 Interface
template<class P>
asynStatus HWInterface<P>::writeBool(const ADSPVInfoPtr& pv_info, epicsInt32 value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    bool val = static_cast<bool>(value);
    return write(pv_info, &val, 1);
}

template<class P>
asynStatus HWInterface<P>::writeInt8(const ADSPVInfoPtr& pv_info, epicsInt32 value) {    
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsInt8 val = static_cast<epicsInt8>(value);
    return write(pv_info, &val, 1);
}

template<class P>
asynStatus HWInterface<P>::readInt8(const ADSPVInfoPtr& pv_info, epicsInt32 *value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsInt8 val;

    asynStatus rc = read(pv_info, &val, 1, nullptr);
    if (rc == asynSuccess) {
        *value = val;
    }

    return rc;
}

template<class P>
asynStatus HWInterface<P>::writeUInt8(const ADSPVInfoPtr& pv_info, epicsInt32 value) {  
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt8 val = static_cast<epicsUInt8>(value);
    return write(pv_info, &val, 1);
}

template<class P>
asynStatus HWInterface<P>::readUInt8(const ADSPVInfoPtr& pv_info, epicsInt32 *value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt8 val;

    asynStatus rc = read(pv_info, &val, 1, nullptr);
if (rc == asynSuccess) {
        *value = val;
    }

    return rc;
}

template<class P>
asynStatus HWInterface<P>::writeInt16(const ADSPVInfoPtr& pv_info, epicsInt32 value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsInt16 val = static_cast<epicsInt16>(value);
    return write(pv_info, &val, 1);
}

template<class P>
asynStatus HWInterface<P>::readInt16(const ADSPVInfoPtr& pv_info, epicsInt32 *value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsInt16 val;

    asynStatus rc = read(pv_info, &val, 1, nullptr);
    if (rc == asynSuccess) {
        *value = val;
    }

    return rc;
}

template<class P>
asynStatus HWInterface<P>::writeUInt16(const ADSPVInfoPtr& pv_info, epicsInt32 value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt16 val = static_cast<epicsUInt16>(value);
    return write(pv_info, &val, 1);
}

template<class P>
asynStatus HWInterface<P>::readUInt16(const ADSPVInfoPtr& pv_info, epicsInt32 *value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt16 val;

    asynStatus rc = read(pv_info, &val, 1, nullptr);
    if (rc == asynSuccess) {
        *value = val;
    }

    return rc;
}

template<class P>
asynStatus HWInterface<P>::writeInt32(const ADSPVInfoPtr& pv_info, epicsInt32 value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return write(pv_info, &value, 1);
}

template<class P>
asynStatus HWInterface<P>::readInt32(const ADSPVInfoPtr& pv_info, epicsInt32 *value) {    
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return read(pv_info, value, 1, nullptr);
}

// asynFloat64 Interface
template<class P>
asynStatus HWInterface<P>::writeUInt32(const ADSPVInfoPtr& pv_info, epicsInt32 value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt32 val = static_cast<epicsUInt32>(value);
    return write(pv_info, &val, 1);
}

template<class P>
asynStatus HWInterface<P>::readUInt32(const ADSPVInfoPtr& pv_info, epicsInt32 *value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt32 val;

    asynStatus rc = read(pv_info, &val, 1, nullptr);
    if (rc == asynSuccess) {
        *value = val;
    }

    return rc;
}

template<class P>
asynStatus HWInterface<P>::writeFloat32(const ADSPVInfoPtr& pv_info, epicsFloat64 value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsFloat32 val = static_cast<epicsFloat32>(value);
    return write(pv_info, &val, 1);
}

template<class P>
asynStatus HWInterface<P>::readFloat32(const ADSPVInfoPtr& pv_info, epicsFloat64 *value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsFloat32 val;
    asynStatus rc = read(pv_info, &val, 1, nullptr);
    if (rc == asynSuccess) {
        *value = static_cast<epicsFloat64>(val);
    }

    return rc;
}

template<class P>
asynStatus HWInterface<P>::writeFloat64(const ADSPVInfoPtr& pv_info, epicsFloat64 value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return write(pv_info, &value, 1);
}

template<class P>
asynStatus HWInterface<P>::readFloat64(const ADSPVInfoPtr& pv_info, epicsFloat64 *value) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return read(pv_info, value, 1, nullptr);
}

// asynUInt32Digital Interface
template<class P>
asynStatus HWInterface<P>::writeBoolDigital(const ADSPVInfoPtr& pv_info, epicsUInt32 value, epicsUInt32 mask) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    value = value & mask;
    bool val = static_cast<bool>(val);
    return write(pv_info, &val, 1);
}

template<class P>
asynStatus HWInterface<P>::writeUInt8Digital(const ADSPVInfoPtr& pv_info, epicsUInt32 value, epicsUInt32 mask) {  
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    value = value & mask;
    epicsUInt8 val = static_cast<epicsUInt8>(value);
    return write(pv_info, &val, 1);
}

template<class P>
asynStatus HWInterface<P>::readUInt8Digital(const ADSPVInfoPtr& pv_info, epicsUInt32 *value, epicsUInt32 mask) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt8 val = 0;
    asynStatus rc = read(pv_info, &val, 1, nullptr);
    if (rc != asynSuccess) {
        *value = val & mask;
    }

    return rc;
}

template<class P>
asynStatus HWInterface<P>::writeUInt16Digital(const ADSPVInfoPtr& pv_info, epicsUInt32 value, epicsUInt32 mask) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    value = value & mask;
    epicsUInt16 val = static_cast<epicsUInt16>(value);
    return write(pv_info, &val, 1);
}

template<class P>
asynStatus HWInterface<P>::readUInt16Digital(const ADSPVInfoPtr& pv_info, epicsUInt32 *value, epicsUInt32 mask) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt16 val = 0;
    asynStatus rc = read(pv_info, value, 1, nullptr);
    if (rc != asynSuccess) {
        *value = val & mask;
    }

    return rc;
}

template<class P>
asynStatus HWInterface<P>::writeUInt32Digital(const ADSPVInfoPtr& pv_info, epicsUInt32 value, epicsUInt32 mask) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    value = value & mask;
    epicsUInt32 val = static_cast<epicsUInt32>(value);
    return write(pv_info, &val, 1);
}

template<class P>
asynStatus HWInterface<P>::readUInt32Digital(const ADSPVInfoPtr& pv_info, epicsUInt32 *value, epicsUInt32 mask) { 
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt32 val = 0;
    asynStatus rc = read(pv_info, value, 1, nullptr);
    if (rc != asynSuccess) {
        *value = val & mask;
    }

    return rc;
}

// asynOctet Interface
template<class P>
asynStatus HWInterface<P>::writeString(const ADSPVInfoPtr& pv_info, const char *value, size_t maxChars, size_t *nActual) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    asynStatus rc = write(pv_info, value, maxChars);
    if (rc == asynSuccess) {
        *nActual = maxChars;
    }

    return rc;
}

template<class P>
asynStatus HWInterface<P>::readString(const ADSPVInfoPtr& pv_info, char *value, size_t maxChars, size_t *nActual, int *eomReason) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    asynStatus rc = read(pv_info, value, maxChars, nActual);
    if (rc == asynSuccess) {
        *eomReason = ASYN_EOM_END;
    }

    return rc;
}

// asynIntXXXArray Interface
template<class P>
asynStatus HWInterface<P>::writeInt8Array(const ADSPVInfoPtr& pv_info, epicsInt8 *value,
                                                                   size_t nElements) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return write(pv_info, value, nElements);
}

template<class P>
asynStatus HWInterface<P>::readInt8Array(const ADSPVInfoPtr& pv_info, epicsInt8 *value, 
                                                      size_t nElements, size_t *nIn) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return read(pv_info, value, nElements, nIn);
}

template<class P>
asynStatus HWInterface<P>::writeInt16Array(const ADSPVInfoPtr& pv_info, epicsInt16 *value,
                                                                   size_t nElements) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return write(pv_info, value, nElements);
}

template<class P>
asynStatus HWInterface<P>::readInt16Array(const ADSPVInfoPtr& pv_info, epicsInt16 *value, 
                                                      size_t nElements, size_t *nIn) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return read(pv_info, value, nElements, nIn);
}

template<class P>
asynStatus HWInterface<P>::writeInt32Array(const ADSPVInfoPtr& pv_info, epicsInt32 *value,
                                                                   size_t nElements) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return write(pv_info, value, nElements);
}

template<class P>
asynStatus HWInterface<P>::readInt32Array(const ADSPVInfoPtr& pv_info, epicsInt32 *value, 
                                                      size_t nElements, size_t *nIn) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return read(pv_info, value, nElements, nIn);
}

// asynFloatXXXArray Interface
template<class P>
asynStatus HWInterface<P>::writeFloat32Array(const ADSPVInfoPtr& pv_info, epicsFloat32 *value,
                                                                   size_t nElements) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return write(pv_info, value, nElements);
}

template<class P>
asynStatus HWInterface<P>::readFloat32Array(const ADSPVInfoPtr& pv_info, epicsFloat32 *value, 
                                                      size_t nElements, size_t *nIn) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return read(pv_info, value, nElements, nIn);
}

template<class P>
asynStatus HWInterface<P>::writeFloat64Array(const ADSPVInfoPtr& pv_info, epicsFloat64 *value,
                                                                   size_t nElements) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return write(pv_info, value, nElements);
}

template<class P>
asynStatus HWInterface<P>::readFloat64Array(const ADSPVInfoPtr& pv_info, epicsFloat64 *value, 
                                                      size_t nElements, size_t *nIn) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    return read(pv_info, value, nElements, nIn);
}

// Interrupt interfaces
// asynInt32Intr interface
template<class P>
void HWInterface<P>::readInt8Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsInt8 val = 0;
    if (read(pv_info, &val, 1, nullptr) != asynSuccess) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "read failed");
        return;
    }

    pportDriver->callCallbacksInt32((epicsInt32)val, pv_info->getId(), 0, asynSuccess);    
}

template<class P>
void HWInterface<P>::readUInt8Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt8 val = 0;
    if (read(pv_info, &val, 1, nullptr) != asynSuccess) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "read failed");
        return;
    }
    
    pportDriver->callCallbacksInt32((epicsInt32)val, pv_info->getId(), 0, asynSuccess);    
}

template<class P>
void HWInterface<P>::readInt16Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsInt16 val = 0;
    if (read(pv_info, &val, 1, nullptr) != asynSuccess) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "read failed");
        return;
    }

    pportDriver->callCallbacksInt32((epicsInt32)val, pv_info->getId(), 0, asynSuccess);    
}

template<class P>
void HWInterface<P>::readUInt16Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt16 val = 0;
    if (read(pv_info, &val, 1, nullptr) != asynSuccess) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "read failed");
        return;
    }
    
    pportDriver->callCallbacksInt32((epicsInt32)val, pv_info->getId(), 0, asynSuccess);    
}

template<class P>
void HWInterface<P>::readInt32Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsInt32 val = 0;
    if (read(pv_info, &val, 1, nullptr) != asynSuccess) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "read failed");
        return;
    }

    pportDriver->callCallbacksInt32(val, pv_info->getId(), 0, asynSuccess);    
}

template<class P>
void HWInterface<P>::readUInt32Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt32 val = 0;
    if (read(pv_info, &val, 1, nullptr) != asynSuccess) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "read failed");
        return;
    }
    
    pportDriver->callCallbacksFloat64((epicsFloat64)val, pv_info->getId(), 0, asynSuccess);    
}

template<class P>
void HWInterface<P>::readFloat32Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsFloat32 val = 0;
    if (read(pv_info, &val, 1, nullptr) != asynSuccess) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "read failed");
        return;
    }
    
    pportDriver->callCallbacksFloat64((epicsFloat64)val, pv_info->getId(), 0, asynSuccess);    
}

template<class P>
void HWInterface<P>::readFloat64Intr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsFloat64 val = 0;
    if (read(pv_info, &val, 1, nullptr) != asynSuccess) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "read failed");
        return;
    }

    pportDriver->callCallbacksFloat64(val, pv_info->getId(), 0, asynSuccess);    
}

template<class P>
void HWInterface<P>::readUInt8DigitalIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt8 val = 0;
    if (read(pv_info, &val, 1, nullptr) != asynSuccess) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "read failed");
        return;
    }
    
    pportDriver->callCallbacksUInt32Digital((epicsUInt32)val, pv_info->getId(), 0, asynSuccess);    
}

template<class P>
void HWInterface<P>::readUInt16DigitalIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt16 val = 0;
    if (read(pv_info, &val, 1, nullptr) != asynSuccess) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "read failed");
        return;
    }
    
    pportDriver->callCallbacksUInt32Digital((epicsUInt32)val, pv_info->getId(), 0, asynSuccess);    
}

template<class P>
void HWInterface<P>::readUInt32DigitalIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    epicsUInt32 val = 0;
    if (read(pv_info, &val, 1, nullptr) != asynSuccess) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "read failed");
        return;
    }

    pportDriver->callCallbacksUInt32Digital(val, pv_info->getId(), 0, asynSuccess);    
}

template<class P>
void HWInterface<P>::readOctetIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    auto ads_var = pv_info->get_ads_variable();
    if (ads_var == nullptr) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "ADS variable is not set");
        return;
    }

    std::pair<uint8_t *, RWLock *> value = ads_var->get_read_data();

    /* callCallbacksOctet expect a null terminated string, which is no
     * guaranteed when value fills the whole read buffer. To mitigate,
     * a dedicated buffer is prepared with the extra null terminator appended. */
    size_t input_data_size = ads_var->addr->get_nelem();
    epicsInt8 buffer[input_data_size + 1];
    memcpy(buffer, (epicsInt8 *)value.first, input_data_size);
    buffer[input_data_size] = '\0';
    value.second->unlock_read();

    pportDriver->callCallbacksOctet((epicsInt8 *)buffer, pv_info->getId(), 0, asynSuccess);    
}

template<class P>
void HWInterface<P>::readInt8ArrayIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    auto ads_var = pv_info->get_ads_variable();
    if (ads_var == nullptr) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "ADS variable is not set");
        return;
    }

    std::pair<uint8_t *, RWLock *> value = ads_var->get_read_data();
    pportDriver->callCallbacksInt8Array((epicsInt8 *)value.first, ads_var->addr->get_nelem(), pv_info->getId(), 0, asynSuccess);
    value.second->unlock_read();
}

template<class P>
void HWInterface<P>::readInt16ArrayIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    auto ads_var = pv_info->get_ads_variable();
    if (ads_var == nullptr) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "ADS variable is not set");
        return;
    }

    std::pair<uint8_t *, RWLock *> value = ads_var->get_read_data();
    pportDriver->callCallbacksInt16Array((epicsInt16 *)value.first, ads_var->addr->get_nelem(), pv_info->getId(), 0, asynSuccess);    
    value.second->unlock_read();
}

template<class P>
void HWInterface<P>::readInt32ArrayIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    auto ads_var = pv_info->get_ads_variable();
    if (ads_var == nullptr) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "ADS variable is not set");
        return;
    }

    std::pair<uint8_t *, RWLock *> value = ads_var->get_read_data();
    pportDriver->callCallbacksInt32Array((epicsInt32 *)value.first, ads_var->addr->get_nelem(), pv_info->getId(), 0, asynSuccess);    
    value.second->unlock_read();
}

template<class P>
void HWInterface<P>::readFloat32ArrayIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    auto ads_var = pv_info->get_ads_variable();
    if (ads_var == nullptr) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "ADS variable is not set");
        return;
    }

    std::pair<uint8_t *, RWLock *> value = ads_var->get_read_data();
    pportDriver->callCallbacksFloat32Array((epicsFloat32 *)value.first, ads_var->addr->get_nelem(), pv_info->getId(), 0, asynSuccess);    
    value.second->unlock_read();
}

template<class P>
void HWInterface<P>::readFloat64ArrayIntr(const ADSPVInfoPtr& pv_info, P *pportDriver, const void *data) {
    LOG_TRACE_ASYN(pv_info->getAsynUsrPtr(), "entering");

    auto ads_var = pv_info->get_ads_variable();
    if (ads_var == nullptr) {
        LOG_ERR_ASYN(pv_info->getAsynUsrPtr(), "ADS variable is not set");
        return;
    }

    std::pair<uint8_t *, RWLock *> value = ads_var->get_read_data();
    pportDriver->callCallbacksFloat64Array((epicsFloat64 *)value.first, ads_var->addr->get_nelem(), pv_info->getId(), 0, asynSuccess);    
    value.second->unlock_read();
}

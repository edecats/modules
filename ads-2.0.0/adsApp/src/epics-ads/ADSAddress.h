/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#ifndef ADSADDRESS_H
#define ADSADDRESS_H

#include <string>
#include <cstdint>
#include <vector>

#include "Types.h"

class ADSAddress {
protected:
    ADSDataType data_type;
    Operation operation;
    std::string variable_name;
    uint16_t ads_port = 0; /* e.g. AMSPORT_R0_PLC_TC3 (851), as defined in AdsDef.h */
    uint32_t index_group = 0;
    uint32_t index_offset = 0;
    uint32_t ads_notification_delay = 0;
    uint32_t nelem = 0;

    bool name_is_resolved = false;

    void parse_register_specifier(std::vector<std::string>& address_tokens);
    void parse_variable_specifier(std::vector<std::string>& address_tokens);

public:
    ADSDataType get_data_type(); /* Data type */
    std::string get_var_name(); /* Variable (symbol) name */
    Operation get_operation(); /* Read/write */
    uint16_t get_ads_port(); /* ADS port, e.g. AMSPORT_R0_PLC_TC3 as defined in AdsDef.h */
    uint32_t get_index_group(); /* ADS index group */
    uint32_t get_index_offset(); /* ADS index offset */
    uint32_t get_notification_delay(); /* ADS notification delay */
    uint32_t get_nelem(); /* Number of elements: 1 for scalars, the rest for waveforms */

    /* True when ADS variable name resolved into group/offset specifiers. Always
     * true for variables addressed using index/offset. */
    bool is_resolved();

    /* Construct from address specifier.
     * 
     * Expected format for register access:
     * DATATYPE[NELEM] OPERATION P=PORT G=INDEX_GROUP O=INDEX_OFFSET D=NOTIFY_DELAY
     *
     * Expected format for variable access:
     * DATATYPE[NELEM] OPERATION P=PORT V=VARIABLE_NAME D=NOTIFY_DELAY
     *
     * [NELEM] is required for STRING datatype. D (ADS notification delay) is
     * optional. */
    ADSAddress(const std::string address);

    /* Provide group/offset specifiers when ADS variable name is resolved, i.e.
     * variable handle is acquired using ADSIGRP_SYM_HNDBYNAME request */
    int resolve(const uint32_t ads_index_group, const uint32_t ads_index_offset);

    /* Called before ADS connection is closed */
    int unresolve();

    /* Return address information in somewhat human-friendly format */
    const std::string info();
};

#endif /* ADSADDRESS_H */

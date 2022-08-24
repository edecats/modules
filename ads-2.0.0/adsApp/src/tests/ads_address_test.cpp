/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#include "gtest/gtest.h"
#include "ADSAddress.h"

TEST(ADSAddressTest, InvalidAddress) {
    EXPECT_THROW(ADSAddress(""), std::invalid_argument);
    EXPECT_THROW(ADSAddress("     "), std::invalid_argument);
    EXPECT_THROW(ADSAddress("invalid"), std::invalid_argument);
    EXPECT_THROW(ADSAddress("LREAL[invalid] W P=PLC_TC3 V=var"), std::invalid_argument);
    EXPECT_THROW(ADSAddress("LREAL[-5] W P=PLC_TC3 V=var"), std::invalid_argument);
    EXPECT_THROW(ADSAddress("LREAL x P=PLC_TC3 V=var"), std::invalid_argument);
    EXPECT_THROW(ADSAddress("inv W P=PLC_TC3 V=var"), std::invalid_argument);
    EXPECT_THROW(ADSAddress("LREAL W P=inv V=var"), std::invalid_argument);
    EXPECT_THROW(ADSAddress("LREAL W P=PLC_TC3 V=var D=invalid"), std::invalid_argument);
}

TEST(ADSAddressTest, MissingParameters) {
    EXPECT_THROW(ADSAddress("W P=PLC_TC3 V=var"), std::invalid_argument);
    EXPECT_THROW(ADSAddress("LREAL[] W P=PLC_TC3 V=var"), std::invalid_argument);
    EXPECT_THROW(ADSAddress("LREAL P=PLC_TC3 V=var"), std::invalid_argument);
    EXPECT_THROW(ADSAddress("LREAL W P= V=var"), std::invalid_argument);
    EXPECT_THROW(ADSAddress("LREAL W P=PLC_TC3"), std::invalid_argument);
    EXPECT_THROW(ADSAddress("LREAL W P=PLC_TC3 V="), std::invalid_argument);
    EXPECT_THROW(ADSAddress("LREAL W P=PLC_TC3 G=1234"), std::invalid_argument);
    EXPECT_THROW(ADSAddress("LREAL W P=PLC_TC3 G=1234 O=1234 D="), std::invalid_argument);
}

TEST(ADSAddressTest, ValidNumericPort) {
    ADSAddress addr("LREAL W P=0x100 V=VariableName");
    ASSERT_EQ(addr.get_data_type(), ADSDataType::LREAL);
    ASSERT_EQ(addr.get_operation(), Operation::Write);
    ASSERT_EQ(addr.get_ads_port(), (uint32_t)0x100);
    ASSERT_EQ(addr.get_var_name(), "VariableName");
    ASSERT_EQ(addr.get_nelem(), (uint32_t)1);
}

TEST(ADSAddressTest, ValidEnumPort) {
    ADSAddress addr("LREAL W P=PLC_TC3 V=VariableName");
    ASSERT_EQ(addr.get_data_type(), ADSDataType::LREAL);
    ASSERT_EQ(addr.get_operation(), Operation::Write);
    ASSERT_EQ(addr.get_ads_port(), (uint32_t)AMSPORT_R0_PLC_TC3);
    ASSERT_EQ(addr.get_var_name(), "VariableName");
    ASSERT_EQ(addr.get_nelem(), (uint32_t)1);
}

TEST(ADSAddressTest, ValidAddressSimple) {
    ADSAddress addr("LREAL W P=PLC_TC3 V=VariableName");
    ASSERT_EQ(addr.get_data_type(), ADSDataType::LREAL);
    ASSERT_EQ(addr.get_operation(), Operation::Write);
    ASSERT_EQ(addr.get_ads_port(), (uint32_t)AMSPORT_R0_PLC_TC3);
    ASSERT_EQ(addr.get_var_name(), "VariableName");
    ASSERT_EQ(addr.get_nelem(), (uint32_t)1);
}

TEST(ADSAddressTest, ValidRegisterAddress) {
    ADSAddress addr("LREAL W P=PLC_TC3 G=0x8502000 O=0x8103E800");
    ASSERT_EQ(addr.get_data_type(), ADSDataType::LREAL);
    ASSERT_EQ(addr.get_operation(), Operation::Write);
    ASSERT_EQ(addr.get_ads_port(), (uint32_t)AMSPORT_R0_PLC_TC3);
    ASSERT_EQ(addr.get_index_group(), (uint32_t)0x8502000);
    ASSERT_EQ(addr.get_index_offset(), (uint32_t)0x8103E800);
    ASSERT_EQ(addr.get_nelem(), (uint32_t)1);
}

TEST(ADSAddressTest, ValidAddressWithNelem) {
    ADSAddress addr("STRING[13] W P=PLC_TC3 V=VariableName");
    ASSERT_EQ(addr.get_data_type(), ADSDataType::STRING);
    ASSERT_EQ(addr.get_operation(), Operation::Write);
    ASSERT_EQ(addr.get_ads_port(), (uint32_t)AMSPORT_R0_PLC_TC3);
    ASSERT_EQ(addr.get_var_name(), "VariableName");
    ASSERT_EQ(addr.get_nelem(), (uint32_t)13);
}

TEST(ADSAddressTest, StringTypeRequiresNelem) {
    EXPECT_THROW(ADSAddress("STRING W P=PLC_TC3 V=VariableName"), std::invalid_argument);
}

TEST(ADSAddressTest, SpacesAreIgnored) {
    ADSAddress addr("STRING[13]   W   P=PLC_TC3   V=VariableName");
    ASSERT_EQ(addr.get_data_type(), ADSDataType::STRING);
    ASSERT_EQ(addr.get_operation(), Operation::Write);
    ASSERT_EQ(addr.get_ads_port(), (uint32_t)AMSPORT_R0_PLC_TC3);
    ASSERT_EQ(addr.get_var_name(), "VariableName");
    ASSERT_EQ(addr.get_nelem(), (uint32_t)13);
}

TEST(ADSAddressTest, RegisterAddressConsideredResolved) {
    ADSAddress addr("INT R P=PLC_TC3 G=0x1234 O=0x5678");
    ASSERT_EQ(addr.is_resolved(), true);
}

TEST(ADSAddressTest, VariableAddressConsideredNotResolved) {
    ADSAddress addr("INT R P=PLC_TC3 V=VariableName");
    ASSERT_EQ(addr.is_resolved(), false);
}

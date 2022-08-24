/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#include <cstdlib>
#include <atomic>

#include <boost/tokenizer.hpp>

#include "gtest/gtest.h"
#include "Connection.h"
#include "SumReadRequest.h"

class ADSConnTest : public testing::Test {
protected:
    std::shared_ptr<Connection> ads_conn = std::make_shared<Connection>();

    virtual void print_usage() {
        std::cout << "Required environment variables:\n"
                     "- ADS_LOCAL_AMS_ID:   Local AMS ID, e.g. '10.68.64.229.1.1'\n"
                     "- ADS_REMOTE_AMS_ID:  Remote AMS ID, e.g. '10.68.79.25.1.1'\n"
                     "- ADS_REMOTE_ADDRESS: Remote address, e.g. '10.68.6.45'\n";
    }

    virtual std::string remote_address_from_env_var(std::string env_var) {
        return std::string(std::getenv(env_var.c_str()));
    }

    virtual AmsNetId ams_id_from_env_var(std::string env_var) {
        std::string value = std::getenv(env_var.c_str());

        std::vector<uint8_t> tokens;
        boost::char_separator<char> separator(".");
        boost::tokenizer<boost::char_separator<char>> tok(value, separator);
        for (auto itr = tok.begin(); itr != tok.end(); itr++) {
            uint8_t v = std::stoul(*itr, nullptr, 10);
            tokens.push_back(v);
        }

        if (tokens.size() != 6) {
            throw std::invalid_argument("invalid format for env. var. '" + env_var + "'");
        }

        return { tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5] };
    }

    virtual void SetUp() {
        srand((unsigned)time(0));

        AmsNetId local_ams_id;
        try {
            local_ams_id = ams_id_from_env_var("ADS_LOCAL_AMS_ID");
        } catch (...) {
            print_usage();
            FAIL() << "Environment variable ADS_LOCAL_AMS_ID is not specified correctly or is missing";
        }

        AmsNetId remote_ams_id;
        try {
            remote_ams_id = ams_id_from_env_var("ADS_REMOTE_AMS_ID");
        } catch (...) {
            print_usage();
            FAIL() << "Environment variable ADS_REMOTE_AMS_ID is not specified correctly or is missing";
        }

        std::string remote_address;
        try {
            remote_address = remote_address_from_env_var("ADS_REMOTE_ADDRESS");
        } catch (...) {
            print_usage();
            FAIL() << "Environment variable ADS_REMOTE_ADDRESS is not specified correctly or is missing";
        }

        ads_conn->set_local_ams_id(local_ams_id);
        int rc = ads_conn->connect(remote_ams_id, remote_address);
        ASSERT_EQ(rc, 0) << "Failed to connect to ADS device";
    }

    virtual void TearDown() {
        ads_conn->disconnect();
    }
};

TEST_F(ADSConnTest, ResolveMultipleVariableNames) {
    std::vector<std::shared_ptr<Variable>> variables;
    variables.push_back(std::shared_ptr<Variable>(new Variable("BOOL R P=PLC_TC3 V=TestPlan.types_bi")));
    variables.push_back(std::shared_ptr<Variable>(new Variable("DINT R P=PLC_TC3 V=TestPlan.types_longin")));
    int rc = ads_conn->resolve_variables(variables);
    ASSERT_EQ(rc, 0) << "Failed to acquire handles for ADS variables";
}

TEST_F(ADSConnTest, ResolveSingleVariableName) {
    std::shared_ptr<Variable> variable(new Variable("BOOL R P=PLC_TC3 V=TestPlan.types_bi"));
    int rc = ads_conn->resolve_variable(variable);
    ASSERT_EQ(rc, 0) << "Failed to acquire handles for ADS variable";
}

TEST_F(ADSConnTest, UnresolveMultipleVariableNames) {
    std::vector<std::shared_ptr<Variable>> variables;
    variables.push_back(std::shared_ptr<Variable>(new Variable("BOOL R P=PLC_TC3 V=TestPlan.types_bi")));
    variables.push_back(std::shared_ptr<Variable>(new Variable("DINT R P=PLC_TC3 V=TestPlan.types_longin")));
    int rc = ads_conn->resolve_variables(variables);
    EXPECT_EQ(rc, 0) << "Failed to acquire handles for ADS variables";

    rc = ads_conn->unresolve_variables(variables);
    ASSERT_EQ(rc, 0) << "Failed to unresolve ADS variables";
}

TEST_F(ADSConnTest, UnresolveSingleVariableName) {
    std::shared_ptr<Variable> variable(new Variable("BOOL R P=PLC_TC3 V=TestPlan.types_bi"));
    int rc = ads_conn->resolve_variable(variable);
    EXPECT_EQ(rc, 0) << "Failed to acquire handles for ADS variable";

    rc = ads_conn->unresolve_variable(variable);
    ASSERT_EQ(rc, 0) << "Failed to unresolve ADS variable";
}

TEST_F(ADSConnTest, MarkUnresolveOnFailure) {
    std::vector<std::shared_ptr<Variable>> variables;
    auto var_fake = std::make_shared<Variable>("BOOL R P=PLC_TC3 V=fake_variable");
    var_fake->addr->resolve(13, 13); /* Set fake values for variable handle */
    variables.push_back(var_fake);

    int rc = ads_conn->unresolve_variables(variables);
    ASSERT_NE(rc, 0);

    ASSERT_EQ(var_fake->addr->is_resolved(), false);
}

TEST_F(ADSConnTest, SumReadVariables) {
    std::vector<std::shared_ptr<Variable>> variables;
    variables.push_back(std::shared_ptr<Variable>(new Variable("DINT R P=PLC_TC3 V=TestPlan.types_longin")));
    variables.push_back(std::shared_ptr<Variable>(new Variable("LREAL R P=PLC_TC3 V=TestPlan.types_ai")));
    variables.push_back(std::shared_ptr<Variable>(new Variable("BOOL R P=PLC_TC3 V=TestPlan.types_bi")));
    variables.push_back(std::shared_ptr<Variable>(new Variable("INT R P=PLC_TC3 V=TestPlan.types_mbbi")));
    variables.push_back(std::shared_ptr<Variable>(new Variable("INT R P=PLC_TC3 V=TestPlan.types_mbbiDirect")));
    int rc = ads_conn->resolve_variables(variables);
    EXPECT_EQ(rc, 0) << "Failed to acquire handles for ADS variables";

    SumReadRequest read_req(2, this->ads_conn);
    rc = read_req.allocate(variables);
    EXPECT_EQ(rc, 0) << "Failed to allocate ADS sum read request";

    rc = read_req.initialize();
    EXPECT_EQ(rc, 0) << "Failed to initialize ADS sum read request";

    rc = read_req.read();
    EXPECT_EQ(rc, 0);
}

TEST_F(ADSConnTest, SumReadTest) {
    std::vector<std::shared_ptr<Variable>> variables;
    std::shared_ptr<Variable> var(new Variable("DINT R P=PLC_TC3 V=TestPlan.sym_int32_counter"));
    variables.push_back(var);
    int rc = ads_conn->resolve_variables(variables);
    EXPECT_EQ(rc, 0) << "Failed to acquire handles for ADS variables";

    auto r = SumReadRequest(1, this->ads_conn);
    EXPECT_EQ(r.allocate(variables), 0);
    EXPECT_EQ(r.initialize(), 0);
    EXPECT_EQ(r.read(), 0);

    int16_t value = 0;
    ASSERT_EQ(var->read_from_buffer(sizeof(value), (uint8_t *)&value), 0);
    EXPECT_NE(value, 0) << "Unlikely event that the counter reached zero";
}

TEST_F(ADSConnTest, VariableReadByAddress) {
    auto var = std::make_shared<Variable>("DINT R P=350 G=0x8502000 O=0x8103E802");
    std::vector<std::shared_ptr<Variable>> variables;
    variables.push_back(var);
    EXPECT_EQ(ads_conn->resolve_variables(variables), 0);

    var->set_connection(ads_conn);

    int32_t value_read = 0;
    uint32_t bytes_read;
    ASSERT_EQ(var->read((uint8_t *)&value_read, sizeof(value_read), &bytes_read), 0);
    ASSERT_EQ(bytes_read, sizeof(value_read));
    EXPECT_NE(value_read, 0) << "Unlikely event that the counter reached zero";
}

TEST_F(ADSConnTest, VariableReadByName) {
    auto var = std::make_shared<Variable>("DINT R P=PLC_TC3 V=TestPlan.sym_int32_counter");
    std::vector<std::shared_ptr<Variable>> variables;
    variables.push_back(var);
    EXPECT_EQ(ads_conn->resolve_variables(variables), 0);

    var->set_connection(ads_conn);

    int32_t value_read = 0;
    uint32_t bytes_read;
    ASSERT_EQ(var->read((uint8_t *)&value_read, sizeof(value_read), &bytes_read), 0);
    ASSERT_EQ(bytes_read, sizeof(value_read));
    EXPECT_NE(value_read, 0) << "Unlikely event that the counter reached zero";
}

TEST_F(ADSConnTest, VariableWriteByAddress) {
    auto var = std::make_shared<Variable>("DINT W P=350 G=0x8502000 O=0x8103E800");
    std::vector<std::shared_ptr<Variable>> variables;
    variables.push_back(var);
    EXPECT_EQ(ads_conn->resolve_variables(variables), 0);

    var->set_connection(ads_conn);

    int32_t value = rand();
    ASSERT_EQ(var->write((uint8_t *)&value, sizeof(value)), 0);
}

TEST_F(ADSConnTest, VariableWriteByName) {
    auto var = std::make_shared<Variable>("DINT W P=PLC_TC3 V=TestPlan.sym_int32_writable");
    std::vector<std::shared_ptr<Variable>> variables;
    variables.push_back(var);
    EXPECT_EQ(ads_conn->resolve_variables(variables), 0);

    var->set_connection(ads_conn);

    int32_t value = rand();
    ASSERT_EQ(var->write((uint8_t *)&value, sizeof(value)), 0);
}

TEST_F(ADSConnTest, VariableWriteReadByName) {
    auto var = std::make_shared<Variable>("DINT W P=PLC_TC3 V=TestPlan.sym_int32_writable");
    std::vector<std::shared_ptr<Variable>> variables;
    variables.push_back(var);
    EXPECT_EQ(ads_conn->resolve_variables(variables), 0);

    var->set_connection(ads_conn);

    int32_t value_write = rand();
    EXPECT_EQ(var->write((uint8_t *)&value_write, sizeof(value_write)), 0);

    int32_t value_read = 0;
    uint32_t bytes_read;
    EXPECT_EQ(var->read((uint8_t *)&value_read, sizeof(value_read), &bytes_read), 0);
    EXPECT_EQ(bytes_read, sizeof(value_read));
    ASSERT_EQ(value_read, value_write);
}

TEST_F(ADSConnTest, VariableWriteReadByAddress) {
    auto var = std::make_shared<Variable>("DINT W P=350 G=0x8502000 O=0x8103E800");
    std::vector<std::shared_ptr<Variable>> variables;
    variables.push_back(var);
    EXPECT_EQ(ads_conn->resolve_variables(variables), 0);

    var->set_connection(ads_conn);

    int32_t value_write = rand();
    EXPECT_EQ(var->write((uint8_t *)&value_write, sizeof(value_write)), 0);

    int32_t value_read = 0;
    uint32_t bytes_read;
    EXPECT_EQ(var->read((uint8_t *)&value_read, sizeof(value_read), &bytes_read), 0);
    EXPECT_EQ(bytes_read, sizeof(value_read));
    ASSERT_EQ(value_read, value_write);
}

TEST_F(ADSConnTest, ReadDeviceInfo) {
    AdsVersion ads_version = {0,0,0};
    char device_info[16];
    memset(device_info, 0, sizeof(device_info));

    int rc = ads_conn->read_device_info(device_info, sizeof(device_info), &ads_version);
    ASSERT_EQ(rc, 0);
    ASSERT_NE(device_info[0], 0);
    ASSERT_NE(ads_version.version, 0);
}

TEST_F(ADSConnTest, ReadDeviceInfoInvalidParam) {
    AdsVersion ads_version = {0,0,0};
    char device_info[16];

    ASSERT_NE(ads_conn->read_device_info(nullptr, sizeof(device_info), &ads_version), 0);
    ASSERT_NE(ads_conn->read_device_info(device_info, 0, &ads_version), 0);
    ASSERT_NE(ads_conn->read_device_info(device_info, sizeof(device_info), nullptr), 0);
}

TEST_F(ADSConnTest, ReadDeviceState) {
    uint16_t device_state = 0;
    ADSState ads_state;

    int rc = ads_conn->read_device_state(&device_state, &ads_state);
    ASSERT_EQ(rc, 0);
}

TEST_F(ADSConnTest, ReadDeviceStateInvalidParam) {
    uint16_t device_state = 0;
    ADSState ads_state;

    ASSERT_NE(ads_conn->read_device_state(nullptr, &ads_state), 0);
    ASSERT_NE(ads_conn->read_device_state(&device_state, nullptr), 0);
}


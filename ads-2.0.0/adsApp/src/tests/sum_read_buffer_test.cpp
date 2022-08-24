/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#include "gtest/gtest.h"
#include "Variable.h"
#include "SumReadBuffer.h"

TEST(ReadBufferTest, InvalidConstructorArguments) {
    EXPECT_THROW(SumReadBuffer(0), std::invalid_argument);
}

TEST(ReadBufferTest, BufferSizeIncreases) {
    auto buf = SumReadBuffer(5);
    ASSERT_EQ(buf.get_size(), (size_t)0);
    ASSERT_EQ(buf.get_num_variables(), (unsigned int)0);

    /* Adding a variable of size 10 should result in buffer size 14 bytes
     * because space for 4 byte result code is also reserved. */
    buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[10] R P=1 V=var")));
    ASSERT_EQ(buf.get_size(), (size_t)14);
    ASSERT_EQ(buf.get_num_variables(), (unsigned int)1);

    /* Adding another variable of size 2 should result in buffer size of 20
     * bytes because of previously reserved 14 bytes + 2 bytes for added
     * variable data and 4 bytes for added variable result code. */
    buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[2] R P=1 V=var")));
    ASSERT_EQ(buf.get_size(), (size_t)20);
    ASSERT_EQ(buf.get_num_variables(), (unsigned int)2);
}

TEST(ReadBufferTest, AtCapacitySize) {
    auto buf = SumReadBuffer(5, 5);
    ASSERT_EQ(buf.at_capacity(), false);
    EXPECT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[5] R P=1 V=v1"))), 0);
    ASSERT_EQ(buf.at_capacity(), true);
}

TEST(ReadBufferTest, AtCapacityNelem) {
    auto buf = SumReadBuffer(2, 100);
    ASSERT_EQ(buf.at_capacity(), false);
    EXPECT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[5] R P=1 V=v2"))), 0);
    EXPECT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[5] R P=1 V=v3"))), 0);
    ASSERT_EQ(buf.at_capacity(), true);
}

TEST(ReadBufferTest, CantAddAfterMaxDataSizeSoftLimit) {
    auto buf = SumReadBuffer(5, 10);
    EXPECT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[5] R P=1 V=v1"))), 0);
    EXPECT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[5] R P=1 V=v2"))), 0);
    ASSERT_NE(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[1] R P=1 V=v3"))), 0);
}

/* Data size soft limit can be exceeded */
TEST(ReadBufferTest, CanExceedSoftDataSizeLimit) {
    auto buf = SumReadBuffer(5, 10);
    EXPECT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[5] R P=1 V=v1"))), 0);
    ASSERT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[15] R P=1 V=v1"))), 0);
}

TEST(ReadBufferTest, MaxNumberOfVariables) {
    auto buf = SumReadBuffer(2);
    ASSERT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[1] R P=1 V=v1"))), 0);
    ASSERT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[1] R P=1 V=v3"))), 0);
    ASSERT_NE(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[1] R P=1 V=v3"))), 0);
}

TEST(ReadBufferTest, InitializeBuffer) {
    auto buf = SumReadBuffer(5);

    ASSERT_EQ(buf.is_initialized(), false);

    buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[1] R P=1 V=v1")));
    buf.initialize_buffer();

    ASSERT_EQ(buf.is_initialized(), true);
}

/* An empty buffer cannot be initialized. */
TEST(ReadBufferTest, CantInitializeEmptyBuffer) {
    auto buf = SumReadBuffer(5);
    ASSERT_NE(buf.initialize_buffer(), 0) << "Should not be able to initialize an empty buffer";
}

/* A buffer can only be initialized once. */
TEST(ReadBufferTest, CantDoubleInitializeBuffer) {
    auto buf = SumReadBuffer(5);
    buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[1] R P=1 V=v1")));
    ASSERT_EQ(buf.initialize_buffer(), 0);
    ASSERT_NE(buf.initialize_buffer(), 0) << "Should not be able to initialize buffer twice";
}

/* Variables can only be added while the buffer is not yet initialized. */
TEST(ReadBufferTest, CantAddVariablesAfterBufferInitialization) {
    auto buf = SumReadBuffer(5);
    buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[1] R P=1 V=v1")));
    buf.initialize_buffer();

    ASSERT_EQ(buf.is_initialized(), true) << "Expected an initialized buffer";
    ASSERT_NE(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[1] R P=1 V=v1"))), 0) << "Should not be able to add a variable to an initialized buffer";
}

TEST(ReadBufferTest, ReadData) {
    auto buf = SumReadBuffer(5);

    /* Add first element */
    int32_t first_data = 0;
    std::shared_ptr<Variable> first_var(new Variable("BYTE[4] R P=1 V=v1"));
    EXPECT_EQ(buf.add_variable(first_var), 0);

    /* Add second element of different type */
    int64_t second_data = 0;
    std::shared_ptr<Variable> second_var(new Variable("BYTE[8] R P=1 V=v2"));
    EXPECT_EQ(buf.add_variable(second_var), 0);

    /* Add third element */
    int16_t third_data = 0;
    std::shared_ptr<Variable> third_var(new Variable("BYTE[2] R P=1 V=v3"));
    EXPECT_EQ(buf.add_variable(third_var), 0);

    /* Initialize (allocate) the buffer */
    EXPECT_EQ(buf.initialize_buffer(), 0);

    /* Buffer is usually marked as valid by buffer writers, i.e. SumReadRequest
     * instances. If buffer is left invalid, reading it's data through Variable
     * instances fails. */
    buf.buffer_state = SumReadBuffer::SumReadBufferState::Valid;

    /* Manually add values to the buffer */
    buf.rwlock.lock_write();
    uint8_t *buffer = buf.get_buffer();
    EXPECT_NE(buffer, nullptr);

    /* Result codes are stored at the beginning of the buffer */
    uint32_t *results = (uint32_t *)buffer;
    *results = 0;
    results++;
    *results = 0;
    results++;
    *results = 1; /* Mark third_data read result as invalid (i.e. not zero) */

    /* Data elements are stored in the buffer after result codes */
    int32_t *first = (int32_t *)(results + 1);
    *first = 0X1A2B3C4D;
    int64_t *second = (int64_t *)(first + 1);
    *second = 0X123456789ABCDEF1;
    int16_t *third = (int16_t *)(second + 1);
    *third = 0xABCD;
    buf.rwlock.unlock_write();

    /* Assert that the values are retrieved as expected */
    int rc = first_var->read_from_buffer(sizeof(first_data), (uint8_t *)&first_data);
    ASSERT_EQ(rc, 0) << "Failed to read data from buffer";
    ASSERT_EQ(first_data, 0X1A2B3C4D);

    rc = second_var->read_from_buffer(sizeof(second_data), (uint8_t *)&second_data);
    ASSERT_EQ(rc, 0) << "Failed to read data from buffer";
    ASSERT_EQ(second_data, 0X123456789ABCDEF1);

    /* Reading the third value should fail, due to its result element being
     * non-zero */
    rc = third_var->read_from_buffer(sizeof(third_data), (uint8_t *)&third_data);
    ASSERT_NE(rc, 0) << "Reading should have failed due to non-zero result element";
}

TEST(ReadBufferTest, ReadInvalidDataFails) {
    auto buf = SumReadBuffer(5);

    uint32_t result;
    uint16_t data;
    EXPECT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[4] R P=1 V=v1"))), 0);
    EXPECT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[2] R P=1 V=v2"))), 0);
    EXPECT_EQ(buf.initialize_buffer(), 0);

    /* Because only two variables were added, specifying result offset '3'
     * should fail because it is out of range. */
    ASSERT_NE(buf.read_data(3, 0, sizeof(data), &result, (uint8_t *)&data), 0);

    /* The specified data offset is larger than the two variables added and
     * should fail because it is out of range. */
    ASSERT_NE(buf.read_data(0, sizeof(data) * 10, sizeof(data), &result, (uint8_t *)&data), 0);
}

TEST(ReadBufferTest, ReadBeforeBufferInitializedFails) {
    auto buf = SumReadBuffer(5);

    uint32_t result;
    uint16_t data;
    EXPECT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[2] R P=1 V=v1"))), 0);
    EXPECT_EQ(buf.add_variable(std::shared_ptr<Variable>(new Variable("BYTE[4] R P=1 V=v2"))), 0);

    /* Reading an uninitialized buffer should fail */
    ASSERT_NE(buf.read_data(0, 0, sizeof(data), &result, (uint8_t *)&data), 0);
}

TEST(ReadBufferTest, NotifyOnUpdateVariables) {
    auto buf = SumReadBuffer(5);
    auto var = std::make_shared<Variable>("BYTE[2] R P=1 V=v1", true);
    EXPECT_EQ(buf.add_variable(var), 0);
    EXPECT_EQ(buf.initialize_buffer(), 0);

    /* Manually add values to the buffer */
    int16_t *data = (int16_t *)buf.get_buffer_data_section();
    *data = 0x1F1F;

    /* Creates a copy and uses it to find variables that have been updated
     * between buffer writes */
    buf.save_buffer();

    /* Manually modifty values in the buffer */
    buf.rwlock.lock_write();
    data = (int16_t *)buf.get_buffer_data_section();
    *data = 0x2E2E;
    buf.rwlock.unlock_write();

    std::vector<std::shared_ptr<Variable>> updated_vars = buf.get_updated_variables();
    ASSERT_EQ(updated_vars.size(), (size_t)1);
}

TEST(ReadBufferTest, NotifyOnUpdatedOnlyOnEnabledVariables) {
    auto buf = SumReadBuffer(5);
    auto regular_var = std::make_shared<Variable>("BYTE[2] R P=1 V=v1", false);
    auto notify_var = std::make_shared<Variable>("BYTE[2] R P=1 V=v2", true);
    EXPECT_EQ(buf.add_variable(regular_var), 0);
    EXPECT_EQ(buf.add_variable(notify_var), 0);
    EXPECT_EQ(buf.initialize_buffer(), 0);

    /* Manually add values to the buffer */
    buf.rwlock.lock_write();
    int16_t *data = (int16_t *)buf.get_buffer_data_section();
    *data = 0x1F1F; /* First (non-notify) variable value */
    data++;
    *data = 0x2F2F; /* Second (notify) variable value */
    buf.rwlock.unlock_write();

    /* Creates a copy and uses it to find variables that have been updated
     * between buffer writes */
    buf.save_buffer();

    /* Modity values */
    buf.rwlock.lock_write();
    data = (int16_t *)buf.get_buffer_data_section();
    *data = 0x3C3C; /* First (non-notify) variable value */
    data++;
    *data = 0x4C4C; /* Second (notify) variable value */
    buf.rwlock.unlock_write();

    std::vector<std::shared_ptr<Variable>> updated_vars = buf.get_updated_variables();
    ASSERT_EQ(updated_vars.size(), (size_t)1);

    auto result = updated_vars[0];
    ASSERT_EQ(result, notify_var);
}

TEST(ReadBufferTest, VariableGetDataPointer) {
    auto buf = SumReadBuffer(5);
    auto first_var = std::make_shared<Variable>("BYTE[2] R P=1 V=v1");
    auto second_var = std::make_shared<Variable>("BYTE[2] R P=1 V=v2");
    buf.add_variable(first_var);
    buf.add_variable(second_var);
    EXPECT_EQ(buf.initialize_buffer(), 0);

    /* Buffer is usually marked as valid by buffer writers, i.e. SumReadRequest
     * instances. If buffer is left invalid, reading it's data throug Variable
     * instances fails. */
    buf.buffer_state = SumReadBuffer::SumReadBufferState::Valid;

    /* Manually add values to the buffer */
    buf.rwlock.lock_write();
    int16_t *data = (int16_t *)buf.get_buffer_data_section();
    *data = 0x1234;
    data++;
    *data = 0xABCD;
    buf.rwlock.unlock_write();

    std::pair<uint8_t *, RWLock *> first_var_data = first_var->get_read_data();
    ASSERT_NE(first_var_data.first, nullptr);
    ASSERT_EQ(*(int16_t *)first_var_data.first, (int16_t)0x1234);
    first_var_data.second->unlock_read();

    std::pair<uint8_t *, RWLock *> second_var_data = second_var->get_read_data();
    ASSERT_NE(second_var_data.first, nullptr);
    ASSERT_EQ(*(int16_t *)second_var_data.first, (int16_t)0xABCD);
    second_var_data.second->unlock_read();
}

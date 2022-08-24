/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#include "gtest/gtest.h"
#include "Connection.h"
#include "SumReadRequest.h"

TEST(SumReadRequestTest, InvalidConstructorArguments) {
    auto conn = std::make_shared<Connection>();

    EXPECT_THROW(SumReadRequest(0, nullptr), std::invalid_argument);
    EXPECT_THROW(SumReadRequest(0, conn), std::invalid_argument);
    EXPECT_THROW(SumReadRequest(5, nullptr), std::invalid_argument);
}

//TEST(SumReadRequestTest, Initialize) {
//    auto conn = std::make_shared<Connection>();
//    auto r = SumReadRequest(2, conn);
//
//    std::vector<std::shared_ptr<Variable>> vars;
//    vars.push_back(std::shared_ptr<Variable>(new Variable("INT R P=1 G=1 O=1")));
//    vars.push_back(std::shared_ptr<Variable>(new Variable("INT R P=1 G=1 O=2")));
//    vars.push_back(std::shared_ptr<Variable>(new Variable("INT R P=1 G=1 O=3")));
//    ASSERT_EQ(r.initialize(), 0);
//}

TEST(SumReadRequestTest, Allocate) {
    auto conn = std::make_shared<Connection>();
    auto r = SumReadRequest(2, conn);

    std::vector<std::shared_ptr<Variable>> vars;
    vars.push_back(std::shared_ptr<Variable>(new Variable("INT R P=1 G=1 O=1")));
    ASSERT_EQ(r.allocate(vars), 0);
}

TEST(SumReadRequestTest, CreatingAdditionalBuffers) {
    auto conn = std::make_shared<Connection>();
    auto r = SumReadRequest(2, conn);

    std::vector<std::shared_ptr<Variable>> vars;
    vars.push_back(std::shared_ptr<Variable>(new Variable("INT R P=1 G=1 O=1")));
    vars.push_back(std::shared_ptr<Variable>(new Variable("INT R P=1 G=1 O=2")));
    vars.push_back(std::shared_ptr<Variable>(new Variable("INT R P=1 G=1 O=3")));
    EXPECT_EQ(r.allocate(vars), 0);
    EXPECT_EQ(r.initialize(), 0);
    ASSERT_EQ(r.get_num_chunks(), 2);
}

TEST(SumReadRequestTest, InitializeFailsIfVarsUnresolved) {
    auto conn = std::make_shared<Connection>();
    auto r = SumReadRequest(2, conn);

    std::vector<std::shared_ptr<Variable>> vars;

    std::shared_ptr<Variable> var_resolved(new Variable("INT R P=1 G=1 O=1"));
    EXPECT_EQ(var_resolved->addr->is_resolved(), true);

    std::shared_ptr<Variable> var_unresolved(new Variable("INT R P=1 V=unresolved"));
    EXPECT_EQ(var_unresolved->addr->is_resolved(), false);

    vars.push_back(std::shared_ptr<Variable>(var_resolved));
    vars.push_back(std::shared_ptr<Variable>(var_unresolved));
    EXPECT_EQ(r.allocate(vars), 0);
    ASSERT_NE(r.initialize(), 0);
}

TEST(SumReadRequestTest, DeallocateClearsVariableBufferReader) {
    auto conn = std::make_shared<Connection>();
    auto r = SumReadRequest(2, conn);

    std::vector<std::shared_ptr<Variable>> vars;

    std::shared_ptr<Variable> var_resolved(new Variable("INT R P=1 G=1 O=1"));
    EXPECT_EQ(var_resolved->addr->is_resolved(), true);

    std::shared_ptr<Variable> var_unresolved(new Variable("INT R P=1 V=unresolved"));
    EXPECT_EQ(var_unresolved->addr->is_resolved(), false);

    vars.push_back(std::shared_ptr<Variable>(var_resolved));
    vars.push_back(std::shared_ptr<Variable>(var_unresolved));
    EXPECT_EQ(r.allocate(vars), 0);
    ASSERT_NE(var_resolved->get_buffer_reader(), EMPTY_BUFFER_DATA_POSITION);

    EXPECT_EQ(r.deallocate(), 0);
    ASSERT_EQ(var_resolved->get_buffer_reader(), EMPTY_BUFFER_DATA_POSITION);
}

TEST(SumReadRequestTest, CantDoubleInitialize) {
    auto conn = std::make_shared<Connection>();
    auto r = SumReadRequest(2, conn);

    std::vector<std::shared_ptr<Variable>> vars;
    vars.push_back(std::shared_ptr<Variable>(new Variable("INT R P=1 G=1 O=1")));
    EXPECT_EQ(r.allocate(vars), 0);
    EXPECT_EQ(r.initialize(), 0);
    EXPECT_NE(r.initialize(), 0);
}

TEST(SumReadRequestTest, CantInitializeWithoutAllocate) {
    auto conn = std::make_shared<Connection>();
    auto r = SumReadRequest(2, conn);

    std::vector<std::shared_ptr<Variable>> vars;
    vars.push_back(std::shared_ptr<Variable>(new Variable("INT R P=1 G=1 O=1")));
    ASSERT_NE(r.initialize(), 0);
}

/* Empty sum-read request instance is allocated e.g. when there are no input
 * records loaded for the port driver instance. */
TEST(SumReadRequestTest, CanAllocateEmpty) {
    auto conn = std::make_shared<Connection>();
    auto r = SumReadRequest(2, conn);

    std::vector<std::shared_ptr<Variable>> vars_empty;
    EXPECT_EQ(r.allocate(vars_empty), 0);
}

TEST(SumReadRequestTest, CantDoubleAllocate) {
    auto conn = std::make_shared<Connection>();
    auto r = SumReadRequest(2, conn);

    std::vector<std::shared_ptr<Variable>> vars;
    vars.push_back(std::shared_ptr<Variable>(new Variable("INT R P=1 G=1 O=1")));
    vars.push_back(std::shared_ptr<Variable>(new Variable("INT R P=1 G=1 O=2")));
    vars.push_back(std::shared_ptr<Variable>(new Variable("INT R P=1 G=1 O=3")));
    EXPECT_EQ(r.allocate(vars), 0);
    ASSERT_NE(r.allocate(vars), 0);
}

/* Empty sum-read request instance is allocated and initialized e.g. when there
 * are no input records loaded for the port driver instance. */
TEST(SumReadRequestTest, CanInitializeEmpty) {
    auto conn = std::make_shared<Connection>();
    auto r = SumReadRequest(2, conn);

    std::vector<std::shared_ptr<Variable>> vars_empty;
    EXPECT_EQ(r.allocate(vars_empty), 0);
    ASSERT_EQ(r.initialize(), 0);
}

// Can't test without writing data to buffers :|
//TEST(SumReadRequestTest, GetUpdatedVariables) {
//    Connection conn;
//    auto r = SumReadRequest(2, &conn);
//
//    std::vector<std::shared_ptr<Variable>> vars;
//    auto var_first_regular = std::make_shared<Variable>("INT R P=1 G=1 O=1", false);
//    auto var_second_notify = std::make_shared<Variable>("INT R P=1 G=1 O=2", true);
//    auto var_third_notify = std::make_shared<Variable>("INT R P=1 G=1 O=3", true);
//    auto var_fourth_regular = std::make_shared<Variable>("INT R P=1 G=1 O=4", false);
//
//    vars.push_back(var_first_regular);
//    vars.push_back(var_second_notify);
//    vars.push_back(var_third_notify);
//    vars.push_back(var_fourth_regular);
//
//    EXPECT_EQ(r.initialize(vars), 0);
//
//
//}

/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#include "gtest/gtest.h"
#include "BufferDataPosition.h"
#include "err.h"
#include "asynDriver.h"

TEST(BufferDataPositionTest, Equality) {
    BufferDataPosition first = {nullptr, 0, 0};
    BufferDataPosition second = {nullptr, 0, 0};
    ASSERT_EQ((first == second), true);
}

TEST(BufferDataPositionTest, Inequality) {
    SumReadBuffer b(1);
    BufferDataPosition first = {&b, 0, 0};
    BufferDataPosition second = {nullptr, 0, 0};
    ASSERT_NE((first == second), true);

    first = {nullptr, 1, 0};
    ASSERT_NE((first == second), true);

    first = {nullptr, 0, 1};
    ASSERT_NE((first == second), true);
}

TEST(LogTest, EpicsLogTest) {
    LOG_TRACE("trace log message");
    LOG_WARN("warning log message");
    LOG_ERR("error log message");
}

TEST(LogTest, AsynLogTest) {
    asynUser *asyn_user = pasynManager->createAsynUser(nullptr, nullptr);
    ASSERT_NE(asyn_user, nullptr);

    pasynTrace->setTraceMask(asyn_user, ASYN_TRACE_FLOW|ASYN_TRACE_WARNING|ASYN_TRACE_ERROR);

    // TODO: can't make TRACE and WARN to print out on console
    LOG_TRACE_ASYN(asyn_user, "asyn trace log message");
    LOG_WARN_ASYN(asyn_user, "asyn warning log message");
    LOG_ERR_ASYN(asyn_user, "asyn error log message");

    pasynManager->freeAsynUser(asyn_user);
}

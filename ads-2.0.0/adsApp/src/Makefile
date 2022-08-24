TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library
LIBRARY += ads

# install ads.dbd into <top>/dbd
DBD += ads.dbd

EPICS_ADS=$(TOP)/adsApp/src

USR_INCLUDES += -I$(TOP)/adsSup/AdsLib
USR_CXXFLAGS += -std=c++11

# Generate version header file
GENVERSION = ads_port_driver_version.h
GENVERSIONMACRO = ADS_PORT_DRIVER_VERSION

# specify all source files to be compiled and added to the library
SRC_DIRS += $(EPICS_ADS)/epics-ads
ads_SRCS += ADSPortDriver.cpp
ads_SRCS += ADSPVInfo.cpp
ads_SRCS += ioc_commands.cpp
ads_SRCS += ADSAddress.cpp
ads_SRCS += Connection.cpp
ads_SRCS += SumReadBuffer.cpp
ads_SRCS += Variable.cpp
ads_SRCS += SumReadRequest.cpp
ads_SRCS += RWLock.cpp
ads_SRCS += Types.cpp
ads_SRCS += err.cpp

# libpthread is needed for POSIX R/W locks used in RWLock.h
ads_SYS_LIBS += pthread

ads_LIBS += DynamicAsynPort
ads_LIBS += AdsLib-Linux
AdsLib-Linux_DIR += $(TOP)/adsSup

ads_LIBS += $(EPICS_BASE_IOC_LIBS)

# Build Google Test library
LIBRARY += googletest
SRC_DIRS += $(EPICS_ADS)/googletest
SRC_DIRS += $(EPICS_ADS)/googletest/src
SRC_DIRS += $(EPICS_ADS)/googletest/include
googletest_SRCS += gtest-all.cc

# Build test suites
PROD += tests
SRC_DIRS += $(EPICS_ADS)/tests

# libpthread is needed for POSIX R/W locks used in RWLock.h
tests_SYS_LIBS += pthread

tests_LIBS += googletest
tests_LIBS += AdsLib-Linux
tests_LIBS += Com
tests_LIBS += asyn

tests_SRCS += gtest_main.cc # Contains main()
tests_SRCS += ads_address_test.cpp
tests_SRCS += sum_read_buffer_test.cpp
tests_SRCS += ads_connection_test.cpp
tests_SRCS += sum_read_request_test.cpp
tests_SRCS += various_test.cpp

tests_OBJS += ADSAddress.o
tests_OBJS += Connection.o
tests_OBJS += Variable.o
tests_OBJS += SumReadBuffer.o
tests_OBJS += SumReadRequest.o
tests_OBJS += RWLock.o
tests_OBJS += Types.o
tests_OBJS += err.o

# install documentation into <top>/doc
DOCS += doc/EPICS_ADS_Architecture.docx
DOCS += doc/EPICS_ADS_Reference_manual.docx

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

# Explicit dependency needed for generated header file
ADSPortDriver$(DEP): $(COMMON_DIR)/$(GENVERSION)


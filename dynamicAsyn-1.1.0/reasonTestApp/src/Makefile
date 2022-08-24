TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = reasonTest
# reasonTest.dbd will be created and installed
DBD += reasonTest.dbd

# reasonTest.dbd will be made up from these files:
reasonTest_DBD += base.dbd

# Include dbd files from all support applications:
reasonTest_DBD += ads_reg.dbd
reasonTest_DBD += asyn.dbd

# Add all the support libraries needed by this IOC
#reasonTest_LIBS += xxx

# reasonTest_registerRecordDeviceDriver.cpp derives from reasonTest.dbd
reasonTest_SRCS += reasonTest_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OS.
reasonTest_SRCS_DEFAULT += reasonTestMain.cpp

reasonTest_SRCS += iocconfig.cpp

reasonTest_SRCS += ADSPVInfo.cpp
reasonTest_SRCS += ADSAsynPort.cpp
reasonTest_SRCS += HWStub.cpp
reasonTest_SRCS += HWBase.cpp
reasonTest_SRCS += TestADSAsynPort.cpp 


# Finally link to the EPICS Base libraries
reasonTest_LIBS += $(EPICS_BASE_IOC_LIBS)

reasonTest_LIBS += asyn
reasonTest_LIBS += DynamicAsynPort

USR_CFLAGS   += -std=c++1y
USR_CPPFLAGS += -std=c++1y

#USR_CFLAGS += -DDEBUG -g
#USR_CPPFLAGS += -DDEBUG -g

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE


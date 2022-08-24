TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

LIBRARY = DynamicAsynPort
# DynamicAsynPort.dbd will be created and installed
#DBD += DynamicAsynPort.dbd

# DynamicAsynPort.dbd will be made up from these files:
DynamicAsynPort_DBD += base.dbd

# Include dbd files from all support applications:
#DynamicAsynPort_DBD += xxx.dbd

INC += PVKeeper.h
INC += PVInfo.h
INC += DynamicAsynPortDriver.h
#INC += mp.h
#INC += AsynHandlersKeeper.h
INC += AsynReadWriteHandlers.h

# Add all the support libraries needed by this IOC
DynamicAsynPort_LIBS += asyn

USR_CFLAGS   += -std=c++1y -ggdb 
USR_CPPFLAGS += -std=c++1y -ggdb 

# Build the main IOC entry point on workstation OSs.
DynamicAsynPort_SRCS += PVKeeper.cpp 
DynamicAsynPort_SRCS += PVInfo.cpp
DynamicAsynPort_SRCS += DynamicAsynPortDriver.cpp


# Finally link to the EPICS Base libraries
DynamicAsynPort_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE


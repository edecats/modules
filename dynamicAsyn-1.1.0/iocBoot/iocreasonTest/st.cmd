#!../../bin/linux-x86_64/reasonTest

## You may have to change reasonTest to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/reasonTest.dbd"
reasonTest_registerRecordDeviceDriver pdbbase

epicsEnvSet("AsynPort",   "TestPort")


ADSConfigure("$(AsynPort)", "", 1);

#asynSetTraceMask "$(AsynPort)",       0, "0xFFFF"

## Load record instances
dbLoadRecords("db/DynamicAsynPortDriver.db", "PORT=$(AsynPort), ADDR=0, TIMEOUT=1, P=Test:, R=Dev:")

dbLoadRecords("db/reasonTestDB.db","PORT=$(AsynPort), ADDR=0, TIMEOUT=1")


cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=codac-devHost"

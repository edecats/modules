# DynamicAsynPortDriver

DynamicAsynPortDriver is an extension of the [asynPortDriver](https://epics.anl.gov/modules/soft/asyn/)

Goal of the Dynamic port is to simplify integration of the devices which has a lot of inputs/signals of different types,
which can be handled in a simmilar way. Initial development is inspired by the [ADS](https://github.com/Beckhoff/ADS) library 
which allows to communicate with a PLC through the TwinCAT protocol. 

DynamicAsynPortDriver allows to provide single entry point (function), which could be parameterized 
in a function like style.
To specify function in EPICS DB standard [INP/OUT](https://wiki-ext.aps.anl.gov/epics/index.php/RRM_3-14_Common) fields are used.

Example:
```
    field(OUT,  "@asyn($(PORT),$(ADDR),$(TIMEOUT)) readDigInp(1)")
```

Where:
readDigInp is a function.
1 is an argument of the function.

There is no need to hardcore and register PVs how it is usually done during asyn 
driver implementation. DynamicAsynPortDriver will find them and register automatically.

To handle readDigInp function in the driver developer has to register asyn handlers.

Example:
```
    asynInt32Handlers.registerReader("readDigInp",
        boost::bind( &HWInterface::readInt8,  adsInterface, _1, _2 ));
```

Registered handler will be called when ever the PV is processed. Routing to the registered 
functions is done by DynamicAsynPortDriver.

Handlers have signature similar to AsynPort, except first argument is a pointer to the [PVInfo]() 
class.

Example:
```
    virtual asynStatus writeInt32(PVInfo*, epicsInt32 value);
    virtual asynStatus readInt32 (PVInfo*, epicsInt32 *value);

    virtual asynStatus readFloat64Array(PVInfo*, epicsFloat64 *value, 
        size_t nElements, size_t *nIn)=0;
    virtual asynStatus writeFloat64Array(PVInfo*, epicsFloat64 *value, 
        size_t nElements)=0;
```

PVInfo class keeps reason information for the PV. Arguments of the reason are 
split and kept in the vector, so interface isn't hardcoded for a specific number of 
arguments. For details see PVInfo class documentation.


### Authors

* **Viacheslav Isaev** - [slava.isaev@gmail.com](mailto:slava.isaev@gmail.com)
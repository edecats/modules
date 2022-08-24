/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#ifndef ADSPVINFO_H
#define ADSPVINFO_H

#include <PVInfo.h>
#include <asynDriver.h>
#include <memory>
#include <AsynReadWriteHandlers.h>
#include <AdsLib.h>
#include <epics-ads/Variable.h>

class ADSPVInfo;

typedef std::shared_ptr<ADSPVInfo> ADSPVInfoPtr;
typedef std::shared_ptr<const ADSPVInfo> ADSPVInfoConstPtr;

typedef SharedPointerHolder<ADSPVInfo> PVInfoHolder;

class ADSPVInfo : public PVInfo {
  friend class ADSPortDriver;
  protected:
    bool first_init = true;
    asynUser *asyn_user;
    uint16_t interruptMask;
    std::shared_ptr<Variable> ads_variable;
    
    static unsigned int global_index_counter;

  public:
    const unsigned int global_index;

    ADSPVInfo(const std::string& reason) : PVInfo(reason), global_index(ADSPVInfo::global_index_counter++) { }
    virtual ~ADSPVInfo() {}

    asynUser *getAsynUsrPtr() { return asyn_user; }

    void set_ads_variable(const std::shared_ptr<Variable> variable) { this->ads_variable = variable; }
    const std::shared_ptr<Variable> get_ads_variable() { return ads_variable; }
};

#endif /* ADSPVINFO_H */

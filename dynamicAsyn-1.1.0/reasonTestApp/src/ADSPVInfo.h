/*
 * ADSPVInfo.h
 *
 *  Created on: 6. jul. 2018
 *      Author: sisaev
 */

#ifndef REASONTESTAPP_SRC_ADSPVINFO_H_
#define REASONTESTAPP_SRC_ADSPVINFO_H_

#include <asynPortDriver.h>

#include <PVInfo.h>
#include <AsynReadWriteHandlers.h>

class ADSPVInfo;

typedef std::shared_ptr<ADSPVInfo> ADSPVInfoPtr;
typedef std::shared_ptr<const ADSPVInfo> ADSPVInfoConstPtr;

typedef SharedPointerHolder<ADSPVInfo> PVInfoHolder;


class ADSPVInfo : public PVInfo
{
public:
	ADSPVInfo(const std::string& reason);
	virtual ~ADSPVInfo();
};

#endif /* REASONTESTAPP_SRC_ADSPVINFO_H_ */

/*
 * pv_info.h
 *
 *  Created on: 9. maj 2018
 *      Author: Vicheslav Isaev
 */

#ifndef PV_INFO_H_
#define PV_INFO_H_

#include <vector>
#include <string>
#include <memory>
#include <functional>

class PVInfo;
typedef std::shared_ptr<PVInfo> PVInfoPtr;
typedef std::shared_ptr<const PVInfo> PVInfoConstPtr;

/**
 *  @brief Keeps AsynReason.
 *  @details It is expected that reason is formated as a function signature.
 *  PVInfo parse the function signature and stores function name and the arguments
 *  separatelly.
 */
class PVInfo
{
public:
	PVInfo(const std::string& reason);
        PVInfo(const std::string& reason, const std::string& pvname);
	virtual ~PVInfo();

	//PVInfo(const PVInfo& ) = delete;
	//PVInfo & operator  = (const PVInfo& ) = delete;

        /**
	 *
	 * @return Returns pv name.
	 */
	const std::string& getPVName() const { return pvname; }

        /**
	 *
	 * Sets function name.
	 */
	void setFunctionName(const std::string& func) { reason = func; }

	/**
	 *
	 * @return Returns function name from the reason.
	 */
	const std::string& getFunctionName() const { return reason; }

	/**
	 *
	 * @return Returns full reason string as is.
	 */
	const std::string& getFullReason() const { return fullReason; }

	// asyn interrupt Id for doCallbacks
	int getId() const { return id; }

	void setId(int val) { id=val; }

	int getArgsNum() const { return Args.size(); };

	/**
	 * Returns string representation of requested argument.
	 * @param idx - Index of the argument
	 * @return string representation of the argument value.
	 * @throws In case index out of range.
	 */
	const std::string& getStrArg(size_t idx) const;

protected:
	int id;
	/// Vector of reason's arguments
	std::vector<std::string> Args;

	std::string reason;
	std::string fullReason;
    std::string pvname;

	void parseReason(const std::string& command);
};

#endif /* PV_INFO_H_ */

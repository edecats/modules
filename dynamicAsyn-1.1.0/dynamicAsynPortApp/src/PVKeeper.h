/*
 * PVKeeper.h
 *
 *  Created on: 9. maj 2018
 *      Author: Viacheslav isaev
 */

#ifndef PVKEEPER_H_
#define PVKEEPER_H_

#include <map>
#include <string>

#include <functional>
#include <cstring> 
#include <stdexcept>

/**
 * A class which holds dynamic PVs.
 * PV Id could be obtained through getPVId function.
 *
 * Object of the PVKeeper can be customized by the function which extract or compose names of the PVs.
 *
 */
template<class T>
class PVKeeper
{
public:

	typedef std::shared_ptr<T> TPtr;
	typedef std::shared_ptr<const T> TConstPtr;

	using PVMap =  std::map<std::string, TPtr>;
	using PVItr = typename PVMap::iterator;
	using GetKeyFunction = std::function<const std::string (const TConstPtr&)> ;

	/**
	 * Default constructor.
	 */
	PVKeeper();

	/**
	 * Parameterized Constructor.
	 * @param  nameExtractor - function to extract PV name from the reason.
	 */
	PVKeeper(GetKeyFunction);
	virtual ~PVKeeper();

	PVKeeper(const PVKeeper&) = delete;
	PVKeeper& operator = (const PVKeeper&) = delete;

	/**
	 * Sets initial value for the counter
	 * @param startCounter - initial value for the counter
	 */
	void setStartCounter(int startCounter);

	/**
	 * Creates a PVInfo object and registers it in PV database.
         *   ! Based on reason !
	 * @param name - name of the
	 * @return
	 */
	TPtr& registerPV (const std::string& reason);

        /**
	 * Creates a PVInfo object and registers it in PV database.
         *   ! Based on pv_name !
	 * @param name - name of the
	 * @return
	 */
	TPtr& registerPV (const std::string& reason, const std::string& pv_name);

	/**
	 * Finds a PV and returns asyn interrupt Id of the PV.
	 * Asyn interrupt Id is used for asynchronous PV notifications.
	 * @param name - Name of the PV in the databse.
	 * @return Returns asyn interrupt Id.
	 */
	int getPVId(const std::string& name);

	PVMap& getList(){ return pvlist; };

protected:
	PVMap pvlist;
	unsigned int pvcounter;
	GetKeyFunction getKey;

};

template<typename T>
PVKeeper<T>::PVKeeper()
	:pvcounter(0),
	 //getKey([](PVInfo* i) -> std::string { return i->Args[0]; })
	 getKey([](const TConstPtr& i) -> std::string { return i->getFullReason(); })
{

}

template<typename T>
PVKeeper<T>::PVKeeper(GetKeyFunction f)
:pvcounter(0),
 getKey(f)
{
}

template<typename T>
PVKeeper<T>::~PVKeeper()
{
	pvlist.clear();
}

template<typename T>
void PVKeeper<T>::setStartCounter(int startCounter)
{
	pvcounter = startCounter;
}

template<typename T>
std::shared_ptr<T>& PVKeeper<T>::registerPV (const std::string& reason)
{
	TPtr info(new T(reason));

	auto key = getKey( std::const_pointer_cast<const T>(info));

	auto itr = pvlist.find(key);

	if ( itr != pvlist.end() )
	{
		return itr->second;
	}else
	{
		++pvcounter;

		info->setId( pvcounter );

		pvlist.insert(std::make_pair(key, info ));
		return pvlist[key];
	}
}

template<typename T>
std::shared_ptr<T>& PVKeeper<T>::registerPV (const std::string& reason, const std::string& pv_name)
{
	TPtr info(new T(reason, pv_name));

	auto key = getKey( std::const_pointer_cast<const T>(info));

	auto itr = pvlist.find(key);

	if ( itr != pvlist.end() )
	{
		return itr->second;
	}else
	{
		++pvcounter;

		info->setId( pvcounter );

		pvlist.insert(std::make_pair(key, info ));
		return pvlist[key];
	}
}

template<typename T>
int PVKeeper<T>::getPVId(const std::string& name)
{
	PVItr itr = pvlist.find(name);

	if ( itr != pvlist.end() )
	{
		return itr->second->getId();
	}

	throw new std::out_of_range("Requested name is not found.");
}
#endif /* PVKEEPER_H_ */

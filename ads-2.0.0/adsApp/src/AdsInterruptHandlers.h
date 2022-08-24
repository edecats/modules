/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#ifndef ADSINTERRUPTHANDLERS_H
#define ADSINTERRUPTHANDLERS_H

#include <map>
#include <string>

template<typename P, typename I>
class AsynInterruptTypeHandlerKeeper
{
  public:
	template<typename ...Args>
	using Function = std::function<void (Args...)>;

	template<typename ...Args>
	using FunctionMap = std::map<std::string, Function<Args...>>;

	FunctionMap<std::shared_ptr<I>, P*, const void*> callbackFunctions;

	typedef typename FunctionMap<std::shared_ptr<I>, P*, const void*>::const_iterator  CallbackFunctionMapItr;

	~AsynInterruptTypeHandlerKeeper()
	{
		callbackFunctions.clear();
	}

	template<typename S, typename FR, typename FW, typename C>
	void reg(S&& name, FR r, C* c)
	{
		reg(name, r, c);
	}

	template<typename S, typename C, typename A1, typename A2, typename A3>
	void reg(S&& name, void(C::*f)(A1, A2*, A3*), C* c)
	{
		callbackFunctions[std::forward<S>(name)]=
				std::bind( f, c, std::placeholders::_1,
                                                 std::placeholders::_2,
                                                 std::placeholders::_3);
	}

	template<typename ...Loc>
	asynStatus invokeCallback(std::shared_ptr<I> info, Loc&& ... args) const
	{
		if (!info) return asynError;

		auto itr =
				callbackFunctions.find(info->getFunctionName());
		if ( itr != callbackFunctions.end() )
		{
			itr->second(info, std::forward<Loc>(args)...);
                        return asynSuccess;
		}
		return asynError;
	}
};

#endif /* ADSINTERRUPTHANDLERS_H */

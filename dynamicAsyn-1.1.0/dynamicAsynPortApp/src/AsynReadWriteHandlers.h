/*
 * type_handlers.hpp
 *
 *  Created on: 2018-05-10
 *      Author: Viacheslav Isaev
 */

#ifndef TYPE_HANDLERS_HPP_
#define TYPE_HANDLERS_HPP_

#include <map>
#include <string>
#include <typeindex>

template<typename T>
class SharedPointerHolder
{
public:
	typedef std::shared_ptr<T> TPtr;

	template<typename A>
	SharedPointerHolder(A&& o):
		object_pointer(std::forward<A>(o)){}

	TPtr& getPointer(){ return object_pointer; }

protected:
	TPtr object_pointer;
};


template<typename A, typename T>
class AsynSimpleTypeHandlerKeeper
{
public:
	typedef T EpicsType;

	typedef std::shared_ptr<A> P;
	typedef std::shared_ptr<const A> CP;

	using ReadFunction = std::function<asynStatus  (const P&,  T*)> ;
	using WriteFunction = std::function<asynStatus (const P&,  T)> ;

	typedef std::map<std::string, ReadFunction>  ReadFunctionMap;
	typedef std::map<std::string, WriteFunction> WriteFunctionMap;

	typedef std::map<std::string, ReadFunction>  ReadArrayFunctionMap;
	typedef std::map<std::string, WriteFunction> WriteArrayFunctionMap;

	ReadFunctionMap  readFunctions;
	WriteFunctionMap writeFunctions;

	~AsynSimpleTypeHandlerKeeper()
	{
		readFunctions.clear();
		writeFunctions.clear();
	}

	template<typename S, typename FR, typename FW, typename C>
	void registerReaderWriter(S&& name, FR r, FW w, C* c)
	{
		reg(name, r, c);
		reg(name, w, c);
	}

	template<typename S, typename FR, typename FW, typename C>
	void reg(S&& name, FR r, FW w, C* c)
	{
		reg(name, r, c);
		reg(name, w, c);
	}

	template<typename S, typename C, typename A1, typename A2>
	void reg(S&& name, asynStatus(C::*f)(A1, A2), C* c)
	{
		writeFunctions[std::forward<S>(name)]=
				std::bind( f, c, std::placeholders::_1, std::placeholders::_2 );
	}

	template<typename S, typename C, typename A1, typename A2>
	void reg(S&& name, asynStatus(C::*f)(A1, A2*), C* c)
	{
		readFunctions[std::forward<S>(name)]=
				std::bind( f, c, std::placeholders::_1, std::placeholders::_2 );
	}

	template<typename ...Loc>
	asynStatus invokeRead(const P& info, Loc&& ... args) const
	{
		if (!info) return asynError;

		auto itr =
				readFunctions.find(info->getFunctionName());
		if ( itr != readFunctions.end() )
		{
			return itr->second(info, std::forward<Loc>(args)...);
		}
		return asynError;
	}

	template<typename ...Loc>
	asynStatus invokeWrite(const P& info, Loc&& ... args) const
	{
		if (!info) return asynError;

		auto itr =
				writeFunctions.find( info->getFunctionName() );

		if ( itr != writeFunctions.end() )
		{
			return itr->second(info, std::forward<Loc>(args)...);
		}
		return asynError;
	}
};

template<typename A, typename T>
class AsynDigitalTypeHandlerKeeper
{
public:
	typedef T EpicsType;

	typedef std::shared_ptr<A> P;
	typedef std::shared_ptr<const A> CP;

	using ReadFunction = std::function<asynStatus  (const P&,  T*, T)> ;
	using WriteFunction = std::function<asynStatus (const P&,  T, T)> ;

	typedef std::map<std::string, ReadFunction>  ReadFunctionMap;
	typedef std::map<std::string, WriteFunction> WriteFunctionMap;

	typedef std::map<std::string, ReadFunction>  ReadArrayFunctionMap;
	typedef std::map<std::string, WriteFunction> WriteArrayFunctionMap;

	ReadFunctionMap  readFunctions;
	WriteFunctionMap writeFunctions;

	~AsynDigitalTypeHandlerKeeper()
	{
		readFunctions.clear();
		writeFunctions.clear();
	}

	template<typename S, typename FR, typename FW, typename C>
	void registerReaderWriter(S&& name, FR r, FW w, C* c)
	{
		reg(name, r, c);
		reg(name, w, c);
	}

	template<typename S, typename FR, typename FW, typename C>
	void reg(S&& name, FR r, FW w, C* c)
	{
		reg(name, r, c);
		reg(name, w, c);
	}

	template<typename S, typename C, typename A1, typename A2, typename A3>
	void reg(S&& name, asynStatus(C::*f)(A1, A2, A3), C* c)
	{
		writeFunctions[std::forward<S>(name)]=
				std::bind( f, c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
	}

	template<typename S, typename C, typename A1, typename A2, typename A3>
	void reg(S&& name, asynStatus(C::*f)(A1, A2*, A3), C* c)
	{
		readFunctions[std::forward<S>(name)]=
				std::bind( f, c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
	}

	template<typename ...Loc>
	asynStatus invokeRead(const P& info, Loc&& ... args) const
	{
		if (!info) return asynError;

		auto itr =
				readFunctions.find(info->getFunctionName());
		if ( itr != readFunctions.end() )
		{
			return itr->second(info, std::forward<Loc>(args)...);
		}
		return asynError;
	}

	template<typename ...Loc>
	asynStatus invokeWrite(const P& info, Loc&& ... args) const
	{
		if (!info) return asynError;

		auto itr =
				writeFunctions.find( info->getFunctionName() );

		if ( itr != writeFunctions.end() )
		{
			return itr->second(info, std::forward<Loc>(args)...);
		}
		return asynError;
	}
};

template<typename A, typename T>
class AsynOctetTypeHandlerKeeper
{
public:
	typedef T EpicsType;

	typedef std::shared_ptr<A> P;
	typedef std::shared_ptr<const A> CP;

	using ReadFunction = std::function<asynStatus  (const P&,  T*, size_t, size_t*, int*)> ;
	using WriteFunction = std::function<asynStatus (const P&,  const T*, size_t, size_t*)> ;

	typedef std::map<std::string, ReadFunction>  ReadFunctionMap;
	typedef std::map<std::string, WriteFunction> WriteFunctionMap;

	typedef std::map<std::string, ReadFunction>  ReadArrayFunctionMap;
	typedef std::map<std::string, WriteFunction> WriteArrayFunctionMap;

	ReadFunctionMap  readFunctions;
	WriteFunctionMap writeFunctions;

	~AsynOctetTypeHandlerKeeper()
	{
		readFunctions.clear();
		writeFunctions.clear();
	}

	template<typename S, typename FR, typename FW, typename C>
	void registerReaderWriter(S&& name, FR r, FW w, C* c)
	{
		reg(name, r, c);
		reg(name, w, c);
	}

	template<typename S, typename FR, typename FW, typename C>
	void reg(S&& name, FR r, FW w, C* c)
	{
		reg(name, r, c);
		reg(name, w, c);
	}

	template<typename S, typename C, typename A1, typename A2, typename A3, typename A4>
	void reg(S&& name, asynStatus(C::*f)(A1, const A2*, A3, A4*), C* c)
	{
		writeFunctions[std::forward<S>(name)]=
				std::bind( f, c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 );
	}

	template<typename S, typename C, typename A1, typename A2, typename A3, typename A4, typename A5>
	void reg(S&& name, asynStatus(C::*f)(A1, A2*, A3, A4*, A5*), C* c)
	{
		readFunctions[std::forward<S>(name)]=
				std::bind( f, c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5 );
	}

	template<typename ...Loc>
	asynStatus invokeRead(const P& info, Loc&& ... args) const
	{
		if (!info) return asynError;

		auto itr =
				readFunctions.find(info->getFunctionName());
		if ( itr != readFunctions.end() )
		{
			return itr->second(info, std::forward<Loc>(args)...);
		}
		return asynError;
	}

	template<typename ...Loc>
	asynStatus invokeWrite(const P& info, Loc&& ... args) const
	{
		if (!info) return asynError;

		auto itr =
				writeFunctions.find( info->getFunctionName() );

		if ( itr != writeFunctions.end() )
		{
			return itr->second(info, std::forward<Loc>(args)...);
		}
		return asynError;
	}
};

template<typename A, typename T>
class AsynArrayTypeHandlerKeeper
{
public:
	typedef T EpicsType;
	typedef std::shared_ptr<A> P;
	typedef std::shared_ptr<const A> CP;

	template<typename ...Args>
	using Function = std::function<asynStatus (const P&, Args...)>;

	typedef Function<T*, size_t, size_t*> ReadArrayFunction;
	typedef Function<T*, size_t> WriteArrayFunction;

	template<typename ...Args>
	using FunctionMap = std::map<std::string, Function<Args...>>;

	FunctionMap<T*, size_t, size_t*> readFunctions;
	FunctionMap<T*, size_t> writeFunctions;

	typedef typename FunctionMap<T*, size_t, size_t*>::const_iterator  ReadFunctionMapItr;
	typedef typename FunctionMap<T*, size_t>::const_iterator WriteFunctionMapItr;

	~AsynArrayTypeHandlerKeeper()
	{
		readFunctions.clear();
		writeFunctions.clear();
	}

	template<typename S, typename FR, typename FW, typename C>
	void reg(S&& name, FR r, FW w, C* c)
	{
		reg(name, r, c);
		reg(name, w, c);
	}

	template<typename S, typename C, typename A1, typename A2, typename A3, typename A4>
	void reg(S&& name, asynStatus (C::*f)(A1, A2, A3, A4), C* c)
	{
		readFunctions[std::forward<S>(name)] =
				std::bind( f, c, std::placeholders::_1,
						std::placeholders::_2,
						std::placeholders::_3,
						std::placeholders::_4);
	}

	template<typename S, typename C, typename A1, typename A2, typename A3>
	void reg(S&& name, asynStatus (C::*f)(A1, A2, A3), C* c)
	{
		writeFunctions[std::forward<S>(name)]=
				std::bind( f, c, std::placeholders::_1,
						std::placeholders::_2,
						std::placeholders::_3);
	}


	template<typename ...Args>
	asynStatus invokeRead(const P& info, Args&& ... args) const
	{
		if (!info) return asynError;
		auto itr =
				readFunctions.find(info->getFunctionName());
		if ( itr != readFunctions.end() )
		{
			return itr->second(info, std::forward<Args>(args)... );
		}
		return asynError;
	}

	template<typename ...Args>
	asynStatus invokeWrite(const P& info, Args&& ... args) const
	{
		if (!info) return asynError;
		auto itr =
				writeFunctions.find( info->getFunctionName() );

		if ( itr != writeFunctions.end() )
		{
			return itr->second(info, std::forward<Args>(args)...);
		}
		return asynError;
	}

};

//template<typename T>
//using AsynInt32HandlerKeeper = AsynSimpleTypeHandlerKeeper<T, epicsInt32>;
//
//template<typename T>
//using AsynFloat64HandlerKeeper = AsynSimpleTypeHandlerKeeper<T, epicsFloat64>;
//
//template<typename T>
//using AsynArrayInt8HandlerKeeper = AsynArrayTypeHandlerKeeper<T, epicsInt8>;
//
//template<typename T>
//using AsynArrayInt16HandlerKeeper = AsynArrayTypeHandlerKeeper<T, epicsInt16>;
//
//template<typename T>
//using AsynArrayInt32HandlerKeeper =  AsynArrayTypeHandlerKeeper<T, epicsInt32>;
//
//template<typename T>
//using AsynArrayFloat32HandlerKeeper = AsynArrayTypeHandlerKeeper<T, epicsFloat32>;
//
//template<typename T>
//using AsynArrayFloat64HandlerKeeper = AsynArrayTypeHandlerKeeper<T, epicsFloat64> ;


#endif /* TYPE_HANDLERS_HPP_ */


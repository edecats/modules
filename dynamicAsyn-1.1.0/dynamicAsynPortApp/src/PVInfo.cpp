/*
 * pv_info.cpp
 *
 *  Created on: 9. maj 2018
 *      Author: Viacheslav Isaev
 */

#include <string>
#include <stdexcept>

#include <errlog.h> /* errlogPrintf */

#include "PVInfo.h"

const std::string whiteSpaces( " \f\n\r\t\v" );

void trimRight( std::string& str,
      const std::string& trimChars = whiteSpaces )
{
   std::string::size_type pos = str.find_last_not_of( trimChars );
   str.erase( pos + 1 );
}

void trimLeft( std::string& str,
      const std::string& trimChars = whiteSpaces )
{
   std::string::size_type pos = str.find_first_not_of( trimChars );
   str.erase( 0, pos );
}

std::string trim(const std::string& str, const std::string& trimChars = whiteSpaces )
{
   std::string result(str);
   trimRight( result, trimChars );
   trimLeft( result, trimChars );
   return result;
}

void PVInfo::parseReason(const std::string& command)
{
    size_t idxStart = command.find(" ");
    size_t idxEnd = command.length();

	if ( idxStart != std::string::npos && idxStart > 0 ) {
        reason = command.substr(0, idxStart);
        if (reason.find("[") != std::string::npos) {
            reason = reason.substr(0, reason.find("["));
        }

		{
			std::string arguments = command.substr(idxStart, std::string::npos);
			idxStart=0;
			size_t length = arguments.length();
			size_t found  = arguments.find_first_of(" ");

			while ( found != std::string::npos )
			{
				idxEnd = found+1;
				length = found - idxStart;

				Args.push_back( trim(arguments.substr(idxStart, length)) );

				found  = arguments.find_first_of(" ", found+1);
				idxStart = idxEnd;
			}

			length = arguments.length() - idxStart;
			Args.push_back( trim(arguments.substr(idxStart, length)) );
		}
	}
}

PVInfo::PVInfo(const std::string& val):
		fullReason(val)
{
	parseReason( fullReason );
}

PVInfo::PVInfo(const std::string& val, const std::string& pvname):
		fullReason(val), pvname(pvname)
{
	parseReason( fullReason );
}

PVInfo::~PVInfo()
{
}

const std::string& PVInfo::getStrArg(size_t idx) const
{
	if (idx >= Args.size())
	{
		throw std::out_of_range("Index is out of range.");
	}
	return Args[idx];
}


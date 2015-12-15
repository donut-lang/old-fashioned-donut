/*
 * EmulatorException.h
 *
 *  Created on: 2011/08/23
 *      Author: psi
 */

#pragma once

#include <iostream>
#include <string>
#include <sstream>

namespace nes {

class EmulatorException
{
public:
	EmulatorException();
	EmulatorException(const char* fmsg);
	EmulatorException(std::string const& fmsg);
	EmulatorException(EmulatorException const& src);
	~EmulatorException();
private:
	std::stringstream msg;
public:
	const std::string getMessage() const;
	template<typename T> EmulatorException& operator<<(T& val)
	{
		this->msg << val;
		return *this;
	}
};

}

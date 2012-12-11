#include "EmulatorException.h"

namespace nes {

EmulatorException::EmulatorException()
{
}
EmulatorException::EmulatorException(const char* fmsg)
{
	this->msg << fmsg;
}
EmulatorException::EmulatorException(std::string const& fmsg)
{
	this->msg << fmsg;
}

EmulatorException::EmulatorException(EmulatorException const& src)
{
	this->msg << src.msg.str();
}
EmulatorException::~EmulatorException()
{

}

const std::string EmulatorException::getMessage() const
{
	return this->msg.str();
}

}

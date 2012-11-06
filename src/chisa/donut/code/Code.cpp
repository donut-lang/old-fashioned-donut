/**
 * Chisa
 * Copyright (C) 2012 psi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Code.h"
#include "Closure.h"

namespace chisa {
namespace donut {

Code::Code()
{
	// TODO Auto-generated constructor stub

}

template<>
unsigned int Code::constCode<bool>(bool const& val)
{
	return Inst::ConstBool;
}

template<>
unsigned int Code::constCode<int>(int const& val)
{
	return Inst::ConstInt;
}

template<>
unsigned int Code::constCode<float>(float const& val)
{
	return Inst::ConstFloat;
}

template<>
unsigned int Code::constCode<Handler<donut::Closure> >(Handler<donut::Closure> const& val)
{
	return Inst::ConstClosure;
}

template<>
unsigned int Code::constCode<std::string>(std::string const& val)
{
	return Inst::ConstString;
}

}}

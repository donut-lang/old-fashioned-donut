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

#include "XArchiver.h"
#include "StringUtil.h"

namespace chisa {
namespace util {

XArchiver::XArchiver()
:array_(new XArray)
,decode_now_(false)
,count_(0)
{
}

XArchiver::XArchiver(XValue const& val)
:array_(val.as<XArray>())
,decode_now_(true)
,count_(0)
{

}

}}


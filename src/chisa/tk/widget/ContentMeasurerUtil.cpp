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

#include "ContentMeasurerUtil.h"
#include "../../util/Regex.h"

namespace chisa {
namespace tk {
namespace widget {

std::string shrinkSpace(const std::string& str)
{
	std::regex reg(R"delimiter(\s+)delimiter");
	std::string out;
	std::string replacement(" ");
	return std::regex_replace(str, reg, replacement);
}

}}}


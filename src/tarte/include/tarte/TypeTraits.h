/**
 * Tarte
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

#pragma once
#include <cstddef>

namespace tarte {

template <typename Derived, typename Base>
struct IsBaseOf{
private:
	typedef char YES;
	typedef struct {
		char v[2];
	} NO;
	static YES check(const Base*);
	static NO check(...);
	static Derived* d;
public:
	static const bool result = (sizeof(check(d)) == sizeof(YES));
};

}

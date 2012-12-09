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

#pragma once
#include "../../Handler.h"
#include "../object/Object.h"
#include "../native/Bind.h"

namespace chisa {
namespace donut {

class NativeClosureEntry : public HandlerBody<NativeClosureEntry> {
public:
	NativeClosureEntry() = default;
	virtual ~NativeClosureEntry() noexcept = default;
public:
	inline bool onFree() noexcept { return false; };
	virtual Handler<NativeClosureObject> createObject( const Handler<Heap>& heap, const std::string& objectProviderName, const std::string& closureName ) = 0;
};

//-----------------------------------------------------------------------------

class PureNativeClosureEntry : public NativeClosureEntry {
public:
	typedef std::function<Handler<Object>(const Handler<Heap>& heap, const Handler<Object>& self, const Handler<DonutObject>& arg)> Signature;
private:
	PureNativeClosureEntry::Signature func_;
public:
	template <typename T>
	PureNativeClosureEntry(T f):NativeClosureEntry(), func_( native::createBind(f) ){}
	virtual ~PureNativeClosureEntry() noexcept {};
	virtual Handler<NativeClosureObject> createObject( const Handler<Heap>& heap, const std::string& objectProviderName, const std::string& closureName ) override;
};

}}

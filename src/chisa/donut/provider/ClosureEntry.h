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
#include "../object/ObjectBase.h"

namespace chisa {
namespace donut {

class ClosureEntry : public HandlerBody<ClosureEntry> {
public:
	ClosureEntry();
	virtual ~ClosureEntry() noexcept = default;
public:
	void onFree() noexcept { delete this; }
	virtual Handler<Object> createObject( World* const world ) = 0;
};

//-----------------------------------------------------------------------------

class PureClosureEntry : public ClosureEntry {
public:
	typedef std::function<Handler<Object>(Handler<Object> self, Handler<BaseObject> arg)> Signature;
private:
	PureClosureEntry::Signature func_;
public:
	PureClosureEntry(PureClosureEntry::Signature func);
	virtual ~PureClosureEntry() noexcept {};
	virtual Handler<Object> createObject( World* const world ) override;
};

}}

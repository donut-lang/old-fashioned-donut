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
#include "code/Code.h"
#include "object/Heap.h"
#include "vm/Machine.h"
#include "../logging/Logger.h"
#include "../Handler.h"
#include "Clock.h"

namespace chisa {
namespace donut {

class Donut {
	DEFINE_MEMBER_REF(public, logging::Logger, log);
private:
	Handler<Clock> clock_;
	Handler<Source> source_;
	Handler<Heap> heap_;
	std::map<std::string, Handler<Machine> > machines_;
public:
	Donut(logging::Logger& log);
	virtual ~Donut() noexcept = default;
public:
	Handler<Machine> queryMachine( const std::string& name = "" );
	void registerGlobalObject( const std::string& name, const Handler<Object>& obj );
	bool existsGlobalObject( const std::string& name );
	Handler<Object> readGlobalObject( const std::string& name );
};

}}


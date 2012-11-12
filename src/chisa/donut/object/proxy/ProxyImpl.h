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
#include "../../../util/ClassUtil.h"
#include "../Object.h"
#include "../World.h"
#include "../../native/PureNativeClosure.h"
#include "Proxy.h"

namespace chisa {
namespace donut {
class World;

//XXX: 具体的なworldの実装が必要なのでここに書いてる。醜すぎる。
template <typename... Args>
void Proxy::registerClosure(const std::string& name, std::function<Object*(Args... args)> f)
{
	this->closureMap_.insert(std::pair<std::string, Handler<PureNativeClosure> >(
			name,
			world()->create<PureNativeClosure>( f )
			));
};

}}

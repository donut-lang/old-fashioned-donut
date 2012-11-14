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
#include "ClosureEntry.h"

namespace tinyxml2{
class XMLDocument;
class XMLElement;
}

namespace chisa {
namespace donut {
class ProviderManager;

class Provider : public HandlerBody<Provider> {
private:
	World* const world_;
	std::string const name_;
public:
	Provider( World* const world, const std::string& name );
	virtual ~Provider() noexcept = default;
public:
	inline void onFree() noexcept { delete this; }
	inline std::string name() const noexcept { return this->name_; };
	inline World* world() const noexcept { return this->world_; };
public:
	virtual tinyxml2::XMLElement* serialize( tinyxml2::XMLDocument* doc, Handler<Object> obj ) = 0;
	virtual Handler<Object> deserialize( tinyxml2::XMLElement* xml ) = 0;
};

//-----------------------------------------------------------------------------

class ObjectProvider : public Provider {
private:
	std::map<std::string, Handler<ClosureEntry> > closureEntry_;
public:
	ObjectProvider( World* const world, const std::string& name );
	virtual ~ObjectProvider() noexcept = default;
protected:
	void registerPureNativeClosure( const std::string& name, PureClosureEntry::Signature func );
public:
	Handler<ClosureEntry> getClosure( const std::string& name );
	bool haveClosure( const std::string& name );
	Handler<BaseObject> createPrototype( Handler<Object> ptoro );
};

//-----------------------------------------------------------------------------

class ClosureProvider : public Provider {
private:
	HandlerW<ProviderManager> manager_;
public:
	ClosureProvider(World* const world, HandlerW<ProviderManager> mgr):Provider(world, "__native_closure__"),manager_(mgr){};
	virtual ~ClosureProvider() noexcept = default;
public:
	virtual tinyxml2::XMLElement* serialize( tinyxml2::XMLDocument* doc, Handler<Object> obj ) override;
	virtual Handler<Object> deserialize( tinyxml2::XMLElement* xml ) override;
};

}}

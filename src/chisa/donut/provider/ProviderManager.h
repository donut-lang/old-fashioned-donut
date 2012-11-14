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
#include "Provider.h"

namespace chisa {
namespace donut {

class ProviderManager : public HandlerBody<ProviderManager> {
private:
	std::map<std::string, Handler<Provider> > providerMap_;
public:
	ProviderManager();
	virtual ~ProviderManager() noexcept = default;
public:
	void registerProvider( Handler<Provider> provider );
	Handler<Provider> getProvider( const std::string& name );
	bool haveProvider( const std::string& name );
public:
	void onFree() noexcept {delete this;}
	tinyxml2::XMLElement* serialize( World* world, tinyxml2::XMLDocument* doc, Handler<Object> obj );
	Handler<Object> deserialize( World* world, tinyxml2::XMLElement* xml );
};

}}

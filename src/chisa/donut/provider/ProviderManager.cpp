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

#include <tinyxml2.h>
#include "ProviderManager.h"
#include "Provider.h"
#include "../Exception.h"

namespace chisa {
namespace donut {

ProviderManager::ProviderManager()
{
	this->registerProvider( Handler<Provider>(new ClosureProvider( this->self() )) );
}

void ProviderManager::registerProvider( Handler<Provider> provider )
{
	if(haveProvider( provider->name() )){
		this->providerMap_.erase( provider->name() );
	}
	this->providerMap_.insert( std::pair<std::string, Handler<Provider> >(provider->name(), provider) );
}
Handler<Provider> ProviderManager::getProvider( const std::string& name )
{
	auto it = this->providerMap_.find(name);
	if(it != this->providerMap_.end()){
		return it->second;
	}
	return Handler<Provider>();
}
bool ProviderManager::haveProvider( const std::string& name )
{
	return this->providerMap_.find(name) != this->providerMap_.end();
}

tinyxml2::XMLElement* ProviderManager::serialize( World* world, tinyxml2::XMLDocument* doc, Handler<Object> obj )
{
	std::string providerName = obj->providerName(world);
	if( !haveProvider(providerName) ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. there is no provider for %s", providerName.c_str());
	}
	Handler<Provider> provider = this->getProvider( providerName );
	tinyxml2::XMLElement* elem = provider->serialize( world, doc, obj );
	tinyxml2::XMLElement* wrapper = doc->NewElement("obj");
	wrapper->SetAttribute( "provider", providerName.c_str() );
	wrapper->InsertFirstChild( elem );
	return wrapper;
}

Handler<Object> ProviderManager::deserialize( World* world, tinyxml2::XMLElement* xml )
{
	if( std::string("obj") != xml->Name() ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. wrong element name: %s != \"obj\"", xml->Name());
	}
	std::string providerName( xml->Attribute("provider") );
	if(providerName.empty()){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. save data is broken.");
	}else if( !haveProvider(providerName) ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. there is no provider for %s", providerName.c_str());
	}
	tinyxml2::XMLElement* elem = xml->FirstChildElement();
	return this->getProvider(providerName)->deserialize( world, elem);
}

}}

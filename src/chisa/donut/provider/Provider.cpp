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

#include "Provider.h"
#include "ProviderManager.h"
#include "../native/NativeClosure.h"
#include "../Exception.h"
#include <tinyxml2.h>

namespace chisa {
namespace donut {

Provider::Provider( const std::string& name )
:name_(name)
{

}

//-----------------------------------------------------------------------------

ObjectProvider::ObjectProvider( const std::string& name )
:Provider(name)
{

}

void ObjectProvider::registerPureNativeClosure( const std::string& name, PureClosureEntry::Signature func )
{
	this->closureEntry_.insert(
			std::pair<std::string,Handler<ClosureEntry> >(
					name, Handler<ClosureEntry>(new PureClosureEntry(func)) ) );
}

Handler<ClosureEntry> ObjectProvider::getClosure( const std::string& name )
{
	if(haveClosure(name)){
		return this->closureEntry_.find(name)->second;
	}
	return Handler<ClosureEntry>();
}

bool ObjectProvider::haveClosure( const std::string& name )
{
	return this->closureEntry_.find(name) != this->closureEntry_.end();
}


//-----------------------------------------------------------------------------

tinyxml2::XMLElement* ClosureProvider::serialize( World* world, tinyxml2::XMLDocument* doc, Handler<Object> obj_ )
{
	Handler<NativeClosure> obj = obj_.cast<NativeClosure>();
	tinyxml2::XMLElement* elm = doc->NewElement("clos");
	elm->SetAttribute("objectProvider", obj->objectProviderName().c_str());
	elm->SetAttribute("closureName", obj->closureName().c_str());
	return elm;
}
Handler<Object> ClosureProvider::deserialize( World* world, tinyxml2::XMLElement* xml )
{
	if( std::string("clos") != xml->Name() ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. wrong element name: %s != \"clos\"", xml->Name());
	}
	std::string objectProviderName( xml->Attribute("objectProvider") );
	std::string closureName( xml->Attribute("closureName") );
	if(objectProviderName.empty() || closureName.empty()){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. save data is broken.");
	}
	Handler<ProviderManager> mgr = this->manager_.lock();
	if(!mgr){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. ProviderManager is already dead.");
	}
	if( !mgr->haveProvider(objectProviderName) ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. there is no provider for %s", objectProviderName.c_str());
	}
	Handler<ObjectProvider> objProvider = mgr->getProvider( objectProviderName ).tryCast<ObjectProvider>();
	if( !objProvider ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. %s is not object provider", objectProviderName.c_str());
	}
	if( !objProvider->haveClosure(closureName) ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. %s does not have closure: %s", objectProviderName.c_str(), closureName.c_str());
	}
	return objProvider->getClosure(closureName)->createObject( world, objectProviderName, closureName );
}

}}

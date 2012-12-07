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
#include "../object/NativeObject.h"
#include "../object/Heap.h"
#include "../Exception.h"
#include <tinyxml2.h>

namespace chisa {
namespace donut {

Provider::Provider( const Handler<Heap>& heap, const std::string& name )
:heap_(heap), name_(name)
{
	this->prototype_ = heap->createEmptyDonutObject();
}

void Provider::addPrototype( const std::string& name, Handler<NativeClosureEntry> clos )
{
	if(Handler<Heap> heap = this->heap().lock()){
		this->prototype_->store(heap, name, clos->createObject(heap, this->name(), name));
	}
}

util::XValue Provider::save(Handler<Object> const& obj)
{
	using namespace chisa::util;
	if( obj->isObject() ){
		if( Handler<NativeClosureObject> cobj = obj.tryCast<NativeClosureObject>() ) {
			Handler<XObject> val(new XObject);
			val->set("type","closure");
			val->set("closureName", cobj->closureName());
			return val;
		}
	}
	Handler<XObject> val(new XObject);
	val->set("type","object");
	val->set("impl", this->saveImpl(obj));
	return val;
}
Handler<Object> Provider::load(util::XValue const& data)
{
	using namespace chisa::util;
	Handler<XObject> val(data.as<XObject>());
	std::string const type( val->get<XString>("type") );
	if( type == "closure" ){

	}else if( type=="object" ){
		return this->loadImpl( val->get<XValue>("impl") );
	}else{
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object type: %s", type.c_str());
	}
}

//tinyxml2::XMLElement* Provider::serialize( tinyxml2::XMLDocument* doc, Handler<Object> obj_ )
//{
//			Handler<NativeClosure> obj = obj_.cast<NativeClosure>();
//			tinyxml2::XMLElement* elm = doc->NewElement("clos");
//			elm->SetAttribute("objectProvider", obj->objectProviderName().c_str());
//			elm->SetAttribute("closureName", obj->closureName().c_str());
//			return elm;
//}
//Handler<Object> Provider::deserialize( tinyxml2::XMLElement* xml )
//{
//	if( std::string("clos") != xml->Name() ){
//		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. wrong element name: %s != \"clos\"", xml->Name());
//	}
//	std::string objectProviderName( xml->Attribute("objectProvider") );
//	std::string closureName( xml->Attribute("closureName") );
//	if(objectProviderName.empty() || closureName.empty()){
//		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. save data is broken.");
//	}
//	Handler<ProviderManager> mgr = this->manager_.lock();
//	if(!mgr){
//		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. ProviderManager is already dead.");
//	}
//	if( !mgr->haveProvider(objectProviderName) ){
//		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. there is no provider for %s", objectProviderName.c_str());
//	}
//	Handler<ObjectProvider> objProvider = mgr->getProvider( objectProviderName ).tryCast<ObjectProvider>();
//	if( !objProvider ){
//		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. %s is not object provider", objectProviderName.c_str());
//	}
//	if( !objProvider->haveClosure(closureName) ){
//		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. %s does not have closure: %s", objectProviderName.c_str(), closureName.c_str());
//	}
//	return objProvider->getClosure(closureName)->createObject( heap(), objectProviderName, closureName );
//}

}}

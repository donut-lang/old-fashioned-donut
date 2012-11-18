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

#include "World.h"

namespace chisa {
namespace donut {

const static std::string TAG("World");

World::World(logging::Logger& log)
:log_(log)
,code_(new Code)
,timestamp_(0)
,nowTime_(0)
,objectId_(0)
,walkColor_(0)
,donutObjectProvider_()
,boolProvider_()
,intProvider_()
,nullProvider_()
{
	this->bootstrap();
}

World::~World() noexcept
{
	for(HeapObject* obj : this->objectPool_){
		obj->erase();
	}
	(decltype(this->objectPool_)()).swap(this->objectPool_);
}

void World::bootstrap()
{
	this->donutObjectProvider_ = Handler<DonutObjectProvider>( new DonutObjectProvider(this) );
	this->boolProvider_ = Handler<BoolProvider>(new BoolProvider(this));
	this->intProvider_ = Handler<IntProvider>(new IntProvider(this));
	this->nullProvider_ = Handler<NullProvider>(new NullProvider(this));

	this->registerProvider( this->donutObjectProvider() );
	this->registerProvider( this->boolProvider() );
	this->registerProvider( this->intProvider() );
	this->registerProvider( this->nullProvider() );
	this->registerProvider(Handler<Provider>( new FloatProvider(this) ));
	this->registerProvider(Handler<Provider>( new StringProvider(this) ));

	this->objectProto_ = this->donutObjectProvider()->prototype();
	this->intProto_ = this->intProvider()->prototype();
	this->boolProto_ = this->boolProvider()->prototype();
	this->nullProto_ = this->nullProvider()->prototype();

	this->globalObject_ = this->createEmptyDonutObject();
	this->globalObject_->store(this, "Object", this->objectProto());
	this->globalObject_->store(this, "Int", this->intProto());
	this->globalObject_->store(this, "Boolean", this->boolProto());
	this->globalObject_->store(this, "Null", this->nullProto());
}

tinyxml2::XMLElement* World::serialize(tinyxml2::XMLDocument* doc)
{

}

void World::deserialize(tinyxml2::XMLElement* xml)
{

}

unsigned int World::nextTimestamp()
{
	return ++this->timestamp_;
}

int World::nextWalkColor()
{
	return ++this->walkColor_;
}

Handler<Provider> World::getProvider( const std::string& name ) const
{
	auto it = this->providers_.find(name);
	if(it != this->providers_.end()){
		return it->second;
	}
	return Handler<Provider>();
}

void World::registerObject( const Handler<HeapObject>& obj )
{
	obj->id(++this->objectId_);
	this->objectPool_.push_back(obj.get());
	if( this->objectPool_.size() > 10 ) {
		this->gc();
	}
}
Handler<DonutObject> World::createDonutObject()
{
	Handler<DonutObject> obj(new DonutObject(this));
	obj->store(this, "__proto__", this->objectProto());
	this->registerObject(obj);

	return obj;
}

Handler<DonutObject> World::createEmptyDonutObject()
{
	Handler<DonutObject> obj(new DonutObject(this));
	this->registerObject(obj);

	return obj;
}

Handler<StringObject> World::createStringObject(const std::string& val)
{
	Handler<StringObject> obj(new StringObject(this, val));
	this->registerObject(obj);

	return obj;
}

Handler<FloatObject> World::createFloatObject(const float& val)
{
	Handler<FloatObject> obj(new FloatObject(this, val));
	this->registerObject(obj);

	return obj;
}

Handler<DonutClosureObject> World::createDonutClosureObject( const Handler<Closure>& closure, const Handler<Object>& scope )
{
	Handler<DonutClosureObject> obj(new DonutClosureObject(this, closure, scope));
	this->registerObject(obj);

	return obj;
}

Handler<PureNativeClosureObject> World::createPureNativeClosureObject(const std::string& objectProviderName, const std::string& closureName, PureNativeClosureEntry::Signature f)
{
	Handler<PureNativeClosureObject> obj(new PureNativeClosureObject(this, objectProviderName, closureName, f));
	this->registerObject(obj);

	return obj;
}

Handler<Object> World::createInt(const int& val)
{
	return this->intProvider()->create(val);
}

Handler<Object> World::createBool(const bool& val)
{
	return this->boolProvider()->create(val);
}

Handler<Object> World::createNull()
{
	return this->nullProvider()->create();
}

void World::registerProvider( Handler<Provider> provider )
{
	this->providers_.insert(
			std::pair<std::string, Handler<Provider> >(provider->name(), provider)
			);
}

}}

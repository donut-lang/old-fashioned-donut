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

#include "Heap.h"

namespace chisa {
namespace donut {

const static std::string TAG("Heap");

Heap::Heap(logging::Logger& log, const Handler<Clock>& clock)
:log_(log)
,clock_(clock)
,objectId_(0)
,walkColor_(0)
,gcLimit_(1024)
,gcLimitMax_(65536)
,donutObjectProvider_()
,boolProvider_()
,intProvider_()
,nullProvider_()
{
}

bool Heap::onFree() noexcept
{
	for(HeapObject* obj : this->objectPool_){
		obj->erase();
	}
	(decltype(this->objectPool_)()).swap(this->objectPool_);
	return false;
}

void Heap::bootstrap()
{
	Handler<Heap> self = this->self();
	this->donutObjectProvider_ = Handler<DonutObjectProvider>( new DonutObjectProvider(self) );
	this->boolProvider_ = Handler<BoolProvider>(new BoolProvider(self));
	this->intProvider_ = Handler<IntProvider>(new IntProvider(self));
	this->nullProvider_ = Handler<NullProvider>(new NullProvider(self));

	this->registerProvider( this->donutObjectProvider() );
	this->registerProvider( this->boolProvider() );
	this->registerProvider( this->intProvider() );
	this->registerProvider( this->nullProvider() );
	this->registerProvider(Handler<Provider>( new FloatProvider(self) ));
	this->registerProvider(Handler<Provider>( new StringProvider(self) ));

	this->objectProto_ = this->donutObjectProvider()->prototype();
	this->intProto_ = this->intProvider()->prototype();
	this->boolProto_ = this->boolProvider()->prototype();
	this->nullProto_ = this->nullProvider()->prototype();

	this->globalObject_ = this->createEmptyDonutObject();
	this->globalObject_->store(self, "Object", this->objectProto());
	this->globalObject_->store(self, "Int", this->intProto());
	this->globalObject_->store(self, "Boolean", this->boolProto());
	this->globalObject_->store(self, "Null", this->nullProto());
}

tinyxml2::XMLElement* Heap::serialize(tinyxml2::XMLDocument* doc)
{

}

void Heap::deserialize(tinyxml2::XMLElement* xml)
{

}

Handler<Provider> Heap::getProvider( const std::string& name ) const
{
	auto it = this->providers_.find(name);
	if(it != this->providers_.end()){
		return it->second;
	}
	return Handler<Provider>();
}

void Heap::registerObject( const Handler<HeapObject>& obj )
{
	obj->id(++this->objectId_);
	this->objectPool_.push_back(obj.get());
	if( this->objectPool_.size() >= this->gcLimit_ ) {
		this->gc();
		if(this->gcLimit_ < this->gcLimitMax_){
			this->gcLimit_ *= 2;
		}
	}
}
Handler<DonutObject> Heap::createDonutObject()
{
	Handler<DonutObject> obj(new DonutObject(self()));
	obj->store(self(), "__proto__", this->objectProto());
	this->registerObject(obj);

	return obj;
}

Handler<DonutObject> Heap::createEmptyDonutObject()
{
	Handler<DonutObject> obj(new DonutObject(self()));
	this->registerObject(obj);

	return obj;
}

Handler<StringObject> Heap::createStringObject(const std::string& val)
{
	Handler<StringObject> obj(new StringObject(self(), val));
	this->registerObject(obj);

	return obj;
}

Handler<FloatObject> Heap::createFloatObject(const float& val)
{
	Handler<FloatObject> obj(new FloatObject(self(), val));
	this->registerObject(obj);

	return obj;
}

Handler<DonutClosureObject> Heap::createDonutClosureObject( const Handler<Source>& src, unsigned int const& closureIndex, const Handler<Object>& scope )
{
	Handler<DonutClosureObject> obj(new DonutClosureObject(self(), src, closureIndex, scope));
	this->registerObject(obj);

	return obj;
}

Handler<PureNativeClosureObject> Heap::createPureNativeClosureObject(const std::string& objectProviderName, const std::string& closureName, PureNativeClosureEntry::Signature f)
{
	Handler<PureNativeClosureObject> obj(new PureNativeClosureObject(self(), objectProviderName, closureName, f));
	this->registerObject(obj);

	return obj;
}

Handler<Object> Heap::createInt(const int& val)
{
	return this->intProvider()->create(val);
}

Handler<Object> Heap::createBool(const bool& val)
{
	return this->boolProvider()->create(val);
}

Handler<Object> Heap::createNull()
{
	return this->nullProvider()->create();
}

void Heap::registerProvider( Handler<Provider> provider )
{
	this->providers_.insert(
			std::pair<std::string, Handler<Provider> >(provider->name(), provider)
			);
}

void Heap::onSeekNotify()
{
	if(this->log().d()){
		this->log().d(TAG, "Seeking to... %d", this->clock_->now());
	}
	for( Object* const& o : this->objectPool_){
		o->onSeekNotify(self());
	}
}

void Heap::onDiscardFutureNotify()
{
	if(this->log().d()){
		this->log().d(TAG, "Discarding future... now: %d", this->clock_->now());
	}
	for( Object* const& o : this->objectPool_){
		o->onDiscardFutureNotify(self());
	}
}

void Heap::onDiscardHistoryNotify()
{
	if(this->log().d()){
		this->log().d(TAG, "Discarding history... now: %d", this->clock_->now());
	}
	for( Object* const& o : this->objectPool_){
		o->onDiscardHistoryNotify(self());
	}
}


}}

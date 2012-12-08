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

Handler<Provider> Heap::getProvider( const std::string& name ) const
{
	auto it = this->providers_.find(name);
	if(it != this->providers_.end()){
		util::VectorMap<std::string, Handler<Provider> >::Pair const& p = *it;
		return p.second;
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

void Heap::adjustReference( Object*& ref )
{
	if(!ref->isObject()){
		return;
	}
	ref = findHeapObjectFromID(Object::decodeHeapObjectId(ref));
}
Handler<Object> Heap::decodeDescriptor( object_desc_t const& desc )
{
	if( Object::isPrimitiveDescriptor(desc) ) {
		return Handler<Object>::__internal__fromRawPointerWithoutCheck( Object::castToPointer(desc) );
	}
	return Handler<HeapObject>::__internal__fromRawPointerWithoutCheck( findHeapObjectFromID( Object::decodeHeapObjectId( desc) ) );
}

HeapObject* Heap::findHeapObjectFromID( objectid_t const& id )
{
	std::vector<HeapObject*>::iterator const it =
			std::lower_bound( this->objectPool_.begin(), this->objectPool_.end(), id, CompareHeapById());
	HeapObject* obj = *it;
	if( it == this->objectPool_.end() || obj->id() != id ) {
		throw DonutException(__FILE__, __LINE__, "[BUG] Object id: %d not found. Invalid Object Descriptor.", id);
	}
	return obj;
}

Handler<HeapObject> Heap::decodeHeapDescriptor( object_desc_t const& desc )
{
	return Handler<HeapObject>::__internal__fromRawPointerWithoutCheck( findHeapObjectFromID( Object::decodeHeapObjectId(desc) ) );
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

/**********************************************************************************
 * 外部との接続
 **********************************************************************************/
void Heap::registerProvider( Handler<Provider> provider )
{
	this->providers_.insert(provider->name(), provider);
}

void Heap::registerGlobalObject( std::string const& name, Handler<Object> const& obj )
{
	this->global()->store(self(), name, obj);
}

bool Heap::hasGlobalObject( std::string const& name )
{
	return this->global()->haveOwn( self(), name );
}
Handler<Object> Heap::loadGlobalObject( std::string const& name )
{
	return this->global()->load(self(), name);
}
/**********************************************************************************
 * save/load
 **********************************************************************************/

void Heap::bootstrap()
{
	this->objectId_ = 0;
	this->walkColor_ = 0;
	this->initPrimitiveProviders();
	Handler<Heap> const self = this->self();

	this->objectProto_ = this->donutObjectProvider()->prototype();
	this->intProto_ = this->intProvider()->prototype();
	this->boolProto_ = this->boolProvider()->prototype();
	this->nullProto_ = this->nullProvider()->prototype();

	this->globalObject_ = this->createEmptyDonutObject();
	this->globalObject_->store(self, "Object", this->objectProto());
	this->globalObject_->store(self, "Int", this->intProto());
	this->globalObject_->store(self, "Boolean", this->boolProto());
	this->globalObject_->store(self, "Null", this->nullProto());

	this->globalObject_->store(self, "Global", this->globalObject_);
}

void Heap::initPrimitiveProviders()
{
	Handler<Heap> const self = this->self();
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

	for(std::pair<std::string, Handler<Provider> > const& p : this->providers_){
		p.second->bootstrap();
	}
}

util::XValue Heap::save()
{
	using namespace chisa::util;
	Handler<XObject> top(new XObject);
	{ //pool
		Handler<XArray> pool(new XArray);
		for( HeapObject* const& obj : this->objectPool_ ) {
			Handler<XObject> xobj(new XObject);
			xobj->set("provider", obj->providerName().c_str());
			xobj->set("id", obj->id());
			Handler<Provider> provider ( this->getProvider(obj->providerName()) );
			if(!provider){
				throw DonutException(__FILE__, __LINE__, "Provider %s not found.", obj->providerName().c_str());
			}
			xobj->set("content", provider->saveObject(Handler<HeapObject>::__internal__fromRawPointerWithoutCheck(obj)));
			pool->append(xobj);
		}
		top->set("pool", pool);
	}
	top->set("object-prototype", this->donutObjectProvider()->prototype()->toDescriptor());
	top->set("int-prototype", this->intProvider()->prototype()->toDescriptor());
	top->set("bool-prototype", this->boolProvider()->prototype()->toDescriptor());
	top->set("null-prototype", this->nullProvider()->prototype()->toDescriptor());
	top->set("global", this->global()->toDescriptor());
	top->set("object_id", this->objectId_);
	top->set("walk_color", this->walkColor_);
	return top;
}

void Heap::load(util::XValue const& data)
{
	this->initPrimitiveProviders();
	using namespace chisa::util;
	Handler<util::XObject> xobj ( data.as<util::XObject>() );
	for( XValue& val : *(xobj->get<XArray>("pool")) ){ //pool
		Handler<XObject> obj ( val.as<XObject>() );
		std::string const provider = obj->get<XString>("provider");
		objectid_t id = obj->get<objectid_t>("id");
		//中身
		Handler<HeapObject> robj ( this->getProvider(provider)->loadObject( obj->get<XValue>("content") ) );
		robj->id(id);
	}
	this->objectProto_ = this->decodeHeapDescriptor(xobj->get<object_desc_t>("object-prototype")).cast<DonutObject>();
	this->intProto_ = this->decodeHeapDescriptor(xobj->get<object_desc_t>("int-prototype")).cast<DonutObject>();
	this->boolProto_ = this->decodeHeapDescriptor(xobj->get<object_desc_t>("bool-prototype")).cast<DonutObject>();
	this->nullProto_ = this->decodeHeapDescriptor(xobj->get<object_desc_t>("null-prototype")).cast<DonutObject>();
	this->globalObject_ = this->decodeHeapDescriptor(xobj->get<object_desc_t>("global")).cast<DonutObject>();

	this->objectId_ = xobj->get<objectid_t>("object_id");
	this->walkColor_ = xobj->get<int>("walk_color");
}


/**********************************************************************************
 * time functions
 **********************************************************************************/
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

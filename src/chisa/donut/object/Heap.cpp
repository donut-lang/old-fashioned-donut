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
	(decltype(this->objectPoolMarked_)()).swap(this->objectPoolMarked_);
	return false;
}

Handler<HeapObjectProvider> Heap::getHeapProvider( const std::string& name ) const
{
	auto it = this->providers_.find(name);
	if(it != this->providers_.end()){
		util::VectorMap<std::string, Handler<HeapObjectProvider> >::Pair const& p = *it;
		return p.second;
	}
	return Handler<HeapObjectProvider>();
}

Handler<Provider> Heap::getProvider( const std::string& name ) const
{
	auto it = getHeapProvider(name);
	if(it){
		return it;
	}
	if( name == this->intProvider_->name() ){
		return intProvider_;
	}else if(name==this->boolProvider_->name()){
		return boolProvider_;
	}else if(name==this->nullProvider_->name()){
		return nullProvider_;
	}
	return Handler<HeapObjectProvider>();
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


Handler<Object> Heap::decodeDescriptor( object_desc_t const& desc )
{
	if( Object::isPrimitiveDescriptor(desc) ) {
		return Handler<Object>::__internal__fromRawPointerWithoutCheck( Object::castToPointer(desc) );
	}
	return Handler<HeapObject>::__internal__fromRawPointerWithoutCheck( findHeapObjectFromID( Object::decodeObjectId( desc) ) );
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
	return Handler<HeapObject>::__internal__fromRawPointerWithoutCheck( findHeapObjectFromID( Object::decodeObjectId(desc) ) );
}

Handler<DonutObject> Heap::createDonutObject()
{
	Handler<DonutObject> obj(donutObjectProvider_->createDerived());
	obj->set(self(), "__proto__", this->objectProto());
	this->registerObject(obj);

	return obj;
}

Handler<DonutObject> Heap::createEmptyDonutObject()
{
	Handler<DonutObject> obj(donutObjectProvider_->createDerived());
	this->registerObject(obj);

	return obj;
}
class DonutObject;

Handler<StringObject> Heap::createStringObject(const std::string& val)
{
	Handler<StringObject> obj(this->stringProvider_->createDerived());
	obj->bootstrap(self(), val);
	this->registerObject(obj);

	return obj;
}

Handler<FloatObject> Heap::createFloatObject(const float& val)
{
	Handler<FloatObject> obj(this->floatProvider_->createDerived());
	obj->bootstrap(self(), val);
	this->registerObject(obj);

	return obj;
}

Handler<DonutClosureObject> Heap::createDonutClosureObject( const Handler<Source>& src, unsigned int const& closureIndex, const Handler<Object>& scope )
{
	Handler<DonutClosureObject> obj(this->donutClosureObjectProvider_->createDerived());
	obj->bootstrap(self(), src, closureIndex, scope);
	this->registerObject(obj);

	return obj;
}

Handler<PureNativeClosureObject> Heap::createPureNativeClosureObject(const std::string& objectProviderName, const std::string& closureName, PureNativeClosureObject::Signature f)
{
	Handler<PureNativeClosureObject> obj(this->pureNativeClosureProvider_->createDerived());
	obj->bootstrap(objectProviderName, closureName, f);
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
	return this->nullProvider()->createNull();
}

/**********************************************************************************
 * 外部との接続
 **********************************************************************************/
void Heap::registerProvider( Handler<HeapObjectProvider> const& provider )
{
	this->providers_.insert(provider->name(), provider);
}

void Heap::registerGlobalObject( std::string const& name, Handler<Object> const& obj )
{
	this->global()->set(self(), name, obj);
}

bool Heap::hasGlobalObject( std::string const& name )
{
	return this->global()->haveOwn( self(), name );
}
Handler<Object> Heap::loadGlobalObject( std::string const& name )
{
	return this->global()->get(self(), name);
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

	this->intProvider_->bootstrap();
	this->boolProvider_->bootstrap();
	this->nullProvider_->bootstrap();
	for( std::pair<std::string, Handler<HeapObjectProvider>>& p : this->providers_ ){
		p.second->bootstrap();
	}

	this->initPrototypes();

	this->globalObject_ = this->createEmptyDonutObject();
	this->globalObject_->set(self, "Object", this->objectProto());
	this->globalObject_->set(self, "Int", this->intProto());
	this->globalObject_->set(self, "Boolean", this->boolProto());
	this->globalObject_->set(self, "Null", this->nullProto());

	this->globalObject_->set(self, "Global", this->globalObject_);
}
void Heap::load(util::XValue const& data)
{
	Handler<util::XObject> const xobj ( data.as<util::XObject>() );
	Handler<Heap> const self(this->self());
	this->objectId_ = xobj->get<objectid_t>("object_id");
	this->walkColor_ = xobj->get<int>("walk_color");
	this->initPrimitiveProviders();
	using namespace chisa::util;

	{ //とりあえず全オブジェクトを生成し、あとで参照できるようにしておかなければならない
		for( XValue& val : *(xobj->get<XArray>("pool")) ){ //プール、ただし生成だけ
			Handler<XObject> obj ( val.as<XObject>() );
			std::string const provider = obj->get<XString>("provider");
			objectid_t id = obj->get<objectid_t>("id");
			//中身
			HeapObject* robj ( this->getHeapProvider(provider)->create() );
			robj->id(id);
			this->objectPool_.push_back( robj );
		}
	}

	{ //やっとプロバイダのロードができる。
		{ //まずはヒープではない特殊プロバイダ
			Handler<util::XObject> const pobj ( xobj->get<util::XObject>("provider-primitives") );
			this->intProvider_->load(pobj->get<XValue>(this->intProvider_->name()));
			this->boolProvider_->load(pobj->get<XValue>(this->boolProvider_->name()));
			this->nullProvider_->load(pobj->get<XValue>(this->nullProvider_->name()));
		}
		{ //一般のヒーププロバイダ
			Handler<util::XObject> const hobj ( xobj->get<util::XObject>("provider-heaps") );
			for(std::pair<std::string, Handler<HeapObjectProvider> >& p : this->providers_){
				p.second->load(hobj->get<XValue>(p.first));
			}
		}
	}
	{ //オブジェクトのデータの復帰
		Handler<XArray> array(xobj->get<XArray>("pool"));
		auto it = array->begin();
		for(HeapObject*& obj : this->objectPool_){
			if(it == array->end()){
				throw DonutException(__FILE__, __LINE__, "[BUG] Oops. Heap size mismatched while loading.");
			}
			Handler<XObject> const xobj ( (it++)->as<XObject>() );
			objectid_t id = xobj->get<objectid_t>("id");
			if(id != obj->id()){
				throw DonutException(__FILE__, __LINE__, "[BUG] Object ID mismatched while loading.");
			}
			obj->load(self, xobj->get<XValue>("content"));
		}
	}

	initPrototypes();
	this->globalObject_ = this->decodeHeapDescriptor(xobj->get<object_desc_t>("global")).cast<DonutObject>();
}


void Heap::initPrimitiveProviders()
{
	Handler<Heap> const self = this->self();
	this->boolProvider_ = Handler<BoolProvider>(new BoolProvider(self));
	this->intProvider_ = Handler<IntProvider>(new IntProvider(self));
	this->nullProvider_ = Handler<NullProvider>(new NullProvider(self));
	this->registerProvider( this->donutObjectProvider_ = Handler<DonutObjectProvider>(new DonutObjectProvider(self)) );
	this->registerProvider( this->donutClosureObjectProvider_ = Handler<DonutClosureObjectProvider>(new DonutClosureObjectProvider(self)) );
	this->registerProvider( this->stringProvider_ = Handler<StringProvider>( new StringProvider(self) ) );
	this->registerProvider( this->floatProvider_ = Handler<FloatProvider>( new FloatProvider(self) ) );
	this->registerProvider( this->pureNativeClosureProvider_ = Handler<PureNativeObjectProvider>( new PureNativeObjectProvider(self) ) );
}

void Heap::initPrototypes()
{
	this->objectProto_ = this->donutObjectProvider()->prototype();
	this->intProto_ = this->intProvider()->prototype();
	this->boolProto_ = this->boolProvider()->prototype();
	this->nullProto_ = this->nullProvider()->prototype();
}

util::XValue Heap::save()
{
	using namespace chisa::util;
	Handler<XObject> top(new XObject);
	Handler<Heap> self(this->self());
	{ //providers
		{ //特殊プロバイダ
			Handler<util::XObject> const pobj ( new XObject );
			pobj->set(this->intProvider_->name(), this->intProvider_->save());
			pobj->set(this->boolProvider_->name(), this->boolProvider_->save());
			pobj->set(this->nullProvider_->name(), this->nullProvider_->save());
			top->set("provider-primitives", pobj);
		}
		{ //通常プロバイダ
			Handler<util::XObject> const hobj ( new XObject );
			for(std::pair<std::string, Handler<HeapObjectProvider> >& p : this->providers_){
				hobj->set(p.first, p.second->save());
			}
			top->set("provider-heaps", hobj);
		}
	}
	{ //pool
		Handler<XArray> pool(new XArray);
		for( HeapObject* const& obj : this->objectPool_ ) {
			Handler<XObject> xobj(new XObject);
			xobj->set("provider", obj->providerName().c_str());
			xobj->set("id", obj->id());
			Handler<Provider> provider ( this->getHeapProvider(obj->providerName()) );
			if(!provider){
				throw DonutException(__FILE__, __LINE__, "Provider %s not found.", obj->providerName().c_str());
			}
			xobj->set("content", obj->save(self));
			pool->append(xobj);
		}
		top->set("pool", pool);
	}
	top->set("global", this->global()->toDescriptor());
	top->set("object_id", this->objectId_);
	top->set("walk_color", this->walkColor_);
	return top;
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

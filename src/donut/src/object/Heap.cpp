/**
 * Donut
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

#include <donut/object/Heap.h>
#include <donut/source/Source.h>
#include <donut/provider/BoolProvider.h>
#include <donut/provider/IntProvider.h>
#include <donut/provider/NullProvider.h>
#include <donut/provider/DonutProvider.h>
#include <donut/provider/FloatProvider.h>
#include <donut/provider/StringProvider.h>
#include <donut/provider/NativeClosureProvider.h>
#include <donut/provider/HomuraProvider.h>
#include <donut/object/StringObject.h>
#include <donut/object/FloatObject.h>
#include <donut/object/HomuraObject.h>

namespace donut {

const static std::string TAG("Heap");

Heap::Heap(Logger& log, Handler<Clock> const& clock)
:log_(log)
,clock_(clock)
,objectId_(0)
,sourceId_(0)
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

Handler<HeapProvider> Heap::findHeapProvider( std::string const& name ) const
{
	auto it = this->providers_.find(name);
	if(it != this->providers_.end()){
		VectorMap<std::string, Handler<HeapProvider> >::Pair const& p = *it;
		return p.second;
	}
	return Handler<HeapProvider>();
}

Handler<Provider> Heap::findProvider( std::string const& name ) const
{
	auto it = findHeapProvider(name);
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
	return Handler<HeapProvider>();
}

Handler<Source> Heap::registerSource( Handler<Source> const& source )
{
	source->onRegisterToHeap(this, ++this->sourceId_);
	this->sourcePool_.push_back(source.get());
	return source;
}

void Heap::unregisterSource( Source* source )
{
	int const id = source->id();
	std::vector<Source*>::iterator const it =
			std::lower_bound( this->sourcePool_.begin(), this->sourcePool_.end(), id, Source::CompareById());
	Source* src = *it;
	if( it == this->sourcePool_.end() || src != source) {
		DONUT_EXCEPTION(Exception, "[BUG] source unregistration notified, but there is no source with id: %d", id);
	}
	this->sourcePool_.erase(it);
}

void Heap::registerObject( Handler<HeapObject> const& obj )
{
	if( unlikely(obj->id() != 0) ) {
		DONUT_EXCEPTION(Exception, "[BUG] Object \"%s\" is already registered to %d.", obj->repr(self()).c_str(), obj->id());
	}
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
	}else if(!desc){
		return Handler<Object>();
	}
	return Handler<HeapObject>::__internal__fromRawPointerWithoutCheck( findHeapObjectFromID( Object::decodeObjectId( desc) ) );
}

HeapObject* Heap::findHeapObjectFromID( objectid_t const& id )
{
	std::vector<HeapObject*>::iterator const it =
			std::lower_bound( this->objectPool_.begin(), this->objectPool_.end(), id, HeapObject::CompareById());
	HeapObject* obj = *it;
	if( it == this->objectPool_.end() || obj->id() != id ) {
		DONUT_EXCEPTION(Exception, "[BUG] Object id: %d not found. Invalid Object Descriptor.", id);
	}
	return obj;
}

Handler<Source> Heap::decodeSourceID(int const& id)
{
	std::vector<Source*>::iterator const it =
			std::lower_bound( this->sourcePool_.begin(), this->sourcePool_.end(), id, Source::CompareById());
	Source* src = *it;
	if( it == this->sourcePool_.end() || src->id() != id ) {
		DONUT_EXCEPTION(Exception, "[BUG] Source id: %d not found. Invalid Source ID.", id);
	}
	return Handler<Source>::__internal__fromRawPointerWithoutCheck( src );
}

Handler<HeapObject> Heap::decodeHeapDescriptor( object_desc_t const& desc )
{
	return Handler<HeapObject>::__internal__fromRawPointerWithoutCheck( findHeapObjectFromID( Object::decodeObjectId(desc) ) );
}

Handler<DonutObject> Heap::createDonutObject()
{
	Handler<DonutObject> obj(donutObjectProvider_->newInstance());
	obj->set(self(), "__proto__", this->objectProto());
	return obj;
}

Handler<DonutObject> Heap::createEmptyDonutObject()
{
	return donutObjectProvider_->newInstance();
}
class DonutObject;

Handler<StringObject> Heap::createStringObject(std::string const& val)
{
	return this->stringProvider_->newInstance(self(), val);
}

Handler<FloatObject> Heap::createFloatObject(float const& val)
{
	return this->floatProvider_->newInstance(self(), val);
}

Handler<DonutClosureObject> Heap::createDonutClosureObject( Handler<Source> const& src, unsigned int const& closureIndex, Handler<Object> const& scope )
{
	return this->donutClosureObjectProvider_->newInstance(self(), src, closureIndex, scope);
}

Handler<PureNativeClosureObject> Heap::createPureNativeClosureObject(std::string const& objectProviderName, std::string const& closureName, PureNativeClosureObject::Signature f)
{
	return this->pureNativeClosureProvider_->newInstance(objectProviderName, closureName, f);
}

Handler<HomuraObject> Heap::createHomuraObject()
{
	return this->homuraProvider_->newInstance(self());
}
Handler<Object> Heap::createInt(int const& val)
{
	return this->intProvider()->create(val);
}

Handler<Object> Heap::createBool(bool const& val)
{
	return this->boolProvider()->create(val);
}


Handler<Object> Heap::createNull()
{
	return this->nullProvider()->createNull();
}

Handler<Object> Heap::createObject( XValue const& xvalue )
{
	switch(xvalue.type()){
	case XValue::NullT:
		return createNull();
	case XValue::UIntT:
		return createInt(xvalue.as<XUInt>());
	case XValue::SIntT:
		return createInt(xvalue.as<XSInt>());
	case XValue::FloatT:
		return createFloatObject(xvalue.as<XFloat>());
	case XValue::BoolT:
		return createBool(xvalue.as<XBool>());
	case XValue::StringT:
		return createStringObject(xvalue.as<XString>());
	case XValue::BinaryT:
		DONUT_EXCEPTION(Exception, "[BUG] Oops. You cannot convert XBinary to Donut object.");
		return createNull();
	case XValue::ArrayT: {
		Handler<XArray> array(xvalue.as<XArray>());
		Handler<Heap> self(this->self());
		Handler<DonutObject> obj(createEmptyDonutObject());
		int cnt = 0;
		for( XValue const& v : *array ) {
			obj->set(self, cnt++, createObject(v));
		}
		return obj;
	}
	case XValue::ObjectT: {
		Handler<XObject> array(xvalue.as<XObject>());
		Handler<Heap> self(this->self());
		Handler<DonutObject> obj(createEmptyDonutObject());
		for( std::pair<std::string, XValue> const& pair : *array ) {
			obj->set(self, pair.first, createObject(pair.second));
		}
		return obj;
	}
	default:
		DONUT_EXCEPTION(Exception, "[BUG] Oops. Invalid XValue type: \"%s\"", xvalue.typeString().c_str());
	}
}

/**********************************************************************************
 * 外部との接続
 **********************************************************************************/
void Heap::registerProvider( Handler<HeapProvider> const& provider )
{
	this->providers_.insert(provider->name(), provider);
}

void Heap::setGlobalObject( std::string const& name, Handler<Object> const& obj )
{
	if(obj->isObject()){
		Handler<HeapObject> heapObj(Handler<HeapObject>::__internal__fromRawPointerWithoutCheck(static_cast<HeapObject*>(obj.get())));
		if(heapObj->id() == 0){
			this->registerObject(heapObj);
		}
	}
	this->global()->set(self(), name, obj);
}

bool Heap::hasGlobalObject( std::string const& name )
{
	return this->global()->hasOwn( self(), name );
}
Handler<Object> Heap::getGlobalObject( std::string const& name )
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
	for( std::pair<std::string, Handler<HeapProvider>>& p : this->providers_ ){
		p.second->bootstrap();
	}

	this->initPrototypes();

	this->globalObject_ = this->createDonutObject(); //XXX: Emptyの方がいいとお思う
	this->globalObject_->set(self, "Object", this->objectProto());
	this->globalObject_->set(self, "Int", this->intProto());
	this->globalObject_->set(self, "Boolean", this->boolProto());
	this->globalObject_->set(self, "Null", this->nullProto());
	this->globalObject_->set(self, "Homura", this->createHomuraObject());

	this->globalObject_->set(self, "Global", this->globalObject_);
}
void Heap::load(XValue const& data)
{
	Handler<XObject> const xobj ( data.as<XObject>() );
	Handler<Heap> const self(this->self());
	this->objectId_ = xobj->get<objectid_t>("object_id");
	this->walkColor_ = xobj->get<int>("walk_color");
	this->initPrimitiveProviders();

	{//ソースのロード
		for( XValue& sobj : *(xobj->get<XArray>("source")) ){
			this->sourcePool_.push_back(new Source(sobj));
		}
	}

	{ //とりあえず全オブジェクトを生成し、あとで参照できるようにしておかなければならない
		for( XValue& val : *(xobj->get<XArray>("pool")) ){ //プール、ただし生成だけ
			Handler<XObject> obj ( val.as<XObject>() );
			std::string const provider = obj->get<XString>("provider");
			objectid_t id = obj->get<objectid_t>("id");
			//中身
			HeapObject* robj ( this->findHeapProvider(provider)->__internal__createInstanceForLoading() );
			robj->id(id);
			this->objectPool_.push_back( robj );
		}
	}

	{ //やっとプロバイダのロードができる。
		{ //まずはヒープではない特殊プロバイダ
			Handler<XObject> const pobj ( xobj->get<XObject>("provider-primitives") );
			this->intProvider_->load(pobj->get<XValue>(this->intProvider_->name()));
			this->boolProvider_->load(pobj->get<XValue>(this->boolProvider_->name()));
			this->nullProvider_->load(pobj->get<XValue>(this->nullProvider_->name()));
		}
		{ //一般のヒーププロバイダ
			Handler<XObject> const hobj ( xobj->get<XObject>("provider-heaps") );
			for(std::pair<std::string, Handler<HeapProvider> >& p : this->providers_){
				p.second->load(hobj->get<XValue>(p.first));
			}
		}
	}
	{ //オブジェクトのデータの復帰
		Handler<XArray> array(xobj->get<XArray>("pool"));
		auto it = array->begin();
		for(HeapObject*& obj : this->objectPool_){
			if(it == array->end()){
				DONUT_EXCEPTION(Exception, "[BUG] Oops. Heap size mismatched while loading.");
			}
			Handler<XObject> const xobj ( ((XValue&)(*(it++))).as<XObject>() );
			objectid_t id = xobj->get<objectid_t>("id");
			if(id != obj->id()){
				DONUT_EXCEPTION(Exception, "[BUG] Object ID mismatched while loading.");
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
	this->registerProvider( this->donutObjectProvider_ = Handler<DonutProvider>(new DonutProvider(self)) );
	this->registerProvider( this->donutClosureObjectProvider_ = Handler<DonutClosureProvider>(new DonutClosureProvider(self)) );
	this->registerProvider( this->stringProvider_ = Handler<StringProvider>( new StringProvider(self) ) );
	this->registerProvider( this->floatProvider_ = Handler<FloatProvider>( new FloatProvider(self) ) );
	this->registerProvider( this->pureNativeClosureProvider_ = Handler<PureNativeClosureProvider>( new PureNativeClosureProvider(self) ) );
	this->registerProvider( this->reactiveNativeClosureProvider_ = Handler<ReactiveNativeClosureProvider>( new ReactiveNativeClosureProvider(self) ) );
	this->registerProvider( this->homuraProvider_ = Handler<HomuraProvider>( new HomuraProvider(self) ) );
}

void Heap::initPrototypes()
{
	this->objectProto_ = this->donutObjectProvider()->prototype();
	this->intProto_ = this->intProvider()->prototype();
	this->boolProto_ = this->boolProvider()->prototype();
	this->nullProto_ = this->nullProvider()->prototype();
}

XValue Heap::save()
{
	this->gc();
	Handler<XObject> top(new XObject);
	Handler<Heap> self(this->self());
	{//ソースコード
		Handler<XArray> sarray(new XArray);
		for( Source*& src : this->sourcePool_ ){
			sarray->append(src->save());
		}
		top->set("source",sarray);
	}
	{ //providers
		{ //特殊プロバイダ
			Handler<XObject> const pobj ( new XObject );
			pobj->set(this->intProvider_->name(), this->intProvider_->save());
			pobj->set(this->boolProvider_->name(), this->boolProvider_->save());
			pobj->set(this->nullProvider_->name(), this->nullProvider_->save());
			top->set("provider-primitives", pobj);
		}
		{ //通常プロバイダ
			Handler<XObject> const hobj ( new XObject );
			for(std::pair<std::string, Handler<HeapProvider> >& p : this->providers_){
				hobj->set(p.first, p.second->save());
			}
			top->set("provider-heaps", hobj);
		}
	}
	{ //pool
		Handler<XArray> pool(new XArray);
		for( HeapObject* const& obj : this->objectPool_ ) {
			Handler<XObject> xobj(new XObject);
			xobj->set("provider", obj->providerName(self).c_str());
			xobj->set("id", obj->id());
			Handler<Provider> provider ( this->findHeapProvider(obj->providerName(self)) );
			if(!provider){
				DONUT_EXCEPTION(Exception, "Provider %s not found.", obj->providerName(self).c_str());
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
void Heap::onTickNotify()
{
}
void Heap::onBackNotify()
{
	Handler<Heap> self(this->self());
	for( Object* const& o : this->objectPool_){
		o->onBackNotify(self);
	}
}
void Heap::onForwardNotify()
{
	Handler<Heap> self(this->self());
	for( Object* const& o : this->objectPool_){
		o->onForwardNotify(self);
	}
}

void Heap::onDiscardFutureNotify()
{
	for( Object* const& o : this->objectPool_){
		o->onDiscardFutureNotify(self());
	}
}

void Heap::onDiscardHistoryNotify()
{
	for( Object* const& o : this->objectPool_){
		o->onDiscardHistoryNotify(self());
	}
}


}

/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <sstream>
#include <cinamo/String.h>
#include <donut/object/Heap.hpp>
#include <donut/object/StringObject.hpp>
#include <donut/provider/StringProvider.hpp>

namespace donut {
using namespace cinamo;

static const std::string TAG("StringProvider");

StringProvider::StringProvider(Handler<Heap> const& heap )
:Super(heap, "String")
{
	this->registerPureNativeClosure("toInteger", [this](StringObject* self) {
		Handler<Heap> heap(this->heap().lock());
		return parseAs<int>(self->toString(heap), 0);
	});
	this->registerPureNativeClosure("toFloat", [this](StringObject* self) {
		Handler<Heap> heap(this->heap().lock());
		return parseAs<float>(self->toString(heap), false);
	});
	this->registerPureNativeClosure("toBoolean", [this](StringObject* self) {
		Handler<Heap> heap(this->heap().lock());
		return parseAs<bool>(self->toString(heap), false);
	});
	this->registerPureNativeClosure("opAdd", [this](StringObject* self, Object* other) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::string const ostr = other->print(heap);
		return str + ostr;
	});
	this->registerPureNativeClosure("opEq", [this](StringObject* self, Object* other) {
		Handler<Heap> heap(this->heap().lock());
		return other->isString() && self->toString(heap) == other->toString(heap);
	});
	this->registerPureNativeClosure("opNe", [this](StringObject* self, Object* other) {
		Handler<Heap> heap(this->heap().lock());
		return !other->isString() || self->toString(heap) != other->toString(heap);
	});
	this->registerPureNativeClosure("opLt", [this](StringObject* self, StringObject* other) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::string const ostr = other->toString(heap);
		return str < ostr;
	});
	this->registerPureNativeClosure("opLe", [this](StringObject* self, StringObject* other) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::string const ostr = other->toString(heap);
		return str <= ostr;
	});
	this->registerPureNativeClosure("opGt", [this](StringObject* self, StringObject* other) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::string const ostr = other->toString(heap);
		return str > ostr;
	});
	this->registerPureNativeClosure("opGe", [this](StringObject* self, StringObject* other) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::string const ostr = other->toString(heap);
		return str >= ostr;
	});
	this->registerPureNativeClosure("opMul", [this](StringObject* self, int times) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::stringstream ss;
		for(int i=0;i<times;++i){
			ss << str;
		}
		return ss.str();
	});
	this->registerPureNativeClosure("toString", [this](StringObject* self) {
		return self;
	});
	this->registerPureNativeClosure("isEmpty", [this](StringObject* self) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		return str.empty();
	});
}

}

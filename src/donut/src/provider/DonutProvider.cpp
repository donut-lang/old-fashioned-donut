/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/object/Object.hpp>
#include <donut/object/Heap.hpp>
#include <donut/object/DonutClosureObject.hpp>
#include <donut/provider/DonutProvider.hpp>


namespace donut {


DonutProvider::DonutProvider( Handler<Heap> const& heap )
:Super(heap, "DonutObject")
{
	this->registerPureNativeClosure("has",[this](DonutObject* obj, std::string name){
		return obj->has(this->heap().lock(), name);
	});
	this->registerPureNativeClosure("hasOwn",[this](DonutObject* obj, std::string name){
		return obj->has(this->heap().lock(), name);
	});
	this->registerPureNativeClosure("opEq",[this](DonutObject* self, Object* obj){
		return self == obj;
	});
	this->registerPureNativeClosure("opNe",[this](DonutObject* self, Object* obj){
		return self != obj;
	});
}
DonutClosureProvider::DonutClosureProvider( Handler<Heap> const& heap )
:Super(heap,"DonutClosureObject")
{

}

}

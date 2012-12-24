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

#pragma once
#include <string>
#include "../object/NativeClosureObject.h"
#include <tarte/Handler.h>
#include <tarte/XVal.h>
#include <tarte/VectorMap.h>

namespace donut {
using namespace tarte;

class Provider : public HandlerBody<Provider> {
private: //これは実行前に全て設定される情報
	VectorMap<std::string, PureNativeClosureObject::Signature> pureNativeClosures_;
	VectorMap<std::string, ReactiveNativeClosureObject::Signature> reactiveNativeClosures_;
	HandlerW<Heap> const heap_;
	std::string const name_;
private: //実行時に変化し、セーブ・ロードの対象にするのはこれだけ。
	Handler<DonutObject> prototype_;
protected:
	Provider( Handler<Heap> const& heap, std::string const& name );
	template <typename T> bool registerPureNativeClosure( std::string const& name, T f);
	template <typename T> bool registerReactiveNativeClosure( std::string const& name, T f);
public:
	virtual ~Provider() noexcept = default;
	inline bool onFree() noexcept { return false; };
	inline std::string name() const noexcept { return this->name_; };
	inline HandlerW<Heap> heap() const noexcept { return this->heap_; };
	inline Handler<DonutObject> prototype() const noexcept { return this->prototype_; };
public:
	PureNativeClosureObject::Signature const& findPureNativeClosureEntry( std::string const& name );
	ReactiveNativeClosureObject::Signature const& findReactiveNativeClosureEntry( std::string const& name );
public: /* 処理系の保存・復帰をします。 */
	void bootstrap();
	XValue save();
	void load( XValue const& data);
protected:
	virtual XValue saveImpl(); //please override
	virtual void loadImpl( XValue const& data); //please override
};

template <typename T>
bool Provider::registerPureNativeClosure( std::string const& name, T f) {
	return (!pureNativeClosures_.have(name)) && pureNativeClosures_.update( name, native::createBindPure(f) );
}

template <typename T>
bool Provider::registerReactiveNativeClosure( std::string const& name, T f) {
	return (!reactiveNativeClosures_.have(name)) && reactiveNativeClosures_.update( name, native::createBindReactive(f) );
}

//関数ポインタ版
template <>
bool Provider::registerPureNativeClosure<PureNativeClosureObject::Function> ( std::string const& name, PureNativeClosureObject::Function f);
template <>
bool Provider::registerReactiveNativeClosure<ReactiveNativeClosureObject::Function> ( std::string const& name, ReactiveNativeClosureObject::Function f);
// std::function版
template <>
bool Provider::registerPureNativeClosure<PureNativeClosureObject::Signature>( std::string const& name, PureNativeClosureObject::Signature f);
template <>
bool Provider::registerReactiveNativeClosure<ReactiveNativeClosureObject::Signature>( std::string const& name, ReactiveNativeClosureObject::Signature f);

//---------------------------------------------------------------------------------------------------------------------

class HeapProvider : public Provider {
	friend class Heap;
protected:
	HeapProvider( Handler<Heap> const& heap, std::string const& name ):Provider(heap, name){};
	virtual HeapObject* createEmptyObject() = 0;
public:
	virtual ~HeapProvider() noexcept = default;
};

template <typename T>
class HeapProviderImpl : public HeapProvider {
protected:
	HeapProviderImpl( Handler<Heap> const& heap, std::string const& name ):HeapProvider(heap, name){};
public:
	virtual ~HeapProviderImpl() noexcept = default;
public:
	template <typename... Args> inline
	Handler<T> newInstance(Args... args) {
		Handler<T> t ( new T( this ) );
		t->bootstrap(args...);
		return t;
	}
private:
	virtual HeapObject* createEmptyObject() override {
		return new T( this );
	}
};

}

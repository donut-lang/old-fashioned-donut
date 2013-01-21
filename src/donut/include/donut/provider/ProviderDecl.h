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
#include <tarte/Handler.h>
#include <tarte/XVal.h>
#include <tarte/VectorMap.h>
#include <tarte/Dynamic.h>
#include "../object/NativeClosureObjectDecl.h"

namespace donut {
using namespace tarte;

class Provider : public HandlerBody<Provider> {
private: //これは実行前に全て設定される情報
	VectorMap<std::string, PureNativeClosureObject::Signature> pureNativeClosures_;
	HandlerW<Heap> const heap_;
	std::string const name_;
private: //実行時に変化し、セーブ・ロードの対象にするのはこれだけ。
	Handler<DonutObject> prototype_;
protected:
	Provider( Handler<Heap> const& heap, std::string const& name );
	template <typename T>
	bool registerPureNativeClosure( std::string const& name, T f) {
		return pureNativeClosures_.update( name, native::createBindPure(f) );
	}
public:
	virtual ~Provider() noexcept = default;
	inline bool onFree() noexcept { return false; };
	inline std::string name() const noexcept { return this->name_; };
	inline HandlerW<Heap> heap() const noexcept { return this->heap_; };
	inline Handler<DonutObject> prototype() const noexcept { return this->prototype_; };
public:
	PureNativeClosureObject::Signature const& findPureNativeClosureEntry( std::string const& name );
public: /* 処理系の保存・復帰をします。 */
	virtual void bootstrap();
	XValue save();
	void load( XValue const& data);
protected:
	virtual XValue saveImpl(); //please override
	virtual void loadImpl( XValue const& data); //please override
};

//---------------------------------------------------------------------------------------------------------------------

class HeapProvider : public Provider {
	friend class Heap;
protected:
	HeapProvider( Handler<Heap> const& heap, std::string const& name );
	virtual HeapObject* __internal__createInstanceForLoading() = 0;
public:
	virtual ~HeapProvider() noexcept = default;
protected: /* これを使ってオブジェクトを生成してね */
	template <typename ObjectT, typename ProviderT, typename... Args>
	static inline Handler<ObjectT> newInstance(ProviderT* self, Args... args);
};

template <typename Derived, typename T>
class HeapProviderBaseT : public HeapProvider {
protected:
	typedef HeapProviderBaseT<Derived, T> Super;
	HeapProviderBaseT( Handler<Heap> const& heap, std::string const& name ):HeapProvider(heap, name){};
	HeapProviderBaseT( Handler<Heap> const& heap ):HeapProvider(heap, ::tarte::demangle<T>() ){};
public:
	virtual ~HeapProviderBaseT() noexcept = default;
private:
	virtual HeapObject* __internal__createInstanceForLoading() override {
		return new T( static_cast<Derived*>(this) );
	}
public: /* これを使ってオブジェクトを生成してね */
	template <typename... Args> inline Handler<T> newInstance(Args... args) {
		return HeapProvider::newInstance<T>(static_cast<Derived*>(this), args...);
	}
};


//---------------------------------------------------------------------------------------------------------------------
// Reactive
//---------------------------------------------------------------------------------------------------------------------

class ReactiveProvider : public HeapProvider {
protected:
	ReactiveProvider( Handler<Heap> const& heap, std::string const& name ):HeapProvider(heap, name){};
	virtual ~ReactiveProvider() noexcept = default;
public:
	virtual Handler<ReactiveNativeClosure> createReactiveNativeClosure( std::string const& name ) = 0;
};

//副作用に関する依存性を付け加えるのは、オブジェクト階層の最後の最後とする。
template <typename __AntiSideEffect> class ReactiveProviderAspectT;
namespace internal {
template <typename __AntiSideEffect, typename __F>
struct _Reactive_ClosureRegisterer{
	static inline bool exec(ReactiveProviderAspectT< __AntiSideEffect> * self, std::string const& name, __F f){
		return self->reactiveNativeClosures_.update( name, native::createBindReactive(f) );
	}
};
template <typename __AntiSideEffect>
struct _Reactive_ClosureRegisterer<__AntiSideEffect, typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Function> {
	static inline bool exec(ReactiveProviderAspectT<__AntiSideEffect> * self, std::string const& name, typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Function f){
		return self->reactiveNativeClosures_.update( name, f );
	}
};
template <typename __AntiSideEffect>
struct _Reactive_ClosureRegisterer<__AntiSideEffect, typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Signature> {
	static inline bool exec(ReactiveProviderAspectT<__AntiSideEffect> * self, std::string const& name, typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Function f){
		return self->reactiveNativeClosures_.update( name, f );
	}
};
}

template <typename __AntiSideEffect>
class ReactiveProviderAspectT final {
	DISABLE_COPY_AND_ASSIGN(ReactiveProviderAspectT);
private:
	template <typename AntiSideEffect__, typename F__> friend struct internal::_Reactive_ClosureRegisterer;
	// オブジェクトごとの反副作用情報を含んだ関数テーブル
	VectorMap<std::string, typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Signature> reactiveNativeClosures_;
	typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Signature const& findReactiveNativeClosureEntry( ReactiveProvider& self, std::string const& name ) {
		auto it = reactiveNativeClosures_.find(name);
		if(it == this->reactiveNativeClosures_.end()){
			DONUT_EXCEPTION(Exception, "Reactive Native Closure \"%s\" not found in \"%s\"!!", name.c_str(), self.name().c_str());
		}
		typename VectorMap<std::string, typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Signature>::Pair const& p = *it;
		return p.second;
	}
public:
	ReactiveProviderAspectT() = default;
	~ReactiveProviderAspectT() noexcept = default;
public:
	template <typename __F> inline bool registerReactiveNativeClosure( std::string const& name, __F f) {
		return internal::_Reactive_ClosureRegisterer<__AntiSideEffect, __F>::exec(this, name, f);
	}
	inline Handler<ReactiveNativeClosure> createReactiveNativeClosure( ReactiveProvider& self, std::string const& name ) {
		return Handler<ReactiveNativeClosure>( new ReactiveNativeClosureBaseT<__AntiSideEffect>(findReactiveNativeClosureEntry( self, name )) );
	}
	inline void bootstrap(ReactiveProvider& self);
};

#define INJECT_REACTIVE_PROVIDER_ASPECT(__AntiSideEffect__, __Super__) \
public:\
	typedef __AntiSideEffect__ AntiSideEffect;\
	typedef typename ReactiveNativeClosureBaseT<__AntiSideEffect__>::ResultType ResultType;\
private:\
	ReactiveProviderAspectT<__AntiSideEffect__> __reactive_aspect__;\
private:\
	virtual void bootstrap() override final {\
		this->__Super__::bootstrap();\
		this->__reactive_aspect__.bootstrap(*this);\
	}\
protected:\
	template <typename __F> inline bool registerReactiveNativeClosure( std::string const& name, __F f) {\
		return this->__reactive_aspect__.registerReactiveNativeClosure(name, f);\
	}\
	virtual Handler<ReactiveNativeClosure> createReactiveNativeClosure( std::string const& name ) override final{\
		return this->__reactive_aspect__.createReactiveNativeClosure(*this, name);\
	}


template <typename __Derived, typename __Object, typename __AntiSideEffect=XValue>
class ReactiveProviderBaseT : public ReactiveProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(__AntiSideEffect, ReactiveProvider);
	//static_assert(std::is_same<typename __Object::AntiSideEffect, __AntiSideEffect>::value,"Oops. AntiSideEffect type of Derived and Object must be the same.");
protected:
	typedef ReactiveProviderBaseT<__Derived, __Object, __AntiSideEffect> Super;
	ReactiveProviderBaseT( Handler<Heap> const& heap, std::string const& name ):ReactiveProvider(heap, name){};
	ReactiveProviderBaseT( Handler<Heap> const& heap ):ReactiveProvider(heap, ::tarte::demangle<Object>() ){};
public:
	virtual ~ReactiveProviderBaseT() noexcept = default;
private:
	virtual HeapObject* __internal__createInstanceForLoading() override {
		return new __Object( static_cast<__Derived*>(this) );
	}
public: /* これを使ってオブジェクトを生成してね */
	template <typename... Args> inline Handler<__Object> newInstance(Args... args) {
		return HeapProvider::newInstance<__Object>(static_cast<__Derived*>(this), args...);
	}
};


}

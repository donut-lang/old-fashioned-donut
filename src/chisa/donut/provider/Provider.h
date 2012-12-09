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

#pragma once
#include <vector>
#include <string>
#include <algorithm>

#include "../../Handler.h"
#include "../../util/XVal.h"
#include "../../util/VectorMap.h"
#include "../source/Source.h"
#include "../object/Object.h"
#include "../object/NativeObject.h"
#include "../object/DonutObject.h"

namespace chisa {
namespace donut {

class Provider : public HandlerBody<Provider> {
private:
	util::VectorMap<std::string, PureNativeClosureObject::Signature> pureNativeClosures_;
	HandlerW<Heap> const heap_;
	Handler<DonutObject> prototype_;
	std::string const name_;
protected:
	Provider( const Handler<Heap>& heap, const std::string& name );
	template <typename T>
	bool registerPureNativeClosure( const std::string& name, T f)
	{
		return pureNativeClosures_.update( name, native::createBind(f) );
	}
private:
	void addPrototype( const std::string& name, PureNativeClosureObject::Signature clos );
public:
	virtual ~Provider() noexcept = default;
	inline bool onFree() noexcept { return false; };
	inline std::string name() const noexcept { return this->name_; };
	inline HandlerW<Heap> heap() const noexcept { return this->heap_; };
	inline Handler<DonutObject> prototype() const noexcept { return this->prototype_; };
public:
	PureNativeClosureObject::Signature const& findClosureEntry( std::string const& name );
public: /* 処理系の保存・復帰をします。 */
	void bootstrap();
	util::XValue save();
	void load( util::XValue const& data);
protected:
	virtual util::XValue saveImpl() { return util::XValue(); };
	virtual void loadImpl( util::XValue const& data){};
};

//---------------------------------------------------------------------------------------------------------------------

class HeapObjectProvider : public Provider {
protected:
	HeapObjectProvider( const Handler<Heap>& heap, const std::string& name ):Provider(heap, name){};
public:
	virtual ~HeapObjectProvider() noexcept = default;
	virtual HeapObject* create() = 0;
};

template <typename T>
class HeapObjectProviderImpl : public HeapObjectProvider {
protected:
	HeapObjectProviderImpl( const Handler<Heap>& heap, const std::string& name ):HeapObjectProvider(heap, name){};
public:
	virtual ~HeapObjectProviderImpl() noexcept = default;
public:
	T* createDerived() {
		return new T( this->name() );
	}
private:
	virtual HeapObject* create() override {
		return createDerived();
	}
};

//---------------------------------------------------------------------------------------------------------------------
class IntProvider : public Provider {
public:
	IntProvider(const Handler<Heap>& heap);
	virtual ~IntProvider() noexcept = default;
private:
	static constexpr inline int fromPointer(const Object* const ptr) noexcept {
		return reinterpret_cast<std::intptr_t>(ptr) >> 2;
	}
	static constexpr inline Object* toPointer(int const& val) noexcept {
		return reinterpret_cast<Object*>((val << 2) | Object::Tag::Int);
	}
public:
	std::string toString(const Object* ptr) const;
	int toInt(const Object* ptr) const;
	float toFloat(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
public:
	Handler<Object> create( const int& val );
};

class BoolProvider : public Provider {
public:
	BoolProvider(const Handler<Heap>& heap);
	virtual ~BoolProvider() noexcept = default;
public:
	static constexpr inline bool fromPointer(const Object* const ptr) noexcept {
		return reinterpret_cast<std::intptr_t>(ptr) >> 2;
	}
	static constexpr inline Object* toPointer(bool const& val) noexcept {
		return reinterpret_cast<Object*>((val << 2) | Object::Tag::Bool);
	}
public:
	std::string toString(const Object* ptr) const;
	int toInt(const Object* ptr) const;
	float toFloat(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
public:
	Handler<Object> create( const bool& val );
};

class NullProvider : public Provider {
public:
	NullProvider(const Handler<Heap>& heap);
	virtual ~NullProvider() noexcept = default;
private:
	static constexpr inline Object* toPointer() noexcept {
		return reinterpret_cast<Object*>(Object::Tag::Null);
	}
public:
	std::string toString(const Object* ptr) const;
	int toInt(const Object* ptr) const;
	float toFloat(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
public:
	Handler<Object> createNull();
};

//---------------------------------------------------------------------------------------------------------------------

class StringObject;
class StringProvider : public HeapObjectProviderImpl<StringObject> {
public:
	StringProvider(const Handler<Heap>& heap);
	virtual ~StringProvider() noexcept = default;
};

class FloatObject;
class FloatProvider : public HeapObjectProviderImpl<FloatObject> {
public:
	FloatProvider(const Handler<Heap>& heap);
	virtual ~FloatProvider() noexcept = default;
};
//---------------------------------------------------------------------------------------------------------------------

class DonutObject;
class DonutClosureObject;
class DonutObjectProvider : public HeapObjectProviderImpl<DonutObject> {
public:
	DonutObjectProvider( const Handler<Heap>& heap )
	:HeapObjectProviderImpl<DonutObject>(heap, "DonutObject"){};
	virtual ~DonutObjectProvider() noexcept = default;
};
class DonutClosureObjectProvider : public HeapObjectProviderImpl<DonutClosureObject> {
public:
	DonutClosureObjectProvider( const Handler<Heap>& heap )
	:HeapObjectProviderImpl<DonutClosureObject>(heap,"DonutClosureObject"){};
	virtual ~DonutClosureObjectProvider() noexcept = default;
};

//---------------------------------------------------------------------------------------------------------------------

class PureNativeClosureObject;
class PureNativeObjectProvider : public HeapObjectProviderImpl<PureNativeClosureObject> {
public:
	PureNativeObjectProvider( const Handler<Heap>& heap )
	:HeapObjectProviderImpl<PureNativeClosureObject>(heap, "PureNativeObject"){}
	virtual ~PureNativeObjectProvider() noexcept = default;
};

}}

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
#include "../object/Object.h"
#include "NativeClosureEntry.h"

namespace chisa {
namespace donut {

class Provider : public HandlerBody<Provider> {
private:
	util::VectorMap<std::string, Handler<NativeClosureEntry> > nativeClosures_;
	HandlerW<Heap> const heap_;
	Handler<DonutObject> prototype_;
	std::string const name_;
protected:
	Provider( const Handler<Heap>& heap, const std::string& name );
	template <typename T>
	bool registerPureNativeClosure( const std::string& name, T f)
	{
		return nativeClosures_.update( name, Handler<NativeClosureEntry>(new PureNativeClosureEntry(f) ) );
	}
private:
	void addPrototype( const std::string& name, Handler<NativeClosureEntry> clos );
public:
	virtual ~Provider() noexcept = default;
	inline bool onFree() noexcept { return false; };
	inline std::string name() const noexcept { return this->name_; };
	inline HandlerW<Heap> heap() const noexcept { return this->heap_; };
	inline Handler<DonutObject> prototype() const noexcept { return this->prototype_; };
private:
	Handler<NativeClosureEntry> const& findClosureEntry( std::string const& name );
public: /* 処理系の保存・復帰をします。 */
	void bootstrap();
	util::XValue save();
	void load( util::XValue const& data);
public: //結び付けられたオブジェクトの保存・復帰
	util::XValue saveObject(Handler<HeapObject> const& obj);
	Handler<HeapObject> loadObject(util::XValue const& data);
public: //その具体的な実装
	virtual util::XValue saveObjectImpl(Handler<HeapObject> const& obj) = 0;
	virtual Handler<HeapObject> loadObjectImpl(util::XValue const& data) = 0;
};

//---------------------------------------------------------------------------------------------------------------------

class NativeObjectProvider : public Provider {
protected:
	NativeObjectProvider( const Handler<Heap>& heap, const std::string& name ):Provider(heap, name){};
public:
	virtual ~NativeObjectProvider() noexcept = default;
};

//---------------------------------------------------------------------------------------------------------------------
class IntProvider : public NativeObjectProvider {
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
	virtual util::XValue saveObjectImpl(Handler<HeapObject> const& obj) override final;
	virtual Handler<HeapObject> loadObjectImpl(util::XValue const& data) override final;
	Handler<Object> create( const int& val );
};

class BoolProvider : public NativeObjectProvider {
public:
	BoolProvider(const Handler<Heap>& heap);
	virtual ~BoolProvider() noexcept = default;
public:
	static constexpr inline int fromPointer(const Object* const ptr) noexcept {
		return reinterpret_cast<std::intptr_t>(ptr) >> 2;
	}
	static constexpr inline Object* toPointer(int const& val) noexcept {
		return reinterpret_cast<Object*>((val << 2) | Object::Tag::Bool);
	}
public:
	std::string toString(const Object* ptr) const;
	int toInt(const Object* ptr) const;
	float toFloat(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
	virtual util::XValue saveObjectImpl(Handler<HeapObject> const& obj) override final;
	virtual Handler<HeapObject> loadObjectImpl(util::XValue const& data) override final;
	Handler<Object> create( const bool& val );
};

class NullProvider : public NativeObjectProvider {
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
	virtual util::XValue saveObjectImpl(Handler<HeapObject> const& obj) override final;
	virtual Handler<HeapObject> loadObjectImpl(util::XValue const& data) override final;
	Handler<Object> create();
};

class StringProvider : public NativeObjectProvider {
public:
	StringProvider(const Handler<Heap>& heap);
	virtual ~StringProvider() noexcept = default;
public:
	virtual util::XValue saveObjectImpl(Handler<HeapObject> const& obj) override final;
	virtual Handler<HeapObject> loadObjectImpl(util::XValue const& data) override final;
};

class FloatProvider : public NativeObjectProvider {
public:
	FloatProvider(const Handler<Heap>& heap);
	virtual ~FloatProvider() noexcept = default;
public:
	virtual util::XValue saveObjectImpl(Handler<HeapObject> const& obj) override final;
	virtual Handler<HeapObject> loadObjectImpl(util::XValue const& data) override final;
};
//---------------------------------------------------------------------------------------------------------------------

class DonutObjectProvider : public Provider {
public:
	DonutObjectProvider( const Handler<Heap>& heap );
	virtual ~DonutObjectProvider() noexcept = default;
public:
	virtual util::XValue saveObjectImpl(Handler<HeapObject> const& obj) override final;
	virtual Handler<HeapObject> loadObjectImpl(util::XValue const& data) override final;
public:
	Handler<DonutObject> create();
};

}}

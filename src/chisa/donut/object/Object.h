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
#include <cstdint>
#include <map>
#include "../../Handler.h"
#include "../../util/ClassUtil.h"
#include "Slot.h"

namespace chisa {
namespace donut {
class World;
class ObjectWalker;

/**
 * すべての窓口となるクラス。
 */
class Object : public HandlerBody<Object> {
	DISABLE_COPY_AND_ASSIGN(Object);
public:
	enum Tag{
		Mask=3U,
		Obj=0U,
		Int=1U,
		Bool=2U,
		Null=3U
	};
public:
	Object() = default;
	virtual ~Object() noexcept = default;
public: //すべてのオブジェクトに出来なければならないこと
	std::string toString(World* const pool) const;
	int toInt(World* const pool) const;
	float toFloat(World* const pool) const;
	bool toBool(World* const pool) const;
	bool have(World* const pool, const std::string& name) const;
	bool have(World* const pool, const int& idx) const;
	bool haveOwn(World* const pool, const std::string& name) const;
	bool haveOwn(World* const pool, const int& idx) const;
	Handler<Object> store(World* const pool, const std::string& name, Handler<Object> obj);
	Handler<Object> store(World* const pool, const int& idx, Handler<Object> obj);
	Handler<Object> load(World* const pool, const std::string& name) const;
	Handler<Object> load(World* const pool, const int& idx) const;
	std::string providerName(World* const world) const;
public:
	inline bool isObject() const noexcept { return Tag::Obj==tag(); };
	inline bool isNull() const noexcept { return Tag::Null==tag(); };
	inline bool isBool() const noexcept { return Tag::Bool==tag(); };
	inline bool isInt() const noexcept { return Tag::Int==tag(); };
	inline intptr_t tag() const noexcept { return reinterpret_cast<std::uintptr_t>(this) & Tag::Mask; };
	inline void incref( bool check ) { if(isObject()) { this->HandlerBody<Object>::incref(check); } }
	inline void decref() { if(isObject()) { this->HandlerBody<Object>::decref(); } };
	inline void walk(ObjectWalker* walker) { if(isObject()){ this->walkImpl(walker); } }
	inline void seek(timestamp_t time){ if(isObject()){ this->seekImpl(time); } };
protected: /* 実装すべきもの */
	virtual std::string toStringImpl() const = 0;
	virtual std::string providerNameImpl() const = 0;
	virtual int toIntImpl() const = 0;
	virtual float toFloatImpl() const = 0;
	virtual bool toBoolImpl() const = 0;
	virtual bool haveImpl(const std::string& name) const = 0;
	virtual bool haveOwnImpl(const std::string& name) const = 0;
	virtual Handler<Object> storeImpl(const std::string& name, Handler<Object> obj) = 0;
	virtual Handler<Object> loadImpl(const std::string& name) const = 0;
	virtual void walkImpl(ObjectWalker* walker) = 0;
	virtual void seekImpl(timestamp_t time) = 0;
public:
	virtual bool onFree() noexcept = 0;
};

}}
//---------------------------------------------------------------------------------------------------------------------

namespace chisa {
namespace donut {

class HeapObject : public Object {
private:
	World* const world_;
	std::string const providerName_;
	uintptr_t id_;
	bool erased_;
	int walkColor_;
public:
	HeapObject(World* const world, const std::string& providerName);
	virtual ~HeapObject() noexcept = default;
public:
	inline World* world() const noexcept { return this->world_; }
	inline std::string providerName() const noexcept { return this->providerName_; }
	inline uintptr_t id() const noexcept { return this->id_; }
	inline void id(uintptr_t nid) noexcept { this->id_ = nid; }
	inline void erase() noexcept { this->erased_ = true; if(refcount() == 0){ delete this; } };
public:
	virtual bool onFree() noexcept { if(this->erased_){ return false; }else{ return true; } };
	int walkColor() noexcept { return this->walkColor_; };
	inline bool used() { return this->refcount() > 0; };
protected:
	void walkColor(const int color) noexcept { this->walkColor_=color; };
};

}}
//---------------------------------------------------------------------------------------------------------------------

namespace chisa {
namespace donut {
/**
 * ドーナツ言語内でのクロージャやオブジェクトなどのクラス
 */
class DonutObject : public HeapObject {
private:
	std::map<std::string, Slot> slots_;
public:
	DonutObject(World* const world);
	virtual ~DonutObject() noexcept = default;
protected: /* 継承用 */
	DonutObject(World* const world, const std::string& providerName);
protected:
	virtual std::string toStringImpl() const override;
	virtual std::string providerNameImpl() const override;
	virtual int toIntImpl() const override;
	virtual float toFloatImpl() const override;
	virtual bool toBoolImpl() const override;
	virtual bool haveImpl(const std::string& name) const override;
	virtual bool haveOwnImpl(const std::string& name) const override;
	virtual Handler<Object> storeImpl(const std::string& name, Handler<Object> obj) override;
	virtual Handler<Object> loadImpl(const std::string& name) const override;
	virtual void walkImpl(ObjectWalker* walker) override;
	virtual void seekImpl(timestamp_t time) override;
};

}}

//---------------------------------------------------------------------------------------------------------------------

namespace chisa {
namespace donut {

/**
 * StringやDoubleなどの組み込みオブジェクトと、
 * ユーザーの定義するネイティブクラスの元になるクラス
 */
class NativeObject : public HeapObject {
private:
	DonutObject* prototype_;
protected:
	NativeObject(World* const world, const std::string& providerName);
public:
	virtual ~NativeObject() noexcept = default;
protected:
	virtual std::string toStringImpl() const override;
	virtual std::string providerNameImpl() const override { return this->providerName(); }

	virtual int toIntImpl() const override;
	virtual float toFloatImpl() const override;
	virtual bool toBoolImpl() const override;
	virtual bool haveImpl(const std::string& name) const override;
	virtual bool haveOwnImpl(const std::string& name) const override;
	virtual Handler<Object> storeImpl(const std::string& name, Handler<Object> obj) override;
	virtual Handler<Object> loadImpl(const std::string& name) const override;
	virtual void walkImpl(ObjectWalker* walker) override;
	virtual void seekImpl(timestamp_t time) override;
};

}}

//---------------------------------------------------------------------------------------------------------------------

namespace chisa {
namespace donut {

class NativeClosureObject : public HeapObject {
private:
	std::string const closureName_;
public:
	NativeClosureObject(World* const world, const std::string& objectProviderName, const std::string& closureName)
	:HeapObject(world, objectProviderName),closureName_(closureName) {};
	virtual ~NativeClosureObject() noexcept = default;
	std::string closureName() const noexcept { return this->closureName_; };
protected:
	virtual std::string toStringImpl() const override;
	virtual std::string providerNameImpl() const override { return this->providerName(); }

	virtual int toIntImpl() const override;
	virtual float toFloatImpl() const override;
	virtual bool toBoolImpl() const override;
	virtual bool haveImpl(const std::string& name) const override;
	virtual bool haveOwnImpl(const std::string& name) const override;
	virtual Handler<Object> storeImpl(const std::string& name, Handler<Object> obj) override;
	virtual Handler<Object> loadImpl(const std::string& name) const override;
	virtual void walkImpl(ObjectWalker* walker) override;
	virtual void seekImpl(timestamp_t time) override;
};

}}

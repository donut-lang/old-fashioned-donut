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
#include <unordered_map>
#include "../../Handler.h"
#include "../../util/ClassUtil.h"
#include "../Exception.h"
#include "Slot.h"

namespace chisa {
namespace donut {
class Heap;
class ObjectWalker;

/**
 * すべての窓口となるクラス。
 */
class Object : public HandlerBody<Object> {
	DISABLE_COPY_AND_ASSIGN(Object);
public:
	static constexpr const int TagShift = 2U;
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
	std::string toString(const Handler<Heap>& heap) const;
	int toInt(const Handler<Heap>& heap) const;
	float toFloat(const Handler<Heap>& heap) const;
	bool toBool(const Handler<Heap>& heap) const;
	bool have(const Handler<Heap>& heap, const std::string& name) const;
	bool have(const Handler<Heap>& heap, const int& idx) const;
	bool haveOwn(const Handler<Heap>& heap, const std::string& name) const;
	bool haveOwn(const Handler<Heap>& heap, const int& idx) const;
	Handler<Object> store(const Handler<Heap>& heap, const std::string& name, Handler<Object> obj);
	Handler<Object> store(const Handler<Heap>& heap, const int& idx, Handler<Object> obj);
	Handler<Object> load(const Handler<Heap>& heap, const std::string& name) const;
	Handler<Object> load(const Handler<Heap>& heap, const int& idx) const;
	std::string providerName(const Handler<Heap>& heap) const;
	object_desc_t toDescriptor() const noexcept;
public:
	inline bool isObject() const noexcept { return Tag::Obj==tag(); };
	inline bool isNull() const noexcept { return Tag::Null==tag(); };
	inline bool isBool() const noexcept { return Tag::Bool==tag(); };
	inline bool isInt() const noexcept { return Tag::Int==tag(); };
	inline int tag() const noexcept { return static_cast<int>( reinterpret_cast<std::intptr_t>(this) ) & Tag::Mask; };
	inline void incref( bool check ) { if(isObject()) { this->HandlerBody<Object>::incref(check); } }
	inline void decref() { if(isObject()) { this->HandlerBody<Object>::decref(); } };
	inline void mark(const Handler<Heap>& heap, int color) { if(isObject()){ this->markImpl(heap, color); } }
	inline void onSeekNotify(const Handler<Heap>& heap){ if(isObject()){ this->onSeekNotifyImpl(heap); } };
	inline void onDiscardFutureNotify(const Handler<Heap>& heap){ if(isObject()){ this->onDiscardFutureNotifyImpl(heap); } };
	inline void onDiscardHistoryNotify(const Handler<Heap>& heap){ if(isObject()){ this->onDiscardHistoryNotifyImpl(heap); } };
protected: /* 実装すべきもの */
	virtual std::string toStringImpl(const Handler<Heap>& heap) const = 0;
	virtual std::string providerNameImpl(const Handler<Heap>& heap) const = 0;
	virtual int toIntImpl(const Handler<Heap>& heap) const = 0;
	virtual float toFloatImpl(const Handler<Heap>& heap) const = 0;
	virtual bool toBoolImpl(const Handler<Heap>& heap) const = 0;
	virtual bool haveImpl(const Handler<Heap>& heap, const std::string& name) const = 0;
	virtual bool haveOwnImpl(const Handler<Heap>& heap, const std::string& name) const = 0;
	virtual Handler<Object> storeImpl(const Handler<Heap>& heap, const std::string& name, Handler<Object> obj) = 0;
	virtual Handler<Object> loadImpl(const Handler<Heap>& heap, const std::string& name) const = 0;
	virtual void markImpl(const Handler<Heap>& heap, int color) = 0;
	virtual void onSeekNotifyImpl(const Handler<Heap>& heap) = 0;
	virtual void onDiscardHistoryNotifyImpl(const Handler<Heap>& heap) = 0;
	virtual void onDiscardFutureNotifyImpl(const Handler<Heap>& heap) = 0;
	virtual object_desc_t toDescriptorImpl() const noexcept = 0;
public:
	virtual bool onFree() noexcept = 0;
public:
	static inline int isDescriptorPrimitive( object_desc_t const& desc ) noexcept { return (desc & Object::Tag::Mask) != Object::Tag::Obj; };
	static inline Handler<Object> decodePrimitiveDescriptor( object_desc_t const& desc ) noexcept {
		if(!Object::isDescriptorPrimitive(desc)) {
			throw DonutException(__FILE__, __LINE__, "[BUG] Decoding heap object descriptor.");
		}
		return Handler<Object>::__internal__fromRawPointerWithoutCheck( reinterpret_cast<Object*>(static_cast<intptr_t>(desc)) );
	};
	static inline objectid_t decodeHeapObjectDescriptor( object_desc_t const& desc ) noexcept {
		if(Object::isDescriptorPrimitive(desc)) {
			throw DonutException(__FILE__, __LINE__, "[BUG] Decoding primitive descriptor.");
		}
		return desc >> TagShift;
	};
};

}}
//---------------------------------------------------------------------------------------------------------------------

namespace chisa {
namespace donut {

class HeapObject : public Object {
private:
	const HandlerW<Heap> heap_;
	std::string const providerName_;
	objectid_t id_;
	bool erased_;
	int color_;
public:
	HeapObject(const Handler<Heap>& heap, const std::string& providerName);
	virtual ~HeapObject() noexcept = default;
public:
	inline std::string providerName() const noexcept { return this->providerName_; }
	inline objectid_t id() const noexcept { return this->id_; }
	virtual object_desc_t toDescriptorImpl() const noexcept override final {return (this->id_ << Object::TagShift) | Object::Tag::Obj; };
	inline void id(objectid_t const& nid) noexcept { this->id_ = nid; }
	inline void erase() noexcept { this->erased_ = true; if(refcount() == 0){ delete this; } };
public:
	virtual bool onFree() noexcept { if(this->erased_){ return false; }else{ return true; } };
	int color() noexcept { return this->color_; };
	inline bool used() { return this->refcount() > 0; };
protected:
	void color(const int color) noexcept { this->color_=color; };
};

struct CompareHeapById : std::binary_function<HeapObject* const&,HeapObject* const&,bool>{
	bool operator()(HeapObject* const& a, HeapObject* const& b){
		return a->id() < b->id();
	}
	bool operator()(HeapObject* const& a, objectid_t const& b){
		return a->id() < b;
	}
	bool operator()(objectid_t const& a, HeapObject* const& b){
		return a < b->id();
	}
};

}}
//---------------------------------------------------------------------------------------------------------------------

namespace chisa {
namespace donut {
/**
 * ドーナツ言語内でのクロージャやオブジェクトなどのクラス
 */
class DonutObject : public HeapObject {
	friend class DonutObjectProvider;
private:
	std::unordered_map<std::string, Slot> slots_;
public:
	DonutObject(const Handler<Heap>& heap);
	virtual ~DonutObject() noexcept = default;
protected: /* 継承用 */
	DonutObject(const Handler<Heap>& heap, const std::string& providerName);
protected:
	virtual std::string toStringImpl(const Handler<Heap>& heap) const override;
	virtual std::string providerNameImpl(const Handler<Heap>& heap) const override;
	virtual int toIntImpl(const Handler<Heap>& heap) const override;
	virtual float toFloatImpl(const Handler<Heap>& heap) const override;
	virtual bool toBoolImpl(const Handler<Heap>& heap) const override;
	virtual bool haveImpl(const Handler<Heap>& heap, const std::string& name) const override;
	virtual bool haveOwnImpl(const Handler<Heap>& heap, const std::string& name) const override;
	virtual Handler<Object> storeImpl(const Handler<Heap>& heap, const std::string& name, Handler<Object> obj) override;
	virtual Handler<Object> loadImpl(const Handler<Heap>& heap, const std::string& name) const override;
	virtual void markImpl(const Handler<Heap>& heap, int color) override;
	virtual void onSeekNotifyImpl(const Handler<Heap>& heap) override;
	virtual void onDiscardHistoryNotifyImpl(const Handler<Heap>& heap) override;
	virtual void onDiscardFutureNotifyImpl(const Handler<Heap>& heap) override;
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
	NativeObject(const Handler<Heap>& heap, const std::string& providerName);
public:
	virtual ~NativeObject() noexcept = default;
protected:
	virtual std::string toStringImpl(const Handler<Heap>& heap) const override;
	virtual std::string providerNameImpl(const Handler<Heap>& heap) const override { return this->providerName(); }

	virtual int toIntImpl(const Handler<Heap>& heap) const override;
	virtual float toFloatImpl(const Handler<Heap>& heap) const override;
	virtual bool toBoolImpl(const Handler<Heap>& heap) const override;
	virtual bool haveImpl(const Handler<Heap>& heap, const std::string& name) const override;
	virtual bool haveOwnImpl(const Handler<Heap>& heap, const std::string& name) const override;
	virtual Handler<Object> storeImpl(const Handler<Heap>& heap, const std::string& name, Handler<Object> obj) override;
	virtual Handler<Object> loadImpl(const Handler<Heap>& heap, const std::string& name) const override;
	virtual void markImpl(const Handler<Heap>& heap, int color) override;
	virtual void onSeekNotifyImpl(const Handler<Heap>& heap) override;
};

}}

//---------------------------------------------------------------------------------------------------------------------

namespace chisa {
namespace donut {

class NativeClosureObject : public HeapObject {
private:
	std::string const closureName_;
public:
	NativeClosureObject(const Handler<Heap>& heap, const std::string& objectProviderName, const std::string& closureName)
	:HeapObject(heap, objectProviderName),closureName_(closureName) {};
	virtual ~NativeClosureObject() noexcept = default;
	std::string closureName() const noexcept { return this->closureName_; };
protected:
	virtual std::string toStringImpl(const Handler<Heap>& heap) const override;
	virtual std::string providerNameImpl(const Handler<Heap>& heap) const override { return this->providerName(); }

	virtual int toIntImpl(const Handler<Heap>& heap) const override;
	virtual float toFloatImpl(const Handler<Heap>& heap) const override;
	virtual bool toBoolImpl(const Handler<Heap>& heap) const override;
	virtual bool haveImpl(const Handler<Heap>& heap, const std::string& name) const override;
	virtual bool haveOwnImpl(const Handler<Heap>& heap, const std::string& name) const override;
	virtual Handler<Object> storeImpl(const Handler<Heap>& heap, const std::string& name, Handler<Object> obj) override;
	virtual Handler<Object> loadImpl(const Handler<Heap>& heap, const std::string& name) const override;
	virtual void markImpl(const Handler<Heap>& heap, int color) override;
	virtual void onSeekNotifyImpl(const Handler<Heap>& heap) override;
	virtual void onDiscardHistoryNotifyImpl(const Handler<Heap>& heap) override;
	virtual void onDiscardFutureNotifyImpl(const Handler<Heap>& heap) override;
};

}}

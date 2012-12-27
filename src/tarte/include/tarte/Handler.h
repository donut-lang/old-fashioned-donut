/**
 * Tarte
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
#include <utility>
#include <typeinfo>
#include "Thread.h"
#include "Exception.h"

namespace tarte {

/**********************************************************************************************************************
 * Handler
 **********************************************************************************************************************/

template<class S>
class Handler
{
private:
	S* sprite;
	//void *operator new(std::size_t) = delete;
	//void operator delete(void* pv) = delete;
	template <typename T, typename U> friend constexpr bool operator==(Handler<T> const& a, Handler<U> const& b) noexcept;
	template <typename T, typename U> friend constexpr bool operator!=(Handler<T> const& a, Handler<U> const& b) noexcept;
	template <typename T, typename U> friend constexpr bool operator>(Handler<T> const& a, Handler<U> const& b) noexcept;
	template <typename T, typename U> friend constexpr bool operator<(Handler<T> const& a, Handler<U> const& b) noexcept;
	template <typename T, typename U> friend constexpr bool operator>=(Handler<T> const& a, Handler<U> const& b) noexcept;
	template <typename T, typename U> friend constexpr bool operator<(Handler<T> const& a, Handler<U> const& b) noexcept;
public:
	typedef S value_type;
	inline static Handler<S> __internal__fromRawPointerWithoutCheck(S* const sprite) noexcept
	{
		Handler<S> spr;
		spr.sprite = sprite;
		if(spr.sprite){
			spr.sprite->incref(false);
		}
		return spr;
	}
	Handler() noexcept:sprite(nullptr){};
	inline explicit Handler(S* const sprite)
	:sprite(sprite)
	{
		if(this->sprite){
			this->sprite->incref( true );
		}
	}
	inline Handler(Handler<S> const& other) noexcept
	:sprite(other.sprite)
	{
		if(this->sprite){
			this->sprite->incref( false );
		}
	}
	Handler(Handler<S>&& other) noexcept
	:sprite(other.sprite)
	{
		other.sprite = nullptr;
	}
	template <class T>
	inline Handler(Handler<T> const& other) noexcept
	:sprite(other.get())
	{
		if(this->sprite){
			this->sprite->incref( false );
		}
	}
	inline Handler<S>& operator=(Handler<S> const& other) noexcept
	{
		if(other.sprite){
			other.sprite->incref( false );
		}
		if(this->sprite){
			this->sprite->decref();
		}
		this->sprite = other.sprite;
		return *this;
	}
	inline Handler<S>& operator=(Handler<S>&& other) noexcept
	{
		if(this->sprite){
			this->sprite->decref();
			this->sprite = nullptr;
		}
		this->sprite = other.sprite;
		other.sprite = nullptr;
		return *this;
	}
	template <class T>
	Handler<S>& operator=(Handler<T> const& other) noexcept
	{
		if(other.get()){
			other.get()->incref( false );
		}
		if(this->sprite){
			this->sprite->decref();
		}
		this->sprite = other.get();
		return *this;
	}
	template<class T>
	Handler<T> cast() const
	{
		T* spr = dynamic_cast<T*>(this->sprite);
		if(!spr){
			TARTE_EXCEPTION(Exception, "[BUG] Sprite::Handler / failed to cast %s to %s", typeid(this->sprite).name(), typeid(spr).name());
		}
		return Handler<T>::__internal__fromRawPointerWithoutCheck(spr);
	}
	template<class T>
	Handler<T> tryCast() const
	{
		return Handler<T>::__internal__fromRawPointerWithoutCheck( dynamic_cast<T*>(this->sprite) );
	}
	inline ~Handler() noexcept
	{
		if(this->sprite){
			this->sprite->decref();
			this->sprite = nullptr;
		}
	}
	inline S* operator->() const noexcept
	{
		return this->sprite;
	}
	inline S& operator*() const noexcept{
		return *this->sprite;
	}
	inline S* get() const noexcept
	{
		return this->sprite;
	}
	inline explicit operator bool() const noexcept
	{
		return this->sprite != 0;
	}
	inline void swap(Handler<S>& other) noexcept
	{
		using std::swap;
		swap(other.sprite, this->sprite);
	}
	inline void reset() noexcept
	{
		Handler<S>().swap(*this);
	}
	inline int refcount() const noexcept { return this->sprite ? this->sprite->refcount_ : 0; };
};

/**********************************************************************************************************************
 * WeakHandler
 **********************************************************************************************************************/

namespace internal {
class WeakHandlerEntity {
private:
	bool dead_;
	int refcount_;
	WeakHandlerEntity():dead_(false), refcount_(0){}
	~WeakHandlerEntity() noexcept = default;
private:
	WeakHandlerEntity(WeakHandlerEntity const& other) = delete;
	WeakHandlerEntity(WeakHandlerEntity&& other) = delete;
	WeakHandlerEntity& operator=(WeakHandlerEntity const& other) = delete;
	WeakHandlerEntity& operator=(WeakHandlerEntity&& other) = delete;
public:
	inline void notifyDead(){
		this->dead_ = true;
		if(this->refcount_ == 0){
			delete this;
		}
	};
	inline explicit operator bool () const noexcept {
		return !(this->expired());
	}
	inline bool expired () const noexcept {
		return (this->dead_);
	}
	inline void incref() noexcept {
		this->refcount_++;
	}
	inline void decref() noexcept {
		this->refcount_--;
		if(this->refcount_ == 0 && expired()){
			delete this;
		}
	}
	inline int refcount() const noexcept{
		return this->refcount_;
	}
public:
	template <typename S>
	inline static WeakHandlerEntity* create(S* self){
		if(!self){
			return nullptr;
		}
		if(!self->weakEntity_){
			self->weakEntity_ = new WeakHandlerEntity;
		}
		return self->weakEntity_;
	}
};
}

template<class S>
class HandlerW
{
private:
	S* sprite;
	internal::WeakHandlerEntity* entity;
	template <typename T, typename U> friend constexpr bool operator==(HandlerW<T> const& a, HandlerW<U> const& b) noexcept;
	template <typename T, typename U> friend constexpr bool operator!=(HandlerW<T> const& a, HandlerW<U> const& b) noexcept;
	template <typename T, typename U> friend constexpr bool operator>(HandlerW<T> const& a, HandlerW<U> const& b) noexcept;
	template <typename T, typename U> friend constexpr bool operator<(HandlerW<T> const& a, HandlerW<U> const& b) noexcept;
	template <typename T, typename U> friend constexpr bool operator>=(HandlerW<T> const& a, HandlerW<U> const& b) noexcept;
	template <typename T, typename U> friend constexpr bool operator<(HandlerW<T> const& a, HandlerW<U> const& b) noexcept;
public:
	HandlerW()
	:sprite(nullptr)
	,entity(nullptr)
	{
	}
	HandlerW(Handler<S> const& hand)
	:sprite(nullptr)
	,entity(internal::WeakHandlerEntity::create<S>(hand.get()))
	{
		if(this->entity){
			this->entity->incref();
			this->sprite = hand.get();
		}
	}
	template <typename T>
	HandlerW(Handler<T> const& hand)
	:sprite(nullptr)
	 ,entity(internal::WeakHandlerEntity::create<T>(hand.get()))
	{
		if(this->entity){
			this->entity->incref();
			this->sprite = hand.get();
		}
	}
	HandlerW(S* spr)
	:sprite(nullptr)
	,entity(internal::WeakHandlerEntity::create<S>(spr))
	{
		if(this->entity){
			this->entity->incref();
			this->sprite = spr;
		}
	}
	HandlerW(HandlerW<S> const& other)
	:sprite(nullptr)
	,entity(other.entity)
	{
		if(this->entity){
			this->entity->incref();
			this->sprite = other.sprite;
		}
	}
	HandlerW(HandlerW<S>&& other):sprite(other.sprite),entity(other.entity) {
		other.sprite = nullptr;
		other.entity = nullptr;
	}
	inline HandlerW& operator=(HandlerW<S> const& other)
	{
		if(other.entity){
			other.entity->incref();
		}
		if(this->entity){
			this->entity->decref();
		}
		this->sprite = other.sprite;
		this->entity = other.entity;
		return *this;
	}
	template <typename T>
	inline HandlerW& operator=(HandlerW<T> const& other)
	{
		if(other.entity){
			other.entity->incref();
		}
		if(this->entity){
			this->entity->decref();
		}
		this->sprite = other.sprite;
		this->entity = other.entity;
		return *this;
	}
	inline HandlerW& operator=(HandlerW<S>&& other)
	{
		if(this->entity){
			this->entity->decref();
		}
		this->sprite = other.sprite;
		this->entity = other.entity;
		other.entity = nullptr;
		other.sprite = nullptr;
		return *this;
	}
	template <typename T>
	inline HandlerW& operator=(HandlerW<T>&& other)
	{
		if(this->entity){
			this->entity->decref();
		}
		this->sprite = other.sprite;
		this->entity = other.entity;
		other.entity = nullptr;
		other.sprite = nullptr;
		return *this;
	}
	~HandlerW() noexcept{
		if(this->entity){
			this->entity->decref();
		}
		this->entity=nullptr;
		this->sprite=nullptr;
	}
	inline Handler<S> lock() const noexcept {
		if(expired()){
			return Handler<S>();
		}
		return Handler<S>::__internal__fromRawPointerWithoutCheck(sprite);
	}
	inline bool expired() const noexcept {
		return (!this->entity) || this->entity->expired();
	}
	inline void swap(HandlerW<S>& other) noexcept
	{
		using std::swap;
		swap(other.entity, this->entity);
		swap(other.sprite, this->sprite);
	}
	inline void reset() noexcept
	{
		HandlerW<S>().swap(*this);
	}
	inline int refcount() const noexcept { return this->entity ? this->entity->refcount() : 0; };
};

/**********************************************************************************************************************
 * Handler Body
 **********************************************************************************************************************/

template <bool atomic=false>
struct _HandlerBodyTypes{
	typedef int RefCounterType;
	typedef bool DelFlagType;
};

template <>
struct _HandlerBodyTypes<true>{
	typedef std::atomic<int> RefCounterType;
	typedef bool DelFlagType;
};

template <class Derived, bool atomic=false>
class HandlerBody {
private:
	HandlerBody(const HandlerBody<Derived, atomic>& other) = delete;
	HandlerBody(HandlerBody<Derived, atomic>&& other) = delete;
	const HandlerBody<Derived, atomic>& operator=(const HandlerBody<Derived, atomic>& other) = delete;
	const HandlerBody<Derived, atomic>& operator=(const HandlerBody<Derived, atomic>&& other) = delete;
	template <typename T> friend class Handler;
	template <typename T> friend class HandlerW;
	friend class internal::WeakHandlerEntity;
private:
	typename _HandlerBodyTypes<atomic>::RefCounterType refcount_;
	typename _HandlerBodyTypes<atomic>::DelFlagType deleted;
	internal::WeakHandlerEntity* weakEntity_;
protected:
	HandlerBody()
	:refcount_(0), deleted(false), weakEntity_(nullptr) {}
	virtual ~HandlerBody() noexcept (true) {}; //XXX: GCCのバグでデフォルトにできない？
protected:
	inline int refcount() const noexcept { return this->refcount_; };
	inline Handler<const Derived> self() const { return Handler<Derived>::__internal__fromRawPointerWithoutCheck(static_cast<const Derived*>(this)); };
	inline Handler<Derived> self() { return Handler<Derived>::__internal__fromRawPointerWithoutCheck(static_cast<Derived*>(this)); };
protected:
	inline void incref( bool check ) const{
		HandlerBody<Derived, atomic>* const self = const_cast<HandlerBody<Derived, atomic>*>(this);
		if((self->refcount_++) != 0 && check){
			int const val = --self->refcount_;
			TARTE_EXCEPTION(Exception, "[BUG] Handler created, but refcount = %d, not zero.", val);
		}
	}
	inline void decref() const{
		HandlerBody<Derived, atomic>* const self = const_cast<HandlerBody<Derived, atomic>*>(this);
		--self->refcount_;
		if(self->refcount_ < 0){
			int const val = self->refcount_;
			TARTE_EXCEPTION(Exception, "[BUG] Handler refcount = %d < 0", val);
		}else if(self->refcount_ == 0 && !self->deleted){
			self->deleted = true;
			if(self->weakEntity_){
				self->weakEntity_->notifyDead();
				self->weakEntity_ = nullptr;
			}
			if(static_cast<Derived*>(self)->onFree()) {
				//recycle
				self->deleted = false;
			}else{
				delete self;
			}
		}
	}
};

/**********************************************************************************************************************
 * Swap
 **********************************************************************************************************************/

template<class T>
inline void swap(Handler<T>& a, Handler<T>& b) noexcept
{
	a.swap(b);
}

template<class T>
inline void swap(HandlerW<T>& a, HandlerW<T>& b) noexcept
{
	a.swap(b);
}

/**********************************************************************************************************************
 * Handler Compare
 **********************************************************************************************************************/

template<class T, class U>
constexpr inline bool operator==(Handler<T> const& a, Handler<U> const& b) noexcept
{
	return a.sprite == b.sprite;
}

template<class T, class U>
constexpr inline bool operator!=(Handler<T> const& a, Handler<U> const& b) noexcept
{
	return a.sprite != b.sprite;
}

template<class T, class U>
constexpr inline bool operator<(Handler<T> const& a, Handler<U> const& b) noexcept
{
	return a.sprite < b.sprite;
}

template<class T, class U>
constexpr inline bool operator<=(Handler<T> const& a, Handler<U> const& b) noexcept
{
	return a.sprite <= b.sprite;
}

template<class T, class U>
constexpr inline bool operator>(Handler<T> const& a, Handler<U> const& b) noexcept
{
	return a.sprite > b.sprite;
}

template<class T, class U>
constexpr inline bool operator>=(Handler<T> const& a, Handler<U> const& b) noexcept
{
	return a.sprite >= b.sprite;
}

/**********************************************************************************************************************
 * WeakHandler Compare
 **********************************************************************************************************************/

template<class T, class U>
constexpr inline bool operator==(HandlerW<T> const& a, HandlerW<U> const& b) noexcept
{
	return a.sprite == b.sprite;
}

template<class T, class U>
constexpr inline bool operator!=(HandlerW<T> const& a, HandlerW<U> const& b) noexcept
{
	return a.sprite != b.sprite;
}

template<class T, class U>
constexpr inline bool operator<(HandlerW<T> const& a, HandlerW<U> const& b) noexcept
{
	return a.sprite < b.sprite;
}

template<class T, class U>
constexpr inline bool operator<=(HandlerW<T> const& a, HandlerW<U> const& b) noexcept
{
	return a.sprite <= b.sprite;
}

template<class T, class U>
constexpr inline bool operator>(HandlerW<T> const& a, HandlerW<U> const& b) noexcept
{
	return a.sprite > b.sprite;
}

template<class T, class U>
constexpr inline bool operator>=(HandlerW<T> const& a, HandlerW<U> const& b) noexcept
{
	return a.sprite >= b.sprite;
}

}


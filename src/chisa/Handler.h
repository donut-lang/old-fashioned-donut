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
#include <utility>
#include <typeinfo>
#include "util/Thread.h"
#include "logging/Exception.h"

namespace chisa {

template<class S>
class Handler
{
private:
	S* sprite;
	void *operator new(std::size_t) = delete;
	void operator delete(void* pv) = delete;
public:
	static Handler<S> __internal__fromRawPointerWithoutCheck(S* const sprite) noexcept
	{
		Handler<S> spr;
		spr.sprite = sprite;
		if(spr.sprite){
			spr.sprite->incref();
		}
		return spr;
	}
	Handler() noexcept:sprite(nullptr){};
	explicit Handler(S* const sprite)
	:sprite(sprite)
	{
		if(this->sprite){
			if(this->sprite->refcount_ != 0){
				throw logging::Exception(__FILE__, __LINE__, "[BUG] Sprite::Handler created, but refcount = %d, not zero.", this->sprite->refcount_);
			}
			this->sprite->incref();
		}
	}
	Handler(const Handler<S>& other) noexcept
	:sprite(other.sprite)
	{
		if(this->sprite){
			this->sprite->incref();
		}
	}
	Handler(Handler<S>&& other) noexcept
	:sprite(other.sprite)
	{
		other.sprite = nullptr;
	}
	template <class T>
	Handler(const Handler<T>& other) noexcept
	:sprite(other.get())
	{
		if(this->sprite){
			this->sprite->incref();
		}
	}
	Handler<S>& operator=(const Handler<S>& other) noexcept
	{
		if(other.sprite){
			other.sprite->incref();
		}
		if(this->sprite){
			this->sprite->decref();
		}
		this->sprite = other.sprite;
		return *this;
	}
	Handler<S>& operator=(Handler<S>&& other) noexcept
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
	Handler<S>& operator=(const Handler<T>& other) noexcept
	{
		if(other.get()){
			other.get()->incref();
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
			throw logging::Exception(__FILE__, __LINE__, "[BUG] Sprite::Handler / failed to cast %s to %s", typeid(this->sprite).name(), typeid(spr).name());
		}
		return Handler<T>::__internal__fromRawPointerWithoutCheck(spr);
	}
	~Handler() noexcept
	{
		if(this->sprite){
			this->sprite->decref();
			this->sprite = nullptr;
		}
	}
	S* operator->() const noexcept
	{
		return this->sprite;
	}
	S* get() const noexcept
	{
		return this->sprite;
	}
	explicit operator bool() const noexcept
	{
		return this->sprite != 0;
	}
	void swap(Handler<S>& other) noexcept
	{
		using std::swap;
		swap(other.sprite, this->sprite);
	}
	void reset() noexcept
	{
		Handler<S>().swap(*this);
	}
};

namespace internal {
template<class S>
class WeakHandlerEntity {
private:
	S* sprite;
	int refcount_;
	WeakHandlerEntity() = delete;
	~WeakHandlerEntity() { this->sprite = nullptr; }
	WeakHandlerEntity(S* spr):sprite(spr), refcount_(0){}
private:
	WeakHandlerEntity(const WeakHandlerEntity<S>& other) = delete;
	WeakHandlerEntity(WeakHandlerEntity<S>&& other) = delete;
	WeakHandlerEntity<S>& operator=(const WeakHandlerEntity<S>& other) = delete;
	WeakHandlerEntity<S>& operator=(WeakHandlerEntity<S>&& other) = delete;
public:
	void notifyDead(){
		this->sprite = nullptr;
		if(this->refcount_ == 0){
			delete this;
		}
	};
	explicit operator bool () const noexcept {
		return this->sprite;
	}
	bool expired () const noexcept {
		return this->sprite;
	}
	S* read() const noexcept {
		return this->sprite;
	}
	void incref() noexcept {
		this->refcount_++;
	}
	void decref() noexcept {
		this->refcount_--;
		if(this->refcount_ == 0 && !this->sprite){
			delete this;
		}
	}
public:
	static WeakHandlerEntity<S>* create(S* self){
		if(!self){
			return nullptr;
		}
		if(!self->weakEntity_){
			self->weakEntity_ = new WeakHandlerEntity<S>(self);
		}
		return self->weakEntity_;
	}
};
}

template<class S>
class HandlerW
{
private:
	internal::WeakHandlerEntity<S>* entity;
public:
	HandlerW():entity(nullptr){};
	HandlerW(const Handler<S>& hand)
	:entity(internal::WeakHandlerEntity<S>::create(hand.get()))
	{
		if(this->entity){
			this->entity->incref();
		}
	}
	HandlerW(S* spr)
	:entity(internal::WeakHandlerEntity<S>::create(spr))
	{
		if(this->entity){
			this->entity->incref();
		}
	}
	HandlerW(const HandlerW<S>& other):entity(other.entity)
	{
		if(this->entity){
			this->entity->incref();
		}
	}
	HandlerW(HandlerW<S>&& other):entity(other.entity) {}
	HandlerW& operator=(const HandlerW<S>& other)
	{
		if(this->entity){
			this->entity->decref();
		}
		this->entity = other.entity;
		if(this->entity){
			this->entity->incref();
		}
		return *this;
	}
	HandlerW& operator=(HandlerW<S>&& other)
	{
		if(this->entity){
			this->entity->decref();
		}
		this->entity = other.entity;
		return *this;
	}
	~HandlerW() noexcept{
		if(this->entity){
			this->entity->decref();
		}
	}
	Handler<S> lock() noexcept {
		if(expired()){
			return Handler<S>();
		}
		return Handler<S>::__internal__fromRawPointerWithoutCheck(entity->read());
	}
	bool expired() noexcept {
		return !this->entity || !*(this->entity);
	}
	void swap(HandlerW<S>& other) noexcept
	{
		using std::swap;
		swap(other.entity, this->entity);
	}
	void reset() noexcept
	{
		HandlerW<S>().swap(*this);
	}
};

template<class T>
void swap(Handler<T>& a, Handler<T>& b) noexcept
{
	a.swap(b);
}

template<class T, class U>
bool operator==(const Handler<T>& a, const Handler<U>& b) noexcept
{
	return a.get() == b.get();
}

template<class T, class U>
bool operator!=(const Handler<T>& a, const Handler<U>& b) noexcept
{
	return a.get() != b.get();
}

template <class Derived, bool atomic=false>
class HandlerBody {
private:
	HandlerBody(const HandlerBody<Derived, atomic>& other) = delete;
	HandlerBody(HandlerBody<Derived, atomic>&& other) = delete;
	const HandlerBody<Derived, atomic>& operator=(const HandlerBody<Derived, atomic>& other) = delete;
	const HandlerBody<Derived, atomic>& operator=(const HandlerBody<Derived, atomic>&& other) = delete;
	template <typename T> friend class chisa::Handler;
	template <typename T> friend class chisa::HandlerW;
	template <typename T> friend class chisa::internal::WeakHandlerEntity;
private:
	int refcount_;
	bool deleted;
	chisa::internal::WeakHandlerEntity<Derived>* weakEntity_;
protected:
	HandlerBody()
	:refcount_(0), deleted(false), weakEntity_(nullptr) {}
	virtual ~HandlerBody() noexcept (true) {}; //XXX: GCCのバグでデフォルトにできない？
protected:
	Handler<Derived> self() { return Handler<Derived>::__internal__fromRawPointerWithoutCheck(this); };
protected: /* from Handler */
	void increfImpl() noexcept { this->refcount_++; }
	void decrefImpl(){
		this->refcount_--;
		if(this->refcount_ < 0){
			throw logging::Exception(__FILE__, __LINE__, "[BUG] Handler refcount = %d < 0", this->refcount_);\
		}else if(this->refcount_ == 0){
			if(deleted){
				return;
			}
			this->deleted = true;
			if(this->weakEntity_){
				this->weakEntity_->notifyDead();
				this->weakEntity_ = nullptr;
			}
			static_cast<Derived*>(this)->onFree();
		}
	}
private:
	void incref() noexcept { static_cast<Derived*>(this)->increfImpl(); }
	void decref(){ static_cast<Derived*>(this)->decrefImpl(); }
};

template <class Derived>
class HandlerBody<Derived, true> {
private:
	std::mutex ref_mutex_;
	HandlerBody(const HandlerBody<Derived, true>& other) = delete;
	HandlerBody(HandlerBody<Derived, true>&& other) = delete;
	const HandlerBody<Derived, true>& operator=(const HandlerBody<Derived, true>& other) = delete;
	const HandlerBody<Derived, true>& operator=(const HandlerBody<Derived, true>&& other) = delete;
	template <typename T> friend class chisa::Handler;
	template <typename T> friend class chisa::HandlerW;
	template <typename T> friend class chisa::internal::WeakHandlerEntity;
private:
	int refcount_;
	bool deleted;
	chisa::internal::WeakHandlerEntity<Derived>* weakEntity_;
protected:
	HandlerBody()
	:refcount_(0), deleted(false), weakEntity_(nullptr) {}
	virtual ~HandlerBody() noexcept (true) {};//XXX: GCCのバグ？
protected:
	Handler<Derived> self() { return Handler<Derived>::__internal__fromRawPointerWithoutCheck(this); };
protected: /* from Handler */
	void increfImpl() noexcept { std::unique_lock<std::mutex> lock(this->ref_mutex_);this->refcount_++; }
	void decrefImpl(){
		std::unique_lock<std::mutex> lock(this->ref_mutex_);
		this->refcount_--;
		if(this->refcount_ < 0){
			throw logging::Exception(__FILE__, __LINE__, "[BUG] Handler refcount = %d < 0", this->refcount_);\
		}else if(this->refcount_ == 0){
			if(deleted){
				return;
			}
			this->deleted = true;
			if(this->weakEntity_){
				this->weakEntity_->notifyDead();
				this->weakEntity_ = nullptr;
			}
			static_cast<Derived*>(this)->onFree();
		}
	}
private:
	void incref() noexcept { static_cast<Derived*>(this)->increfImpl(); }
	void decref(){ static_cast<Derived*>(this)->decrefImpl(); }
};


}


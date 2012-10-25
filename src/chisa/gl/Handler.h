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
#include "../logging/Exception.h"

namespace chisa {
namespace gl {

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
	Handler() noexcept:sprite(0){};
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
			this->sprite = 0;
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
	S* sprite;
private:
	WeakHandlerEntity() = delete;
	WeakHandlerEntity(const WeakHandlerEntity<S>& o) = delete;
	WeakHandlerEntity(WeakHandlerEntity<S>&& o) = delete;
	WeakHandlerEntity& operator=(const WeakHandlerEntity<S>& o) = delete;
	WeakHandlerEntity& operator=(WeakHandlerEntity<S>&& o) = delete;
	~WeakHandlerEntity() { this->sprite = nullptr; }
	WeakHandlerEntity(S* spr){ this->sprite = spr; };
public:
	void notifyDead(){
		this->sprite = nullptr;
	};
	explicit operator bool () const noexcept {
		return this->sprite;
	}
	bool expired () const noexcept {
		return this->sprite;
	}
	static WeakHandlerEntity<S>* create(S* self){
		if(!self->weakEntity){
			self->weakEntity = new WeakHandlerEntity<S>(self);
		}
		return self->weakEntity;
	}
};
}

template<class S>
class HandlerW
{

};

template<class T>
void swap(Handler<T>& a, Handler<T>& b) noexcept
{
	a.swap(b);
}

}}


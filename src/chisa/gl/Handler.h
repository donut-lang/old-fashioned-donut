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
public:
	static Handler<S> __internal__fromRawPointerWithoutCheck(S* const sprite)
	{
		Handler<S> spr;
		spr.sprite = sprite;
		if(spr.sprite){
			spr.sprite->incref();
		}
		return spr;
	}
	Handler():sprite(0){};
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
	Handler(const Handler<S>& other)
	:sprite(other.sprite)
	{
		if(this->sprite){
			this->sprite->incref();
		}
	}
	Handler(Handler<S>&& other)
	:sprite(other.sprite)
	{
		other.sprite = nullptr;
	}
	template <class T>
	Handler(const Handler<T>& other)
	:sprite(other.get())
	{
		if(this->sprite){
			this->sprite->incref();
		}
	}
	Handler<S>& operator=(const Handler<S>& other)
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
	Handler<S>& operator=(const Handler<T>& other)
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
	virtual ~Handler()
	{
		if(this->sprite){
			this->sprite->decref();
			this->sprite = 0;
		}
	}
	S* operator->() const
	{
		return this->sprite;
	}
	S* get() const
	{
		return this->sprite;
	}
	explicit operator bool() const
	{
		return this->sprite != 0;
	}
	void swap(Handler<S>& other)
	{
		using std::swap;
		swap(other.sprite, this->sprite);
	}
	void reset()
	{
		Handler<S>().swap(*this);
	}
};

template<class T>
void swap(Handler<T>& a, Handler<T>& b)
{
	a.swap(b);
}

}}


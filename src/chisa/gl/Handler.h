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

#ifndef Chisa_HANDLER_H__CPP_
#define Chisa_HANDLER_H__CPP_


#include <utility>
#include "../logging/Exception.h"

namespace chisa {
namespace gl {

template <typename T>
class Handler
{
private:
	T* obj_;
protected:
	Handler():obj_(nullptr){};
	explicit Handler(T* const obj)
	:obj_(obj)
	{
		if(this->obj_){
			if(this->obj_->refcount_ != 0){
				throw logging::Exception(__FILE__, __LINE__, "[BUG] Sprite::Handler created, but refcount = %d, not zero.", this->obj_->refcount_);
			}
			this->obj_->incref();
		}
	}
public:
	Handler(const Handler<T>& other)
	:obj_(other.obj_)
	{
		if(this->obj_){
			this->obj_->incref();
		}
	}
	Handler<T>& operator=(const Handler<T>& other)
	{
		if(other.obj_){
			other.obj_->incref();
		}
		if(this->obj_){
			this->obj_->decref();
		}
		this->obj_ = other.obj_;
		return *this;
	}
	virtual ~Handler()
	{
		if(this->obj_){
			this->obj_->decref();
			this->obj_ = 0;
		}
	}
	T* operator->() const
	{
		return this->obj_;
	}
	T* get() const
	{
		return this->obj_;
	}
	explicit operator bool() const
	{
		return this->obj_ != 0;
	}
	void swap(Handler<T>& other)
	{
		using std::swap;
		swap(other.obj_, this->obj_);
	}
	void reset()
	{
		Handler<T>().swap(*this);
	}
protected:
	T* obj() const
	{
		return this->obj_;
	}
};

template<class T>
void swap(Handler<T>& a, Handler<T>& b)
{
	a.swap(b);
}

}}


#endif /* INCLUDE_GUARD */

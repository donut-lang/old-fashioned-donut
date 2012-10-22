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
#include "Sprite.h"

namespace chisa {
namespace gl {

Sprite::Sprite(Canvas* const canvas)
:refcount_(0)
,canvas_(canvas)
{
}

Sprite::~Sprite()
{
}

void Sprite::incref()
{
	std::unique_lock<std::mutex> lock(this->ref_mutex_);
	this->refcount_++;
}
void Sprite::decref()
{
	std::unique_lock<std::mutex> lock(this->ref_mutex_);
	this->refcount_--;
	if(this->refcount_ < 0){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Sprite::Handler refcount = %d < 0", this->refcount_);
	}else if(this->refcount_ == 0){
		this->onFree();
	}
}

}}

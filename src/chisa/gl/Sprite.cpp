/*
 * Sprite.cpp
 *
 *  Created on: Oct 9, 2012
 *      Author: psi
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

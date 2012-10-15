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

#include "Thread.h"

namespace chisa {
namespace util {

void*  Thread::_run_impl(void* thread)
{
	reinterpret_cast<Thread*>(thread)->run();
	return nullptr;
}

Thread::Thread()
:thread_(nullptr)
,stopQueried_(false)
{
}

Thread::~Thread()
{
	if(this->thread_){
		this->join();
		delete this->thread_;
		this->thread_ = nullptr;
	}
}

void Thread::start()
{
	pthread_create(this->thread_, nullptr, Thread::_run_impl,this);
}

bool Thread::isRunning()
{
	if(this->thread_){
		return pthread_kill(*this->thread_, 0) == 0;
	}
	return false;
}

void Thread::join()
{
	if(this->thread_){
		pthread_join(*this->thread_, nullptr);
	}
}

void Thread::queryStop()
{
	if(!this->isRunning()){
		return;
	}
	{
		RWLock::WriteLock(this->stopLock_);
		this->stopQueried_ = true;
	}
}
bool Thread::isStopQueried()
{
	if(!this->isRunning()){
		return true;
	}
	{
		RWLock::ReadLock(this->stopLock_);
		return this->stopQueried_;
	}
}


}}

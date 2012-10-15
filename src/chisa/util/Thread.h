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

#ifndef _CXX_Chisa_THREAD_H_
#define _CXX_Chisa_THREAD_H_

#include <pthread.h>

namespace chisa {
namespace util {

class RWLock {
private:
	pthread_rwlock_t_* lock_;
public:
	RWLock(){
		pthread_rwlock_init(&this->lock_, nullptr);
	}
	virtual ~RWLock(){
		pthread_rwlock_destroy(&this->lock_);
	}
public:
	class WriteLock {
	private:
		RWLock& parent_;
	public:
		WriteLock(RWLock& lock):parent_(lock){
			pthread_rwlock_wrlock(&parent_.lock_);
		}
		virtual ~WriteLock(){
			pthread_rwlock_unlock(&parent_.lock_);
		}
	};
	class ReadLock {
	private:
		RWLock& parent_;
	public:
		ReadLock(RWLock& lock):parent_(lock){
			pthread_rwlock_rdlock(&parent_.lock_);
		}
		virtual ~ReadLock(){
			pthread_rwlock_unlock(&parent_.lock_);
		}
	};
};

class Thread {
private:
	pthread_t thread_;
	RWLock stopLock_;
	bool stopQueried_;
public:
	Thread();
	virtual ~Thread();
	void start();
	void join();
	void queryStop();
	bool isRunning();
	bool isStopQueried();
protected:
	virtual void run() = 0;
private:
	static void* _run_impl(void* thread);
};

}}
#endif /* INCLUDE_GUARD */

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

class Thread {
private:
	pthread_t* thread_;
public:
	Thread();
	virtual ~Thread();
	void start();
	void join();
	bool isKilled();
protected:
	virtual void run() = 0;
private:
	static void* _run_impl(void* thread);
};

}}
#endif /* INCLUDE_GUARD */

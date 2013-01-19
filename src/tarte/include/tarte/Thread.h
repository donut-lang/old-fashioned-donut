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
#include "Common.h"
#include "Platform.h"

#if CHISA_WINDOWS || CHISA_ANDROID
#include <boost/thread.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include <boost/atomic.hpp>
namespace std {
using boost::mutex;
using boost::recursive_mutex;
using boost::lock_guard;
using boost::condition_variable;
using boost::unique_lock;
using boost::thread;
using boost::atomic;
namespace chrono {
using namespace boost::chrono;
}
}
#else
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#endif

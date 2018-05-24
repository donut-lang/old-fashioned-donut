/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "Common.h"
#include "Platform.h"

#if (CINAMO_WINDOWS || CINAMO_ANDROID)

#if HAVE_BOOST /* windows/androidのg++でatomic関係が使えない過渡的な措置 */
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

#define CINAMO_HAVE_THREAD 1
#else
#define CINAMO_HAVE_THREAD 0
#endif

#else
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#define CINAMO_HAVE_THREAD 1
#endif

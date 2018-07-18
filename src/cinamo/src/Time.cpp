/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/Time.h>
#if CINAMO_WINDOWS
#include <sys/timeb.h>
#endif

#ifdef CINAMO_LINUX
#include <sys/time.h>
#endif

namespace cinamo {


#ifdef HAVE_ULONGLONG
unsigned long long getTimeInMills()
{
	typedef unsigned long long Ret;
#else
unsigned long getTimeInMills();
{
	typedef unsigned long Ret;
#endif

#ifdef CINAMO_LINUX
	struct timeval t;
	gettimeofday(&t, nullptr);
	return Ret(t.tv_sec) * 1000 + Ret(t.tv_usec) / 1000;
#endif

#ifdef CINAMO_WINDOWS
	struct _timeb t;
	_ftime(&t);
	return Ret(t.time) * 1000 + Ret(t.millitm);
#endif
}


}

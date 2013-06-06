/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/Platform.h>

#if CINAMO_ANDROID
#include <GLES/gl.h>
#define IS_GLES (1)
#define IS_GLES11 (1)
#define IS_GLES20 (0)
#define IS_GL (0)
#elif CINAMO_IOS
#include <OpenGLES/ES1/gl.h>
#define IS_GLES (1)
#define IS_GLES11 (1)
#define IS_GLES20 (0)
#define IS_GL (0)
#else
#include <GL/gl.h>
#define IS_GLES (0)
#define IS_GLES11 (0)
#define IS_GLES20 (0)
#define IS_GL (1)
#endif


#undef min
#undef max

#ifndef GL_ABGR_EXT
#define GL_ABGR_EXT 0x8000
#endif
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif
#ifndef GL_UNSIGNED_SHORT_5_6_5
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#endif

#ifndef GL_UNSIGNED_SHORT_5_5_5_1
#define GL_UNSIGNED_SHORT_5_5_5_1  0x8034
#endif
#ifndef GL_UNSIGNED_SHORT_1_5_5_5_REV
#define GL_UNSIGNED_SHORT_1_5_5_5_REV  0x8066
#endif

#ifndef GL_CLAMP_TO_BORDER
#define GL_CLAMP_TO_BORDER			0x812D
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE			0x812F
#endif

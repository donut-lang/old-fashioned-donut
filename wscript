# -*- coding: utf-8 -*-
import sys
import os

APPNAME = 'chisa'
VERSION = '1.0.0'

srcdir = '.'
blddir = 'build'

def enum(dirname, exclude=[]):
	COMPILED_EXT=['.cpp','.c']
	f = []
	for root,dirs,files in os.walk(dirname):
		matched = False
		for e in exclude:
			if root.startswith(e):
				matched = True
				break
		if matched:
			continue
		for fitem in files:
			fabs = os.path.join(root, fitem)
			_, ext = os.path.splitext(fabs)
			if ext in COMPILED_EXT:
				f.append(fabs)
	return f

def udir(f):
	return os.path.join(*(f.split('/')))

TINYXML2_DIR=os.path.join(os.path.abspath(os.path.dirname(srcdir)), 'external', 'tinyxml2')
TINYXML2_SRC=[os.path.join('.', 'external','tinyxml2','tinyxml2.cpp')]

def options(opt):
	opt.load('compiler_c compiler_cxx')
	opt.load('eclipse')
	opt.load('boost')
def configure(conf):
	conf.load('compiler_c compiler_cxx')
	conf.env.append_value('CXXFLAGS', ['-O3', '-std=c++0x', '-std=c++11', '-D__GXX_EXPERIMENTAL_CXX0X__=1'])
	conf.env.append_value('CXXFLAGS', ['-I'+TINYXML2_DIR])
	conf.check_cfg(package='icu-uc', uselib_store='ICU', mandatory=True, args='--cflags --libs')
	conf.check_cfg(package='libpng', uselib_store='LIBPNG', mandatory=True, args='--cflags --libs')
	conf.check_cfg(package='freetype2', uselib_store='FREETYPE2', mandatory=True, args='--cflags --libs')
	conf.check_cfg(package='cairo cairo-ft', uselib_store='CAIRO', mandatory=True, args='--cflags --libs')
	conf.check(features='cxx cxxprogram', lib=['gtest','gtest_main','pthread'], cflags=['-Wall'], uselib_store='GTEST')
	conf.check(features='cxx cxxprogram', lib=['pthread'], cflags=['-Wall'], uselib_store='PTHREAD')
	# プラットフォーム依存
	if sys.platform == 'win32':
		#opengl
		conf.check(features='cxx cxxprogram', lib=['glfw', 'opengl32',], cflags=['-Wall'], defines=['TEST=TEST'], uselib_store='OPENGL')
		#boost
		conf.env.append_value('CXXFLAGS', ['-DBOOST_THREAD_USE_LIB=1'])
		conf.load('boost')
		conf.check_boost(lib='system thread chrono')
	elif sys.platform in ['linux2', 'linux']:
		#opengl
		conf.check(features='cxx cxxprogram', lib=['glfw','GL', 'X11','rt','Xrandr','pthread'], cflags=['-Wall'], defines=['TEST=TEST'], uselib_store='OPENGL')

TEST_SRC=TINYXML2_SRC+enum('src', [udir('src/entrypoint')])+enum('test')
MAIN_SRC=TINYXML2_SRC+enum('src', [udir('src/entrypoint')])+enum(udir('src/entrypoint/pc/'))

def build(bld):
	bld(features = 'cxx cprogram', source = MAIN_SRC, target = 'chisa', use=['PTHREAD', 'OPENGL','LIBPNG','FREETYPE2','CAIRO','BOOST'])
	bld(features = 'cxx cprogram', source = TEST_SRC, target = 'chisa_test', use=['PTHREAD', 'OPENGL','FREETYPE2','CAIRO','GTEST','LIBPNG','BOOST'])

def shutdown(ctx):
	pass


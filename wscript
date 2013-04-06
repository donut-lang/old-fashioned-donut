# -*- coding: utf-8 -*-
import sys
import os

sys.path.append('external/WafHelper')
import Util

APPNAME = 'chisa'
VERSION = '1.0.0'

srcdir = '.'
blddir = 'build'

TINYXML2_DIR=os.path.join(os.path.abspath(os.path.dirname(srcdir)), 'external', 'tinyxml2')
TINYXML2_SRC=[os.path.join('.', 'external','tinyxml2','tinyxml2.cpp')]

TARTE_INCLUDE_DIR=os.path.join(os.path.abspath(os.path.dirname(srcdir)), 'src', 'tarte','include')
TARTE_SRC=Util.enum('src/tarte')

DONUT_INCLUDE_DIR=os.path.join(os.path.abspath(os.path.dirname(srcdir)), 'src', 'donut','include')
DONUT_SRC=Util.enum('src/donut')

CHISA_DIR=os.path.join(os.path.abspath(os.path.dirname(srcdir)), 'src', 'chisa')
CHISA_SRC=Util.enum('src/chisa')

def options(opt):
	opt.add_option('--coverage', action='store_true', default=False, help='Enabling coverage measuring.')
	opt.add_option('--debug', action='store_true', default=False, help='debug build')
	opt.load('compiler_c compiler_cxx')
	opt.load('boost')
	opt.load('cinamo', tooldir='external/WafHelper')

def configure(conf):
	conf.setenv('release')
	conf.env.append_value('CXXFLAGS', ['-O3', '-Wall', '-std=gnu++11', '-D__GXX_EXPERIMENTAL_CXX0X__=1'])
	configureLibrary(conf)
	
	conf.setenv('debug')
	denv = conf.env;
	conf.env.append_value('CXXFLAGS', ['-ggdb','-O0', '-Wall', '-std=gnu++11', '-D__GXX_EXPERIMENTAL_CXX0X__=1','-DDEBUG'])
	configureLibrary(conf)
	if conf.options.coverage:
		conf.setenv('coverage', denv)
		conf.env.append_value('LINKFLAGS', '-fprofile-arcs')
		conf.env.append_value('CXXFLAGS', ['-fprofile-arcs','-ftest-coverage'])

def configureLibrary(conf):
	conf.load('compiler_c compiler_cxx')
	conf.env.append_value('CXXFLAGS', ['-I'+TINYXML2_DIR])
	conf.check_cfg(package='icu-uc icu-i18n', uselib_store='ICU', mandatory=True, args='--cflags --libs')
	conf.check_cfg(package='libpng', uselib_store='LIBPNG', mandatory=True, args='--cflags --libs')
	conf.check_cfg(package='freetype2', uselib_store='FREETYPE2', mandatory=True, args='--cflags --libs')
	conf.check_cfg(package='sdl2', uselib_store='SDL2', mandatory=True, args='--cflags --libs')
	conf.check(features='cxx cxxprogram', lib=['gtest', 'gtest_main', 'pthread'], cflags=['-Wall'], uselib_store='GTEST')
	conf.check(features='cxx cxxprogram', lib=['antlr3c'], cflags=['-Wall'], uselib_store='ANTLR')
	conf.check(features='cxx cxxprogram', lib='pthread', cflags=['-Wall'], uselib_store='PTHREAD')
	conf.check_cinamo()
	# プラットフォーム依存
	if sys.platform == 'win32':
		#opengl
		conf.check(features='cxx cxxprogram', lib=['opengl32'], cflags=['-Wall'], defines=['TEST=TEST'], uselib_store='OPENGL')
		#boost
		conf.env.append_value('CXXFLAGS', ['-DBOOST_THREAD_USE_LIB=1'])
		conf.load('boost')
		conf.check_boost(lib='system thread chrono')
	elif sys.platform in ['linux2', 'linux']:
		#opengl
		conf.check(features='cxx cxxprogram', lib=['GL', 'X11', 'rt', 'Xrandr', 'pthread'], cflags=['-Wall'], uselib_store='OPENGL')
		try:
			conf.check(features='cxx cxxprogram', lib=['tcmalloc','profiler'], cflags=['-Wall'], uselib_store='PPROF')
		except conf.errors.ConfigurationError:
			conf.to_log("Google perftools not found, so performance will not measureable.")
	#リリースとデバッグで変更

TEST_SRC=\
		TINYXML2_SRC+\
		TARTE_SRC+\
		DONUT_SRC+\
		CHISA_SRC+\
		Util.enum('src/nes')+\
		Util.enum('test')

MAIN_SRC=\
		TINYXML2_SRC+\
		TARTE_SRC+\
		DONUT_SRC+\
		CHISA_SRC+\
		Util.enum('src/nes')+\
		Util.enum('src/entrypoint/pc')

DONUT_SRC=\
		TINYXML2_SRC+\
		TARTE_SRC+\
		DONUT_SRC+\
		Util.enum('src/entrypoint/donut')

def build(bld):
	if not bld.variant:
		bld.set_env(bld.all_envs['debug' if (bld.options.debug) else 'release'])
	bld(
		features = 'cxx cprogram',
		source = DONUT_SRC,
		target = 'donut',
		use=['CINAMO','PPROF','PTHREAD','BOOST','ICU','ANTLR'],
		includes=[TARTE_INCLUDE_DIR, DONUT_INCLUDE_DIR])
	bld(
		features = 'cxx cprogram',
		source = MAIN_SRC,
		target = 'chisa',
		use=['CINAMO','PPROF','PTHREAD', 'SDL2', 'OPENGL','LIBPNG','FREETYPE2','CAIRO','BOOST','ICU','ANTLR'],
		includes=[TARTE_INCLUDE_DIR, DONUT_INCLUDE_DIR])
	bld(
		features = 'cxx cprogram',
		source = TEST_SRC,
		target = 'chisa_test',
		env = bld.all_envs["coverage"] if ("coverage" in bld.all_envs) else bld.env,
		use=['CINAMO','PTHREAD', 'SDL2', 'OPENGL','FREETYPE2','CAIRO','GTEST','LIBPNG','BOOST','ICU','ANTLR'],
		includes=[TARTE_INCLUDE_DIR, DONUT_INCLUDE_DIR])

def shutdown(ctx):
	pass

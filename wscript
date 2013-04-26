# -*- coding: utf-8 -*-
import sys
import os

sys.path.append('external/WafHelper')
import Util

APPNAME = 'chisa'
VERSION = '1.0.0'

srcdir = '.'
blddir = 'build'

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
	elif sys.platform in ['linux2', 'linux']:
		#opengl
		conf.check(features='cxx cxxprogram', lib=['GL', 'X11', 'rt', 'Xrandr', 'pthread'], cflags=['-Wall'], uselib_store='OPENGL')
		try:
			conf.check(features='cxx cxxprogram', lib=['tcmalloc','profiler'], cflags=['-Wall'], uselib_store='PPROF')
		except conf.errors.ConfigurationError:
			conf.to_log("Google perftools not found, so performance will not measureable.")
	#リリースとデバッグで変更

TEST_APP_SRC=\
		Util.enum('test')

MAIN_APP_SRC=\
		Util.enum('src/nes')+\
		Util.enum('src/entrypoint/pc')

DONUT_APP_SRC=\
		Util.enum('src/entrypoint/donut')

def build(bld):
	if not bld.variant:
		bld.set_env(bld.all_envs['debug' if (bld.options.debug) else 'release'])
	bld(
		features = 'cxx cxxstlib',
		source = DONUT_SRC,
		target = 'donut',
		use=['CINAMO','PPROF','PTHREAD','BOOST','ICU','ANTLR'],
		includes=[DONUT_INCLUDE_DIR])
	bld(
		features = 'cxx cxxstlib',
		source = CHISA_SRC,
		target = 'chisa',
		use=['CINAMO','PPROF','PTHREAD', 'SDL2', 'OPENGL','LIBPNG','FREETYPE2','CAIRO','BOOST','ICU','ANTLR', 'donut'],
		includes=[DONUT_INCLUDE_DIR])
	bld(
		features = 'cxx cprogram',
		source = DONUT_APP_SRC,
		target = 'donutApp',
		use=['CINAMO','PPROF','PTHREAD','BOOST','ICU','ANTLR', 'donut'],
		includes=[DONUT_INCLUDE_DIR])
	bld(
		features = 'cxx cprogram',
		source = MAIN_APP_SRC,
		target = 'chisaApp',
		use=['CINAMO','PPROF','PTHREAD', 'SDL2', 'OPENGL','LIBPNG','FREETYPE2','CAIRO','BOOST','ICU','ANTLR', 'donut','chisa'],
		includes=[DONUT_INCLUDE_DIR])
	bld(
		features = 'cxx cprogram',
		source = TEST_APP_SRC,
		target = 'testApp',
		env = bld.all_envs["coverage"] if ("coverage" in bld.all_envs) else bld.env,
		use=['CINAMO','PTHREAD', 'SDL2', 'OPENGL','FREETYPE2','CAIRO','GTEST','LIBPNG','BOOST','ICU','ANTLR', 'donut', 'chisa'],
		includes=[DONUT_INCLUDE_DIR])

def shutdown(ctx):
	pass

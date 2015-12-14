# -*- coding: utf-8 -*-
import sys
import os

sys.path.append('.helper')
import Util

APPNAME = 'chisa'
VERSION = '1.0.0'

srcdir = '.'
blddir = 'build'

DONUT_INCLUDE_DIR=os.path.join(os.path.abspath(os.path.dirname(srcdir)), 'src', 'donut','include')
DONUT_SRC=Util.enum('src/donut')
CHISA_SRC=Util.enum('src/chisa')

TEST_APP_SRC=Util.enum('test');

MAIN_APP_SRC=\
		Util.enum('src/nes')+\
		Util.enum('src/entrypoint/pc');

DONUT_APP_SRC=Util.enum('src/entrypoint/donut');

def options(opt):
	opt.add_option('--coverage', action='store_true', default=False, help='Enabling coverage measuring.')
	opt.add_option('--debug', action='store_true', default=False, help='debug build')
	opt.add_option('--static', action='store_true', default=False, help='use static library')
	opt.load('compiler_c compiler_cxx')
	opt.load('boost')

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
	pkgflags = '--cflags --libs --static' if conf.options.static else '--cflags --libs';
	conf.check_cfg(package='libpng', uselib_store='LIBPNG', mandatory=True, args=pkgflags)
	conf.check_cfg(package='freetype2', uselib_store='FREETYPE2', mandatory=True, args=pkgflags)
	conf.check_cfg(package='sdl2', uselib_store='SDL2', mandatory=True, args=pkgflags)
	conf.check_cfg(package='libcinamo', uselib_store='CINAMO', mandatory=True, args=pkgflags)
	#conf.check(features='cxx cxxprogram', lib=['gtest', 'gtest_main', 'pthread'], cflags=['-Wall'], uselib_store='GTEST')
	conf.check(features='cxx cxxprogram', lib=['antlr3c'], cflags=['-Wall'], uselib_store='ANTLR')
	conf.check(features='cxx cxxprogram', lib='pthread', cflags=['-Wall'], uselib_store='PTHREAD')
	# プラットフォーム依存
	if sys.platform == 'win32':
		#opengl
		conf.check(
			features='cxx cxxprogram',
			lib=['opengl32'],
			cflags=['-Wall'],
			defines=['TEST=TEST'],
			uselib_store='OPENGL')
	elif sys.platform in ['linux2', 'linux']:
		#opengl
		conf.check(
				features='cxx cxxprogram',
				lib=['GL', 'X11', 'rt', 'Xrandr', 'pthread'],
				cflags=['-Wall'],
				uselib_store='OPENGL')
		if not conf.check(
					features='cxx cxxprogram',
					lib=['tcmalloc','profiler'],
					cflags=['-Wall'],
					mandatory=False,
					uselib_store='PPROF'):
			conf.to_log("Google perftools not found, so performance will not measureable.")
	#リリースとデバッグで変更

def build(bld):
	if not bld.variant:
		bld.set_env(bld.all_envs['debug' if (bld.options.debug) else 'release'])
	bld(
		features = 'cxx cxxstlib',
		source = DONUT_SRC,
		target = 'donut',
		use=['CINAMO','PPROF','PTHREAD','ANTLR'],
		includes=[DONUT_INCLUDE_DIR])
	bld(
		features = 'cxx cxxstlib',
		source = CHISA_SRC,
		target = 'chisa',
		use=['CINAMO','PPROF','PTHREAD','SDL2','OPENGL','LIBPNG','FREETYPE2','CAIRO','ANTLR','donut'],
		includes=[DONUT_INCLUDE_DIR])
	bld(
		features = 'cxx cxxprogram',
		source = DONUT_APP_SRC,
		target = 'donutApp',
		use=['CINAMO','PPROF','PTHREAD','donut'],
		includes=[DONUT_INCLUDE_DIR])
	bld(
		features = 'cxx cxxprogram',
		source = MAIN_APP_SRC,
		target = 'chisaApp',
		use=['CINAMO','PPROF','PTHREAD','SDL2','OPENGL','FREETYPE2','CAIRO','donut','chisa'],
		includes=[DONUT_INCLUDE_DIR])
	bld(
		features = 'cxx cxxprogram',
		source = TEST_APP_SRC,
		target = 'testApp',
		env = bld.all_envs["coverage"] if ("coverage" in bld.all_envs) else bld.env,
		use=['CINAMO','PTHREAD', 'SDL2', 'OPENGL','GTEST','donut', 'chisa'],
		includes=[DONUT_INCLUDE_DIR])

def shutdown(ctx):
	pass

# -*- coding: utf-8 -*-
import sys
import os

sys.path.append('.helper')
import Util

APPNAME = 'Template'
VERSION = '1.0.0'

srcdir = '.'
blddir = 'build'

MAINLIB_DIR=os.path.join(os.path.abspath(os.path.dirname(srcdir)), 'src', 'main')
MAINLIB_SRC=Util.enum('src/main')

def options(opt):
	opt.add_option('--coverage', action='store_true', default=False, help='Enabling coverage measuring.')
	opt.add_option('--debug', action='store_true', default=False, help='debug build')
	opt.load('compiler_c compiler_cxx')
	opt.load('boost')
	opt.load('cinamo', tooldir='.helper')

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
	conf.check_cfg(package='sdl2', uselib_store='SDL2', mandatory=True, args='--cflags --libs')
	conf.check(features='cxx cxxprogram', lib=['gtest', 'gtest_main', 'pthread'], cflags=['-Wall'], uselib_store='GTEST')
	conf.check(features='cxx cxxprogram', lib='pthread', cflags=['-Wall'], uselib_store='PTHREAD')
	conf.check_cinamo()

TEST_APP_SRC=Util.enum('test')

MAIN_APP_SRC=\
		Util.enum('src/entrypoint')

def build(bld):
	if not bld.variant:
		bld.set_env(bld.all_envs['debug' if (bld.options.debug) else 'release'])
	bld(
		features = 'cxx cxxstlib',
		source = MAINLIB_SRC,
		target = 'main',
		use=['CINAMO','PPROF','PTHREAD', 'SDL2', 'ICU'])
	bld(
		features = 'cxx cxxprogram',
		source = MAIN_APP_SRC,
		target = 'mainApp',
		use=['CINAMO','PPROF','PTHREAD', 'SDL2', 'ICU', 'main'])
	bld(
		features = 'cxx cxxprogram',
		source = TEST_APP_SRC,
		target = 'testApp',
		env = bld.all_envs["coverage"] if ("coverage" in bld.all_envs) else bld.env,
		use=['CINAMO','PPROF','PTHREAD', 'SDL2', 'ICU', 'GTEST', 'superpx'])

def shutdown(ctx):
	pass

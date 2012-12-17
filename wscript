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
	opt.add_option('--coverage', action='store_true', default=False, help='Enabling coverage measuring.')
	opt.load('compiler_c compiler_cxx')
	opt.load('eclipse')
	opt.load('boost')

def configure(conf):
	conf.setenv('release')
	conf.env.append_value('CXXFLAGS', ['-O3', '-Wall', '-std=c++0x', '-std=c++11', '-D__GXX_EXPERIMENTAL_CXX0X__=1'])
	configureLibrary(conf)
	
	conf.setenv('debug')
	denv = conf.env;
	conf.env.append_value('CXXFLAGS', ['-g','-O0', '-Wall', '-std=c++0x', '-std=c++11', '-D__GXX_EXPERIMENTAL_CXX0X__=1'])
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
	conf.check_cfg(package='cairo cairo-ft', uselib_store='CAIRO', mandatory=True, args='--cflags --libs')
	conf.check(features='cxx cxxprogram', lib=['gtest', 'gtest_main', 'pthread'], cflags=['-Wall'], uselib_store='GTEST')
	conf.check(features='cxx cxxprogram', lib=['antlr3c'], cflags=['-Wall'], uselib_store='ANTLR')
	conf.check(features='cxx cxxprogram', lib='pthread', cflags=['-Wall'], uselib_store='PTHREAD')
	# プラットフォーム依存
	if sys.platform == 'win32':
		#opengl
		conf.check(features='cxx cxxprogram', lib=['glfw', 'opengl32'], cflags=['-Wall'], defines=['TEST=TEST'], uselib_store='OPENGL')
		#boost
		conf.env.append_value('CXXFLAGS', ['-DBOOST_THREAD_USE_LIB=1'])
		conf.load('boost')
		conf.check_boost(lib='system thread chrono')
	elif sys.platform in ['linux2', 'linux']:
		#opengl
		conf.check(features='cxx cxxprogram', lib=['glfw', 'GL', 'X11', 'rt', 'Xrandr', 'pthread'], cflags=['-Wall'], uselib_store='OPENGL')
		try:
			conf.check(features='cxx cxxprogram', lib=['tcmalloc','profiler'], cflags=['-Wall'], uselib_store='PPROF')
		except conf.errors.ConfigurationError:
			conf.to_log("Google perftools not found, so performance will not measureable.")
	#リリースとデバッグで変更

ANTLR_IN_SRC = [
		udir('src/chisa/donut/parser/Compiler.g'),
		udir('src/chisa/donut/parser/Donut.g')]
ANTLR_OUT_DIR=udir('src/chisa/donut/parser/output')
ANTLR_OUT_SRC = [
		udir('{0}/Compiler.cpp'.format(ANTLR_OUT_DIR)),
		udir('{0}/DonutLexer.cpp'.format(ANTLR_OUT_DIR)),
		udir('{0}/DonutParser.cpp'.format(ANTLR_OUT_DIR))]
TEST_SRC=\
		ANTLR_OUT_SRC+\
		TINYXML2_SRC+\
		enum('src', [udir('src/entrypoint'), ANTLR_OUT_DIR])+\
		enum('test')
MAIN_SRC=\
		ANTLR_OUT_SRC+\
		TINYXML2_SRC+\
		enum('src', [udir('src/entrypoint'), ANTLR_OUT_DIR])+\
		enum(udir('src/entrypoint/pc/'))
DONUT_SRC=\
		ANTLR_OUT_SRC+\
		TINYXML2_SRC+\
		enum(udir('src/chisa/donut'), [ANTLR_OUT_DIR])+\
		enum(udir('src/chisa/logging'))+\
		enum(udir('src/chisa/util'))+\
		enum(udir('src/entrypoint/donut'))

def build(bld):
	if not bld.variant:
		bld.fatal('call "waf build_debug" or "waf build_release", and try "waf --help"')
	srcdir=repr(bld.path)
	bld(
		features = 'cxx cprogram',
		source = DONUT_SRC,
		target = 'donut',
		use=['PPROF','PTHREAD','BOOST','ICU','ANTLR'])
	bld(
			features = 'cxx cprogram',
			source = MAIN_SRC,
			target = 'chisa',
			use=['PPROF','PTHREAD', 'OPENGL','LIBPNG','FREETYPE2','CAIRO','BOOST','ICU','ANTLR'])
	
	test_env = None
	if "coverage" in bld.all_envs:
		test_env = bld.all_envs["coverage"]
	else:
		test_env = bld.env
	bld(
		features = 'cxx cprogram',
		source = TEST_SRC,
		target = 'chisa_test',
		env = test_env,
		use=['PTHREAD', 'OPENGL','FREETYPE2','CAIRO','GTEST','LIBPNG','BOOST','ICU','ANTLR'])

# from http://docs.waf.googlecode.com/git/book_16/single.html#_custom_build_outputs
from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext

for x in 'debug release'.split():
	for y in (BuildContext, CleanContext, InstallContext, UninstallContext):
		name = y.__name__.replace('Context','').lower()
		class tmp(y):
			cmd = name + '_' + x
			variant = x

def shutdown(ctx):
	pass


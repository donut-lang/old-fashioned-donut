# -*- coding: utf-8 -*-
import sys

APPNAME = 'chisa'
VERSION = '1.0.0'

srcdir = '.'
blddir = 'build'
def options(opt):
	opt.load('compiler_c compiler_cxx')
def configure(conf):
	conf.load('compiler_c compiler_cxx')
	conf.env.append_value('CXXFLAGS', ['-O3', '-std=c++0x', '-std=c++11', '-D__GXX_EXPERIMENTAL_CXX0X__=1'])
	if sys.platform == 'win32': 
		conf.check(features='cxx cxxprogram', lib=['glut', 'opengl32',], cflags=['-Wall'], defines=['TEST=TEST'], uselib_store='OPENGL')
	elif sys.platform == 'linux2':
		conf.check(features='cxx cxxprogram', lib=['glut'], cflags=['-Wall'], defines=['TEST=TEST'], uselib_store='OPENGL')
	
	conf.check_cfg(package='icu-uc')
	conf.check(features='cxx cxxprogram', lib=['gtest','gtest_main','pthread'], cflags=['-Wall'], uselib_store='GTEST')

def build(bld):
	bld(features = 'cxx cprogram', source = 'src/entrypoint/pc/Chisa.cpp', target = 'chisa', use=['OPENGL'])
	bld(features = 'cxx cprogram', source = 'test/main.cpp', target = 'chisa_test', use=['OPENGL','GTEST'])

def shutdown(ctx):
	pass


# -*- coding: utf-8 -*-

# Donut
#
# Copyright 2012-2013, PSI

import os
from waflib.Configure import conf

def options(opt):
	opt.add_option('--with-cinamo', type='string', default='', dest='cinamo', help='''path to cinamo project dir''')

@conf
def check_cinamo(ctx, *k ,**kw):
	mandatory = kw.get('mandatory', True)
	cpath = None
	if ctx.options.cinamo != '':
		cpath = ctx.options.cinamo
	elif 'cinamo' in kw:
		cpath=kw['cinamo']
	else:
		cpath=None
	store = kw.get('uselib_store', 'CINAMO')

	have_cinamo=False
	if cpath:
		cpath=ctx.options.cinamo;
		# まずブーストを使っているかどうかを先にチェック
		useboost = ctx.check(features='cxx cxxprogram',
				compile_filename='tmp.cpp',
				cxxflags=['-I'+os.path.join(cpath, 'build', ctx.variant)],
				mandatory=False,
				execute=True,
				use=store,
				msg='checking whether cinamo is built with boost',
				code='''
#include <config.h>
int main(){
	return !HAVE_BOOST;
}
				''');
		if useboost:
			ctx.env.append_value('CXXFLAGS', ['-DBOOST_THREAD_USE_LIB=1'])
			ctx.check_boost(lib='system thread chrono', uselib_store=store, mandatory=True)
		# そして残りのチェック
		lib = ctx.check(features='cxx cxxprogram',
				lib=['cinamo'],
				linkflags=['-L'+os.path.join(cpath, 'build', ctx.variant)],
				mandatory=False,
				uselib_store=store,
				msg='checking cinamo libs')
		header = ctx.check(features='cxx cxxprogram',
				header_name='cinamo/Cinamo.h',
				cxxflags=['-I'+os.path.join(cpath, 'include'), '-I'+os.path.join(cpath, 'external', 'tinyxml2')],
				mandatory=False,
				msg='checking cinamo header',
				uselib_store=store)
		ctx.check(features='cxx cxxprogram',
				header_name='tinyxml2.h',
				cxxflags=['-I'+os.path.join(cpath, 'external', 'tinyxml2')],
				mandatory=False,
				msg='checking cinamo tinyxml2 header',
				uselib_store=store)
		cfg = ctx.check(features='cxx cxxprogram',
				header_name='config.h',
				cxxflags=['-I'+os.path.join(cpath, 'build', ctx.variant)],
				mandatory=False,
				msg='checking cinamo config header',
				uselib_store=store)
		exlib = ctx.check_cfg(
			package='icu-uc icu-i18n',
			uselib_store=store,
			mandatory=False,
			args='--cflags --libs')
		if header and lib and cfg and exlib:
			have_cinamo=True
		ctx.define(ctx.have_define(store), 1 if have_cinamo else 0)
	if (not have_cinamo) and ctx.check_cfg(package='cinamo', uselib_store=store, mandatory=False, args='--cflags --libs'):
		have_cinamo=True
	if (not have_cinamo) and mandatory:
		ctx.fatal('Failed to find cinamo!')
	return  have_cinamo

def configure(ctx):
	pass


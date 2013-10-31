# -*- coding: utf-8 -*-

# Donut
#
# Copyright 2012-2013, PSI

import os
import sys
from waflib.Configure import conf

def enum(dirname, exclude=[], exts=['.cpp','.c']):
	'''
		指定したディレクトリ以下のファイルを列挙する
	'''
	dirname = os.path.join(*(dirname.split('/')))
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
			if ext in exts:
				f.append(os.path.relpath(fabs))
	return f

# from http://docs.waf.googlecode.com/git/book_16/single.html#_custom_build_outputs
from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext
for x in 'debug release'.split():
	for y in (BuildContext, CleanContext, InstallContext, UninstallContext):
		name = y.__name__.replace('Context','').lower()
		class tmp(y):
			cmd = name + '_' + x
			variant = x

#!/usr/bin/env python
import os, sys

def error(val):
	if val != 0:
		print "Error during build."
		sys.exit(1)

os.chdir('box2d/Box2D/Build')
error(os.system('cmake ..'))
error(os.system('make'))

os.chdir('../../../cmake')
error(os.system('cmake .'))
error(os.system('make'))

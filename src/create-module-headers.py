#! /usr/bin/python

import sys, os
from glob import glob

def checkVersion():
	if sys.hexversion >= 0x03000000:
		print("This script does not support Python 3. Please run using Python 2.")
		exit()

def childDirs(parentDir):
	return [x.strip('/') for x in glob('*/')]

def moduleGlobalHeaderName(module):
	return module + "_global.h"

class MainClass:
	rootDir = ""
	currentModule = ""

	def __init__(self):
		self.rootDir = os.path.abspath(".")

	def processModule(self):
		print("Processing directory " + self.currentModule + "...")

	def process(self):
		for subdir in childDirs('.'):
			self.currentModule = subdir
			self.processModule()

def main():
	checkVersion()
	print("Running script using Python:\n" + sys.version + "\n")

	prog = MainClass()
	prog.process()

# ======================================
# START
# ======================================

if __name__ == '__main__':
  main()
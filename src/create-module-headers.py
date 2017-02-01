#! /usr/bin/python

import sys, os, re
from glob import glob

def checkVersion():
	if sys.hexversion >= 0x03000000:
		print("This script does not support Python 3. Please run using Python 2.")
		exit()

def childDirs(parentDir):
	return [x.strip('/') for x in glob('*/')]

def openFile(filename, mode):
	try:
		f = open(filename, mode)

	except OSError:
	    return None

	except IOError:
	   	return None

	except:
	    print("Unexpected error opening " + filename + " in mode '" + mode + "':", sys.exc_info()[0])
	    raise

	return f

def moduleExportDef(moduleName):
	return re.sub('[^A-Za-z0-9]', '', moduleName.upper()) + "SHARED_EXPORT"

class MainClass:
	rootDir = ""
	currentModule = ""

	globalHeaderPath = ""
	moduleExportDef = ""

	def __init__(self):
		self.rootDir = os.path.abspath(".")

	def moduleGlobalHeaderName(self):
		return self.currentModule + "_global.h"

	def moduleDirPath(self):
		return self.rootDir + "/" + self.currentModule

	def setModuleVars(self):
		self.globalHeaderPath = self.rootDir + "/" + self.currentModule + "/" + self.moduleGlobalHeaderName()
		self.moduleExportDef = moduleExportDef(self.currentModule)

	def fileContainsContainsModuleExport(self, contents):
		return self.moduleExportDef in contents

	def processModule(self, module):
		self.currentModule = module
		self.setModuleVars()

		print("Processing directory " + self.currentModule + "...")

		if not os.path.isfile(self.globalHeaderPath):
			print("Could not find global header for module, skipping.")
			return

		for root, dirs, files in os.walk(self.moduleDirPath()):
			print (files)	# TODO: Process each file

	def process(self):
		for subdir in childDirs('.'):
			self.processModule(subdir)

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
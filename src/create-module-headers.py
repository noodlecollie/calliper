#! /usr/bin/python

import sys, os
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

class MainClass:
	rootDir = ""
	currentModule = ""

	def __init__(self):
		self.rootDir = os.path.abspath(".")

	def moduleGlobalHeaderName(self):
		return self.currentModule + "_global.h"

	def processModule(self):
		print("Processing directory " + self.currentModule + "...")

		globalHeaderPath = self.rootDir + "/" + self.currentModule + "/" + self.moduleGlobalHeaderName()

		headerFile = openFile(globalHeaderPath, 'r')
		if headerFile is None:
			print("Could not open global header, skipping.")
			return

		print("Opened header successfully")
		headerFile.close()

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
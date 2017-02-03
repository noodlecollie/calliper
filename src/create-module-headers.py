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

	targetFilePath = ""
	targetFileContents = ""

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

	def getRelativeFilePath(self, absolutePath):
		commonPrefix = os.path.commonprefix([absolutePath, self.rootDir])
		return os.path.relpath(absolutePath, commonPrefix)

	def writeIncludeFile(self):
		fileToWrite = openFile(self.targetFilePath, 'w')

		if fileToWrite is None:
			print("Unable to open " + self.targetFilePath + " for writing.")
			return

		fileToWrite.write(self.targetFileContents)
		fileToWrite.close()

	def createInclude(self, fileName, absolutePath):
		relativePath = self.getRelativeFilePath(absolutePath)
		relativeDir = os.path.dirname(relativePath)

		pathFromIncludeDir = "../../" + relativeDir + "/" + fileName
		self.targetFileContents = '#include "' + pathFromIncludeDir + '"'
		self.targetFilePath = self.rootDir + "/include/" + self.currentModule + "/" + fileName
		
		if not os.path.exists(self.rootDir + "/include/" + self.currentModule):
			os.makedirs(self.rootDir + "/include/" + self.currentModule)

		print("Creating include for '" + self.targetFilePath + "'")
		self.writeIncludeFile()

	def walkDirs(self):
		for root, dirs, files in os.walk(self.moduleDirPath()):
			# Root is the path to the current directory.
			# Dirs are the next subdirectories
			# Files are the files present in the current directory.
			
			for file in files:
				if not file.lower().endswith(('.h')):
					continue

				absoluteFilePath = root + "/" + file

				f = openFile(absoluteFilePath, 'r')
				if f is None:
					continue

				contents = f.read()
				f.close()

				if not self.fileContainsContainsModuleExport(contents):
					continue

				self.createInclude(file, absoluteFilePath)

	def processModule(self, module):
		self.currentModule = module
		self.setModuleVars()

		print("Processing directory " + self.currentModule + "...")

		if not os.path.isfile(self.globalHeaderPath):
			print("Could not find global header for module, skipping.")
			return

		self.walkDirs()

	def process(self):
		if not os.path.exists("./include"):
			os.makedirs("./include")

		for subdir in childDirs('.'):
			if subdir != "include":
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
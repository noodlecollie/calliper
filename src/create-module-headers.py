#! /usr/bin/python

import sys, os, re, getopt, posixpath
from glob import glob

def checkVersion():
	if sys.hexversion >= 0x03000000:
		print("This script does not support Python 3. Please run using Python 2.")
		sys.exit(1)

def childDirs(parentDir):
	return [x.rstrip(os.sep) for x in glob('*/')]

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
	verbose = False
	overwriteExisting = False

	globalHeaderPath = ""
	moduleExportDef = ""

	targetFilePath = ""
	targetFileContents = ""

	def __init__(self, argv):
		self.vlog("Running script using Python:\n" + sys.version + "\n")
	
		self.rootDir = os.path.abspath(".")
		
		try:
			opts, args = getopt.getopt(argv, "vx")
		except getopt.GetoptError as err:
			print("Usage:")
			print("create-module-headers.py: -v -x")
			print("  -v: Print verbose output.")
			print("  -x: Overwrite include files that already exist.")
			sys.exit(1)
		
		for opt, arg in opts:
			if opt == "-v":
				self.verbose = True
				print("Verbose mode enabled.")
		
			elif opt == "-x":
				self.overwriteExisting = True
				print("Existing headers will be overwritten.")
	
	def vlog(self, msg):
		if self.verbose:
			print(msg)

	def moduleGlobalHeaderName(self):
		return self.currentModule + "_global.h"

	def moduleDirPath(self):
		return os.path.normpath(self.rootDir + "/" + self.currentModule)

	def setModuleVars(self):
		self.globalHeaderPath = os.path.normpath(self.rootDir + "/" + self.currentModule +
		"/" + self.moduleGlobalHeaderName())
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

		pathFromIncludeDir = os.path.normpath("../../" + relativeDir + "/" + fileName)
		self.targetFileContents = '#include "' + pathFromIncludeDir.replace("\\", "/") + '"'
		self.targetFilePath = os.path.normpath(self.rootDir + "/include/"
		+ self.currentModule + "/" + fileName)
		
		self.vlog("Ready to create include file:")
		self.vlog("  File: '" + self.targetFilePath + "'")
		self.vlog("  Contents: '" + self.targetFileContents + "'")
		
		modulePath = os.path.normpath(self.rootDir + "/include/" + self.currentModule)
		if not os.path.exists(modulePath):
			os.makedirs(modulePath)
			
		if not self.overwriteExisting and os.path.isfile(self.targetFilePath):
			print("Include file '" + self.targetFilePath + "' already exists, skipping.")
			return

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

				absoluteFilePath = os.path.normpath(root + "/" + file)

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
		
		self.vlog("Attempting to find global header '" + self.globalHeaderPath + "'...")
		
		if not os.path.isfile(self.globalHeaderPath):
			print("Could not find global header for module, skipping.")
			return

		self.walkDirs()

	def process(self):
		if not os.path.exists(os.path.normpath("./include")):
			os.makedirs("./include")

		for subdir in childDirs('.'):
			if subdir != "include":
				self.processModule(subdir)

def main(argv):
	checkVersion()

	prog = MainClass(argv)
	prog.process()

# ======================================
# START
# ======================================

if __name__ == '__main__':
  main(sys.argv[1:])
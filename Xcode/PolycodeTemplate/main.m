//
//  main.m
//  PolycodeTemplate
//
//  Created by Ivan Safrin on 5/7/11.
//  Copyright 2011 Tomatogon. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "MacCommandLineArgs.h"

int main(int argc, char *argv[])
{
	cmdlArgc = argc;
	cmdlArgv = (const char**)argv;
	return NSApplicationMain(argc, (const char **)argv);
}

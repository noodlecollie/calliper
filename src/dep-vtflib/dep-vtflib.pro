#-------------------------------------------------
#
# Project created by QtCreator 2016-11-26T18:29:21
#
#-------------------------------------------------

QT       -= gui

TARGET = dep-vtflib
TEMPLATE = lib

DEFINES += DEPVTFLIB_LIBRARY

SOURCES += \
    VTFLib/src/Error.cpp \
    VTFLib/src/FileReader.cpp \
    VTFLib/src/FileWriter.cpp \
    VTFLib/src/Float16.cpp \
    VTFLib/src/MemoryReader.cpp \
    VTFLib/src/MemoryWriter.cpp \
    VTFLib/src/Proc.cpp \
    VTFLib/src/ProcReader.cpp \
    VTFLib/src/ProcWriter.cpp \
    VTFLib/src/VMTFile.cpp \
    VTFLib/src/VMTGroupNode.cpp \
    VTFLib/src/VMTIntegerNode.cpp \
    VTFLib/src/VMTNode.cpp \
    VTFLib/src/VMTSingleNode.cpp \
    VTFLib/src/VMTStringNode.cpp \
    VTFLib/src/VMTValueNode.cpp \
    VTFLib/src/VMTWrapper.cpp \
    VTFLib/src/VTFFile.cpp \
    VTFLib/src/VTFLib.cpp \
    VTFLib/src/VTFMathlib.cpp \
    VTFLib/src/VTFWrapper.cpp \
    VTFLib/src/unix/UnixError.cpp \
    VTFLib/src/unix/UnixFileReader.cpp \
    VTFLib/src/unix/UnixFileWriter.cpp \
    VTFLib/src/unix/UnixVTFLib.cpp \
    VTFLib/src/windows/WindowsError.cpp \
    VTFLib/src/windows/WindowsFileReader.cpp \
    VTFLib/src/windows/WindowsFileWriter.cpp \
    VTFLib/src/windows/WindowsVTFLib.cpp

HEADERS +=\
        dep-vtflib_global.h \
    VTFLib/src/Error.h \
    VTFLib/src/FileReader.h \
    VTFLib/src/FileWriter.h \
    VTFLib/src/Float16.h \
    VTFLib/src/MemoryReader.h \
    VTFLib/src/MemoryWriter.h \
    VTFLib/src/Proc.h \
    VTFLib/src/ProcReader.h \
    VTFLib/src/ProcWriter.h \
    VTFLib/src/Reader.h \
    VTFLib/src/Readers.h \
    VTFLib/src/resource.h \
    VTFLib/src/stdafx.h \
    VTFLib/src/VMTFile.h \
    VTFLib/src/VMTGroupNode.h \
    VTFLib/src/VMTIntegerNode.h \
    VTFLib/src/VMTNode.h \
    VTFLib/src/VMTNodes.h \
    VTFLib/src/VMTSingleNode.h \
    VTFLib/src/VMTStringNode.h \
    VTFLib/src/VMTValueNode.h \
    VTFLib/src/VMTWrapper.h \
    VTFLib/src/VTFDXTn.h \
    VTFLib/src/VTFFile.h \
    VTFLib/src/VTFFormat.h \
    VTFLib/src/VTFLib.h \
    VTFLib/src/VTFMathlib.h \
    VTFLib/src/VTFWrapper.h \
    VTFLib/src/Writer.h \
    VTFLib/src/Writers.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

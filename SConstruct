# Graphics Language Renderer (glr) SConstruct file

import os, sys
import glob
import json

def parseShadersIntoHeader():
	print('Parsing Shaders into header ShaderData.h')
	
	shaderDataOutputFilename = "ShaderData.h"
	shaderListLocation = "data/shaders/"
	#shaderListExtension = ".json"
	
	fileList = glob.glob( os.path.join(shaderListLocation, '*') )
	
	file = open(fileList[0], 'r')
	data = file.read()
	
	# Save json as a std::string in a .h file
	shaderDataFile = open( os.path.join('src/shaders', shaderDataOutputFilename), 'w' )
	
	cpp = """
/**
 * This file is automatically generated.  Changes made to this file will
 * not be reflected after compiling.
 *
 * If you wish to make changes to shader information for Glr, edit the shaders
 * and shader programs in the 'data/shaders/' directory.
 *
 */
 
#include <map> 
 
namespace glr {

namespace shaders {

static std::map<std::string, std::string> SHADER_DATA = {
"""
	current = 0
	for filename in fileList:
		file = open(filename, 'r')
		data = file.read()
		
		if (current > 0):
			cpp += ", "
		
		cpp += """
{\""""

		filename = filename.replace(shaderListLocation, "")
		filename = filename.replace(".glsl", "")
		cpp += filename
		
		cpp +="""\", std::string(
	R"<STRING>(
"""

		cpp += data

		cpp += """
)<STRING>"
)}	
"""
		current += 1
	
	cpp += """
};

}

}
"""
		
	shaderDataFile.write(cpp)
	shaderDataFile.close()
	
	print('Done parsing Shaders into header ShaderData.h')


# Parse our shader programs and create .h files out of them
parseShadersIntoHeader()


# Tell SCons to create our build files in the 'build' directory
VariantDir('build', 'src', duplicate=0)

# Set our source files
source_files = Glob('build/*.cpp', 'build/*.h')

source_files = source_files + Glob('build/common/compatibility/*.cpp', 'build/common/compatibility/*.h')
source_files = source_files + Glob('build/common/math/*.cpp', 'build/common/math/*.h')
source_files = source_files + Glob('build/common/utilities/*.cpp', 'build/common/utilities/*.h')
source_files = source_files + Glob('build/common/io/*.cpp', 'build/common/io/*.h')
#source_files = source_files + Glob('build/vmath/*.cpp', 'build/vmath/*.h')

source_files = source_files + Glob('build/*.cpp', 'build/*.h')
source_files = source_files + Glob('build/exceptions/*.cpp', 'build/exceptions/*.h')
source_files = source_files + Glob('build/gui/*.cpp', 'build/gui/*.h')
source_files = source_files + Glob('build/models/*.cpp', 'build/models/*.h')
source_files = source_files + Glob('build/shaders/*.cpp', 'build/shaders/*.h')

env = Environment(ENV = os.environ, CCFLAGS=[]) 


# Set our required libraries
libraries = [
'GL',
'GLU',
'liblibberkelium_d',
'Xi',
'sfml-window',
'assimp',
'boost_log', 
'boost_date_time', 
'boost_thread',
'boost_wave',
'boost_regex'
]

library_paths = ['../lwis/build']

# Set our g++ compiler flags
cpp_flags = [
'-std=c++11',
'-Wall',
#'-D_GLIBCXX_DEBUG'
]

cpp_defines = []

if (os.name == "nt" or os.name == "win32"):
	cpp_flags.append( '-I"C:\lib\Assimp\include"' )


#debug = ARGUMENTS.get('debug', 0)
#if int(debug):
#	cpp_flags.append('-g')
#	cpp_defines.append('DEBUG')
#else:
#	cpp_defines.append('NDEBUG')
cpp_flags.append('-g')
cpp_defines.append('DEBUG')	
	
	
#for key, value in ARGLIST:
#	if key == 'define':
#		cppdefines.append(value)
       

env.Append( CPPFLAGS = cpp_flags )
env.Append( CPPDEFINES = cpp_defines )

env.SetOption('num_jobs', 4)

# Tell SCons the program to build
#env.Program('build/iceengine', source_files, LIBS = libraries, LIBPATH = library_paths)
env.StaticLibrary('build/iceengine', source_files, LIBS = libraries, LIBPATH = library_paths)

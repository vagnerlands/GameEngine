# CMakeList.txt : CMake project for tests1, include source and define
# project specific logic here.
#

cmake_minimum_required (VERSION 3.8)

set(SOURCES_UTILITIES_CORE
	CClockOGL.cpp
	Debugger.cpp
	IClock.cpp
	InvalidTexture.cpp
	SocketUtils.cpp
	Logger\ILogger.cpp
	Rendered\SceneItemBoundingBox.cpp
	Rendered\Skybox.cpp
	Shaders\glsl.cpp
	)	
	
set(HEADERS_UTILITIES_CORE 
	CClockOGL.h
	Debugger.h
	IClock.h
	InvalidTexture.h
	OGLTypes.h
	SocketUtils.h
	Logger\ILogger.h
	Rendered\SceneItemBoundingBox.h
	Rendered\Skybox.h
	Shaders\glsl.h
	zlib\ioapi.h
	zlib\unzip.h
	zlib\zconf.h
	zlib\zip.h	
	zlib\zlib.h
	)

####### ENTIRE PROJECT FILES ########	
set(SOURCES 
	${SOURCES_UTILITIES_CORE}
	)

set(HEADERS 
	${HEADERS_UTILITIES_CORE}
	)
	
####### DIRECTORIES EXPOSED TO USERS OF THIS MODULE INTERNALS #########
set(UTILITIES_CORE_SOURCE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}
CACHE STRING "Source file directories of Utilities Core")


####### ENTIRE PROJECT FILES ########	
add_library(UtilitiesCore STATIC ${SOURCES} ${HEADERS})


##### PROJECT ADDITIONAL INCLUDE FOLDER (SUPPORT DEPENDENCIES) ######
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PARENT_DIR)
cmake_path(APPEND 3RDPARTY_FUNCTION_PATH "${PARENT_DIR}" "cmake" )
cmake_path(APPEND_STRING  3RDPARTY_FUNCTION_PATH "/" "3rdParty.cmake" )
include(${3RDPARTY_FUNCTION_PATH})

cmake_path(APPEND HSID_INC_DIR "${PARENT_DIR}" "HSID" )

target_include_directories (UtilitiesCore PUBLIC 
	# Self Directories
	
	${3RDPARTY_INCLUDE_DIRS}				# From: 3rdParty.cmake
	${TRACKER_COMMON_SOURCE_DIRECTORIES}	# From: TrackerCommon
	${UTILITIES_SOURCE_DIRECTORIES}			# From: OpticalTrackerUtilities
	${HSID_INC_DIR}							# same as ../HSID/ only "path safe"
)

### LINKER DIRECTORIES ###
target_link_directories(UtilitiesCore PUBLIC 
	${3RDPARTY_LIBRARIES_DIRS}	# From: 3rdParty.cmake 
)

# support C++20
# We need C++ latest, and for version less than cmake v21, cxx20 produce latest
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (${CMAKE_MINOR_VERSION} LESS 21 )
	set_property(TARGET UtilitiesCore PROPERTY CXX_STANDARD 20)
else()
	set_property(TARGET UtilitiesCore PROPERTY CXX_STANDARD 23)
endif()
	
	
	
### Visual Studio Support Features ###
if ( MSVC )	
	# Set Folder on Visual Studio
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

endif( MSVC )


# DEBUG CONFIGURATION
install(
    TARGETS
    UtilitiesCore
    CONFIGURATIONS
    Debug
    DESTINATION
    lib/Debug
)

# RELEASE CONFIGURATION
install(
    TARGETS
    UtilitiesCore
    CONFIGURATIONS
    Release
    DESTINATION
    lib/Release
)

# file required for deployment
set(DATA_RECORDER_INSTALL_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/DataRecorderManager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/DataRecorderConfiguration.hpp
CACHE STRING "common install header files")

# REQUIRED PUBLIC HEADER FILES
install(FILES	
${UTILITIES_CORE_INSTALL_HEADERS}
	DESTINATION include)

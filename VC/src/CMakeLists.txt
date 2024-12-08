cmake_minimum_required (VERSION 3.5)

project(OgreBasicApp)

if(WIN32)
	set(CMAKE_MODULE_PATH "$ENV{OGRE_PATH}/CMake/;${CMAKE_MODULE_PATH}")
	set(OGRE_DIR $ENV{OGRE_PATH}/CMake/)
	set(OGRE_BINARY_DIR $ENV{OGRE_PATH}/bin)
endif(WIN32)
 
if(UNIX)
	if(EXISTS "/usr/local/lib/OGRE/cmake")
	  set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	  set(OGRE_DIR /usr/local/lib/OGRE/cmake/)
	  set(OGRE_BINARY_DIR /usr/local/lib/OGRE/bin)
	elseif(EXISTS "/usr/lib/OGRE/cmake")
	  set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	  set(OGRE_DIR /usr/lib/OGRE/cmake/)
	  set(OGRE_BINARY_DIR /usr/lib/OGRE/bin)
	else ()
	  message(SEND_ERROR "Failed to find module path.")
	endif(EXISTS "/usr/local/lib/OGRE")
endif(UNIX)

# required for Ogre 13.2+
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_DEBUG_POSTFIX "_d")
 
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

## [discover_ogre]
# specify which version and components you need
find_package(OGRE 14.3 REQUIRED COMPONENTS Bites RTShaderSystem)


add_executable(
	OgreBasicApp
	BaseApplication.cpp 
	BaseApplication.h 
	TutorialApplication.cpp
	TutorialApplication.h
)

set_target_properties(OgreBasicApp PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(OgreBasicApp OgreBites OgreRTShaderSystem ${OGRE_LIBRARIES})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/media)

# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command( TARGET OgreBasicApp PRE_BUILD
		COMMAND if not exist .\\dist\\bin mkdir .\\dist\\bin )
	add_custom_command( TARGET OgreBasicApp POST_BUILD
		COMMAND copy \"$(TargetPath)\" .\\dist\\bin )
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)

if(WIN32)
	install(TARGETS OgreBasicApp
		RUNTIME DESTINATION bin
		CONFIGURATIONS All)
 
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/Media
		DESTINATION ./
		CONFIGURATIONS Release RelWithDebInfo MinSizeRel Debug
	)
 
	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins.cfg
		${CMAKE_SOURCE_DIR}/dist/bin/resources.cfg
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo MinSizeRel Debug
	)
 
	install(FILES ${OGRE_BINARY_DIR}/OgreMain.dll
		${OGRE_BINARY_DIR}/OgreBites.dll
		${OGRE_BINARY_DIR}/OgreRTShaderSystem.dll
		${OGRE_BINARY_DIR}/RenderSystem_GL.dll
		${OGRE_BINARY_DIR}/Codec_STBI.dll
		${OGRE_BINARY_DIR}/OgreOverlay.dll
		${OGRE_BINARY_DIR}/zlib.dll
		${OGRE_BINARY_DIR}/SDL2.dll
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo MinSizeRel Debug
	)
endif(WIN32)

if(UNIX)
	install(TARGETS OgreBasicApp
		RUNTIME DESTINATION bin
		CONFIGURATIONS All)
 
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/media
		DESTINATION ./
		CONFIGURATIONS Release RelWithDebInfo MinSizeRel Debug
	)
 
	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins.cfg
		${CMAKE_SOURCE_DIR}/dist/bin/resources.cfg
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo MinSizeRel Debug
	) 
endif(UNIX)

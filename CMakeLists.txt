cmake_minimum_required (VERSION 2.6)

project(vorticity)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMakeModules)

find_package (OpenGL)
find_package (GLEW)
include_directories ( ${GLEW_INCLUDE_PATH} )

find_package (Cg)
include_directories ( ${CG_INCLUDE_DIR} )

include_directories ( ${PROJECT_SOURCE_DIR}/include )
include_directories ( ${PROJECT_SOURCE_DIR}/src )


set ( vorticity_SRCS
	src/core/application.cpp
	src/core/core.cpp
	src/core/entity.cpp
	)
	
set ( vorticity_PUBLIC_HEADERS 
	include/vorticity/core/application.h
	include/vorticity/core/core.h
	include/vorticity/core/entity.h
	include/vorticity/core/fonts.h
	include/vorticity/core/renderer.h
	include/vorticity/core/timer.h
	)
	
set ( vorticity_PRIVATE_HEADERS
	src/platforms/win32/console.h
	src/platforms/win32/fontmanager.h
	)
	
add_library (libvorticity ${vorticity_SRCS} ${vorticity_PUBLIC_HEADERS} ${vorticity_PRIVATE_HEADERS} )
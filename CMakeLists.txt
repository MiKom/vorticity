cmake_minimum_required (VERSION 2.6)

project(vorticity)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMakeModules)

find_package (OpenGL)
if(OPENGL_FOUND)
	include_directories ( ${GLEW_INCLUDE_PATH} )
endif(OPENGL_FOUND)

find_package (GLEW)
if(GLEW_FOUND)
	include_directories ( ${GLEW_INCLUDE_PATH} )
endif(GLEW_FOUND)

find_package (Cg)
if(CG_FOUND)
	include_directories ( ${CG_INCLUDE_DIR} )
endif(CG_FOUND)

include_directories ( ${PROJECT_SOURCE_DIR}/include )
include_directories ( ${PROJECT_SOURCE_DIR}/src )

option (DYNAMIC_VORTICITY "Set ON for dynamic linking" ON)
if(DYNAMIC_VORTICITY)
	add_definitions(-DVORTICITY_LIBRARY)
else(DYNAMIC_VORTICITY)
	add_definitions(-DVORTICITY_LIBRARY_STATIC)
endif(DYNAMIC_VORTICITY)

if(OPENGL_FOUND)
	option (OPENGL_COMPATIBILITY_RENDERER "Build compatibility profile OpenGL renderer" ON)
endif(OPENGL_FOUND)

#containers for source files, public header files, private header files and additional libraries
set (vorticity_SRCS)
set (vorticity_PUBLIC_HEADERS)
set (vorticity_PRIVATE_HEADERS)
set (vorticity_ADDITIONAL_LIBRARIES)

#core
	source_group(core\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]core[/\\].*\\.cpp")
	source_group("core\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]core[/\\].*")
	source_group("core\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]core[/\\].*\\.h")

	set ( vorticity_core_SRCS
		src/core/application.cpp
		src/core/core.cpp
		src/core/entity.cpp
	)
	set (vorticity_SRCS ${vorticity_SRCS} ${vorticity_core_SRCS})

	set (vorticity_core_PUBLIC_HEADERS
		include/vorticity/core/application.h
		include/vorticity/core/core.h
		include/vorticity/core/entity.h
		include/vorticity/core/fonts.h
		include/vorticity/core/renderer.h
		include/vorticity/core/timer.h
	)
	set (vorticity_PUBLIC_HEADERS ${vorticity_PUBLIC_HEADERS} ${vorticity_core_PUBLIC_HEADERS})
	#No private headers

#overlay
	source_group(overlay\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]overlay[/\\].*\\.cpp")
	source_group("overlay\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]overlay[/\\].*")
	source_group("overlay\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]overlay[/\\].*\\.h")
	
	#sources
	set(vorticity_overlay_SRCS
		src/overlay/overlay.cpp
	)
	set (vorticity_SRCS ${vorticity_SRCS} ${vorticity_overlay_SRCS} )
	
	#public headers
	set(vorticity_overlay_PUBLIC_HEADERS
		include/vorticity/overlay/overlay.h
	)
	set (vorticity_PUBLIC_HEADERS ${vorticity_PUBLIC_HEADERS} ${vorticity_overlay_PUBLIC_HEADERS})
	
#platforms
	source_group(platforms\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]platforms[/\\].*\\.cpp")
	source_group("platforms\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]platforms[/\\].*")
	source_group("platforms\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]platforms[/\\].*\\.h")

	set (vorticity_platform_SRCS)
	set (vorticity_platforms_PUBLIC_HEADERS)
	set (vorticity_platforms_PRIVATE_HEADERS)
	if(WIN32)
		set (vorticity_platforms_SRCS
			src/platforms/win32/win32.cpp
		)
		set (vorticity_platforms_PUBLIC_HEADERS
			include/vorticity/platforms/win32/win32.h
		)
		set ( vorticity_platforms_PRIVATE_HEADERS
			src/platforms/win32/console.h
			src/platforms/win32/fontmanager.h
		)
	else(WIN32)
	endif(WIN32)
	
	set (vorticity_PUBLIC_HEADERS ${vorticity_SRCS} ${vorticity_platforms_SRCS})
	set (vorticity_PUBLIC_HEADERS ${vorticity_PUBLIC_HEADERS} ${vorticity_platforms_PUBLIC_HEADERS})
	set (vorticity_PRIVATE_HEADERS ${vorticity_PRIVATE_HEADERS} ${vorticity_platforms_PRIVATE_HEADERS})


#render
	source_group(render\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]render[/\\].*\\.cpp")
	source_group("render\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]render[/\\].*")
	source_group("render\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]render[/\\].*\\.h")
	
	set(vorticity_render_SRCS)
	set(vorticity_render_PUBLIC_HEADERS)
	
	#opengl Compatibility mode renderer
	if (OPENGL_FOUND)
		if(OPENGL_COMPATIBILITY_RENDERER)
			add_definitions(-DOPENGL_COMPATIBILITY_RENDERER)
			set(vorticity_render_SRCS ${vorticity_render_SRCS}
				src/render/opengl.cpp
				src/render/gloverlay.cpp
			)
			set(vorticity_render_PUBLIC_HEADERS ${vorticity_render_PUBLIC_HEADERS}
				include/vorticity/render/opengl.h				
			)
			set(vorticity_render_PRIVATE_HEADERS ${vorticity_render_PRIVATE_HEADERS} 
				src/render/gloverlay.h
			)			
		endif(OPENGL_COMPATIBILITY_RENDERER)
		set (vorticity_ADDITIONAL_LIBRARIES ${vorticity_ADDITIONAL_LIBRARIES} ${OPENGL_LIBRARIES} ${CG_GL_LIBRARY} ${CG_LIBRARY} ${GLEW_LIBRARY})
	endif(OPENGL_FOUND)	
	set (vorticity_SRCS ${vorticity_SRCS} ${vorticity_render_SRCS} )
	set (vorticity_PRIVATE_HEADERS ${vorticity_PRIVATE_HEADERS} ${vorticity_render_PRIVATE_HEADERS} )
	set (vorticity_PUBLIC_HEADERS ${vorticity_PUBLIC_HEADERS} ${vorticity_render_PUBLIC_HEADERS} )
	
#other ungrouped files (top level: /src/ )	
	set ( vorticity_other_PUBLIC_HEADERS 
		include/vorticity/vorticity.h
		include/vorticity/vorticity_base.h
		)
	set (vorticity_PUBLIC_HEADERS ${vorticity_PUBLIC_HEADERS} ${vorticity_other_PUBLIC_HEADERS})
	
	set ( vorticity_other_PRIVATE_HEADERS
		src/config.h
	) 
	set (vorticity_PRIVATE_HEADERS ${vorticity_PRIVATE_HEADERS} ${vorticity_other_PRIVATE_HEADERS})

#Actual library directive	
if(DYNAMIC_VORTICITY)	
	add_library (libvorticity SHARED ${vorticity_SRCS} ${vorticity_PUBLIC_HEADERS} ${vorticity_PRIVATE_HEADERS} )
else(DYNAMIC_VORTICITY)
	add_library (libvorticity STATIC ${vorticity_SRCS} ${vorticity_PUBLIC_HEADERS} ${vorticity_PRIVATE_HEADERS} )
endif(DYNAMIC_VORTICITY)
target_link_libraries(libvorticity ${vorticity_ADDITIONAL_LIBRARIES})

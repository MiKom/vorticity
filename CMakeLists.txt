# 
# Copyright © 2011 Miłosz Kosobucki
# 
# All rights reserved.
# 
# This file is part of Vorticity.
# 
# Vorticity is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Vorticity is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with Vorticity.  If not, see <http://www.gnu.org/licenses/>.
# 

cmake_minimum_required (VERSION 2.6)

project(vorticity)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMakeModules)

include(${PROJECT_SOURCE_DIR}/CMakeModules/DetermineProcessor.cmake)

include_directories ( ${PROJECT_SOURCE_DIR}/include )
include_directories ( ${PROJECT_SOURCE_DIR}/src )

option (DYNAMIC_VORTICITY "Set ON for dynamic linking" ON)
if(DYNAMIC_VORTICITY)
	add_definitions(-DVORTICITY_LIBRARY)
else(DYNAMIC_VORTICITY)
	add_definitions(-DVORTICITY_LIBRARY_STATIC)
endif(DYNAMIC_VORTICITY)

# setting c++0x support in gcc
if(${CMAKE_COMPILER_IS_GNUCXX})
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x)
endif(${CMAKE_COMPILER_IS_GNUCXX})

#containers for source files, public header files, private header files and additional libraries
set (vorticity_SRCS)
set (vorticity_PUBLIC_HEADERS)
set (vorticity_PRIVATE_HEADERS)
set (vorticity_ADDITIONAL_LIBRARIES)

#types library
	source_group(types\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]types[/\\].*\\.cpp")
	source_group("types\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]types[/\\].*")
	source_group("types\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]types[/\\].*\\.h")
	
	#public headers
	set (vorticity_types_PUBLIC_HEADERS
		include/vorticity/types/math.h
		include/vorticity/types/matrix4.h
		include/vorticity/types/string.h
		include/vorticity/types/vector2.h
		include/vorticity/types/vector3.h
		include/vorticity/types/vector4.h
	)
	set (vorticity_PUBLIC_HEADERS ${vorticity_PUBLIC_HEADERS} ${vorticity_types_PUBLIC_HEADERS})
	
	#no private headers

	#no sources

#core
	#source_group sets up filters in Visual Studio
	source_group(core\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]core[/\\].*\\.cpp")
	source_group("core\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]core[/\\].*")
	source_group("core\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]core[/\\].*\\.h")

	#public headers
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
	
	#sources
	set ( vorticity_core_SRCS
		src/core/application.cpp
		src/core/core.cpp
		src/core/entity.cpp
	)
	set (vorticity_SRCS ${vorticity_SRCS} ${vorticity_core_SRCS})


#overlay
	#source_group sets up filters in Visual Studio
	source_group(overlay\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]overlay[/\\].*\\.cpp")
	source_group("overlay\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]overlay[/\\].*")
	source_group("overlay\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]overlay[/\\].*\\.h")
	
	#public headers
	set(vorticity_overlay_PUBLIC_HEADERS
		include/vorticity/overlay/overlay.h
	)
	set (vorticity_PUBLIC_HEADERS ${vorticity_PUBLIC_HEADERS} ${vorticity_overlay_PUBLIC_HEADERS})

	#no private headers
	
	#sources
	set(vorticity_overlay_SRCS
		src/overlay/overlay.cpp
	)
	set (vorticity_SRCS ${vorticity_SRCS} ${vorticity_overlay_SRCS} )
	
#io subsystem

	find_package(DevIL)
	if(IL_FOUND)
		include_directories( ${IL_INCLUDE_DIR} )
		set (vorticity_ADDITIONAL_LIBRARIES ${vorticity_ADDITIONAL_LIBRARIES} ${IL_LIBRARIES})
	endif(IL_FOUND)

	#source_group sets up filters in Visual Studio
	source_group(io\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]io[/\\].*\\.cpp")
	source_group("io\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]io[/\\].*")
	source_group("io\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]io[/\\].*\\.h")
	
	#public headers
	set(vorticity_io_PUBLIC_HEADERS
		include/vorticity/io/iff_reader.h
		include/vorticity/io/image_loader.h
		include/vorticity/io/iobase.h
		include/vorticity/io/stdmesh_loader.h
		include/vorticity/io/text_loader.h
	)
	set(vorticity_PUBLIC_HEADERS ${vorticity_PUBLIC_HEADERS} ${vorticity_io_PUBLIC_HEADERS} )
	
	#no private headers
	
	#sources
	set(vorticity_io_SRCS
		src/io/image_loader.cpp
		src/io/iobase.cpp
		src/io/stdmesh_loader.cpp
		src/io/text_loader.cpp
	)	
	set(vorticity_SRCS ${vorticity_SRCS} ${vorticity_io_SRCS} )

#ppfx
	#source_group sets up filters in Visual Studio
	source_group(ppfx\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]ppfx[/\\].*\\.cpp")
	source_group("ppfx\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]ppfx[/\\].*")
	source_group("ppfx\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]ppfx[/\\].*\\.h")
	
	#public headers
	set(vorticity_ppfx_PUBLIC_HEADERS
		include/vorticity/ppfx/ppfx.h
		include/vorticity/ppfx/ppfx_hdr.h
		include/vorticity/ppfx/ppfx_simple.h
	)
	set(vorticity_PUBLIC_HEADERS ${vorticity_PUBLIC_HEADERS} ${vorticity_ppfx_PUBLIC_HEADERS} )
	
	#no private headers
	
	#sources
	set(vorticity_ppfx_SRCS
		src/ppfx/ppfx.cpp
		src/ppfx/ppfx_hdr.cpp
		src/ppfx/ppfx_simple.cpp
	)
	set(vorticity_SRCS ${vorticity_SRCS} ${vorticity_ppfx_SRCS} )
	
#resources
	#source_group sets up filters in Visual Studio
	source_group(resources\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]resources[/\\].*\\.cpp")
	source_group("resources\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]resources[/\\].*")
	source_group("resources\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]resources[/\\].*\\.h")
	
	#public headers
	set(vorticity_resources_PUBLIC_HEADERS
		include/vorticity/resources/backdrop.h
		include/vorticity/resources/image.h
		include/vorticity/resources/material.h
		include/vorticity/resources/mesh.h
		include/vorticity/resources/resource.h
		include/vorticity/resources/rset.h
		include/vorticity/resources/shader.h
		include/vorticity/resources/submesh.h
	)
	set(vorticity_PUBLIC_HEADERS ${vorticity_PUBLIC_HEADERS} ${vorticity_resources_PUBLIC_HEADERS} )
	
	#no private headers
	
	#sources
	set(vorticity_resources_SRCS
		src/resources/backdrop.cpp
		src/resources/image.cpp
		src/resources/material.cpp
		src/resources/mesh.cpp
		src/resources/rset.cpp
		src/resources/shader.cpp
		src/resources/submesh.cpp
	)	
	set(vorticity_SRCS ${vorticity_SRCS} ${vorticity_resources_SRCS} )	

#sg (Scene graph) AKA big S.O.B.
	#source_group sets up filters in Visual Studio
	source_group(sg\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]sg[/\\].*\\.cpp")
	source_group("sg\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]sg[/\\].*")
	source_group("sg\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]sg[/\\].*\\.h")

	set(vorticity_sg_PUBLIC_HEADERS
		include/vorticity/sg/array.h
		include/vorticity/sg/attribute.h
		include/vorticity/sg/controller.h
		include/vorticity/sg/keyframe_ctrl.h
		include/vorticity/sg/modifier.h
		include/vorticity/sg/node.h
		include/vorticity/sg/node_camera.h
		include/vorticity/sg/node_dummy.h
		include/vorticity/sg/node_lc.h
		include/vorticity/sg/node_light.h
		include/vorticity/sg/node_light_sm.h
		include/vorticity/sg/node_material.h
		include/vorticity/sg/node_mesh.h
		include/vorticity/sg/node_ppfx.h
		include/vorticity/sg/node_sc.h
		include/vorticity/sg/node_shader.h
		include/vorticity/sg/object.h
		include/vorticity/sg/proxy.h
		include/vorticity/sg/reference.h
		include/vorticity/sg/vevaluate.h
		include/vorticity/sg/vrender.h
		include/vorticity/sg/vrender_simple.h
		include/vorticity/sg/xnode.h
	)
	set(vorticity_PUBLIC_HEADERS ${vorticity_PUBLIC_HEADERS} ${vorticity_sg_PUBLIC_HEADERS} )

	#no private headers
	
	#sources
	set(vorticity_sg_SRCS
		src/sg/array.cpp
		src/sg/attribute.cpp
		src/sg/controller.cpp
		src/sg/modifier.cpp
		src/sg/node.cpp
		src/sg/node_camera.cpp
		src/sg/node_dummy.cpp
		src/sg/node_light.cpp
		src/sg/node_light_sm.cpp
		src/sg/node_material.cpp
		src/sg/node_mesh.cpp
		src/sg/node_shader.cpp
		src/sg/object.cpp
		src/sg/proxy.cpp
		src/sg/reference.cpp
		src/sg/vevaluate.cpp
		src/sg/vrender.cpp
		src/sg/vrender_simple.cpp
		src/sg/xnode.cpp
	)
	set(vorticity_SRCS ${vorticity_SRCS} ${vorticity_sg_SRCS} )
#platforms
	#source_group sets up filters in Visual Studio
	source_group(platforms\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]platforms[/\\].*\\.cpp")
	source_group("platforms\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]platforms[/\\].*")
	source_group("platforms\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]platforms[/\\].*\\.h")

	set (vorticity_platform_SRCS)
	set (vorticity_platforms_PUBLIC_HEADERS)
	set (vorticity_platforms_PRIVATE_HEADERS)
	if(WIN32)
		#source_group sets up filters in Visual Studio
		source_group(platforms\\Sources\\win32 REGULAR_EXPRESSION "[/\\]src[/\\]platforms[/\\].*\\.cpp")
		source_group("platforms\\Public Headers\\win32" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]platforms[/\\].*")
		source_group("platforms\\Private Headers\\win32" REGULAR_EXPRESSION "[/\\]src[/\\]platforms[/\\].*\\.h")
		
		set (vorticity_platforms_PUBLIC_HEADERS
			include/vorticity/platforms/win32/win32.h
		)
		set ( vorticity_platforms_PRIVATE_HEADERS
			src/platforms/win32/console.h
			src/platforms/win32/fontmanager.h
		)
		
		set (vorticity_platforms_SRCS
			src/platforms/win32/fontmanager.cpp
			src/platforms/win32/console.cpp
			src/platforms/win32/win32.cpp
		)
		
		set(vorticity_ADDITIONAL_LIBRARIES ${vorticity_ADDITIONAL_LIBRARIES} winmm.lib)
	else(WIN32)
	endif(WIN32)
	
	set (vorticity_SRCS ${vorticity_SRCS} ${vorticity_platforms_SRCS})
	set (vorticity_PUBLIC_HEADERS ${vorticity_PUBLIC_HEADERS} ${vorticity_platforms_PUBLIC_HEADERS})
	set (vorticity_PRIVATE_HEADERS ${vorticity_PRIVATE_HEADERS} ${vorticity_platforms_PRIVATE_HEADERS})


#render
	source_group(render\\Sources REGULAR_EXPRESSION "[/\\]src[/\\]render[/\\].*\\.cpp")
	source_group("render\\Public Headers" REGULAR_EXPRESSION "[/\\]include[/\\]vorticity[/\\]render[/\\].*")
	source_group("render\\Private Headers" REGULAR_EXPRESSION "[/\\]src[/\\]render[/\\].*\\.h")
	
	set(vorticity_render_SRCS)
	set(vorticity_render_PUBLIC_HEADERS)
	
	find_package (OpenGL)
	if(OPENGL_FOUND)
		include_directories ( ${OPENGL_INCLUDE_DIR} )
		option (OPENGL_COMPATIBILITY_RENDERER "Build OpenGL compatibility profile renderer" ON)
		set (vorticity_ADDITIONAL_LIBRARIES ${vorticity_ADDITIONAL_LIBRARIES} ${OPENGL_LIBRARIES} )

		find_package (GLEW)
		if(GLEW_FOUND)
			include_directories ( ${GLEW_INCLUDE_PATH} )
			set (vorticity_ADDITIONAL_LIBRARIES ${vorticity_ADDITIONAL_LIBRARIES} ${GLEW_LIBRARY})
		endif(GLEW_FOUND)
		
		find_package (Cg)
		if(CG_FOUND)
			include_directories ( ${CG_INCLUDE_DIR} )
			set (vorticity_ADDITIONAL_LIBRARIES ${vorticity_ADDITIONAL_LIBRARIES} ${CG_GL_LIBRARY} ${CG_LIBRARY})
		endif(CG_FOUND)
		
		#opengl Compatibility mode renderer
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
	add_library (vorticity SHARED ${vorticity_SRCS} ${vorticity_PUBLIC_HEADERS} ${vorticity_PRIVATE_HEADERS} )
else(DYNAMIC_VORTICITY)
	add_library (vorticity STATIC ${vorticity_SRCS} ${vorticity_PUBLIC_HEADERS} ${vorticity_PRIVATE_HEADERS} )
endif(DYNAMIC_VORTICITY)
target_link_libraries(vorticity ${vorticity_ADDITIONAL_LIBRARIES})

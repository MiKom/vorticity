set ( vorticity_SRCS ${VORTICITY_SRCS} 
	core/application.cpp
	core/core.cpp
	core/entity.cpp
	)
	
add_library (libvorticity ${vorticity_SRCS} )
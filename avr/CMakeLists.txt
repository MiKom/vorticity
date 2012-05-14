#
# Copyright © 2012 Miłosz Kosobucki
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

set( libavr_SRCS
	src/avrfile.cpp
	src/avrmaterial.cpp
	src/avrmesh.cpp
	src/avrtexture.cpp
	src/avrutil.cpp
)

set( libavr_PUBLIC_HEADERS
	include/avr/avrfile.h
	include/avr/avrmaterial.h
	include/avr/avrmesh.h
	include/avr/avrtexture.h
	include/avr/avrmath.h
	include/avr/avr++.h
)

set( libavr_PRIVATE_HEADERS
	include/avr/avrutil.h
)


include_directories( include/avr include src )

add_subdirectory(examples)

add_definitions( -DAVR_COMPILATION )
add_library( avr SHARED
	${libavr_SRCS}
	${libavr_PUBLIC_HEADERS}
	${libavr_PRIVATE_HEADERS}
)

set_target_properties(avr PROPERTIES PUBLIC_HEADER "${libavr_PUBLIC_HEADERS}" )

install(TARGETS avr
	LIBRARY		DESTINATION 	lib
	PUBLIC_HEADER	DESTINATION	include/avr
)

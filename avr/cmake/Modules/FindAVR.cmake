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

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)

FIND_PATH(AVR_INCLUDE_DIR avr/avr++.h 
  PATH_SUFFIXES include IL
  DOC "The path the the directory that contains avr/avr++.h"
)



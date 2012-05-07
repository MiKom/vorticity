/* 
 * Copyright © 2012 Miłosz Kosobucki
 *
 * All rights reserved.
 * 
 * This file is part of Vorticity.
 *
 * Vorticity is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vorticity is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vorticity.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

namespace AVR
{

/**
  This function writes header of the chunk to output stream.
  it returns position in the file, where writing was started.
  This position should be later passed to endChunk(). This
  function also reserves space for writing size of the chunk when it is closed.
  \param os output stream, that the header will be written to
  \param id id of the chunk. Usually FourCC. The same id must be passed to
  corresponding endChunk() call
  */
std::ios::pos_type
startChunk(std::ostream& os, const std::string& id);

/**
  This function goes to the beginning of current chunk, as specified by
  startPos, writes the chunk size after the chunk id and goes back to the end of
  the chunk.
  \param os output stream that is worked on,
  \param id id of the chunk, the same as in corresponding startChunk call.
  Usually FourCC
  \param startPos starting position of the chunk being ended. This must be the
  value that was return by the corresponding call to startChunk
  */  
void 
endChunk(std::ostream& os,
         const std::string& id,
         const std::ios::pos_type& startPos);

} //AVR

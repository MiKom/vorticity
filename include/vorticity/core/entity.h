/* 
 * Copyright © 2009, 2010 Michał Siejak
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

#ifndef __VORTICITY_ENTITY_H
#define __VORTICITY_ENTITY_H

namespace Vorticity {

class VAPI Entity
{
protected:
	bool	storableFlag;
protected:
	Entity();
public:
	virtual ~Entity();

	// Basic entity class and type
	virtual string getClass() const = 0;
	virtual string getType() const = 0;

	// Object serialization
	virtual bool store(std::ofstream &file)	{ return false; }
	virtual bool load(std::ifstream &file)	{ return false; }

	// Getters and setters
	bool isStorable() const	{ return storableFlag; }

	// Factory
	static Entity*	create() { return NULL; }
	virtual Entity* clone()  { return NULL; }
};

} // Vorticity

#endif
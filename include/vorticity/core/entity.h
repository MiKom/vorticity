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
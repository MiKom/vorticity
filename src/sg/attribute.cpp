#include "config.h"
#include "attribute.h"

using namespace Vorticity;

NullAttribute NullAttribute::Null;

AnyAttribute::AnyAttribute() : Entity()
{
	dirtyFlag	 = true;
	parent		 = NULL;
}

AnyAttribute::AnyAttribute(const string &name, bool storable) : Entity()
{
	this->name			= name;
	this->storableFlag	= storable;
	dirtyFlag			= true;
	parent				= NULL;
}

AnyAttribute::~AnyAttribute()
{ }

void AnyAttribute::setParent(const ArrayAttribute *parent)
{ this->parent = const_cast<ArrayAttribute*>(parent); }

ArrayAttribute* AnyAttribute::getParent() const
{ return parent; }

bool AnyAttribute::load(std::fstream &file)
{ return name.read(file); }

bool AnyAttribute::store(std::fstream &file)
{ return name.write(file); }
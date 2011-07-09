#ifndef __VORTICITY_BACKDROP_H
#define __VORTICITY_BACKDROP_H

namespace Vorticity {

class Shader;
class VAPI Backdrop : public Resource
{
protected:
	int target;
	int target_w, target_h;
	Shader* shader;
public:
	Backdrop(const int target, Shader* shader=NULL);

	virtual string getType() const
	{ return "backdrop"; }

	Shader* 	getShader() const			{ return shader; }
	void 		setShader(Shader* shader) 	{ this->shader = shader; }
	int			getTarget() const			{ return target; }
	void		setTarget(const int target);

	virtual bool draw();
};

} // Vorticity

#endif
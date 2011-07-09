#ifndef __VORTICITY_VEVALUATE_H
#define __VORTICITY_VEVALUATE_H

namespace Vorticity {

class VAPI EvaluateVisitor : public AbstractVisitor
{
public:
	EvaluateVisitor();
	virtual void traverse(Node* root);
};

}

#endif
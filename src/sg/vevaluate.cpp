#include "config.h"
#include "attribute.h"
#include "node.h"
#include "xnode.h"
#include "vevaluate.h"

using namespace Vorticity;

EvaluateVisitor::EvaluateVisitor() : AbstractVisitor()
{
}

void EvaluateVisitor::traverse(Vorticity::Node *root)
{
	if(!root->getParent())
		root->clearEvaluated();

	if(root->isEvaluated()) return;
	root->setEvaluated();

	if(!root->isEnabled())
		return;
	NodeIterator it = root->enumAffectedBy(0);
	while(Node* n = root->getAffectedBy(it))
		traverse(n);

	root->evaluate();
	if(root->hasChildren())
	{
		it = ((XNode*)root)->enumChildren(0);
		while(Node* n = ((XNode*)root)->getChild(it))
			traverse(n);
	}
}
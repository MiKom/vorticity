/* 
 * Copyright © 2009, 2010 Micha³ Siejak
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

#include "config.h"
#include "vorticity/sg/attribute.h"
#include "vorticity/sg/node.h"
#include "vorticity/sg/xnode.h"
#include "vorticity/sg/vevaluate.h"

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
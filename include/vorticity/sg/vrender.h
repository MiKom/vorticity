#ifndef __VORTICITY_VRENDER_H
#define __VORTICITY_VRENDER_H

namespace Vorticity {

class Renderer;
class Node;

class VAPI ModifierCache
{
protected:
	std::vector<Modifier*>	modifiers;
	XNode *node;
public:
	ModifierCache();

	void		setNode(XNode *n)
	{ node = n; }
	XNode*		getNode() const
	{ return node; }

	bool		isModifier(const string& type) const;
	Modifier*	updateModifier(const string& type, XNode *node);
	Modifier*	getModifier(const string &type) const;

	void		addModifier(Modifier *mod)
	{ modifiers.push_back(mod); }

	void		clear()
	{ modifiers.clear(); }
};

typedef std::vector<ModifierCache> ModifierCacheArray;

class NodeCamera;
class VAPI RenderVisitor : public AbstractVisitor
{
private:
	static const string NullGroupName;
	bool cached;
protected:
	Renderer *device;
	NodeCamera *camera;

	std::map<string, ModifierCacheArray> group;
	std::vector<Modifier*> shaders;

	virtual void setupShaderParameters(const ModifierCache &cache, const int shader);
	virtual void createNodeCache(XNode* node, ModifierCache &cache);

	// DO NOT under any circumstances change the second parameter to a reference.
	// This HAS to be passed by-value since every recursion owns a copy of ModifierCache.
	virtual void createRenderGroups(XNode* node, ModifierCache cache);
	virtual void traverseRenderGroups();
public:
	RenderVisitor(NodeCamera* cam);
	virtual void traverse(Node* root);

	void	recache() 
	{ cached = false;	}
	bool	isCached() const
	{ return cached;	}
};

} // Vorticity

#endif
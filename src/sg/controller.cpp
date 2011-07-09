#include "config.h"
#include "attribute.h"
#include "node.h"
#include "controller.h"

using namespace Vorticity;

Controller::Controller(const string& name, XNode *parent) : Node(name, parent)
{
}

Controller::~Controller()
{
}

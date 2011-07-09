#ifndef __VORTICITY_H
#define __VORTICITY_H

#include "vorticity_base.h"
#include "overlay/overlay.h"

#include "sg/attribute.h"
#include "sg/node.h"
#include "sg/xnode.h"
#include "sg/object.h"
#include "sg/node_dummy.h"
#include "sg/vevaluate.h"

#include "io/iobase.h"
#include "io/iff_reader.h"
#include "io/image_loader.h"
#include "io/text_loader.h"
#include "io/stdmesh_loader.h"

#include "resources/resource.h"
#include "resources/shader.h"
#include "resources/material.h"
#include "resources/mesh.h"
#include "resources/image.h"
#include "resources/backdrop.h"

#include "sg/modifier.h"
#include "sg/proxy.h"
#include "sg/node_shader.h"
#include "sg/node_camera.h"
#include "sg/node_mesh.h"
#include "sg/node_material.h"
#include "sg/node_light.h"
#include "sg/controller.h"
#include "sg/keyframe_ctrl.h"
#include "sg/node_lc.h"
#include "sg/node_sc.h"
#include "sg/vrender.h"
#include "sg/vrender_simple.h"

#include "sg/node_ppfx.h"
#include "ppfx/ppfx.h"
#include "ppfx/ppfx_simple.h"
#include "ppfx/ppfx_hdr.h"

#endif
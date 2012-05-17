#include<avr/avr++.h>
#include<iostream>
#include<cstdlib>
#include<string>

using namespace AVR;

void usage(char* exName)
{
	std::cout << "Usage:\n"
	             "For uniform size on all axes:\n"
	             	"\t" << exName << " [size] [filename]\n"
	             	"\t e.g.: " << exName << " 1.0 cube.avr\n"
	             "For varying sizes on axes:\n"
	             	"\t" << exName << " [xsize] [ysize] [zsize] [filename]\n"
	             	"\t e.g.:" << exName << " 1.0 2.0 0.5 cube.avr\n";
}

/*
  Vertex numbering:
     7             6
     +-----------  +
    /|            /|
  3/ |          2/ |
  +-------------+  |
  |  |          |  |
  |  |          |  |
  |  +----------|--+
  | /4          | /5
  |/            |/
  +-------------+
  0             1
  
*/

int main(int argc, char** argv)
{
	float xsize, ysize, zsize;
	std::string filename;
	
	if(argc != 3 && argc != 5) {
		usage(argv[0]);
		exit(1);
	}
	
	int argvPos = 1;
	if(argc == 3) {
		xsize = ysize = zsize = atof(argv[argvPos++]); 
	} else if(argc == 5) {
		xsize = atof(argv[argvPos++]);
		ysize = atof(argv[argvPos++]);
		zsize = atof(argv[argvPos++]);
	}
	filename = std::string(argv[argvPos++]);
	
	AVRFile file;
	std::vector<vec3> vertices = {
		vec3{-xsize, -ysize,  zsize},
		vec3{ xsize, -ysize,  zsize},
		vec3{ xsize,  ysize,  zsize},
		vec3{-xsize,  ysize,  zsize},
		vec3{-xsize, -ysize, -zsize},
		vec3{ xsize, -ysize, -zsize},
		vec3{ xsize,  ysize, -zsize},
	        vec3{-xsize,  ysize, -zsize}
	};
	
	//TODO: Fix normals
	vec3 normals[6] = {
	};
	std::vector<vec3> normalsVec = {
		vec3{0,  0, -1}, vec3{ 1,  0, 0}, vec3{0,  1,  0},
		vec3{-1, 0,  0}, vec3{ 0, -1, 0}, vec3{0,  0,  1}
	};
	
	std::vector<AVRFace> faces = {
		AVRFace{{0, 1, 3}, {0, 0, 0}, {0, 0, 0}},
		AVRFace{{1, 2, 3}, {0, 0, 0}, {0, 0, 0}},
		AVRFace{{1, 6, 2}, {3, 3, 3}, {0, 0, 0}},
		AVRFace{{1, 5, 6}, {3, 3, 3}, {0, 0, 0}},
		AVRFace{{1, 0, 5}, {2, 2, 2}, {0, 0, 0}},
		AVRFace{{0, 4, 5}, {2, 2, 2}, {0, 0, 0}},
		AVRFace{{0, 7, 4}, {1, 1, 1}, {0, 0, 0}},
		AVRFace{{0, 3, 7}, {1, 1, 1}, {0, 0, 0}},
		AVRFace{{3, 2, 6}, {4, 4, 4}, {0, 0, 0}},
		AVRFace{{3, 6, 7}, {4, 4, 4}, {0, 0, 0}},
		AVRFace{{4, 7, 5}, {5, 5, 5}, {0, 0, 0}},
		AVRFace{{5, 7, 6}, {5, 5, 5}, {0, 0, 0}} 
	};
	
	AVRMesh mesh;
	mesh.setName("box");
	file.addMesh(&mesh);
	mesh.addVertices(vertices);
	mesh.addNormals(normalsVec);
	mesh.addTextCoord(vec2{0.0f, 0.0f});
	mesh.addFaces(faces);
	mesh.setMaterialId(0);
	
	
	AVRMaterial material;
	material.setName("red");
	material.setDiffuse(0.5f, 0.0f, 0.0f);
	material.setSpecular(0.9f, 0.9f, 0.9f);
	material.setAmbient(0.0f, 0.0f, 0.0f);
	material.setEmissive(1.0f, 0.0f, 0.0f);
	material.setOpacity(1.0f);
	material.setSpecularLevel(0.25f);
	material.setGlossiness(0.15f);
	
	file.addMaterial(material);
	file.save(filename);
	return 0;
}

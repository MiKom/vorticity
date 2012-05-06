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
	vec3 vertices[8] = {
		{ xsize, -ysize, -zsize},
		{ xsize,  ysize, -zsize},
		{-xsize,  ysize, -zsize},
		{-xsize, -ysize,  zsize},
		{ xsize, -ysize,  zsize},
		{ xsize,  ysize,  zsize},
		{-xsize,  ysize,  zsize}
	};
	std::vector<vec3> verticesVec = {
		vertices[0],vertices[1],vertices[2],vertices[3],
		vertices[4],vertices[5],vertices[6],vertices[7],
	};
	//TODO: implement rest
	file.save(filename);
	return 0;
}

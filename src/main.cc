// ---------------------------------------------------------------------------------------
// Salvador Galindo
// sagalind
// CMPS 109
// Professor David Harrison
// Lab 4
// Due 04/29/18
// ---------------------------------------------------------------------------------------

/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include "point.h"
#include "circle.h"
#include "polygon.h"
#include "reuleauxtriangle.h"
#include "sphere.h"
#include "reuleauxtetrahedron.h"
#include "cube.h"

#define USAGE "USAGE %s test-file\n"

// CITATION ---------------------------------------------------------------------------------------------------------
// Function that checks if a string is a valid double:
// https://stackoverflow.com/questions/29169153/how-do-i-verify-a-string-is-valid-double-even-if-it-has-a-point-in-it 
bool is_number(const std::string& s) {
    try
    {
        std::stod(s);
    }
    catch(...)
    {
        return false;
    }
    return true;
}
// END CITATION -----------------------------------------------------------------------------------------------------

static Point2D getPoint(const std::string token) {
	std::stringstream stream(token);
	std::string value;
	double x,y;
	if (std::getline(stream, value, ',')) {
		if (is_number(value))
			std::stringstream(value) >> x;
		if (std::getline(stream, value, ',')) {
			if (is_number(value))
				std::stringstream(value) >> y;
			return Point2D(x, y);
		}
	}
	throw "Invalid Point2D: " + token;
}

static Point3D getPoint3D(const std::string token) {
	std::stringstream stream(token);
	std::string value;
	double x,y,z;
	if (std::getline(stream, value, ',')) {
		if (is_number(value))
			std::stringstream(value) >> x;
		if (std::getline(stream, value, ',')) {
			if (is_number(value))
				std::stringstream(value) >> y;
			if (std::getline(stream, value, ',')) {
				if(is_number(value))
					std::stringstream(value) >> z;
				return Point3D(x, y, z);
			}
		}
	}
	throw "Invalid Point3D: " + token;
}

static double getRadius(const std::string token) {
	double radius;
	if (is_number(token)) {
		std::stringstream(token) >> radius;
		return radius;
	}
	throw "Invalid Radius: " + token;
}

static void parse(const std::string line) {
	std::stringstream stream(line);
	std::string token;

	Circle               *innerC = NULL;
	Circle               *outerC = NULL;
	RegularConvexPolygon *innerP = NULL;
	RegularConvexPolygon *outerP = NULL;
	ReuleauxTriangle     *innerRT = NULL;
	ReuleauxTriangle     *outerRT = NULL;
	Sphere               *innerS = NULL;
	Sphere               *outerS = NULL;
	ReuleauxTetrahedron  *innerRTetra = NULL;
	ReuleauxTetrahedron  *outerRTetra = NULL;
	Cube                 *innerCube = NULL;
	Cube                 *outerCube = NULL;

	bool expected = false;
	while (stream >> token) {
		// CITATION -----------------------------------------------------------------------------------------------------
		// Part of this function is from Professor Harrison during lecture.
		// https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=f1b2136b-19ef-4971-b4c6-504eadc46eac
		if (token == "Circle") {
			stream >> token;
			Point2D center = getPoint(token);
			stream >> token;
			double radius = getRadius(token);
			if (innerC == NULL && innerP == NULL && innerRT == NULL && innerS == NULL && innerRTetra == NULL && innerCube == NULL) {
				innerC = new Circle(center, radius);
				std::cout << "    Inner Circle: (" << innerC->center().x << "," << 
					innerC->center().y << ") radius: " << innerC->radius() << std::endl;
			} else {
				outerC = new Circle(center, radius);
				std::cout << "    Outer Circle: (" << outerC->center().x << "," << 
					outerC->center().y << ") radius: " << outerC->radius() << std::endl;
			}
		} 
		// END CITATION --------------------------------------------------------------------------------------------------- 
		if (token == "RegularConvexPolygon") {
			std::stringstream::pos_type pos = stream.tellg();
			std::stringstream nStream(line); 
			std::string nextToken;
			nStream.seekg(pos);
			std::vector<Point2D> vertices;
			while (nStream >> nextToken) {
				//std::cout << "Next: " << nextToken << std::endl;
				if (nextToken.find(',') != std::string::npos) {
					stream >> token;
					//std::cout << "Current: " << token << std::endl;
					vertices.push_back(getPoint(token));
				} 
				else 
					break;
			}
			if (innerC == NULL && innerP == NULL && innerRT == NULL && innerS == NULL && innerRTetra == NULL && innerCube == NULL) {
				innerP = new RegularConvexPolygon(vertices);
				std::cout << "    Inner Polygon verts: " << innerP->vertexCount() << std::endl;
			} else {
				outerP = new RegularConvexPolygon(vertices);
				std::cout << "    Outer Polygon verts: " << outerP->vertexCount() << std::endl;
			}
		}
		if (token == "ReuleauxTriangle") {
			std::stringstream::pos_type pos = stream.tellg();
			std::stringstream nStream(line); 
			std::string nextToken;
			nStream.seekg(pos);
			Point2D vertices[3];
			int i = 0;
			while (nStream >> nextToken) {
				if (nextToken.find(',') != std::string::npos) {
					stream >> token;
					vertices[i] = getPoint(token);
					i++;
				}
				else
					break;
			}
			if (innerC == NULL && innerP == NULL && innerRT == NULL && innerS == NULL && innerRTetra == NULL && innerCube == NULL) {
				innerRT = new ReuleauxTriangle(vertices);
				Point2D *v = innerRT->vertices();
				std::cout << "    Inner ReuleauxTriangle: ";
				for (int i = 0; i < 3; i++) {
					std::cout << "(" << v[i].x << "," << v[i].y << ") ";
				}
				std::cout << std::endl;
			} else {
				outerRT = new ReuleauxTriangle(vertices);
				Point2D *v = outerRT->vertices();
				std::cout << "    Outer ReuleauxTriangle: ";
				for (int i = 0; i < 3; i++) {
					std::cout << "(" << v[i].x << "," << v[i].y << ") ";
				}
				std::cout << std::endl;
			}
		}
		if (token == "Sphere") {
			stream >> token;
			Point3D center = getPoint3D(token);
			stream >> token;
			double radius = getRadius(token);
			if (innerC == NULL && innerP == NULL && innerRT == NULL && innerS == NULL && innerRTetra == NULL && innerCube == NULL) {
				innerS = new Sphere(center, radius);
				std::cout << "    Inner Sphere: (" << innerS->center().x << "," << 
					innerS->center().y << "," << innerS->center().z << ") radius: " << 
					innerS->radius() << std::endl;
			} else {
				outerS = new Sphere(center, radius);
				std::cout << "    Outer Sphere: (" << outerS->center().x << "," << 
					outerS->center().y << "," << outerS->center().z << ") radius: " << 
					outerS->radius() << std::endl;
			}
		}
		if (token == "ReuleauxTetrahedron") {
			std::stringstream::pos_type pos = stream.tellg();
			std::stringstream nStream(line); 
			std::string nextToken;
			nStream.seekg(pos);
			Point3D vertices[4];
			int i = 0;
			while (nStream >> nextToken) {
				if (nextToken.find(',') != std::string::npos) {
					stream >> token;
					vertices[i] = getPoint3D(token);
					i++;
				}
				else
					break;
			}
			if (innerC == NULL && innerP == NULL && innerRT == NULL && innerS == NULL && innerRTetra == NULL && innerCube == NULL) {
				innerRTetra = new ReuleauxTetrahedron(vertices);
				Point3D *v = innerRTetra->vertices();
				std::cout << "    Inner ReuleauxTetrahedron: ";
				for (int i = 0; i < 4; i++) {
					std::cout << "(" << v[i].x << "," << v[i].y << "," << v[i].z << "), ";
				}
				std::cout << std::endl;
			} else {
				outerRTetra = new ReuleauxTetrahedron(vertices);
				Point3D *v = outerRTetra->vertices();
				std::cout << "    Outer ReuleauxTetrahedron: ";
				for (int i = 0; i < 4; i++) {
					std::cout << "(" << v[i].x << "," << v[i].y << "," << v[i].z << "), ";
				}
				std::cout << std::endl;
			}
		}
		if (token == "Cube") {
			std::stringstream::pos_type pos = stream.tellg();
			std::stringstream nStream(line); 
			std::string nextToken;
			nStream.seekg(pos);
			Point3D upperFace[4];
			Point3D lowerFace[4];
			int i = 0;
			int j = 0;
			while (nStream >> nextToken) {
				if (nextToken.find(',') != std::string::npos) {
					stream >> token;
					if (i < 4) {
						upperFace[i] = getPoint3D(token);
						i++;
					}
					else if (j < 4) {
						lowerFace[j] = getPoint3D(token);
						j++;
					} 
				}
				else
					break;
			}
			if (innerC == NULL && innerP == NULL && innerRT == NULL && innerS == NULL && innerRTetra == NULL && innerCube == NULL) {
				innerCube = new Cube(upperFace, lowerFace);
				Point3D *vUp = innerCube->upperFace();
				Point3D *vLo = innerCube->lowerFace();
				std::cout << "    Inner Cube: ";
				for (int i = 0; i < 4; i++) {
					std::cout << "(" << vUp[i].x << "," << vUp[i].y << "," << vUp[i].z << "), ";
				}
				for (int i = 0; i < 4; i++) {
					std::cout << "(" << vLo[i].x << "," << vLo[i].y << "," << vLo[i].z << "), ";
				}
				std::cout << std::endl;
			} else {
				outerCube = new Cube(upperFace, lowerFace);
				Point3D *vUp = outerCube->upperFace();
				Point3D *vLo = outerCube->lowerFace();
				std::cout << "    Outer Cube: ";
				for (int i = 0; i < 4; i++) {
					std::cout << "(" << vUp[i].x << "," << vUp[i].y << "," << vUp[i].z << ") ";
				}
				for (int i = 0; i < 4; i++) {
					std::cout << "(" << vLo[i].x << "," << vLo[i].y << "," << vLo[i].z << ") ";
				}
				std::cout << std::endl;
			}
		} 
		if (token == "true") {
			expected = true;
		}
	}
	bool got = false;
	if (innerC != NULL) {
		if (outerC != NULL) {
			got = innerC->containedWithin(*outerC);
		}
		else if (outerP != NULL) {
			got = innerC->containedWithin(*outerP);
		}
		else if (outerRT != NULL) {
			got = innerC->containedWithin(*outerRT);
		}
	} 
	else if (innerP != NULL) {
		if (outerC != NULL) {
			got = innerP->containedWithin(*outerC);
		}
		else if (outerP != NULL) {
			got = innerP->containedWithin(*outerP);
		}
		else if (outerRT != NULL) {
			got = innerP->containedWithin(*outerRT);
		}
	}
	else if (innerRT != NULL) {
		if (outerC != NULL) {
			got = innerRT->containedWithin(*outerC);
		}
		else if (outerP != NULL) {
			got = innerRT->containedWithin(*outerP);
		}
		else if (outerRT != NULL) {
			got = innerRT->containedWithin(*outerRT);
		}
	}
	else if (innerS != NULL) {
		if (outerS != NULL) {
			got = innerS->containedWithin(*outerS);
		}
		else if (outerRTetra != NULL) {
			got = innerS->containedWithin(*outerRTetra);
		}
		else if (outerCube != NULL) {
			got = innerS->containedWithin(*outerCube);
		}
	}
	else if (innerRTetra != NULL) {
		if (outerS != NULL) {
			got = innerRTetra->containedWithin(*outerS);
		}
		else if (outerRTetra != NULL) {
			got = innerRTetra->containedWithin(*outerRTetra);
		}
		else if (outerCube != NULL) {
			got = innerRTetra->containedWithin(*outerCube);
		}
	}
	else if (innerCube != NULL) {
		if (outerS != NULL) {
			got = innerCube->containedWithin(*outerS);
		}
		else if (outerRTetra != NULL) {
			got = innerCube->containedWithin(*outerRTetra);
		}
		else if (outerCube != NULL) {
			got = innerCube->containedWithin(*outerCube);
		}
	}
	if ((innerC == NULL && innerP == NULL && innerRT == NULL && innerS == NULL && innerRTetra == NULL && innerCube == NULL) ||
		 (outerC == NULL && outerP == NULL && outerRT == NULL && outerS == NULL && outerRTetra == NULL && outerCube == NULL)) {
		throw "Invalid line.";
	}

	std::cout << (got == expected ? "PASS" : "FAIL") << std::endl;
}

int main(int argc, char *argv[]) 
{

    if (argc < 2) {
        printf(USAGE, argv[0]);
        return -1;
    }
    
    std::cout << "Test file: " << argv[1] << std::endl;

    // CITATION -----------------------------------------------------------------------------------------------------
    // Some of this code is from Professor Harrison during lecture.
    // https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=f1b2136b-19ef-4971-b4c6-504eadc46eac
    std::ifstream ifs(argv[1]);
    if (ifs) {
    	std::string line;
    	while (std::getline(ifs, line)) {
    		if (line.find('#') == std::string::npos) {
    			try {
    				parse(line);
    			}
    			catch(...) {
    				std::cout << "Error: invalid test input." << std::endl;
    			}
    		}
    	}
    }
    // END CITATION -------------------------------------------------------------------------------------------------
    return 0;
}

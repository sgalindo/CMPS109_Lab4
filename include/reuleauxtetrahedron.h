// ---------------------------------------------------------------------------------------
// Salvador Galindo
// sagalind
// CMPS 109
// Professor David Harrison
// Lab 4
// Due 04/29/18
// ---------------------------------------------------------------------------------------

#ifndef _REULEAUXTETRAHEDRON_H_
#define _REULEAUXTETRAHEDRON_H_

#include "containable.h"
#include "point.h"
#include "sphere.h"
#include "cube.h"
#include "sphere.h"

class Sphere;

class ReuleauxTetrahedron : public Containable3D {
	private:
		Point3D vertices_[4];

	public: 
		ReuleauxTetrahedron(Point3D vertices[4]);
		Point3D *vertices();

		bool containedWithin(Sphere &sphere);
		bool containedWithin(Cube &cube);
		bool containedWithin(ReuleauxTetrahedron &rt);
};

#endif
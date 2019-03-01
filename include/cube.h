// ---------------------------------------------------------------------------------------
// Salvador Galindo
// sagalind
// CMPS 109
// Professor David Harrison
// Lab 4
// Due 04/29/18
// ---------------------------------------------------------------------------------------

#ifndef _CUBE_H_
#define _CUBE_H_

#include "containable.h"
#include "point.h"
#include "sphere.h"
#include "reuleauxtetrahedron.h"

class Cube : public Containable3D {
	private:
		Point3D upperFace_[4];
		Point3D lowerFace_[4];

	public: 
		Cube(Point3D upperFace[4], Point3D lowerFace[4]);
		
		Point3D *upperFace();
		void setUpperFace(Point3D upperFace[4]);

		Point3D *lowerFace();
		void setLowerFace(Point3D lowerFace[4]);

		bool containedWithin(Sphere &sphere);
		bool containedWithin(Cube &cube);
		bool containedWithin(ReuleauxTetrahedron &rt);
};

#endif
// ---------------------------------------------------------------------------------------
// Salvador Galindo
// sagalind
// CMPS 109
// Professor David Harrison
// Lab 4
// Due 04/29/18
// ---------------------------------------------------------------------------------------

#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "containable.h"
#include "point.h"
#include "reuleauxtetrahedron.h"
#include "cube.h"

class Sphere : public Containable3D {
	private:
		Point3D center_;
		double radius_;
	public:
		Sphere(Point3D center, double radius);

		Point3D center();
		void setCenter(Point3D center);

		double radius();
		void setRadius(double radius);

		bool containedWithin(Sphere &sphere);
		bool containedWithin(Cube &cube);
		bool containedWithin(ReuleauxTetrahedron &rt);
};

#endif
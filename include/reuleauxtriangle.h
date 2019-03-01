// ---------------------------------------------------------------------------------------
// Salvador Galindo
// sagalind
// CMPS 109
// Professor David Harrison
// Lab 4
// Due 04/29/18
// ---------------------------------------------------------------------------------------

#ifndef _REULEAUXTRIANGLE_H_
#define _REULEAUXTRIANGLE_H_

#include <vector>

#include "containable.h"
#include "point.h"
#include "circle.h"
#include "line.h"

class Circle;

class ReuleauxTriangle : public Containable2D {
	private:
		Point2D vertices_[3];

	public: 
		ReuleauxTriangle(Point2D vertices[3]);
		Point2D *vertices();

		bool containedWithin(Circle &circle);
		bool containedWithin(RegularConvexPolygon &polygon);
		bool containedWithin(ReuleauxTriangle &rt);
};

#endif
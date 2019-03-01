// ---------------------------------------------------------------------------------------
// Salvador Galindo
// sagalind
// CMPS 109
// Professor David Harrison
// Lab 4
// Due 04/29/18
// ---------------------------------------------------------------------------------------

#include <iostream>

#include "cube.h"
#include "reuleauxtetrahedron.h"
#include "sphere.h"
#include "geom.h"
#include "point.h"

Cube::Cube(Point3D upperFace[4], Point3D lowerFace[4]) {
	setUpperFace(upperFace);
	setLowerFace(lowerFace);
}

Point3D *Cube::upperFace() {
	return upperFace_;
}

void Cube::setUpperFace(Point3D upperFace[4]) {
	for (int i = 0; i < 4; i++) {
		upperFace_[i] = upperFace[i];
	}
}

Point3D *Cube::lowerFace() {
	return lowerFace_;
}

void Cube::setLowerFace(Point3D lowerFace[4]) {
	for (int i = 0; i < 4; i++) {
		lowerFace_[i] = lowerFace[i];
	}
}

bool Cube::containedWithin(Sphere &sphere) {
	for (int i = 0; i < 4; i++) {
		if (Geom::distance3D(upperFace_[i], sphere.center()) > sphere.radius() ||
			Geom::distance3D(lowerFace_[i], sphere.center()) > sphere.radius())
			return false;
	}
	return true;
}

bool Cube::containedWithin(Cube &cube) {
	std::vector<Point2D> vertices;
    std::cout << "Checking Polygon: ";
    for (int i = 0; i < 4; i++) {
        vertices.push_back(Point2D(upperFace()[i].x, upperFace()[i].z));
        std::cout << "(" << vertices[i].x << "," << vertices[i].y << "), ";
    }
    std::cout << std::endl;
    RegularConvexPolygon topIn = RegularConvexPolygon(vertices);

    vertices.clear();
    std::cout << "with Polygon: ";
    for (int i = 0; i < 4; i++) {
        vertices.push_back(Point2D(cube.upperFace()[i].x, cube.upperFace()[i].z));
        std::cout << "(" << vertices[i].x << "," << vertices[i].y << "), ";
    }
    std::cout << std::endl;
    RegularConvexPolygon topOut = RegularConvexPolygon(vertices);
    
    if (!topIn.containedWithin(topOut)) {
        std::cout << "Intersection!" << std::endl;
        return false;
    }

    vertices.clear();
    std::cout << "Checking Polygon: ";
    vertices.push_back(Point2D(upperFace()[0].x, upperFace()[0].y));
    vertices.push_back(Point2D(upperFace()[1].x, upperFace()[1].y));
    double distance = Geom::distance3D(upperFace()[0], upperFace()[1]);
    std::cout << "distance: " << distance << std::endl;

    for (int i = 1; i >= 0; i--) {
        for (int j = 0; j < 4; j++) {
            std::cout << "Checking distance: " << Geom::distance3D(upperFace()[i], lowerFace()[j]) << std::endl;
            if (Geom::distance3D(upperFace()[i], lowerFace()[j]) == distance) {
                vertices.push_back(Point2D(lowerFace()[j].x, lowerFace()[j].y));
                break;
            }
        }
    }

    RegularConvexPolygon sideIn = RegularConvexPolygon(vertices);

    for (int i = 0; i < 4; i++) {
        std::cout << "(" << sideIn.vertices()[i].x << "," << sideIn.vertices()[i].y << "), ";
    }
    std::cout << std::endl;

    vertices.clear();
    std::cout << "Checking Polygon: ";
    vertices.push_back(Point2D(cube.upperFace()[0].x, cube.upperFace()[0].y));
    vertices.push_back(Point2D(cube.upperFace()[1].x, cube.upperFace()[1].y));
    distance = Geom::distance3D(cube.upperFace()[0], cube.upperFace()[1]);
    std::cout << "distance: " << distance << std::endl;

    for (int i = 1; i >= 0; i--) {
        for (int j = 0; j < 4; j++) {
            std::cout << "Checking distance: " << Geom::distance3D(cube.upperFace()[i], cube.lowerFace()[j]) << std::endl;
            if (Geom::distance3D(cube.upperFace()[i], cube.lowerFace()[j]) == distance) {
                vertices.push_back(Point2D(cube.lowerFace()[j].x, cube.lowerFace()[j].y));
                break;
            }
        }
    }

    RegularConvexPolygon sideOut = RegularConvexPolygon(vertices);

    for (int i = 0; i < 4; i++) {
        std::cout << "(" << sideOut.vertices()[i].x << "," << sideOut.vertices()[i].y << "), ";
    }
    std::cout << std::endl;

    if (!sideIn.containedWithin(sideOut)) {
        std::cout << "Intersection!" << std::endl;
        return false;
    }

    return true;
}

bool Cube::containedWithin(ReuleauxTetrahedron &rt) {
	Point3D *verts = rt.vertices();
    double distance = Geom::distance3D(verts[0], verts[1]);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (Geom::distance3D(upperFace()[i], verts[j]) > distance)
                return false;
            if (Geom::distance3D(lowerFace()[i], verts[j]) > distance)
                return false;
        }
    }
    return true;
}
// ---------------------------------------------------------------------------------------
// Salvador Galindo
// sagalind
// CMPS 109
// Professor David Harrison
// Lab 4
// Due 04/29/18
// ---------------------------------------------------------------------------------------

#include <iostream>
#include <vector>

#include "sphere.h"
#include "reuleauxtetrahedron.h"
#include "cube.h"
#include "geom.h"
#include "line.h"
#include "point.h"
#include "circle.h"
#include "polygon.h"

Sphere::Sphere(Point3D center, double radius) {
	setCenter(center);
	setRadius(radius);
}

Point3D Sphere::center() {
    return center_;
}

void Sphere::setCenter(Point3D center) {
    center_.x = center.x;
    center_.y = center.y;
    center_.z = center.z;
}

double Sphere::radius() {
    return radius_;
}

void Sphere::setRadius(double radius) {
    radius_ = radius;
}

bool Sphere::containedWithin(Sphere &sphere) {
	return Geom::distance3D(center(), sphere.center()) <= (sphere.radius() - radius());
}

bool Sphere::containedWithin(Cube &cube) {
    Circle circle = Circle(Point2D(center().x, center().z), radius());
    std::vector<Point2D> vertices;
    std::cout << "Checking Polygon: ";
    for (int i = 0; i < 4; i++) {
        vertices.push_back(Point2D(cube.upperFace()[i].x, cube.upperFace()[i].z));
        std::cout << "(" << vertices[i].x << "," << vertices[i].y << "), ";
    }
    std::cout << std::endl;
    RegularConvexPolygon top = RegularConvexPolygon(vertices);

    std::cout << "with Circle: (" << circle.center().x << "," << circle.center().y << ") " << circle.radius() << std::endl;
    
    if (!circle.containedWithin(top)) {
        std::cout << "Intersection!" << std::endl;
        return false;
    }

    vertices.clear();
    circle = Circle(Point2D(center().x, center().y), radius());
    std::cout << "Checking Polygon: ";
    vertices.push_back(Point2D(cube.upperFace()[0].x, cube.upperFace()[0].y));
    vertices.push_back(Point2D(cube.upperFace()[1].x, cube.upperFace()[1].y));
    double distance = Geom::distance3D(cube.upperFace()[0], cube.upperFace()[1]);
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

    RegularConvexPolygon side = RegularConvexPolygon(vertices);

    for (int i = 0; i < 4; i++) {
        std::cout << "(" << side.vertices()[i].x << "," << side.vertices()[i].y << "), ";
    }
    std::cout << std::endl;

    std::cout << "with Circle: (" << circle.center().x << "," << circle.center().y << ") " << circle.radius() << std::endl;

    if (!circle.containedWithin(side)) {
        std::cout << "Intersection!" << std::endl;
        return false;
    }

    return true;
}

bool Sphere::containedWithin(ReuleauxTetrahedron &rt) {
	Point3D *verts = rt.vertices();
    double distance = Geom::distance3D(verts[0], verts[1]);
    for (int i = 0; i < 4; i++) {
        //printf("Radius of circle at vertex: %f\n", distance);
        //printf("Distance from center to vertex: %f\n", Geom::distance(center(), verts[i]));
        //printf("Distance - radius = %f\n", distance - radius());
        if (Geom::distance3D(center(), verts[i]) > distance - radius())
            return false;
    }
    return true;
}
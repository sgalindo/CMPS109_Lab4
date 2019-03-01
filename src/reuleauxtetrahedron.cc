// ---------------------------------------------------------------------------------------
// Salvador Galindo
// sagalind
// CMPS 109
// Professor David Harrison
// Lab 4
// Due 04/29/18
// ---------------------------------------------------------------------------------------

#include <iostream>

#include "reuleauxtetrahedron.h"
#include "sphere.h"
#include "cube.h"
#include "geom.h"
#include "point.h"
#include "line.h"

ReuleauxTetrahedron::ReuleauxTetrahedron(Point3D vertices[4]) {
	for (int i = 0; i < 4; i++) {
        vertices_[i] = vertices[i];
    }
}

Point3D *ReuleauxTetrahedron::vertices() {
	return vertices_;
}

bool ReuleauxTetrahedron::containedWithin(Sphere &sphere) {
	double distance = Geom::distance3D(vertices_[0], vertices_[1]);
    //printf("Distances between vertices: %f\n", distance); 
    for (int i = 0; i < 4; i++) {
        //printf("Checking vertex (%f, %f)\n", vertices_[i].x, vertices_[i].y);
        if (Geom::distance3D(vertices_[i], sphere.center()) > sphere.radius())
            return false;
    }
    if (distance >= sphere.radius())
        return true;
    else {
        for (int i = 0; i < 4; i++) {
            if (Geom::distance3D(vertices_[i], sphere.center()) <= sphere.radius() - distance)
                return true;
        }
        return false;
    }
}

// CITATION ---------------------------------------------------------------------------------------------------------
// Function that checks if a point is inside a polygon.
// Taken from: https://wrf.ecse.rpi.edu//Research/Short_Notes/pnpoly.html
int pointInPoly(Point2D point, RegularConvexPolygon poly) {
    int i, j, c = 0;
    int nvert = poly.vertexCount();
    for (i = 0, j = nvert-1; i < nvert; j = i++) {
        if (((poly.vertices()[i].y > point.y) != (poly.vertices()[j].y > point.y)) &&
            (point.x < (poly.vertices()[j].x - poly.vertices()[i].x) * 
            (point.y - poly.vertices()[i].y) / (poly.vertices()[j].y - poly.vertices()[i].y) + poly.vertices()[i].x)) {
            c = !c;
        }
    }
    return c;
}
// END CITATION -----------------------------------------------------------------------------------------------------

static bool onSegment(Point2D p, Point2D q, Point2D r)
{
    return (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y));
}

bool ReuleauxTetrahedron::containedWithin(Cube &cube) {
    std::vector<Point2D> vertices;
    std::cout << "Checking Polygon: ";
    for (int i = 0; i < 4; i++) {
        vertices.push_back(Point2D(cube.upperFace()[i].x, cube.upperFace()[i].z));
        std::cout << "(" << vertices[i].x << "," << vertices[i].y << "), ";
    }
    std::cout << std::endl;
    RegularConvexPolygon top = RegularConvexPolygon(vertices);
    std::vector<Line> edges;
    int index;
    for (int i = 0; i < 4; i++) {
        (i < 3) ? index = i + 1 : index = 0;
        edges.push_back(Line(Point2D(top.vertices()[i].x, top.vertices()[i].y), 
            Point2D(top.vertices()[index].x, top.vertices()[index].y)));
    }

    bool onLine;
    bool inside;
    for (int i = 0; i < 4; i++) {
        inside = false;
        onLine = false;
        if (pointInPoly(vertices_[i], top))
            inside = true;
        for (int j = 0; j < 4; j++) {
            if (onSegment(edges[i].a, vertices_[i],  edges[i].b))
                onLine = true;
        }
        if (!inside && !onLine)
            return false;
    }

    double distance = Geom::distance(vertices_[0], vertices_[1]);

    Circle circles[3] = {
        Circle(Point2D(vertices_[0].x, vertices_[0].y), distance),
        Circle(Point2D(vertices_[1].x, vertices_[1].y), distance),
        Circle(Point2D(vertices_[2].x, vertices_[2].y), distance)
    };

    double cx = (vertices_[0].x + vertices_[1].x + vertices_[2].x) / 3;
    double cy = (vertices_[0].y + vertices_[1].y + vertices_[2].y) / 3;
    Point2D center = Point2D(cx, cy);
    bool intersectsAll;
    double dist;

    for (int i = 0; i < top.vertexCount(); i++) {
        intersectsAll = true;
        for (int j = 0; j < 3; j++) {
            if (!Geom::intersects(edges[i], circles[j])) {
                intersectsAll = false;
                break;
            }
        }
        if (intersectsAll == true) {
            printf("INTERSECTS ALL\n");
            dist = abs((center.y - edges[i].a.y) * (edges[i].b.x - edges[i].a.x) - 
                    (center.x -  edges[i].a.x) * (edges[i].b.y - edges[i].a.y)) / 
                        Geom::distance(edges[i].a, edges[i].b);
            printf("Line Distance: %f\n", dist);
            printf("Center distance: %f\n", Geom::distance(vertices_[0], center));
            if (dist < Geom::distance(vertices_[0], center))
                return false;
        } 
        else
            break;
    }

    vertices.clear();
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

    RegularConvexPolygon side = RegularConvexPolygon(vertices);
    
    edges.clear();
    for (int i = 0; i < 4; i++) {
        (i < 3) ? index = i + 1 : index = 0;
        edges.push_back(Line(Point2D(side.vertices()[i].x, side.vertices()[i].y), 
            Point2D(side.vertices()[index].x, side.vertices()[index].y)));
    }

    for (int i = 0; i < 4; i++) {
        inside = false;
        onLine = false;
        if (pointInPoly(vertices_[i], side))
            inside = true;
        for (int j = 0; j < 4; j++) {
            if (onSegment(edges[i].a, vertices_[i],  edges[i].b))
                onLine = true;
        }
        if (!inside && !onLine)
            return false;
    }

    distance = Geom::distance(vertices_[0], vertices_[1]);

    for (int i = 0; i < side.vertexCount(); i++) {
        intersectsAll = true;
        for (int j = 0; j < 3; j++) {
            if (!Geom::intersects(edges[i], circles[j])) {
                intersectsAll = false;
                break;
            }
        }
        if (intersectsAll == true) {
            printf("INTERSECTS ALL\n");
            dist = abs((center.y - edges[i].a.y) * (edges[i].b.x - edges[i].a.x) - 
                    (center.x -  edges[i].a.x) * (edges[i].b.y - edges[i].a.y)) / 
                        Geom::distance(edges[i].a, edges[i].b);
            printf("Line Distance: %f\n", dist);
            printf("Center distance: %f\n", Geom::distance(vertices_[0], center));
            if (dist < Geom::distance(vertices_[0], center))
                return false;
        } 
        else
            break;
    }
    
    return true;
}

bool ReuleauxTetrahedron::containedWithin(ReuleauxTetrahedron &rt) {
	Point3D *verts = rt.vertices();
    double innerDistance = Geom::distance3D(vertices_[0], vertices_[1]);
    double outerDistance = Geom::distance3D(verts[0], verts[1]);
    bool circleInside;
    for (int i = 0; i < 4; i++) {
        circleInside = false;
        for (int j = 0; j < 4; j++) {
            if (Geom::distance3D(vertices_[i], verts[j]) > outerDistance)
                return false;
        }
        for (int k = 0; k < 4; k++) {
            if (Geom::distance3D(vertices_[i], verts[k]) > outerDistance - innerDistance)
                circleInside = true;
        }
        if (!circleInside)
            return false;
    } 
    return true;
}
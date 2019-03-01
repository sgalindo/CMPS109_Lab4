// ---------------------------------------------------------------------------------------
// Salvador Galindo
// sagalind
// CMPS 109
// Professor David Harrison
// Lab 4
// Due 04/29/18
// ---------------------------------------------------------------------------------------

#include <iostream>

#include "reuleauxtriangle.h"
#include "circle.h"
#include "polygon.h"
#include "geom.h"
#include "line.h"
#include "point.h"

ReuleauxTriangle::ReuleauxTriangle(Point2D vertices[3]) {
    for (int i = 0; i < 3; i++) {
        vertices_[i] = vertices[i];
    }
}

Point2D *ReuleauxTriangle::vertices() {
    return vertices_;
}

bool ReuleauxTriangle::containedWithin(Circle &circle) {
    double distance = Geom::distance(vertices_[0], vertices_[1]);
    //printf("Distances between vertices: %f\n", distance); 
    for (int i = 0; i < 3; i++) {
        //printf("Checking vertex (%f, %f)\n", vertices_[i].x, vertices_[i].y);
        if (Geom::distance(vertices_[i], circle.center()) > circle.radius())
            return false;
    }
    if (distance >= circle.radius())
        return true;
    else {
        for (int i = 0; i < 3; i++) {
            if (Geom::distance(vertices_[i], circle.center()) <= circle.radius() - distance)
                return true;
        }
        return false;
    }
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
// From geom.cc
static bool onSegment(Point2D p, Point2D q, Point2D r)
{
    return (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y));
}

/*double lineDistance(const &Line line, const &Point2D point) {
    return abs((point.y - line.a.y) * (line.b.x - line.a.x) - 
        (point.x -  line.a.x) * (line.b.y - line.a.y)) / 
            Geom::distance(line.a, line.b);
}*/

bool ReuleauxTriangle::containedWithin(RegularConvexPolygon &polygon) {
    std::vector<Point2D> verts = polygon.vertices();
    std::vector<Line> lines = polygon.edges();
    for (int k = 0; k < 3; k++) {
        // CITATION ---------------------------------------------------------------------------------------------------------
        // Function that checks if a point is inside a polygon.
        // Taken from: https://wrf.ecse.rpi.edu//Research/Short_Notes/pnpoly.html
        int i, j, c = 0; 
        int nvert = polygon.vertexCount();
        bool onLine = false;
        for (i = 0, j = nvert-1; i < nvert; j = i++) {
            if (((verts[i].y > vertices_[k].y) != (verts[j].y > vertices_[k].y)) &&
                (vertices_[k].x < (verts[j].x - verts[i].x) * 
                (vertices_[k].y - verts[i].y) / (verts[j].y - verts[i].y) + verts[i].x)) {
                c = !c;
            }
        // END CITATION -------------------------------------------------------------------------------------------------------
            if (onSegment(lines[i].a, vertices_[k], lines[i].b))
                onLine = true;
        }
        if (c == 0 && !onLine)
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

    for (int i = 0; i < polygon.vertexCount(); i++) {
        intersectsAll = true;
        for (int j = 0; j < 3; j++) {
            if (!Geom::intersects(lines[i], circles[j])) {
                intersectsAll = false;
                break;
            }
        }
        if (intersectsAll == true) {
            printf("INTERSECTS ALL\n");
            dist = abs((center.y - lines[i].a.y) * (lines[i].b.x - lines[i].a.x) - 
                    (center.x -  lines[i].a.x) * (lines[i].b.y - lines[i].a.y)) / 
                        Geom::distance(lines[i].a, lines[i].b);
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

bool ReuleauxTriangle::containedWithin(ReuleauxTriangle &rt) {
    Point2D *verts = rt.vertices();
    double innerDistance = Geom::distance(vertices_[0], vertices_[1]);
    double outerDistance = Geom::distance(verts[0], verts[1]);
    bool circleInside;
    for (int i = 0; i < 3; i++) {
        circleInside = false;
        for (int j = 0; j < 3; j++) {
            if (Geom::distance(vertices_[i], verts[j]) > outerDistance)
                return false;
        }
        for (int k = 0; k < 3; k++) {
            if (Geom::distance(vertices_[i], verts[k]) > outerDistance - innerDistance)
                circleInside = true;
        }
        if (!circleInside)
            return false;
    } 
    return true;
}

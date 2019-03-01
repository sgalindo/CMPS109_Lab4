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

#include "polygon.h"
#include "geom.h"
#include "reuleauxtriangle.h"

RegularConvexPolygon::RegularConvexPolygon(std::vector<Point2D> vertices) {
    vertices_ = vertices;
}

std::vector<Point2D> RegularConvexPolygon::vertices() {
    return vertices_;
}

std::vector<Line> RegularConvexPolygon::edges() {
    std::vector<Line> edges;
    for (unsigned int i = 0; i < vertices_.size()-1; i++) 
        edges.push_back(Line(vertices_[i],vertices_[i+1]));
    edges.push_back(Line(vertices_[vertices_.size()-1],vertices_[0]));
    return edges;
}

int RegularConvexPolygon::vertexCount() {
    return vertices_.size();
}

bool RegularConvexPolygon::containedWithin(Circle &circle) {
    // CITATION -----------------------------------------------------------------------------------------------------
    // Code by Professor David Harrison, UCSC
    // https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=956436f9-b748-4ab7-b7b7-2e27605f1667
    for (Point2D const &vertex : vertices()) {
        if (Geom::distance(vertex, circle.center()) > circle.radius())
            return false;
    }
    return true;
    // END CITATION ------------------------------------------------------------------------------------------------
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
// Taken from geom.cc
static bool onSegment(Point2D p, Point2D q, Point2D r)
{
    return (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y));
}

bool RegularConvexPolygon::containedWithin(RegularConvexPolygon &polygon) {
    std::vector<Point2D> verts = polygon.vertices();
    std::vector<Line> lines = polygon.edges();
    for (Point2D const &vertex : vertices()) {
        // CITATION ---------------------------------------------------------------------------------------------------------
        // Function that checks if a point is inside a polygon.
        // Taken from: https://wrf.ecse.rpi.edu//Research/Short_Notes/pnpoly.html
        int i, j, c = 0; 
        int nvert = polygon.vertexCount();
        bool onLine = false;
        for (i = 0, j = nvert-1; i < nvert; j = i++) {
            if (((verts[i].y > vertex.y) != (verts[j].y > vertex.y)) &&
                (vertex.x < (verts[j].x - verts[i].x) * 
                (vertex.y - verts[i].y) / (verts[j].y - verts[i].y) + verts[i].x)) {
                c = !c;
            }
        // END CITATION -------------------------------------------------------------------------------------------------------
            if (onSegment(lines[i].a, vertex, lines[i].b))
                onLine = true;
        }
        if (c == 0 && !onLine)
            return false;
    }
    return true;
}

bool RegularConvexPolygon::containedWithin(ReuleauxTriangle &rt) {
    Point2D *verts = rt.vertices();
    double distance = Geom::distance(verts[0], verts[1]);
    //printf("Radii of circles of reuleaux triangle: %f\n", distance);
    for (Point2D const &vertex : vertices()) {
        //printf("Checking vertex (%f, %f)...\n", vertex.x, vertex.y);
        for (int i = 0; i < 3; i++) {
            if (Geom::distance(vertex, verts[i]) > distance)
                return false;
        }
        //printf("...vertex inside all three reuleaux triangle circles.\n");
    }
    return true;
}

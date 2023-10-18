#define _USE_MATH_DEFINES
#include <iostream>

#include "xprop/section.h"


#include <cmath>
int main() {

    double width = 10;
    double depth = 20;

    std::vector<Point> points;
    points.emplace_back(0.0, 0.0);
    points.emplace_back(width, 0.0);
    points.emplace_back(width, depth);
    points.emplace_back(0.0, depth);
    points.emplace_back(0.0, 0.0);

    Section XS(points);
    XS.computeProperties();

    std::cout << "Area  = " << XS.area() << std::endl;
    std::cout << "Ix    = " << XS.Ix() << std::endl;
    std::cout << "Iy    = " << XS.Iy() << std::endl;
    std::cout << "Ixy   = " << XS.Ixy() << std::endl;

    auto [area, centroid, Q, I] = XS.computeProperties(M_PI_4, 5);
    std::cout << "Cut Section " << std::endl;
    std::cout << "Area  = " << area << std::endl;
    std::cout << "Centroid    = \n" << centroid << std::endl;
    std::cout << "Q    = \n" << Q << std::endl;
    std::cout << "I   = \n" << I << std::endl;
}
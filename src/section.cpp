#include <iostream>

#include "xprop/section.h"

Section::Section() : m_area(0.0), m_sides(0){
    m_centroid.reSize(2, 1);
    m_firstMoment.reSize(2,1);
    m_inertia.reSize(2, 2);
}

Section::Section(const std::vector<Point> &points) : m_points(points), m_area(0.0){
    m_sides = m_points.size() - 1;
    m_centroid.reSize(2, 1);
    m_firstMoment.reSize(2,1);
    m_inertia.reSize(2, 2);

}

void Section::computeProperties() {
    // Reset variables
    m_area = 0;
    m_firstMoment.reSize(2,1);
    m_inertia.reSize(2,2);

    Matrix xi(2,1);             // Vector xi
    Matrix xj(2,1);             // Vector xj
    double dArea;
    for (size_t i = 0; i < m_sides; i++)
    {
        // Extract vectors at start and end points of each side
        xi(1,1) = m_points[i].x();
        xi(2,1) = m_points[i].y();
        xj(1,1) = m_points[i+1].x();
        xj(2,1) = m_points[i+1].y();

        // Compute area, first moment of area and moment of inertia of each triangle
        dArea = (xi(1,1) * xj(2,1) - xi(2,1) * xj(1,1)) * 0.5;
        m_area += dArea;
        m_firstMoment += (xi + xj).scale(dArea/3.0);
        m_inertia += ((xi * xi.transpose()).scale(2.0) + xi * xj.transpose() + xj * xi.transpose() + (xj * xj.transpose()).scale(2.0)).scale(dArea/12.0);
    }
    m_centroid = m_firstMoment.scale(1.0/m_area);
    m_inertia -= (m_centroid * m_centroid.transpose()).scale(m_area);
}

double Section::area() const {
    return m_area;
}

double Section::Ix() const {
    return m_inertia(2,2);
}

double Section::Iy() const {
    return m_inertia(1,1);
}

double Section::Ixy() const {
    return m_inertia(1,2);
}

std::tuple<double, Matrix, Matrix, Matrix>
Section::computeProperties(double theta, double distance) {
    m_cutPoints.clear();
    double eps = 1e-8;
    Matrix m(2,1);
    double projection;
    double zeta = 0.0;
    Matrix xi(2,1), xj(2,1);
    // Construct normal vector to cut section
    Matrix n(2,1);
    n(1,1) = cos(theta);
    n(2,1) = sin(theta);
    // Distance varies from -1 to +1 and needs to be converted to actual dimension
    double minDistance = getMinimumDistance(theta);
    double maxDistance = getMaximumDistance(theta);
    distance = distance * (maxDistance + minDistance) * 0.5 + (maxDistance + minDistance) * 0.5 - minDistance;
    for (size_t i = 0; i < m_sides; i++)
    {
        // Position vector for start and end point of polygon edge
        xi(1,1) = m_points[i].x();
        xi(2,1) = m_points[i].y();
        xj(1,1) = m_points[i+1].x();
        xj(2,1) = m_points[i+1].y();
        // direction vector of the edge
        m = xj - xi;
        // Fraction Distance for intersection point along the edge
        if (((xj - xi).transpose()*n)(1,1) != 0)
            zeta = (((m_centroid - xi).transpose()*n)(1,1) + distance) / ((xj - xi).transpose() * n)(1, 1);
        else
            zeta = 0;
        m_cutPoints.emplace_back(xi(1,1), xi(2,1));
        if (zeta > 0.0 && zeta < 1.0)
            m_cutPoints.emplace_back((xi + m.scale(zeta))(1,1), (xi + m.scale(zeta))(2,1));
    }
    // Trim points below the cut line
    std::erase_if(m_cutPoints, [&](Point pt){
        // Position vector for start point of polygon edge
        xi(1,1) = pt.x();
        xi(2,1) = pt.y();
        // Projection of each point along normal vector to cut line
        projection = ((xi - (m_centroid + n.scale(distance))).transpose() * n)(1, 1) + eps;
        return projection < 0;
    });
    // Close the polygon
    m_cutPoints.push_back(m_cutPoints[0]);
    // Calculate section properties of cut section
    double dArea;
    double area = 0;
    Matrix firstMoment(2,1), centroid(2,1), inertia(2,2);
    size_t sides = m_cutPoints.size() - 1;
    for (size_t i = 0; i < sides; i++)
    {
        // Extract vectors at start and end points of each side
        xi(1,1) = m_cutPoints[i].x();
        xi(2,1) = m_cutPoints[i].y();
        xj(1,1) = m_cutPoints[i+1].x();
        xj(2,1) = m_cutPoints[i+1].y();

        // Compute area, first moment of area and moment of inertia of each triangle
        dArea = (xi(1,1) * xj(2,1) - xi(2,1) * xj(1,1)) * 0.5;
        area += dArea;
        firstMoment += (xi + xj).scale(dArea/3.0);
        inertia += ((xi * xi.transpose()).scale(2.0) + xi * xj.transpose() + xj * xi.transpose() + (xj * xj.transpose()).scale(2.0)).scale(dArea/12.0);
    }
    centroid = firstMoment.scale(1.0/area);
    inertia -= (centroid * centroid.transpose()).scale(area);
    if (area == 0) {
        centroid(1,1) = 0.0;
        centroid(2,1) = 0.0;
        firstMoment(1,1) = 0.0;
        firstMoment(2,1) = 0.0;
        inertia(1,1) = 0.0; inertia(1,2) = 0.0;
        inertia(2,1) = 0.0; inertia(2,2) = 0.0;
    }
    return std::make_tuple(area, centroid, firstMoment, inertia);
}

Matrix Section::centroid() const {
    return m_centroid;
}
Matrix Section::firstMoment() const {
    return m_firstMoment;
}
std::vector<Point> Section::getPoints() const {
    return m_points;
}
std::vector<Point> Section::getCutPoints() const {
    return m_cutPoints;
}
void Section::setCoordinates(std::vector<Point>& points) {
    m_points = points;
    m_sides = m_points.size() - 1;
}
double Section::getMaximumDistance(double theta) {
    // Construct normal vector to cut section
    Matrix n(2,1);
    n(1,1) = cos(theta);
    n(2,1) = sin(theta);
    // Max distance from centroid to polygon coordinate
    Matrix k1(2,1), k2(2,1);
    auto maxPoint = std::max_element(std::begin(m_points), std::end(m_points), [this, &n, &k1, &k2](const Point& p1, const Point& p2){
        k1(1,1) = p1.x() - m_centroid(1,1);
        k1(2,1) = p1.y() - m_centroid(2,1);
        k2(1,1) = p2.x() - m_centroid(1,1);
        k2(2,1) = p2.y() - m_centroid(2,1);
        return (k1.transpose()*n)(1,1) < (k2.transpose()*n)(1,1) ;
    });
    k1(1,1) = maxPoint->x() - m_centroid(1,1);
    k1(2,1) = maxPoint->y() - m_centroid(2,1);
    return (k1.transpose()*n)(1,1);
}

double Section::getMinimumDistance(double theta) {
    // Construct normal vector to cut section
    Matrix n(2,1);
    n(1,1) = cos(theta);
    n(2,1) = sin(theta);
    // Max distance from centroid to polygon coordinate
    Matrix k1(2,1), k2(2,1);
    auto minPoint = std::min_element(std::begin(m_points), std::end(m_points), [this, &n, &k1, &k2](const Point& p1, const Point& p2){
        k1(1,1) = p1.x() - m_centroid(1,1);
        k1(2,1) = p1.y() - m_centroid(2,1);
        k2(1,1) = p2.x() - m_centroid(1,1);
        k2(2,1) = p2.y() - m_centroid(2,1);
        return (k1.transpose()*n)(1,1) > (k2.transpose()*n)(1,1) ;
    });
    k1(1,1) = minPoint->x() - m_centroid(1,1);
    k1(2,1) = minPoint->y() - m_centroid(2,1);
    return (k1.transpose()*n)(1,1);
}
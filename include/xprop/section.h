#pragma once
#include <vector>
#include <algorithm>

#include "point.h"
#include "matrix.h"

class Section
{
private:
    std::vector<Point> m_points;
    std::vector<Point> m_cutPoints;
    size_t m_sides;
    double m_area;
    Matrix m_firstMoment;
    Matrix m_centroid;
    Matrix m_inertia;
public:
    // Constructor
    Section();
    explicit Section(const std::vector<Point> &points);

    // Compute Section Properties
    void computeProperties();

    // Compute First Moment of Area -> returns section properties in following order :
    // Area, Centroid, First Moment of Inertia, Second Moment of Inertia
    // theta - angle between the x-axis and the cut-line (in radians)
    // distance - perpendicular distance measured between the cut line and centroid of the section
    std::tuple<double, Matrix, Matrix, Matrix> computeProperties(double theta, double distance);

    // Find maximum/minimum distance from centroid to the polygon when it is rotated with a direction vector (n)
    // n -> unit vector originating from centroid with an angle with respect to x-axis.
    double getMaximumDistance(double theta);
    double getMinimumDistance(double theta);

    // Getters
    [[nodiscard]] double area() const;
    [[nodiscard]] Matrix centroid() const;
    [[nodiscard]] Matrix firstMoment() const;
    [[nodiscard]] double Ix() const;
    [[nodiscard]] double Iy() const;
    [[nodiscard]] double Ixy() const;
    [[nodiscard]] std::vector<Point> getPoints() const;
    [[nodiscard]] std::vector<Point> getCutPoints() const;

    // Setters
    void setCoordinates(std::vector<Point>& points);

};


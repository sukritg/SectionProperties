#pragma once

class Point
{
private:
    double m_x, m_y, m_z;
public:
    // Constructor
    Point();
    explicit Point(double x, double y = 0 , double z = 0);

    // Getters
    [[nodiscard]] double x() const;
    [[nodiscard]] double y() const;
    [[nodiscard]] double z() const;

    // Setters
    void setX(double x);
    void setY(double y);
    void setZ(double z);

};
#include "xprop/point.h"

Point::Point() : m_x(0.0), m_y(0.0), m_z(0.0) {

}

Point::Point(double x, double y, double z) :m_x(x), m_y(y), m_z(z) {

}

double Point::x() const {
    return m_x;
}

double Point::y() const {
    return m_y;
}

double Point::z() const {
    return m_z;
}
void Point::setX(double x) {
    m_x = x;
}
void Point::setY(double y) {
    m_y = y;
}
void Point::setZ(double z) {
    m_z = z;
}

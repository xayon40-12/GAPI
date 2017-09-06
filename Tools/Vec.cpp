//
//  Vec.cpp
//  RayTracer
//
//  Created by Nathan Touroux on 03/03/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Vec.hpp"

Vec::Vec(): _x(0), _y(0), _z(0), plan(false) {}

Vec::Vec(double x, double y): _x(x), _y(y), _z(0), plan(true) {}
Vec::Vec(double x, double y, double z): _x(x), _y(y), _z(z), plan(false) {}
Vec::Vec(Vec const & v): _x(v.x()), _y(v.y()), _z(v.z()), plan(v.plan){}

Vec Vec::operator+(Vec const & v){ return Vec(_x+v.x(), _y+v.y(), _z+v.z()); }
void Vec::operator+=(Vec const & v){
    _x += v.x();
    _y += v.y();
    _z += v.z();
}
Vec Vec::operator-(Vec const & v){ return Vec(_x-v.x(), _y-v.y(), _z-v.z()); }
Vec Vec::operator-(){
    return -1*(*this);
}
void Vec::operator-=(Vec const & v){
    _x -= v.x();
    _y -= v.y();
    _z -= v.z();
}
double Vec::operator*(Vec const & v){ return _x*v.x() + _y*v.y() + _z*v.z(); }
Vec Vec::operator|(Vec const & v){ return Vec(_x*v.x(), _y*v.y(), _z*v.z()); }
Vec Vec::operator^(Vec const & v){
    return Vec(_y*v.z() - _z*v.y(), _z*v.x() - _x*v.z(), _x*v.y() - _y*v.x());
}

bool Vec::operator==(Vec const & v){ return (_x == v.x()) && (_y == v.y()) && (_z == v.z()); }
bool Vec::operator!=(Vec const & v){ return !(*this == v); }

double Vec::norm(){ return sqrt(_x*_x + _y*_y + _z*_z); }
double Vec::squareNorm(){ return _x*_x + _y*_y + _z*_z; }
void Vec::normalize(){
    double n = norm();
    if(n == 0)return;
    _x /= n;
    _y /= n;
    _z /= n;
}
Vec Vec::normalized(){
    Vec v(*this);
    v.normalize();
    return v;
}
Vec Vec::rotate(Vec axe, double angle){
    Vec U(*this);
    double c = cos(angle), s = sin(angle);
    axe.normalize();
    return c*U + (1-c)*(U*axe)*axe + s*(axe^U);
}

double Vec::x() const{ return _x; }
double Vec::y() const{ return _y; }
double Vec::z() const{ return _z; }

bool Vec::isPlanar() const{return plan;}

Vec operator*(double d, Vec const & v){
    if(v.isPlanar())
        return Vec(d*v.x(), d*v.y());
    else
        return Vec(d*v.x(), d*v.y(), d*v.z());
}

std::ostream &operator<<(std::ostream & os, Vec const & v){
    if(v.isPlanar())
        return os << "(" << v.x() << ", " << v.y() << ")";
    else
        return os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
}

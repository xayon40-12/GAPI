//
//  Vec.h
//  RayTracer
//
//  Created by Nathan Touroux on 03/03/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Vec_h
#define Vec_h

#include <ostream>
#include <cmath>

class Vec{
private:
    double _x, _y, _z;
    bool plan;
public:
    Vec();
    Vec(double x, double y);
    Vec(double x, double y, double z);
    Vec(Vec const & v);
    
    Vec operator+(Vec const & v);
    void operator+=(Vec const & v);
    Vec operator-(Vec const & v);
    Vec operator-();
    void operator-=(Vec const & v);
    double operator*(Vec const & v);
    Vec operator|(Vec const & v);
    Vec operator^(Vec const & v);
    
    bool operator==(Vec const & v);
    bool operator!=(Vec const & v);
    
    double norm();
    double squareNorm();
    void normalize();
    Vec normalized();
    
    Vec rotate(Vec axe, double angle);
    
    double x() const;
    double y() const;
    double z() const;
    
    bool isPlanar() const;
};

Vec operator*(double d, Vec const & v);

std::ostream &operator<<(std::ostream & os, Vec const & v);

#endif /* Vec_h */

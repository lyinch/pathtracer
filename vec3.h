//
// Created by Thierry Backes on 20/10/18.
//

#ifndef PATHTRACER_VEC3_H
#define PATHTRACER_VEC3_H

#include <cmath>
#include <ostream>

class Vec3{
public:
    Vec3(){ x = y = z = 0;};
    Vec3(float x, float y, float z):x(x),y(y),z(z){};
    float x,y,z;

    inline void normalize();
    inline float length() const{return sqrtf(x*x+y*y+z*z);}
    inline float squared_length() const{return x*x+y*y+z*z;}

    inline Vec3& operator+= (const Vec3 &v);
    inline Vec3& operator/= (float f);

};

/** Function Implementations **/
inline void Vec3::normalize(){
    float k = squared_length();
    x /= k;
    y /= k;
    z /= k;
}

/** Operator Overloading **/
inline Vec3 operator/ (const Vec3 &vec1,float t ){
    return {vec1.x/t,vec1.y/t,vec1.z/t};
}

inline Vec3 operator*(float t, const Vec3 &v1){
    return {t*v1.x,t*v1.y,t*v1.z};
}

inline Vec3 operator*(const Vec3 &v1,float t ){
    return t*v1;
}

inline Vec3 operator+ (const Vec3 &v1, const Vec3 &v2){
    return {v1.x+v2.x,v1.y+v2.y,v1.z+v2.z};
}

inline Vec3 operator- (const Vec3 &v1, const Vec3 &v2){
    return {v1.x-v2.x,v1.y-v2.y,v1.z-v2.z};
}

inline Vec3 operator* (const Vec3 &v1, const Vec3 &v2){
    return {v1.x*v2.x,v1.y*v2.y,v1.z*v2.z};
}

inline Vec3 operator+ (const Vec3 &v1,float t){
    return {v1.x+t,v1.y+t,v1.z+t};
}

inline float dot(const Vec3 &v1, const Vec3 &v2){
    return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}


Vec3 &Vec3::operator+=(const Vec3 &v) {
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    return *this;
}


Vec3 &Vec3::operator/=(float f) {
    this->x /= f;
    this->y /= f;
    this->z /= f;
    return *this;
}


/** Global Functions **/
inline Vec3 normalized(Vec3 v){
    return v/v.length();
}

std::ostream &operator<< (std::ostream &os, Vec3 const &v) {
    os << "(" << v.x << "," << v.y << "," << v.z << ")";
    return os;
}

inline Vec3 sqrtvec(Vec3 v){
    return {sqrtf(v.x),sqrtf(v.y),sqrtf(v.z)};
}

#endif //PATHTRACER_VEC3_H

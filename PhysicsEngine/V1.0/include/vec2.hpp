#pragma once

#include <cmath>
#include <iostream>


#define EPSILON 1e-10
const double PI = 3.14159265358979323846;

class vec2{
public:
    double x;
    double y;

    vec2() : x(0), y(0) {} // Default constructor
    vec2(double x_init, double y_init) : x(x_init), y(y_init) {
        // Constructor initializes x and y
    }
    ~vec2(){}


    double dot(vec2 other){
        return x*other.x + y*other.y;
    }

    double norm(){
        return x*x+y*y;
    }

    double length(){
        return sqrt(norm());
    }

    // normalizes the vector
    void normalize(){
        double mag = length();
        if(mag > EPSILON){
            x = x/mag;
            y = y/mag;
        }else{
            x = 0;
            y = 0;
        }
    }

    // returns the normalized version of the vector
    vec2 normalized(){
        double mag = length();
        if(mag > EPSILON){
            return vec2(x/mag, y/mag);
        }else{
            return vec2(0,0);
        }
    }

    bool eq(const vec2 other) const {
        return (fabs(x-other.x) <= EPSILON) && (fabs(y-other.y) <= EPSILON);
    }

    bool orth(vec2 other){
        return fabs(dot(other)) <= EPSILON;
    }

    void print(int vertical=0){
        if(vertical){
            std::cout << "[" << x << "]" << std::endl;
            std::cout << "[" << y << "]" << std::endl;
        }else{
            std::cout << "[" << x << ", " << y << "]" << std::endl; 
        }
    }

    vec2 operator+(const vec2 other) const {
        return vec2(x + other.x, y + other.y);
    }

    vec2 operator-(const vec2 other) const {
        return vec2(x - other.x, y - other.y);
    }

    void operator+=(const vec2 other){
        x += other.x;
        y += other.y;
    }

    void operator-=(const vec2 other){
        x -= other.x;
        y -= other.y;
    }

    vec2 operator*(double s) const {
        return vec2(x * s, y * s);
    }

    void operator*=(double s){
        x = x*s;
        y = y*s;
    }

    bool operator==(const vec2 other) const {
        return eq(other);
    }

    bool operator!=(const vec2 other) const {
        return !eq(other);
    }

    void operator=(const vec2 other){
        x = other.x;
        y = other.y;
    }

};

vec2 rotatePoint(vec2 point, double theta) {
    double rad = theta * PI / 180.0; // Convert degrees to radians
    double cosTheta = cos(rad);
    double sinTheta = sin(rad);
    return vec2(
        point.x * cosTheta - point.y * sinTheta,
        point.x * sinTheta + point.y * cosTheta
    );
}

double distSq(vec2 A, vec2 B){
    return ((A.x-B.x)*(A.x-B.x) + (A.y-B.y)*(A.y-B.y));
}

double dist(vec2 A, vec2 B){
    return sqrt((A.x-B.x)*(A.x-B.x) + (A.y-B.y)*(A.y-B.y));
}
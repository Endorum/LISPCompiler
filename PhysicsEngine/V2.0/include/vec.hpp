#pragma once

#include <cmath>
#include <iostream>
#include <array>

#define EPSILON 1e-10
const double PI = 3.14159265358979323846;

class vec2{
public:
    double x;
    double y;

    vec2() : x(0), y(0) {} // Default constructor
    vec2(double x_init, double y_init) : x(x_init), y(y_init) {}
    ~vec2(){}

    constexpr vec2& operator+= (const vec2& other){
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr vec2& operator-= (const vec2& other){
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr vec2& operator*= (const vec2& other){
        x *= other.x;
        y *= other.y;
        return *this;
    }

    constexpr vec2& operator/= (const vec2& other){
        x /= other.x;
        y /= other.y;
        return *this;
    }

    constexpr vec2& operator+= (const double value){
        x += value;
        y += value;
        return *this;
    }

    constexpr vec2& operator-= (const double value){
        x -= value;
        y -= value;
        return *this;
    }

    constexpr vec2& operator*= (const double value){
        x *= value;
        y *= value;
        return *this;
    }

    constexpr vec2& operator/= (const double value){
        x /= value;
        y /= value;
        return *this;
    }

    vec2 operator- (const vec2& other) const {
        return vec2(x-other.x, y-other.y);
    }

    vec2 operator+ (const vec2& other) const {
        return vec2(x+other.x, y+other.y);
    }

    vec2 operator* (const vec2& other) const {
        return vec2(x*other.x, y*other.y);
    }

    vec2 operator/ (const vec2& other) const {
        return vec2(x/other.x, y/other.y);
    }

    vec2 operator+ (const double value) const {
        return vec2(x+value, y+value);
    }

    vec2 operator* (const double value) const {
        return vec2(x*value, y*value);
    }

    vec2 operator/ (const double value) const {
        return vec2(x/value, y/value);
    }

    constexpr bool operator== (const vec2& other){
        return (fabs(x-other.x) <= EPSILON) && (fabs(y-other.y) <= EPSILON);
    }

    // constexpr bool operator!= (const vec2& other){
    //     return !(other == *this);
    // }

    constexpr double dot (const vec2& other) const {
        return x * other.x + y * other.y;
    }

    constexpr double mag() const {
        return sqrt(x*x + y*y);
    }

    constexpr double mag_sqr() const {
        return (x*x + y*y);
    }

    vec2 absolute() const {
        return vec2(abs(x), abs(y));
    }

    constexpr double distance(const vec2& other) const {
        return sqrt((x-other.x) * (x-other.x) + (y-other.y)*(y-other.y));
    }

    constexpr double sqr_distance(const vec2& other) const {
        return ((x-other.x) * (x-other.x) + (y-other.y)*(y-other.y));
    }

    vec2 normalized() const {
        double len = mag();
        return vec2(x / len, y / len);
    }

    double cross(const vec2& other) const {
        return x * other.y - y * other.x;
    }

    vec2 perpendicular() const {
        return vec2(y, -x);
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

std::pair<double, double> project_vertices(const std::array<vec2, 4>& vertices, const vec2& axis) {
    double min_proj = axis.x * vertices[0].x + axis.y * vertices[0].y;
    double max_proj = min_proj;

    for (int i = 1; i < 4; ++i) {
        double projection = axis.x * vertices[i].x + axis.y * vertices[i].y;
        min_proj = std::min(min_proj, projection);
        max_proj = std::max(max_proj, projection);
    }

    return {min_proj, max_proj};
}

bool line_intersection(const vec2& A, const vec2& B, const vec2& C, const vec2& D, vec2& intersection) {
    vec2 AB = B - A;
    vec2 CD = D - C;
    vec2 AC = C - A;

    double denominator = AB.cross(CD);
    if (denominator == 0.0) {
        return false; // Lines are parallel or coincident
    }

    double t = AC.cross(CD) / denominator;
    double u = AC.cross(AB) / denominator;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        intersection = A + AB * t;
        return true;
    }

    return false; // Intersection is out of bounds of the segments
}
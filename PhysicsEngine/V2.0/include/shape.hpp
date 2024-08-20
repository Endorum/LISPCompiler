#pragma once

#include <SDL2/SDL.h>

#include "vec.hpp"
#include "draw.hpp"

typedef enum {
    ST_RECT,
    ST_CIRC
}ShapeType;

class Shape{
public:
    vec2 center;
    double angle;

    ShapeType type;


    Shape(const vec2& c, double a = 0.0) : center(c), angle(a) {}
    virtual ~Shape() = default; // Virtual destructor for proper cleanup

    virtual double area() const = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
    virtual double get_MOI(double mass, vec2 axis=vec2()) const = 0;

};

class Rect : public Shape{
public:
    double width;
    double height;

    vec2 A,B,C,D;

    Rect(const vec2& c, double w, double h, double a = 0.0)
        : Shape(c, a), width(w), height(h) {
            type = ST_RECT;
            update_ABCD();
        }

    double area() const override {
        return width * height;
    }

    void draw(SDL_Renderer* renderer) override {
        update_ABCD();
        drawRect(renderer, center, A, B, C, D);
    }

    double get_MOI(double mass, vec2 axis=vec2()) const override {
        double I_center = (1/12.0) * mass * ( (width*width) + (height*height) );
        if(axis==center){
            return I_center;
        }else{
            // around an arbitrary axis
            return I_center + mass * center.sqr_distance(axis);
        }
    }

    void update_ABCD() {
        A = vec2(-width / 2,-height / 2);
        B = vec2( width / 2,-height / 2);
        C = vec2( width / 2, height / 2);
        D = vec2(-width / 2, height / 2);

        if(angle >= EPSILON){
            A = rotatePoint(A, angle);
            B = rotatePoint(B, angle);
            C = rotatePoint(C, angle);
            D = rotatePoint(D, angle);
        }

        A += center;
        B += center;
        C += center;
        D += center;
    }

    vec2 get_highest_corner() {
        // Initialize the highest point as A
        vec2 highest = A;
        if (B.y < highest.y) highest = B;
        if (C.y < highest.y) highest = C;
        if (D.y < highest.y) highest = D;

        return highest;
    }

    vec2 get_lowest_corner() {
        // Initialize the highest point as A
        vec2 lowest = A;
        if (B.y > lowest.y) lowest = B;
        if (C.y > lowest.y) lowest = C;
        if (D.y > lowest.y) lowest = D;

        return lowest;
    }


};

class Circle : public Shape{
public:
    double radius;

    Circle(const vec2& c, double r, double a = 0.0)
        : Shape(c, a), radius(r) {
            type = ST_CIRC;
        }

    double area() const override {
        return radius*radius*PI;
    }

    void draw(SDL_Renderer* renderer) override{
        drawCircle(renderer, center, radius, angle);
    }

    double get_MOI(double mass, vec2 axis=vec2()) const override {
        double I_center = (mass / 2.0) * pow(radius,2);
        

        if(axis==center){
            return I_center;
        }else{
            // around an arbitrary axis
            return I_center + mass * center.sqr_distance(axis);
        }
    }
};
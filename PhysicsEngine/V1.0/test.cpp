#include <cmath>
#include <iostream>
#include <memory>

#include "include/vec2.hpp"


/*
Testing inheritance with shape
*/


class Shape{
public:
    vec2 center;
    double angle;

    Shape(const vec2& c, double a = 0.0) : center(c), angle(a) {}
    virtual ~Shape() = default; // Virtual destructor for proper cleanup

    virtual double area() const = 0;

};

class Rect : public Shape{
public:
    double width;
    double height;

    Rect(const vec2& c, double w, double h, double a = 0.0)
        : Shape(c, a), width(w), height(h) {}

    double area() const override {
        return width * height;
    }
};

class Circle : public Shape{
public:
    double radius;

    Circle(const vec2& c, double r, double a = 0.0)
        : Shape(c, a), radius(r) {}

    double area() const override {
        return radius*radius*PI;
    }
};

class Body{
public:
    std::unique_ptr<Shape> shape;


    Body(std::unique_ptr<Shape> s) : shape(std::move(s)) {}

    double area() const {
        return shape->area();
    }

};




int main(){

    // Create a Body with a Rect
    Body rectBody(std::make_unique<Rect>(vec2(0, 0), 10, 20));

    // Create a Body with a Circle
    Body circleBody(std::make_unique<Circle>(vec2(0, 0), 15));

    // Access the area of the shapes
    std::cout << "Rectangle Area: " << rectBody.area() << std::endl;
    std::cout << "Circle Area: " << circleBody.area() << std::endl;

    rectBody.shape->center;
    

    return 0;
}
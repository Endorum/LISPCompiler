#pragma once

#include <SDL2/SDL.h>
#include "vec2.hpp"



typedef enum{
    ST_CIRC,
    ST_RECT
}ShapeType;



typedef struct ABCD{
    vec2 A, B, C, D; // top left top right bottom right bottom left

    
}ABCD;

class Rect{
public:
    vec2 center;
    double width;
    double height;
    double angle; // counter clockwise around the center point

    ABCD abcd; 

    Rect(vec2 c, double w, double h, double a = 0.0)
        : center(c), width(w), height(h), angle(a),
        abcd{vec2(), vec2(), vec2(), vec2()} {
            to_ABCD();
        }

    

    double area() const {
        return width*height;
    }



    void to_ABCD(){
        abcd.A = vec2(-width / 2,-height / 2);
        abcd.B = vec2( width / 2,-height / 2);
        abcd.C = vec2( width / 2, height / 2);
        abcd.D = vec2(-width / 2, height / 2);

        if(angle != 0){
            abcd.A = rotatePoint(abcd.A, angle);
            abcd.B = rotatePoint(abcd.B, angle);
            abcd.C = rotatePoint(abcd.C, angle);
            abcd.D = rotatePoint(abcd.D, angle);
        }

        abcd.A += center;
        abcd.B += center;
        abcd.C += center;
        abcd.D += center;

        
    }

    vec2 lowestCorner(){
        if(
            abcd.A.y >= abcd.B.y && 
            abcd.A.y >= abcd.C.y && 
            abcd.A.y >= abcd.D.y
        ) return abcd.A;
        if(
            abcd.B.y >= abcd.A.y && 
            abcd.B.y >= abcd.C.y && 
            abcd.B.y >= abcd.D.y
        ) return abcd.B;
        if(
            abcd.C.y >= abcd.B.y && 
            abcd.C.y >= abcd.A.y && 
            abcd.C.y >= abcd.D.y
        ) return abcd.C;
        if(
            abcd.D.y >= abcd.B.y && 
            abcd.D.y >= abcd.C.y && 
            abcd.D.y >= abcd.A.y
        ) return abcd.D;
    }

};

class Circle{
public:
    vec2 center;
    double radius;
    double angle;

    Circle(vec2 c, double r) 
        : center(c), radius(r), angle(0) {}

    double area() const {
        return radius*radius*PI;
    }

    

    
};


class Shape{ 
public:
    ShapeType type;
    
    union{
        Rect rect;
        Circle circ;
    };

    Shape(Rect r){
        type = ST_RECT;
        rect = r;
    }

    Shape(Circle c){
        type = ST_CIRC;
        circ = c;
    }


    ~Shape(){}
};


double MOI_disk(Circle circ, vec2 axis, double mass){
    double I_center = (mass / 2.0) * pow(circ.radius,2);
    

    if(axis==circ.center){
        return I_center;
    }else{
        // around an arbitrary axis
        return I_center + mass * distSq(circ.center, axis);
    }
}

double MOI_Rect(Rect rect, vec2 axis, double mass){
    double I_center = (1/12.0) * mass * ( (rect.width*rect.width) + (rect.height*rect.height) );
    if(axis==rect.center){
        return I_center;
    }else{
        // around an arbitrary axis
        return I_center + mass * distSq(rect.center, axis);
    }
}


vec2 center_of_shape_Read(Shape shape){
    switch(shape.type){
        default:return vec2(0,0);
        case ST_CIRC: return shape.circ.center;
        case ST_RECT: return shape.rect.center;
    }
}

void center_of_shape_Write(Shape& shape, vec2 pos){
    switch(shape.type){
        default:break;
        case ST_CIRC: shape.circ.center = pos; break;
        case ST_RECT: shape.rect.center = pos; break;
    }
}

double angle_of_shape(Shape shape){
    switch(shape.type){
        default:return 0;
        case ST_CIRC: return shape.circ.angle;
        case ST_RECT: return shape.rect.angle;
    }
}

class RigidBody2D{
public:
    Shape shape;
    vec2 velocity;
    vec2 acceleration;
    vec2 force;

    double mass;
    double MOI;

    double angle;
    double angular_velocity;
    double torque;

    vec2 center;

    RigidBody2D(Shape s, double m=1)
        : shape(s), mass(m){
            velocity = vec2(0,0);
            acceleration = vec2(0,0);
            force = vec2(0,0);
            
            switch(s.type){
                case ST_CIRC: MOI = MOI_disk(s.circ, s.circ.center, mass); break;
                case ST_RECT: MOI = MOI_Rect(s.rect, s.rect.center, mass); break;
            }
            
            torque = 0.0;
            angle = angle_of_shape(shape);
            center = center_of_shape_Read(shape);
        }

    void apply_force(vec2 f, vec2 point){
        force += f;

        
        // calculate torque based on the point of application
        vec2 r = point - center_of_shape_Read(shape);
        torque += r.x * f.y - r.y * r.x; // crossproduct ( r x f )
        
        
    }


    void update(double deltaTime=0.01){

        // linear updates ( F = ma -> a = F/m)
        acceleration = force * (1/mass);
        velocity += acceleration * deltaTime;
        center_of_shape_Write(shape, center_of_shape_Read(shape) + (velocity * deltaTime));

        //Angular updates (T = I*alpha -> alpha = T/I)
        double angularAcceleration = torque * (1/MOI);
        
        angular_velocity += angularAcceleration * deltaTime;
        if(shape.type == ST_RECT){
            shape.rect.angle += angular_velocity * deltaTime;
            //shape.rect.to_ABCD();
        }

        // Collision detection and response eg:
        

        force = vec2(0,0);
        torque = 0;

        double damping = 0.99;
        velocity *= damping;
        angular_velocity *= damping;


    }



    // void draw(SDL_Renderer* renderer) const {
    //     if(shape) shape->draw(renderer);
        
    // }
};
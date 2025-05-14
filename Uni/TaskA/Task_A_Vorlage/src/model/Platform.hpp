#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics/RectangleShape.hpp>

class PlayerControl;

// model class for a platform
class Platform {
public:
    // constructor, initializes the platform shape at the given position
    Platform(sf::Vector2f position);

    // virtual destructor
    virtual ~Platform();

    // getter for the shape of the platform
    const sf::RectangleShape& get_shape() const;

protected:
    sf::RectangleShape shape;
};

#endif

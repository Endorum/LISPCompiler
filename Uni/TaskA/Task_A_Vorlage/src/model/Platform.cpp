#include "Platform.hpp"
#include "Player.hpp"
#include "../control/PlayerControl.hpp"

Platform::Platform(sf::Vector2f position) : shape(sf::Vector2f({50, 10})) {
    shape.setPosition(position);
}

Platform::~Platform() {};

const sf::RectangleShape& Platform::get_shape() const {
    return shape;
}
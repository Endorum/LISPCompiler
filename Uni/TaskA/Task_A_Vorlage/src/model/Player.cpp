#include "Player.hpp"

Player::Player() :
    texture(),
    sprite(texture) {
    // load texture
    // (use "assets/images/spritesheet_bread.png" for the jumping loaf of bread, original can be found here https://caz-creates-games.itch.io/bread) 
    if (!texture.loadFromFile(("assets/images/spritesheet.png")))
        throw std::invalid_argument("Spritesheet not found");
    // set up sprite
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect({0, 0}, {100, 100}));
    sprite.setOrigin({50,100});
    sprite.setPosition({300,0});
}

// TODO definition of methods of the player class

const sf::Sprite& Player::get_sprite() const {
    return sprite;
}

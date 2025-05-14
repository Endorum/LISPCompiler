#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

enum Direction{
    VER_DIR,
    HOR_DIR,
};


// model class for player character
class Player {

public:
    // constructor, initializes sprite
    Player();
   
    // TODO declaration of additional methods

    // returns the sprite depicting the character
    const sf::Sprite &get_sprite() const;

    void setPosition(sf::Vector2f& p){
        position = p;
    }

    sf::Vector2f getPosition(){
        return position;
    }

    void setDirection(Direction new_dir){
        current_direction = new_dir;
    }

    Direction getDirection(){
        return current_direction;
    }



private:

    sf::Texture texture;

    sf::Sprite sprite;

    sf::Vector2f position;

    Direction current_direction;
};

#endif
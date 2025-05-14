#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include <SFML/Graphics.hpp>

#include "../model/Player.hpp"
#include "../view/Layer.hpp"
#include "../model/Platform.hpp"

class PlayerControl {
public:
    // initialize control with layer
    PlayerControl(Layer &layer);

    // determine the position of the character depending on the current movement
    void update_player(float elapsed_time);

    // draw the character to the layer
    void draw_player();

    // TODO implement more methods to control the character's movements

private:
    // player object
    Player player;

    // layer on which the character is drawn
    Layer &layer;

    // time in current jump
    float jump_time = 0;

    // height at which the current jump started
    float jump_start_height = 0;
};

#endif

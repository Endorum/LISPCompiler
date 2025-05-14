#include "PlayerControl.hpp"

PlayerControl::PlayerControl(Layer &layer) : layer(layer) {

}

void PlayerControl::update_player(float elapsed_time) {
    // TODO update the character's position
}

void PlayerControl::draw_player() {
    layer.add_to_layer(player.get_sprite());
}

// TODO implement methods

#include "Layer.hpp"

Layer::Layer(sf::RenderWindow &window) :
    window(window),
    target(window.getSize()),
    sprite(target.getTexture()) {
    target.display();
    if (!target.resize(window.getSize()))
        throw std::runtime_error("undefined window size");
}

void Layer::add_to_layer(const sf::Drawable &drawable) {
    target.draw(drawable);
}

void Layer::draw() {
    // update the render target and draw it inside the window
    target.display();
    window.draw(sprite);
}

void Layer::clear() {
    target.clear({0,0,0,0});
}

void Layer::set_view(const sf::View &view) {
    target.setView(view);
}

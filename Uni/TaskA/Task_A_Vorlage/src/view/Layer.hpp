#ifndef LAYER_H
#define LAYER_H

#include <SFML/Graphics.hpp>

// Layer class
class Layer {
public:   
    // Initializes the layer in the given window
    Layer(sf::RenderWindow &window);

    // adds the drawable element to the layer
    void add_to_layer(const sf::Drawable &drawable);

    // draws the scene to the window
    void draw();

    // clear the layer, remove all drawable elements
    void clear();

    // adjust the layer's view
    void set_view(const sf::View &view);

private:
    // window in which the layer is shown
    sf::RenderWindow &window;

    // render target
    sf::RenderTexture target;

    // sprite to which the target is drawn, that gets shown in the window
    sf::Sprite sprite;
};

#endif
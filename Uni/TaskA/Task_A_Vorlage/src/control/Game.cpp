#include "Game.hpp"

#include <SFML/Window/Keyboard.hpp>

Game::Game() 
    :   window(sf::VideoMode({600u, 600u}), "Spiel"),
        view(sf::FloatRect(sf::Vector2f({0,-600}), sf::Vector2f({600,600}))),
        game_layer(window),
        player_control(game_layer)  {
            
    // limit frame rate
    window.setFramerateLimit(60);

    Platform platform0({300, 550});
    Platform platform1({400, 570});
    Platform platform2({200, 570});

    platforms.push_back(platform0);
    platforms.push_back(platform1);
    platforms.push_back(platform2);

    // set the view (visible area) for our game
    game_layer.set_view(view);
}

void Game::start() {
    // The clock is needed to control the speed of movement
    sf::Clock clock;

    while (window.isOpen()) {
        // Restart the clock and save the elapsed time into elapsed_time
        sf::Time elapsed_time = clock.restart();
 
        // handle input, check if window is still open
        if (!input()) {
            // update the scene according to the passed time
            update(elapsed_time.asSeconds());
            // draw the scene
            draw();
        }
    }
}

// returns true, if the window has been closed
bool Game::input() {
    while (std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            // quit
            window.close();
            return true;
        }
        // TODO: Process other events
        // examples:
        //if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            //if (keyPressed->code == sf::Keyboard::Key::Right) { // right arrow key pressed
                // ...
        // if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            // if (keyReleased->code == sf::Keyboard::Key::Right) { // right arrow released
                // ...
    }
    return false;
}

void Game::update(float time_passed) {
    // TODO: check if the character jumps on any platform

    // update the character's position with the current time stamp
    player_control.update_player(time_passed);
}

void Game::draw() {
    window.clear();

    // draw the game elements (platforms and character)
    game_layer.clear();
    player_control.draw_player();
    game_layer.draw();

    for(int i=0;i<platforms.size();i++){
        sf::RectangleShape platform = platforms.at(i).get_shape();
        // platform.setFillColor(sf::Color(255,255,255));
        window.draw(platform);
    }

    window.display();
}

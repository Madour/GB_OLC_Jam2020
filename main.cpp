
#include "NasNas.h"

#include "Game.hpp"

auto main() -> int {

    /*
     * Atos - Greek boy
     * Adil - Egyptian boy
     * Izel - Maya girl
     * Yuki - Japanese girl
     */

    ns::Config::debug = false;
    ns::Config::Window::key_repeat = false;
    ns::Config::Window::vertical_sync = true;

    ns::Config::Inputs::setButtonKey("A", sf::Keyboard::X);
    ns::Config::Inputs::setButtonKey("B", sf::Keyboard::C);
    ns::Config::Inputs::setButtonKey("start", sf::Keyboard::Return);
    ns::Config::Inputs::setButtonKey("select", sf::Keyboard::RShift);

    ns::Res::load("assets");
    ns::Res::printTree();

    Game g;
    g.run();

    ns::Res::dispose();

    return 0;
}

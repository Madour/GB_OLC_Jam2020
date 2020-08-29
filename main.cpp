
#include "NasNas.h"

#include "Game.hpp"

auto main() -> int {

    ns::Config::debug = false;
    ns::Config::Window::key_repeat = false;

    ns::Res::load("assets");
    ns::Res::printTree();

    Game g;
    g.run();

    ns::Res::dispose();

    return 0;
}

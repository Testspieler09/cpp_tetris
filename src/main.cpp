#include "engine/game.hpp"
#include "ui/renderer.hpp"

int main() {
    Game game;
    Renderer renderer(game);

    renderer.run();

    return 0;
}

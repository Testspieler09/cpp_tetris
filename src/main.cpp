#include <memory>
#include <string>

#include "engine/core/game.hpp"
#include "engine/modes/infinite_mode.hpp"
#include "engine/modes/puzzle_mode.hpp"
#include "ui/renderer.hpp"

int main(int argc, char *argv[]) {
    std::unique_ptr<IGameMode> mode;

    if (argc > 1 && std::string(argv[1]) == "puzzle") {
        mode = std::make_unique<PuzzleMode>();
    } else {
        mode = std::make_unique<InfiniteMode>();
    }

    Game game(std::move(mode));
    Renderer renderer(game);

    renderer.run();

    return 0;
}

#include "manager.hpp"
class Game {
private:
    World world;
public:
    Game() {
        InitWindow(1200, 800, "voxel engine");
        this->world.setup();
        this->world.genWorld();
    }
    void update() {
        this->world.update();
    }
};
int main(void)
{
    Game _game;
    _game.update();
    return 0;
}

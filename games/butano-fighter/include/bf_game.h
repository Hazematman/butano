#ifndef BF_GAME_H
#define BF_GAME_H

#include "bf_game_background.h"
#include "bf_game_hero.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_hero_bullets.h"
#include "bf_game_enemies.h"
#include "bf_game_scoreboard.h"

namespace bf::game
{

class game
{

public:
    explicit game(btn::sprite_text_generator& text_generator);

    void update();

private:
    background _background;
    hero _hero;
    hero_bomb _hero_bomb;
    hero_bullets _hero_bullets;
    enemies _enemies;
    scoreboard _scoreboard;
};

}

#endif

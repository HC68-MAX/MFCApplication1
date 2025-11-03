#pragma once
// Game/Core/SpriteConfig.h

// 贴图集中各精灵的坐标配置
struct SSpriteCoord
{
    int x, y;           // 在贴图集中的坐标
    int width, height;  // 精灵尺寸

    SSpriteCoord(int x = 0, int y = 0, int w = 0, int h = 0)
        : x(x), y(y), width(w), height(h) {
    }
};

class CSpriteConfig
{
public:
    // 马里奥相关精灵
    static const SSpriteCoord MARIO_SMALL_STAND_RIGHT;
    static const SSpriteCoord MARIO_SMALL_WALK1_RIGHT;
    static const SSpriteCoord MARIO_SMALL_WALK2_RIGHT;
    static const SSpriteCoord MARIO_SMALL_JUMP_RIGHT;

    static const SSpriteCoord MARIO_BIG_STAND_RIGHT;
    static const SSpriteCoord MARIO_BIG_WALK1_RIGHT;
    static const SSpriteCoord MARIO_BIG_WALK2_RIGHT;
    static const SSpriteCoord MARIO_BIG_JUMP_RIGHT;

    static const SSpriteCoord MARIO_FIRE_STAND_RIGHT;
    static const SSpriteCoord MARIO_FIRE_WALK1_RIGHT;
    static const SSpriteCoord MARIO_FIRE_WALK2_RIGHT;
    static const SSpriteCoord MARIO_FIRE_JUMP_RIGHT;

    // 砖块和地形
    static const SSpriteCoord BRICK_NORMAL;
    static const SSpriteCoord BRICK_QUESTION;
    static const SSpriteCoord BRICK_QUESTION_HIT;
    static const SSpriteCoord BRICK_HARD;

    // 水管
    static const SSpriteCoord PIPE_TOP_LEFT;
    static const SSpriteCoord PIPE_TOP_RIGHT;
    static const SSpriteCoord PIPE_BODY_LEFT;
    static const SSpriteCoord PIPE_BODY_RIGHT;

    // 地面和背景
    static const SSpriteCoord GROUND;
    static const SSpriteCoord GROUND_EDGE_LEFT;
    static const SSpriteCoord GROUND_EDGE_RIGHT;
    static const SSpriteCoord BACKGROUND_CLOUD;
    static const SSpriteCoord BACKGROUND_BUSH;
    static const SSpriteCoord BACKGROUND_HILL;

    // 敌人
    static const SSpriteCoord GOOMBA_WALK1;
    static const SSpriteCoord GOOMBA_WALK2;
    static const SSpriteCoord GOOMBA_SQUISHED;

    // 金币
    static const SSpriteCoord COIN_SPIN1;
    static const SSpriteCoord COIN_SPIN2;
    static const SSpriteCoord COIN_SPIN3;
    static const SSpriteCoord COIN_SPIN4;
};
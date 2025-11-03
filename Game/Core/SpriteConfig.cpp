// Game/Core/SpriteConfig.cpp
#include "SpriteConfig.h"

// 根据您的实际贴图集调整这些坐标
// 这里只是示例，您需要根据您的贴图集实际布局来设置

// 马里奥 - 小
const SSpriteCoord CSpriteConfig::MARIO_SMALL_STAND_RIGHT = { 0, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::MARIO_SMALL_WALK1_RIGHT = { 16, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::MARIO_SMALL_WALK2_RIGHT = { 32, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::MARIO_SMALL_JUMP_RIGHT = { 48, 0, 16, 16 };

// 马里奥 - 大
const SSpriteCoord CSpriteConfig::MARIO_BIG_STAND_RIGHT = { 0, 16, 16, 32 };
const SSpriteCoord CSpriteConfig::MARIO_BIG_WALK1_RIGHT = { 16, 16, 16, 32 };
const SSpriteCoord CSpriteConfig::MARIO_BIG_WALK2_RIGHT = { 32, 16, 16, 32 };
const SSpriteCoord CSpriteConfig::MARIO_BIG_JUMP_RIGHT = { 48, 16, 16, 32 };

// 马里奥 - 火焰
const SSpriteCoord CSpriteConfig::MARIO_FIRE_STAND_RIGHT = { 0, 48, 16, 32 };
const SSpriteCoord CSpriteConfig::MARIO_FIRE_WALK1_RIGHT = { 16, 48, 16, 32 };
const SSpriteCoord CSpriteConfig::MARIO_FIRE_WALK2_RIGHT = { 32, 48, 16, 32 };
const SSpriteCoord CSpriteConfig::MARIO_FIRE_JUMP_RIGHT = { 48, 48, 16, 32 };

// 砖块
const SSpriteCoord CSpriteConfig::BRICK_NORMAL = { 64, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::BRICK_QUESTION = { 80, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::BRICK_QUESTION_HIT = { 96, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::BRICK_HARD = { 112, 0, 16, 16 };

// 水管
const SSpriteCoord CSpriteConfig::PIPE_TOP_LEFT = { 128, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::PIPE_TOP_RIGHT = { 144, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::PIPE_BODY_LEFT = { 128, 16, 16, 16 };
const SSpriteCoord CSpriteConfig::PIPE_BODY_RIGHT = { 144, 16, 16, 16 };

// 地面
const SSpriteCoord CSpriteConfig::GROUND = { 160, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::GROUND_EDGE_LEFT = { 176, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::GROUND_EDGE_RIGHT = { 192, 0, 16, 16 };

// 背景元素
const SSpriteCoord CSpriteConfig::BACKGROUND_CLOUD = { 208, 0, 32, 24 };
const SSpriteCoord CSpriteConfig::BACKGROUND_BUSH = { 240, 0, 32, 16 };
const SSpriteCoord CSpriteConfig::BACKGROUND_HILL = { 272, 0, 48, 32 };

// 敌人
const SSpriteCoord CSpriteConfig::GOOMBA_WALK1 = { 320, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::GOOMBA_WALK2 = { 336, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::GOOMBA_SQUISHED = { 352, 0, 16, 8 };

// 金币
const SSpriteCoord CSpriteConfig::COIN_SPIN1 = { 368, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::COIN_SPIN2 = { 384, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::COIN_SPIN3 = { 400, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::COIN_SPIN4 = { 416, 0, 16, 16 };
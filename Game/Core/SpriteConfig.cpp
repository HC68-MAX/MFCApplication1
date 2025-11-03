// Game/Core/SpriteConfig.cpp
#include "SpriteConfig.h"

// 根据您的实际贴图集调整这些坐标
// 这里只是示例，您需要根据您的贴图集实际布局来设置

// === 主精灵表坐标 ===

// 马里奥 - 小
const SSpriteCoord CSpriteConfig::MARIO_SMALL_STAND_RIGHT = { 0, 0, 8, 8 };
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
const SSpriteCoord CSpriteConfig::BRICK_NORMAL = { 16, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::BRICK_QUESTION = { 0, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::BRICK_QUESTION_HIT = { 0, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::BRICK_HARD = { 48, 0, 16, 16 };

// 水管
const SSpriteCoord CSpriteConfig::PIPE_TOP_LEFT = { 0, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::PIPE_TOP_RIGHT = { 0, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::PIPE_BODY_LEFT = { 0, 160, 16, 16 };
const SSpriteCoord CSpriteConfig::PIPE_BODY_RIGHT = { 0, 0, 16, 16 };

// === 瓦片精灵表坐标 ===

// 地面
const SSpriteCoord CSpriteConfig::GROUND = { 0, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::GROUND_EDGE_LEFT = { 0, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::GROUND_EDGE_RIGHT = { 0, 0, 16, 16 };

// 背景元素
const SSpriteCoord CSpriteConfig::BACKGROUND_CLOUD = { 48, 0, 32, 24 };
const SSpriteCoord CSpriteConfig::BACKGROUND_BUSH = { 80, 0, 32, 16 };
const SSpriteCoord CSpriteConfig::BACKGROUND_HILL = { 112, 0, 48, 32 };

// === 敌人物品精灵表坐标 ===

// 敌人
const SSpriteCoord CSpriteConfig::GOOMBA_WALK1 = { 0, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::GOOMBA_WALK2 = { 16, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::GOOMBA_SQUISHED = { 32, 0, 16, 8 };

// 金币
const SSpriteCoord CSpriteConfig::COIN_SPIN1 = { 48, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::COIN_SPIN2 = { 64, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::COIN_SPIN3 = { 80, 0, 16, 16 };
const SSpriteCoord CSpriteConfig::COIN_SPIN4 = { 96, 0, 16, 16 };

// 工具函数：根据精灵坐标确定使用哪个精灵表
CString CSpriteConfig::GetSpritesheetForSprite(const SSpriteCoord& coord)
{
    // 这里根据坐标范围判断使用哪个精灵表
    // 你需要根据实际的精灵表布局来调整这个逻辑

    // 简单判断：如果y坐标在某个范围内，就属于某个精灵表
    if (coord.y < 64) {
        // 假设主精灵表包含y坐标0-63的精灵
        return _T("SpritesheetMain");
    }
    else if (coord.y < 128) {
        // 假设瓦片精灵表包含y坐标64-127的精灵
        return _T("SpritesheetTiles");
    }
    else {
        // 其他情况使用敌人物品精灵表
        return _T("SpritesheetEnemies");
    }

    // 更精确的方法是根据具体的精灵坐标来判断
    // 这里只是示例，你需要根据实际贴图布局来实现
}
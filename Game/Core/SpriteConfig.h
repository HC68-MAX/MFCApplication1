#pragma once
// Game/Core/SpriteConfig.h
#include <afxwin.h>

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
    // 新增：贴图集名称常量
    static const CString TILESET_MAIN;
    static const CString TILESET_MARIO;
    static const CString TILESET_ENEMIES;
    static const CString TILESET_MIKU;
    // 新增：根据精灵类型获取对应的贴图集名称
    static CString GetSpritesheetForMario();
    static CString GetSpritesheetForBrick();
    static CString GetSpritesheetForPipe();
    static CString GetSpritesheetForMiku();
    static CString GetSpritesheetForTile(int tileID);

    // === 主精灵表 (SpritesheetMain) ===
      // 初音未来精灵坐标

    // 马里奥 - 小
    static const SSpriteCoord MARIO_SMALL_STAND_LEFT;
    static const SSpriteCoord MARIO_SMALL_WALK1_LEFT;
    static const SSpriteCoord MARIO_SMALL_WALK2_LEFT;
    static const SSpriteCoord MARIO_SMALL_JUMP_LEFT;
    static const SSpriteCoord MARIO_SMALL_DEAD;
    // 马里奥 - 小
    static const SSpriteCoord MARIO_SMALL_STAND_RIGHT;
    static const SSpriteCoord MARIO_SMALL_WALK1_RIGHT;
    static const SSpriteCoord MARIO_SMALL_WALK2_RIGHT;
    static const SSpriteCoord MARIO_SMALL_WALK3_RIGHT;
    static const SSpriteCoord MARIO_SMALL_JUMP_RIGHT;
    // 马里奥 - 大
    static const SSpriteCoord MARIO_BIG_STAND_RIGHT;
    static const SSpriteCoord MARIO_BIG_WALK1_RIGHT;
    static const SSpriteCoord MARIO_BIG_WALK2_RIGHT;
    static const SSpriteCoord MARIO_BIG_WALK3_RIGHT;
    static const SSpriteCoord MARIO_BIG_JUMP_RIGHT;

    // 马里奥 - 火焰
    static const SSpriteCoord MARIO_FIRE_STAND_RIGHT;
    static const SSpriteCoord MARIO_FIRE_WALK1_RIGHT;
    static const SSpriteCoord MARIO_FIRE_WALK2_RIGHT;
    static const SSpriteCoord MARIO_FIRE_WALK3_RIGHT;
    static const SSpriteCoord MARIO_FIRE_JUMP_RIGHT;

    // 砖块
    static const SSpriteCoord BRICK_NORMAL;
    static const SSpriteCoord BRICK_QUESTION;
    static const SSpriteCoord BRICK_QUESTION_HIT;
    static const SSpriteCoord BRICK_HARD;

    // 水管
    static const SSpriteCoord PIPE_TOP_LEFT;
    static const SSpriteCoord PIPE_TOP_RIGHT;
    static const SSpriteCoord PIPE_BODY_LEFT;
    static const SSpriteCoord PIPE_BODY_RIGHT;

    // Flagpole
    static const SSpriteCoord FLAGPOLE_TOP;
    static const SSpriteCoord FLAGPOLE_POLE;
    static const SSpriteCoord FLAGPOLE_FLAG;

    // === 瓦片精灵表 (SpritesheetTiles) ===

    // 地面
    static const SSpriteCoord GROUND;
    static const SSpriteCoord GROUND_EDGE_LEFT;
    static const SSpriteCoord GROUND_EDGE_RIGHT;

    // 背景元素
    static const SSpriteCoord BACKGROUND_CLOUD;
    static const SSpriteCoord BACKGROUND_BUSH;
    static const SSpriteCoord BACKGROUND_HILL;

    // === 敌人物品精灵表 ===

    // 敌人
    static const SSpriteCoord GOOMBA_WALK1;
    static const SSpriteCoord GOOMBA_WALK2;
    static const SSpriteCoord GOOMBA_SQUISHED;

    // 金币
    static const SSpriteCoord COIN_SPIN1 ;
    static const SSpriteCoord COIN_SPIN2 ;
    static const SSpriteCoord COIN_SPIN3 ;
    static const SSpriteCoord COIN_SPIN4 ;

    // === 工具函数 ===
    static CString GetSpritesheetForSprite(const SSpriteCoord& coord);
};

#pragma once
// Game/Core/GameConfig.cpp

#include <afxwin.h>

class CGameConfig
{
public:
    // 贴图尺寸
    static const int TILE_SIZE = 32;           // 瓦片基本尺寸
    static const int MARIO_SMALL_WIDTH = 32;   // 小马里奥宽度
    static const int MARIO_SMALL_HEIGHT = 64;  // 小马里奥高度
    static const int MARIO_BIG_WIDTH = 48;     // 大马里奥宽度  
    static const int MARIO_BIG_HEIGHT = 72;    // 大马里奥高度
    static const int BRICK_WIDTH = 32;         // 砖块宽度
    static const int BRICK_HEIGHT = 32;        // 砖块高度
    static const int PIPE_WIDTH = 64;          // 水管宽度
    static const int PIPE_HEIGHT = 96;         // 水管高度
    static const int COIN_SIZE = 24;           // 金币尺寸
    static const int ENEMY_WIDTH = 32;         // 敌人宽度
    static const int ENEMY_HEIGHT = 32;        // 敌人高度

    // 屏幕和游戏设置
    static const int SCREEN_WIDTH = 800;       // 屏幕宽度
    static const int SCREEN_HEIGHT = 600;      // 屏幕高度
    static const int WORLD_WIDTH = 2400;       // 世界宽度
    static const int WORLD_HEIGHT = 600;       // 世界高度
    static const int FPS = 60;                 // 目标帧率
    static const int TILE_MAP_WIDTH = 75;      // 瓦片地图宽度 (2400/32=75)
    static const int TILE_MAP_HEIGHT = 15;     // 瓦片地图高度 (600/32=18.75，向上取整)

    // 物理参数
    static const float GRAVITY;                // 重力
    static const float MARIO_MAX_SPEED;        // 马里奥最大速度
    static const float MARIO_ACCELERATION;     // 马里奥加速度
    static const float MARIO_JUMP_FORCE;       // 马里奥跳跃力
    static const float MARIO_JUMP_MAX_TIME;    // 马里奥最大跳跃时间

    // 颜色配置（用于占位符）
    static const COLORREF COLOR_MARIO_RED = RGB(220, 0, 0);
    static const COLORREF COLOR_BRICK = RGB(180, 80, 0);
    static const COLORREF COLOR_QUESTION = RGB(255, 220, 0);
    static const COLORREF COLOR_HARD_BRICK = RGB(150, 150, 150);
    static const COLORREF COLOR_PIPE = RGB(0, 160, 0);
    static const COLORREF COLOR_GROUND = RGB(120, 60, 0);
    static const COLORREF COLOR_BACKGROUND_ = RGB(100, 180, 255);
    static const COLORREF COLOR_COIN = RGB(255, 215, 0);
    static const COLORREF COLOR_ENEMY = RGB(100, 60, 20);

    // 调试设置
    static const BOOL DEBUG_SHOW_COLLISION = FALSE;  // 默认不显示碰撞框
    static const BOOL DEBUG_SHOW_FPS = TRUE;         // 默认显示FPS
};
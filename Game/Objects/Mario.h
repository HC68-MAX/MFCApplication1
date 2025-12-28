#pragma once
// Game/Objects/Mario.h
#include <afxwin.h>
#include "GameObject.h"
#include <vector>
#include "../Core/GameConfig.h"
#include "../Core/ResourceManager.h"
#include "../Core/SpriteRenderer.h"
#include "../Core/SpriteConfig.h"

// 马里奥皮肤枚举
enum class MarioSkin
{
    MARIO,      // 原始马里奥
    MIKU        // 初音未来
};

// 马里奥状态枚举
enum class MarioState
{
    SMALL,      // 小马里奥
    BIG,        // 大马里奥  
    FIRE,        // 火焰马里奥
    DEAD        // 死亡状态 - 新增
};

// 方向枚举
enum class Direction
{
    LEFT,
    RIGHT
};

// CMario类，代表游戏中的主角马里奥
class CMario : public CGameObject
{
public:
    // =================================================================
    // 构造与析构
    // =================================================================
    CMario(); // 默认构造函数
    CMario(int x, int y); // 带初始位置的构造函数
    virtual ~CMario(); // 析构函数

    // =================================================================
    // 主要游戏循环接口
    // =================================================================
    void Draw(CDC* pDC) ; // 绘制马里奥
    void DrawAt(CDC* pDC, int screenX, int screenY); // 在指定屏幕坐标绘制
    void Update(float deltaTime);
    // =================================================================
    // 输入与移动控制
    // =================================================================
    void HandleInput(BOOL left, BOOL right, BOOL jump); // 处理玩家输入
    void MoveLeft();                // 向左移动
    void MoveRight();               // 向右移动
    void Jump();                    // 跳跃
    void StopMoving();              // 停止移动

    // =================================================================
    // 状态管理 (大小, 皮肤)
    // =================================================================
    MarioState GetState() const { return m_State; } // 获取当前状态（大小）
    void SetState(MarioState state); // 设置当前状态
    MarioSkin GetSkin() const { return m_Skin; } // 获取当前皮肤
    void SetSkin(MarioSkin skin); // 设置当前皮肤
    Direction GetDirection() const { return m_Direction; } // 获取当前朝向

    // =================================================================
    // 物理与碰撞
    // =================================================================
    void ApplyPhysics(float deltaTime); // 应用物理效果（如重力）
    void CheckCollisions(const std::vector<CRect>& platforms); // 检测与平台的碰撞
    
    // 碰撞区域获取
    CRect GetFeetRect() const;    // 获取脚部碰撞区域
    CRect GetHeadRect() const;    // 获取头部碰撞区域
    CRect GetBodyRect() const;    // 获取身体碰撞区域

    // 碰撞状态查询
    BOOL IsOnGround() const { return m_bIsOnGround; } // 是否在地面上
    BOOL IsJumping() const { return m_bIsJumping; } // 是否在跳跃中
    BOOL IsMoving() const; // 是否在移动中
    BOOL IsMovingUp() const { return m_fVelocityY < 0; } // 判断是否正在向上移动（用于头部碰撞）
    void StartJump();
    // 碰撞响应处理
    void OnHeadCollision(); // 头部碰撞响应
    void OnFeetCollision(int surfaceY); // 脚部碰撞响应
    void OnLeftCollision(int surfaceX); // 左侧碰撞响应
    void OnRightCollision(int surfaceX); // 右侧碰撞响应

    void DrawWithSprite(CDC* pDC, int screenX, int screenY); // 使用精灵图绘制
    // =================================================================
    // 调试与获取参数
    // =================================================================
    float GetVelocityX() const { return m_fVelocityX; } // 获取X轴速度
    float GetVelocityY() const { return m_fVelocityY; } // 获取Y轴速度
    void SetVelocity(float vx, float vy) { m_fVelocityX = vx; m_fVelocityY = vy; } // 设置X和Y轴速度
    float GetGravity() const { return m_fGravity; } // 获取重力加速度
    float GetMaxSpeed() const { return m_fMaxSpeed; } // 获取最大移动速度
    float GetJumpForce() const { return m_fJumpForce; } // 获取跳跃力

    // 死亡状态管理
    void Die();                    // 触发死亡
    BOOL IsDying() const { return m_bIsDying; }
    BOOL IsDead() const { return m_State == MarioState::DEAD; }
    void Respawn();                // 重生
    void UpdateDeathAnimation(float deltaTime);
    void DieFromFall();            // 新增：从高处掉落死亡
private:
    // =================================================================
    // 私有辅助函数
    // =================================================================
    void UpdateSize(); // 更新马里奥大小（根据状态）
    void UpdateSkinResources(); // 根据皮肤更新图集和尺寸
    void UpdateMikuAnimation(float deltaTime); // 更新Miku皮肤的动画
    SSpriteCoord GetMikuSpriteCoord() const; // 获取Miku皮肤当前的精灵坐标

private:
    // =================================================================
    // 成员变量
    // =================================================================
     // 死亡状态相关
    BOOL m_bIsDying;               // 是否正在死亡动画中
    float m_fDeathTimer;           // 死亡动画计时器
    float m_fDeathJumpVelocity;    // 死亡时向上的弹跳速度
    float m_fDeathAnimationTime;   // 死亡动画总时间

    // 状态与方向
    MarioState m_State;               // 当前状态（小、大、火焰）
    MarioSkin  m_Skin;                // 当前皮肤
    Direction  m_Direction;           // 面向方向
    BOOL       m_bIsMoving;           // 是否正在移动

    // 输入状态
    BOOL m_bInputLeft;                // 是否接收到向左的输入
    BOOL m_bInputRight;               // 是否接收到向右的输入
    BOOL m_bInputJump;                // 是否接收到跳跃的输入

    // 碰撞状态
    BOOL m_bHeadCollision;            // 头部是否发生碰撞
    BOOL m_bFeetCollision;            // 脚部是否发生碰撞
    BOOL m_bLeftCollision;            // 左侧是否发生碰撞
    BOOL m_bRightCollision;           // 右侧是否发生碰撞

    // Miku动画相关
    float m_fMikuAnimTimer;           // Miku动画计时器
    int   m_nMikuCurrentFrame;        // Miku动画当前帧
    static const int MIKU_WALK_FRAMES = 32; // Miku行走动画的总帧数
    static const int MIKU_FRAME_WIDTH = 48; // Miku每帧的宽度
    static const int MIKU_FRAME_HEIGHT = 48; // Miku每帧的高度

    // 物理与速度
    float m_fVelocityX, m_fVelocityY; // X和Y方向速度
    float m_fAcceleration;            // 加速度
    float m_fMaxSpeed;                // 最大速度
    float m_fGravity;                 // 重力

    // 跳跃相关（完全重写）
    float m_fJumpForce;               // 跳跃初始力
    float m_fJumpHoldTime;            // 跳跃键按住的时间
    float m_fMaxJumpHoldTime;         // 最大跳跃按住时间
    float m_fJumpGravity;             // 跳跃时的重力（较小）
    float m_fFallGravity;             // 下落时的重力（较大）
    BOOL  m_bIsJumping;               // 是否正在跳跃
    BOOL  m_bIsOnGround;              // 是否在地面上
    BOOL  m_bJumpPressed;             // 当前帧跳跃键是否被按下
    BOOL  m_bJumpWasPressed;          // 上一帧跳跃键是否被按下
    BOOL  m_bCanJump;                 // 是否可以跳跃

    // 新增：跳跃峰值检测
    float m_fPreviousVelocityY;       // 上一帧的Y速度
    BOOL  m_bReachedJumpPeak;         // 是否到达跳跃峰值
    float m_fPeakHoldTime;            // 峰值悬停时间
    float m_fMaxPeakHoldTime;         // 最大峰值悬停时间
};
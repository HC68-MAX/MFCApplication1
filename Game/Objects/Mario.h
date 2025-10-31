#pragma once
// Game/Objects/Mario.h
#include <afxwin.h>  // 包含MFC基础头文件
#include "GameObject.h"  // 包含基类
#include <vector>
#include "../Core/GameConfig.h"  // 添加这行

// 马里奥状态枚举
enum class MarioState
{
    SMALL,      // 小马里奥
    BIG,        // 大马里奥  
    FIRE        // 火焰马里奥
};

// 方向枚举
enum class Direction
{
    LEFT,
    RIGHT
};

class CMario : public CGameObject
{
public:
    // 构造函数和析构函数
    CMario();
    CMario(int x, int y);
    virtual ~CMario();
    // 重写基类的虚函数
    virtual void Update(float deltaTime) override;
    virtual void Draw(CDC* pDC) override;
    // 获取物理参数（用于调试）
    float GetVelocityX() const { return m_fVelocityX; }
    float GetVelocityY() const { return m_fVelocityY; }
    float GetGravity() const { return m_fGravity; }
    float GetMaxSpeed() const { return m_fMaxSpeed; }
    float GetJumpForce() const { return m_fJumpForce; }
    void MoveLeft();                // 向左移动
    void MoveRight();               // 向右移动
    void Jump();                    // 跳跃
    void StopMoving();              // 停止移动
    // 新增方法
    void ApplyPhysics(float deltaTime);
    void HandleInput(BOOL left, BOOL right, BOOL jump);
    // 新增：使用屏幕坐标绘制
    void DrawAt(CDC* pDC, int screenX, int screenY);
    // 新增碰撞相关方法
    void  CheckCollisions(const std::vector<CRect>& platforms);
    CRect GetFeetRect() const;    // 获取脚部碰撞区域
    CRect GetHeadRect() const;    // 获取头部碰撞区域
    CRect GetBodyRect() const;    // 获取身体碰撞区域
    // 碰撞相关方法
    BOOL IsOnGround() const { return m_bIsOnGround; }
    BOOL IsJumping() const { return m_bIsJumping; }
    BOOL IsMoving() const { return m_bIsMoving; }
    // 碰撞响应
    void OnHeadCollision();
    void OnFeetCollision(int surfaceY);
    void OnLeftCollision(int surfaceX);
    void OnRightCollision(int surfaceX);

    // 设置速度
    void SetVelocity(float vx, float vy) { m_fVelocityX = vx; m_fVelocityY = vy; }
   
    // 状态管理
    MarioState GetState() const { return m_State; }
    void SetState(MarioState state);

    void DrawSmallMario(CDC* pDC, COLORREF skinColor, COLORREF overallsColor,
        COLORREF hatColor, COLORREF shoeColor);
    void DrawBigMario(CDC* pDC, COLORREF skinColor, COLORREF overallsColor,
        COLORREF hatColor, COLORREF shoeColor);
    void DrawWithGeometry(CDC* pDC);
private:
    // 更新马里奥大小（根据状态）
    void UpdateSize();

private:
    // 速度相关
    float m_fVelocityX, m_fVelocityY; // X和Y方向速度
    float m_fAcceleration;            // 加速度
    float m_fMaxSpeed;                // 最大速度

    // 跳跃相关
    float m_fJumpForce;               // 跳跃力
    float m_fGravity;                 // 重力
    BOOL m_bIsJumping;                // 是否正在跳跃
    BOOL m_bIsOnGround;               // 是否在地面上

    // 移动状态
    BOOL m_bIsMoving;                 // 是否正在移动
    Direction m_Direction;            // 面向方向

    // 新增输入状态变量
    BOOL m_bInputLeft;
    BOOL m_bInputRight;
    BOOL m_bInputJump;
    // 新增碰撞状态
    BOOL m_bHeadCollision;
    BOOL m_bFeetCollision;
    BOOL m_bLeftCollision;
    BOOL m_bRightCollision;
    // 新增物理变量
    float m_fJumpTime;           // 跳跃计时器
    float m_fMaxJumpTime;        // 最大跳跃持续时间
    BOOL m_bCanJump;             // 是否可以跳跃
    // 马里奥状态
    MarioState m_State;               // 当前状态

};
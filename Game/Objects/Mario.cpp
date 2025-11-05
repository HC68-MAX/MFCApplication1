
// Game/Objects/Mario.cpp
#include <afxwin.h>  // 直接包含MFC头文件
#include "Mario.h"
#include "../Core/ResourceManager.h"
#include "../Core/SpriteRenderer.h"
#include "../Core/SpriteConfig.h"

// 构造函数
CMario::CMario() : CGameObject(100, 400, CGameConfig::MARIO_SMALL_WIDTH, CGameConfig::MARIO_SMALL_HEIGHT)  // 调用基类构造函数
{

    m_fVelocityX = 0.0f;
    m_fVelocityY = 0.0f;

    // 使用全局配置的物理参数
    m_fAcceleration = CGameConfig::MARIO_ACCELERATION;
    m_fMaxSpeed = CGameConfig::MARIO_MAX_SPEED;
    m_fJumpForce = CGameConfig::MARIO_JUMP_FORCE;
    m_fGravity = CGameConfig::GRAVITY;


    // 初始化状态
    m_bIsJumping = FALSE;
    m_bIsOnGround = FALSE;
    m_bIsMoving = FALSE;

    m_Direction = Direction::RIGHT;
    m_State = MarioState::SMALL;
    // 初始化输入状态
    m_bInputLeft = FALSE;
    m_bInputRight = FALSE;
    m_bInputJump = FALSE;

    // 初始化跳跃相关
    m_fJumpTime = 0.0f;
    m_fMaxJumpTime = CGameConfig::MARIO_JUMP_MAX_TIME;  // 使用全局配置
    m_bCanJump = TRUE;
    // 根据状态更新大小
    UpdateSize();
}

// 带参数的构造函数 —— 使用委托构造（正确调用默认构造）
CMario::CMario(int x, int y) : CMario()
{
    // 调用默认构造函数初始化大多数值
    CMario();

    // 设置指定位置
    m_nX = x;
    m_nY = y;
}

// 析构函数
CMario::~CMario()
{
    // 目前没有需要手动释放的资源
}
// 新增：使用精灵渲染器绘制马里奥
void CMario::DrawWithSprite(CDC* pDC, int screenX, int screenY)
{
    if (!m_bVisible) return;

    CResourceManager& resMgr = CResourceManager::GetInstance();
    // 使用马里奥专用的贴图集
    CBitmap* pBitmap = resMgr.GetBitmap(CSpriteConfig::GetSpritesheetForMario());


    if (!pBitmap) {
        // 如果没有贴图，使用几何绘制
        DrawWithGeometry(pDC);
        return;
    }

    SSpriteCoord spriteCoord;

    // 根据状态和动作选择精灵
    switch (m_State)
    {
    case MarioState::SMALL:
        if (m_bIsJumping)
            spriteCoord = CSpriteConfig::MARIO_SMALL_JUMP_RIGHT;
        else if (m_bIsMoving)
            spriteCoord = (GetTickCount64() % 500 < 250) ?
            CSpriteConfig::MARIO_SMALL_WALK1_RIGHT :
            CSpriteConfig::MARIO_SMALL_WALK2_RIGHT;
        else
            spriteCoord = CSpriteConfig::MARIO_SMALL_STAND_RIGHT;
        break;

    case MarioState::BIG:
        if (m_bIsJumping)
            spriteCoord = CSpriteConfig::MARIO_BIG_JUMP_RIGHT;
        else if (m_bIsMoving)
            spriteCoord = (GetTickCount64() % 500 < 250) ?
            CSpriteConfig::MARIO_BIG_WALK1_RIGHT :
            CSpriteConfig::MARIO_BIG_WALK2_RIGHT;
        else
            spriteCoord = CSpriteConfig::MARIO_BIG_STAND_RIGHT;
        break;

    case MarioState::FIRE:
        if (m_bIsJumping)
            spriteCoord = CSpriteConfig::MARIO_FIRE_JUMP_RIGHT;
        else if (m_bIsMoving)
            spriteCoord = (GetTickCount64() % 500 < 250) ?
            CSpriteConfig::MARIO_FIRE_WALK1_RIGHT :
            CSpriteConfig::MARIO_FIRE_WALK2_RIGHT;
        else
            spriteCoord = CSpriteConfig::MARIO_FIRE_STAND_RIGHT;
        break;
    }

    // 直接使用精灵渲染器绘制
    CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX, screenY,
        32,64,
        spriteCoord.x, spriteCoord.y,
        spriteCoord.width, spriteCoord.height, TRUE);
}
// 修改现有的DrawAt方法，使用新的精灵绘制方法
void CMario::DrawAt(CDC* pDC, int screenX, int screenY)
{
    if (!m_bVisible) return;

    // 保存原始位置
    int originalX = m_nX;
    int originalY = m_nY;

    // 设置到屏幕位置
    m_nX = screenX;
    m_nY = screenY;

    // 使用精灵渲染器绘制
    DrawWithSprite(pDC, screenX, screenY);

    // 恢复位置
    m_nX = originalX;
    m_nY = originalY;
}

// 修改现有的Draw方法
void CMario::Draw(CDC* pDC)
{
    DrawWithSprite(pDC, m_nX, m_nY);
}
// 更新马里奥状态
void CMario::Update(float deltaTime)
{
    // 处理输入
    HandleInput(m_bInputLeft, m_bInputRight, m_bInputJump);

    // 应用物理
    ApplyPhysics(deltaTime);

    // 简单的边界检查（临时）
    if (m_nX < 0)
    {
        m_nX = 0;
        m_fVelocityX = 0;
    }

}
// 新的输入处理方法
void CMario::HandleInput(BOOL left, BOOL right, BOOL jump)
{
    m_bIsMoving = FALSE;

    // 处理左右移动
    if (left && !right)
    {
        m_fVelocityX = -m_fMaxSpeed;
        m_Direction = Direction::LEFT;
        m_bIsMoving = TRUE;
    }
    else if (right && !left)
    {
        m_fVelocityX = m_fMaxSpeed;
        m_Direction = Direction::RIGHT;
        m_bIsMoving = TRUE;
    }
    else
    {
        // 没有移动输入时应用摩擦力
        if (m_fVelocityX > 0)
        {
            m_fVelocityX -= m_fAcceleration * 2; // 增加摩擦力
            if (m_fVelocityX < 0) m_fVelocityX = 0;
        }
        else if (m_fVelocityX < 0)
        {
            m_fVelocityX += m_fAcceleration * 2;
            if (m_fVelocityX > 0) m_fVelocityX = 0;
        }
    }

    // 处理跳跃
    if (jump && m_bCanJump && m_bIsOnGround)
    {
        m_fVelocityY = m_fJumpForce;
        m_bIsJumping = TRUE;
        m_bIsOnGround = FALSE;
        m_bCanJump = FALSE; // 防止连续跳跃
        m_fJumpTime = 0.0f;
    }

    // 跳跃键释放时重置跳跃能力
    if (!jump)
    {
        m_bCanJump = TRUE;
    }

    // 跳跃持续时间控制（实现可变高度跳跃）
    if (m_bIsJumping && jump && m_fJumpTime < m_fMaxJumpTime)
    {
        m_fVelocityY = m_fJumpForce * (1.0f - m_fJumpTime / m_fMaxJumpTime);
        m_fJumpTime += 1.0f / 60.0f; // 假设60fps
    }
}

// 新的物理应用方法
void CMario::ApplyPhysics(float deltaTime)
{
    // 应用重力（如果不在地面上）
    if (!m_bIsOnGround)
    {
        m_fVelocityY += m_fGravity;
    }

    // 限制最大下落速度
    if (m_fVelocityY > 20.0f)
    {
        m_fVelocityY = 20.0f;
    }

    // 更新位置
    m_nX += static_cast<int>(m_fVelocityX);
    m_nY += static_cast<int>(m_fVelocityY);
}

// 新增：检查碰撞
// 全新的碰撞检测方法 - 简单可靠
void CMario::CheckCollisions(const std::vector<CRect>& solidRects)
{
    // 先假设不在地面上
    m_bIsOnGround = FALSE;

    // 检查与每个实体的碰撞
    for (const auto& rect : solidRects)
    {
        CRect intersection;
        CRect marioRect = GetRect();

        if (intersection.IntersectRect(&marioRect, &rect))
        {
            // 计算碰撞的深度和方向
            int overlapLeft = marioRect.right - rect.left;
            int overlapRight = rect.right - marioRect.left;
            int overlapTop = marioRect.bottom - rect.top;
            int overlapBottom = rect.bottom - marioRect.top;

            // 找出最小的重叠方向
            int minOverlap = min(min(overlapLeft, overlapRight), min(overlapTop, overlapBottom));

            // 根据最小重叠方向解决碰撞
            if (minOverlap == overlapTop && m_fVelocityY > 0)
            {
                // 从上方碰撞（站在平台上）
                m_nY = rect.top - m_nHeight;
                m_fVelocityY = 0;
                m_bIsOnGround = TRUE;
                m_bIsJumping = FALSE;
            }
            else if (minOverlap == overlapBottom && m_fVelocityY < 0)
            {
                // 从下方碰撞（顶到东西）
                m_nY = rect.bottom;
                m_fVelocityY = 0;
                m_bIsJumping = FALSE;
            }
            else if (minOverlap == overlapLeft && m_fVelocityX > 0)
            {
                // 从左侧碰撞
                m_nX = rect.left - m_nWidth;
                m_fVelocityX = 0;
            }
            else if (minOverlap == overlapRight && m_fVelocityX < 0)
            {
                // 从右侧碰撞
                m_nX = rect.right;
                m_fVelocityX = 0;
            }
        }
    }

    // 检查是否掉出屏幕底部（临时地面）
    if (m_nY + m_nHeight > 500) // 地面在Y=500
    {
        m_nY = 500 - m_nHeight;
        m_fVelocityY = 0;
        m_bIsOnGround = TRUE;
        m_bIsJumping = FALSE;
    }
}
// 修改现有的移动方法，改为设置输入状态
void CMario::MoveLeft()
{
    m_bInputLeft = TRUE;
}

void CMario::MoveRight()
{
    m_bInputRight = TRUE;
}

void CMario::Jump()
{
    m_bInputJump = TRUE;
}

void CMario::StopMoving()
{
    m_bInputLeft = FALSE;
    m_bInputRight = FALSE;
}

// 新增：几何绘制备用方法
void CMario::DrawWithGeometry(CDC* pDC)
{
    // 这里保留原来的几何绘制代码作为备用
    COLORREF skinColor = RGB(255, 204, 153);
    COLORREF overallsColor = RGB(0, 0, 255);
    COLORREF hatColor = RGB(255, 0, 0);
    COLORREF shoeColor = RGB(139, 69, 19);

    // ... 原来的几何绘制代码 ...
}

// 设置状态并更新大小
void CMario::SetState(MarioState state)
{
    m_State = state;
    UpdateSize();
}
// 根据状态更新马里奥大小
void CMario::UpdateSize()
{
    switch (m_State)
    {
    case MarioState::SMALL:
        m_nWidth = CGameConfig::MARIO_SMALL_WIDTH;
        m_nHeight = CGameConfig::MARIO_SMALL_HEIGHT;
        break;
    case MarioState::BIG:
        m_nWidth = CGameConfig::MARIO_BIG_WIDTH;
        m_nHeight = CGameConfig::MARIO_BIG_HEIGHT;
        break;
    case MarioState::FIRE:
        m_nWidth = CGameConfig::MARIO_BIG_WIDTH;
        m_nHeight = CGameConfig::MARIO_BIG_HEIGHT;
        break;
    }
}

// 新增：获取脚部碰撞区域
CRect CMario::GetFeetRect() const
{
    // 脚部区域：身体底部的一小部分区域
    int feetHeight = 8;
    return CRect(m_nX + 5, m_nY + m_nHeight - feetHeight,
        m_nX + m_nWidth - 5, m_nY + m_nHeight);
}

// 新增：获取头部碰撞区域
CRect CMario::GetHeadRect() const
{
    // 头部区域：身体顶部的一小部分区域
    int headHeight = 10;
    return CRect(m_nX + 5, m_nY,
        m_nX + m_nWidth - 5, m_nY + headHeight);
}

// 新增：获取身体碰撞区域
CRect CMario::GetBodyRect() const
{
    // 身体区域：排除脚部和头部的中间部分
    return CRect(m_nX + 3, m_nY + 10,
        m_nX + m_nWidth - 3, m_nY + m_nHeight - 8);
}

// 新增：头部碰撞响应
void CMario::OnHeadCollision()
{
    m_fVelocityY = 0; // 停止上升
    m_bIsJumping = FALSE;
}

// 新增：脚部碰撞响应
void CMario::OnFeetCollision(int surfaceY)
{
    m_nY = surfaceY - m_nHeight; // 站在平台上
    m_fVelocityY = 0;
    m_bIsOnGround = TRUE;
    m_bIsJumping = FALSE;
    m_fJumpTime = 0.0f;
}

// 新增：左侧碰撞响应
void CMario::OnLeftCollision(int surfaceX)
{
    m_nX = surfaceX; // 停在障碍物右侧
    m_fVelocityX = 0;
}

// 新增：右侧碰撞响应
void CMario::OnRightCollision(int surfaceX)
{
    m_nX = surfaceX - m_nWidth; // 停在障碍物左侧
    m_fVelocityX = 0;
}


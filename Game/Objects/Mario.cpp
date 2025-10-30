
// Game/Objects/Mario.cpp
#include <afxwin.h>  // 直接包含MFC头文件
#include "Mario.h"

// 在静态常量定义后添加
const float CMario::GRAVITY = 0.8f;           // 增加重力
const float CMario::MAX_SPEED = 8.0f;         // 增加最大速度
const float CMario::ACCELERATION = 0.5f;      // 增加加速度
const float CMario::JUMP_FORCE = -20.0f;      // 增加跳跃力
// 构造函数
CMario::CMario() : CGameObject(100, 400, 30, 45)  // 调用基类构造函数
{

    m_fVelocityX = 0.0f;
    m_fVelocityY = 0.0f;

    // 初始化物理参数
    m_fAcceleration = ACCELERATION;
    m_fMaxSpeed = MAX_SPEED;
    m_fJumpForce = JUMP_FORCE;
    m_fGravity = GRAVITY;

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
    m_fMaxJumpTime = 0.3f;  // 最大跳跃持续时间0.3秒
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
void CMario::CheckCollisions(const std::vector<CRect>& platforms)
{
    // 重置碰撞状态
    m_bHeadCollision = FALSE;
    m_bFeetCollision = FALSE;
    m_bLeftCollision = FALSE;
    m_bRightCollision = FALSE;

    // 获取马里奥的各个碰撞区域
    CRect feetRect = GetFeetRect();
    CRect headRect = GetHeadRect();
    CRect bodyRect = GetBodyRect();

    // 检查与每个平台的碰撞
    for (const auto& platform : platforms)
    {
        CRect intersection;

        // 检查脚部碰撞（站在平台上）
        if (intersection.IntersectRect(&feetRect, &platform))
        {
            if (m_fVelocityY > 0) // 只有在下落时才响应脚部碰撞
            {
                OnFeetCollision(platform.top);
                m_bFeetCollision = TRUE;
            }
        }

        // 检查头部碰撞（顶到东西）
        if (intersection.IntersectRect(&headRect, &platform))
        {
            if (m_fVelocityY < 0) // 只有在上升时才响应头部碰撞
            {
                OnHeadCollision();
                m_bHeadCollision = TRUE;
            }
        }

        // 检查左侧碰撞
        CRect leftRect(bodyRect.left - 2, bodyRect.top + 5, bodyRect.left + 2, bodyRect.bottom - 5);
        if (intersection.IntersectRect(&leftRect, &platform))
        {
            if (m_fVelocityX < 0) // 只有在向左移动时才响应左侧碰撞
            {
                OnLeftCollision(platform.right);
                m_bLeftCollision = TRUE;
            }
        }

        // 检查右侧碰撞
        CRect rightRect(bodyRect.right - 2, bodyRect.top + 5, bodyRect.right + 2, bodyRect.bottom - 5);
        if (intersection.IntersectRect(&rightRect, &platform))
        {
            if (m_fVelocityX > 0) // 只有在向右移动时才响应右侧碰撞
            {
                OnRightCollision(platform.left);
                m_bRightCollision = TRUE;
            }
        }
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
// 绘制马里奥
void CMario::Draw(CDC* pDC)
{
    if (!m_bVisible) return;

    // 根据状态选择颜色
    COLORREF skinColor = RGB(255, 204, 153);  // 皮肤色
    COLORREF overallsColor = RGB(0, 0, 255);  // 工装裤蓝色
    COLORREF hatColor = RGB(255, 0, 0);       // 帽子红色
    COLORREF shoeColor = RGB(139, 69, 19);    // 鞋子棕色

    switch (m_State)
    {
    case MarioState::BIG:
    case MarioState::FIRE:
        // 大马里奥和火焰马里奥使用相同的尺寸，但火焰马里奥有特殊颜色
        if (m_State == MarioState::FIRE)
        {
            overallsColor = RGB(255, 165, 0); // 火焰马里奥的橙色工装裤
        }
        DrawBigMario(pDC, skinColor, overallsColor, hatColor, shoeColor);
        break;
    case MarioState::SMALL:
    default:
        DrawSmallMario(pDC, skinColor, overallsColor, hatColor, shoeColor);
        break;
    }
}

// 新增：绘制小马里奥
void CMario::DrawSmallMario(CDC* pDC, COLORREF skinColor, COLORREF overallsColor,
    COLORREF hatColor, COLORREF shoeColor)
{
    // 小马里奥的简化绘制 - 更精细的像素风格

    // 绘制帽子
    pDC->FillSolidRect(m_nX, m_nY, m_nWidth, m_nHeight / 4, hatColor);

    // 绘制脸部（皮肤色）
    pDC->FillSolidRect(m_nX, m_nY + m_nHeight / 4, m_nWidth, m_nHeight / 3, skinColor);

    // 绘制胡子（在脸部下方）
    int mustacheY = m_nY + m_nHeight / 4 + m_nHeight / 6;
    pDC->FillSolidRect(m_nX, mustacheY, m_nWidth, 2, RGB(0, 0, 0));

    // 绘制眼睛
    int eyeSize = m_nWidth / 8;
    int eyeX = (m_Direction == Direction::RIGHT) ?
        m_nX + m_nWidth - eyeSize * 2 :
        m_nX + eyeSize;
    pDC->FillSolidRect(eyeX, m_nY + m_nHeight / 4 + 2, eyeSize, eyeSize, RGB(0, 0, 0));

    // 绘制工装裤
    pDC->FillSolidRect(m_nX, m_nY + m_nHeight / 2, m_nWidth, m_nHeight / 3, overallsColor);

    // 绘制工装裤背带
    pDC->FillSolidRect(m_nX, m_nY + m_nHeight / 2, 3, m_nHeight / 6, RGB(0, 0, 0));
    pDC->FillSolidRect(m_nX + m_nWidth - 3, m_nY + m_nHeight / 2, 3, m_nHeight / 6, RGB(0, 0, 0));

    // 绘制鞋子
    pDC->FillSolidRect(m_nX, m_nY + m_nHeight - m_nHeight / 6, m_nWidth, m_nHeight / 6, shoeColor);
}

// 新增：绘制大马里奥
void CMario::DrawBigMario(CDC* pDC, COLORREF skinColor, COLORREF overallsColor,
    COLORREF hatColor, COLORREF shoeColor)
{
    // 大马里奥的绘制 - 更详细的像素风格

    // 绘制帽子
    pDC->FillSolidRect(m_nX, m_nY, m_nWidth, m_nHeight / 5, hatColor);

    // 绘制帽檐
    pDC->FillSolidRect(m_nX - 2, m_nY + m_nHeight / 5, m_nWidth + 4, 3, hatColor);

    // 绘制脸部（皮肤色）
    pDC->FillSolidRect(m_nX, m_nY + m_nHeight / 5 + 3, m_nWidth, m_nHeight / 4, skinColor);

    // 绘制胡子
    int mustacheY = m_nY + m_nHeight / 5 + m_nHeight / 8 + 3;
    pDC->FillSolidRect(m_nX, mustacheY, m_nWidth, 3, RGB(0, 0, 0));

    // 绘制眼睛
    int eyeSize = m_nWidth / 8;
    int eyeX = (m_Direction == Direction::RIGHT) ?
        m_nX + m_nWidth - eyeSize * 2 :
        m_nX + eyeSize;
    pDC->FillSolidRect(eyeX, m_nY + m_nHeight / 5 + 5, eyeSize, eyeSize, RGB(0, 0, 0));

    // 绘制工装裤上半部分
    pDC->FillSolidRect(m_nX, m_nY + m_nHeight / 5 + m_nHeight / 4 + 3, m_nWidth, m_nHeight / 4, overallsColor);

    // 绘制工装裤背带
    pDC->FillSolidRect(m_nX + 5, m_nY + m_nHeight / 5 + m_nHeight / 4 + 3, 4, m_nHeight / 6, RGB(0, 0, 0));
    pDC->FillSolidRect(m_nX + m_nWidth - 9, m_nY + m_nHeight / 5 + m_nHeight / 4 + 3, 4, m_nHeight / 6, RGB(0, 0, 0));

    // 绘制工装裤下半部分
    pDC->FillSolidRect(m_nX, m_nY + m_nHeight / 2 + m_nHeight / 4, m_nWidth, m_nHeight / 4, overallsColor);

    // 绘制袖子（皮肤色）
    pDC->FillSolidRect(m_nX - 3, m_nY + m_nHeight / 5 + m_nHeight / 4 + 3, 3, m_nHeight / 6, skinColor);
    pDC->FillSolidRect(m_nX + m_nWidth, m_nY + m_nHeight / 5 + m_nHeight / 4 + 3, 3, m_nHeight / 6, skinColor);

    // 绘制鞋子
    pDC->FillSolidRect(m_nX, m_nY + m_nHeight - m_nHeight / 6, m_nWidth, m_nHeight / 6, shoeColor);

    // 如果是火焰马里奥，绘制火焰效果
    if (m_State == MarioState::FIRE)
    {
        // 在帽子周围绘制简单的火焰效果
        for (int i = 0; i < 3; i++)
        {
            int flameX = m_nX + (m_nWidth / 4) * i;
            pDC->FillSolidRect(flameX, m_nY - 5, m_nWidth / 4, 3, RGB(255, 255, 0)); // 黄色火焰
        }
    }
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
        m_nWidth = 30;
        m_nHeight = 45;
        break;
    case MarioState::BIG:
        m_nWidth = 36;
        m_nHeight = 54;
        break;
    case MarioState::FIRE:
        m_nWidth = 36;
        m_nHeight = 54;
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
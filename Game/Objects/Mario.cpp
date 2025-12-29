// Game/Objects/Mario.cpp
#include <afxwin.h>  // 直接包含MFC头文件
#include "Mario.h"

// 构造函数：初始化马里奥的默认状态
CMario::CMario() : CGameObject(100, 400, CGameConfig::MARIO_SMALL_WIDTH, CGameConfig::MARIO_SMALL_HEIGHT)  // 调用基类构造函数，设置初始位置和大小
{
    // 初始化速度
    m_fVelocityX = 0.0f;
    m_fVelocityY = 0.0f;
    m_fPreviousVelocityY = 0.0f;

    // 使用全局配置的物理参数
    m_fAcceleration = CGameConfig::MARIO_ACCELERATION;
    m_fMaxSpeed = CGameConfig::MARIO_MAX_SPEED;
    m_fJumpForce = CGameConfig::MARIO_JUMP_FORCE;
    m_fGravity = CGameConfig::GRAVITY;

    // 跳跃相关参数
    m_fJumpHoldTime = 0.0f;
    m_fMaxJumpHoldTime = CGameConfig::MARIO_JUMP_HOLD_TIME;
    m_fJumpGravity = CGameConfig::MARIO_JUMP_GRAVITY;
    m_fFallGravity = CGameConfig::MARIO_FALL_GRAVITY;

    // 跳跃峰值相关
    m_bReachedJumpPeak = FALSE;
    m_fPeakHoldTime = 0.0f;
    m_fMaxPeakHoldTime = CGameConfig::MARIO_PEAK_HOLD_TIME;

    // 初始化状态
    m_bIsJumping = FALSE;
    m_bIsOnGround = FALSE;
    m_bJumpPressed = FALSE;
    m_bJumpWasPressed = FALSE;
    m_bCanJump = TRUE;
    m_bIsMoving = FALSE;

    m_Direction = Direction::RIGHT;
    m_State = MarioState::SMALL;
    m_Skin = MarioSkin::MARIO;

    // 初始化输入状态
    m_bInputLeft = FALSE;
    m_bInputRight = FALSE;
    m_bInputJump = FALSE;

    // 初始化死亡状态
    m_bIsDying = FALSE;
    m_fDeathTimer = 0.0f;
    m_fDeathJumpVelocity = -10.0f;
    m_fDeathAnimationTime = 2.0f;

    // Miku动画相关
    m_fMikuAnimTimer = 0.0f;
    m_nMikuCurrentFrame = 1;

    // 根据状态更新大小
    UpdateSize();
}

// 带参数的构造函数：使用委托构造，先调用默认构造函数，再设置指定位置
CMario::CMario(int x, int y) : CMario()
{
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
// 新增：使用精灵渲染器绘制马里奥
void CMario::DrawWithSprite(CDC* pDC, int screenX, int screenY)
{
    if (!m_bVisible) return; // 如果不可见，则不绘制

    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = nullptr;
    SSpriteCoord spriteCoord;

    // 根据皮肤选择图集
    if (m_Skin == MarioSkin::MIKU)
    {
        pBitmap = resMgr.GetBitmap(CSpriteConfig::GetSpritesheetForMiku());
        // 使用新的动画系统获取Miku精灵坐标
        spriteCoord = GetMikuSpriteCoord();
    }
    else
    {
        pBitmap = resMgr.GetBitmap(CSpriteConfig::GetSpritesheetForMario());

        // 死亡状态使用特定精灵
        if (m_bIsDying || m_State == MarioState::DEAD)
        {
            // 死亡时使用跳跃或站立的精灵（根据需要选择）
            // 这里使用小马里奥跳跃精灵作为死亡状态
            spriteCoord = CSpriteConfig::MARIO_SMALL_DEAD;
        }
        else
        {
            // 正常的绘制逻辑
            // 根据状态和动作选择精灵（全部使用向右的贴图）
            switch (m_State)
            {
            case MarioState::SMALL:
                if (m_bIsJumping)
                    spriteCoord = CSpriteConfig::MARIO_SMALL_JUMP_RIGHT;
                else if (IsMoving())
                {
                    // 三个行走动作循环
                    DWORD animTime = GetTickCount64() % CGameConfig::MARIO_WALK_SPEED;
                    if (animTime < 250)
                        spriteCoord = CSpriteConfig::MARIO_SMALL_WALK1_RIGHT;
                    else if (animTime < 500)
                        spriteCoord = CSpriteConfig::MARIO_SMALL_WALK2_RIGHT;
                    else
                        spriteCoord = CSpriteConfig::MARIO_SMALL_WALK3_RIGHT;
                }
                else
                    spriteCoord = CSpriteConfig::MARIO_SMALL_STAND_RIGHT;
                break;

            case MarioState::BIG:
                if (m_bIsJumping)
                    spriteCoord = CSpriteConfig::MARIO_BIG_JUMP_RIGHT;
                else if (IsMoving())
                {
                    DWORD animTime = GetTickCount64() % CGameConfig::MARIO_WALK_SPEED;
                    if (animTime < 250)
                        spriteCoord = CSpriteConfig::MARIO_BIG_WALK1_RIGHT;
                    else if (animTime < 500)
                        spriteCoord = CSpriteConfig::MARIO_BIG_WALK2_RIGHT;
                    else
                        spriteCoord = CSpriteConfig::MARIO_BIG_WALK3_RIGHT;
                }
                else
                    spriteCoord = CSpriteConfig::MARIO_BIG_STAND_RIGHT;
                break;

            case MarioState::FIRE:
                if (m_bIsJumping)
                    spriteCoord = CSpriteConfig::MARIO_FIRE_JUMP_RIGHT;
                else if (IsMoving())
                {
                    DWORD animTime = GetTickCount64() % CGameConfig::MARIO_WALK_SPEED;
                    if (animTime < 250)
                        spriteCoord = CSpriteConfig::MARIO_FIRE_WALK1_RIGHT;
                    else if (animTime < 500)
                        spriteCoord = CSpriteConfig::MARIO_FIRE_WALK2_RIGHT;
                    else
                        spriteCoord = CSpriteConfig::MARIO_FIRE_WALK3_RIGHT;
                }
                else
                    spriteCoord = CSpriteConfig::MARIO_FIRE_STAND_RIGHT;
                break;
            }
        }
    }

    // 判断是否需要水平翻转
    BOOL flipHorizontal = (m_Direction == Direction::LEFT);

    // 使用精灵渲染器绘制，添加翻转参数
    CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX, screenY,
        m_nWidth, m_nHeight, spriteCoord.x, spriteCoord.y,
        spriteCoord.width, spriteCoord.height, flipHorizontal);
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
BOOL CMario::IsMoving() const
{
    // 只有当水平速度的绝对值大于一个很小的阈值时，才认为是在移动
    return abs(m_fVelocityX) > 0.1f;
}
// 根据皮肤更新资源和尺寸
void CMario::UpdateSkinResources()
{
    // 可以根据需要调整不同皮肤的大小
    switch (m_Skin)
    {
    case MarioSkin::MARIO:
        // 使用默认的马里奥尺寸
        UpdateSize(); // 调用原有的尺寸更新方法
        break;
    case MarioSkin::MIKU:
        // 为Miku设置合适的尺寸
        m_nWidth  = CGameConfig::MIKU_WIDTH;   // 根据Miku图集调整
        m_nHeight = CGameConfig::MIKU_HEIGHT;  // 根据Miku图集调整
        break;
    }
}
// 修改现有的Draw方法

// 设置皮肤方法
void CMario::SetSkin(MarioSkin skin)
{
    if (m_Skin != skin)
    {
        // 保存当前状态的底部位置（为了切换皮肤后脚部位置不变）
        int currentBottom = m_nY + m_nHeight;

        m_Skin = skin;

        // 更新皮肤相关资源，这可能会改变马里奥的高度
        UpdateSkinResources();

        // 调整Y坐标，保持脚部位置不变
        m_nY = currentBottom - m_nHeight;

        // 重置动画状态，防止切换皮肤后动画错乱
        m_fMikuAnimTimer = 0.0f;
        m_nMikuCurrentFrame = 1;
    }
}
// 更新马里奥状态
void CMario::Update(float deltaTime)
{
    // 如果正在死亡动画中
    if (m_bIsDying)
    {
        UpdateDeathAnimation(deltaTime);
        return;
    }

    // 应用物理（包括跳跃逻辑）
    ApplyPhysics(deltaTime);

    // 更新移动状态
    m_bIsMoving = (abs(m_fVelocityX) > 0.1f);

    // 边界检查
    if (m_nX < 0)
    {
        m_nX = 0;
        m_fVelocityX = 0;
    }

    // 更新Miku动画
    if (m_Skin == MarioSkin::MIKU)
    {
        UpdateMikuAnimation(deltaTime);
    }
}
// 添加死亡动画更新方法
void CMario::UpdateDeathAnimation(float deltaTime)
{
    m_fDeathTimer += deltaTime;

    // 应用重力，但减弱一些
    m_fVelocityY += m_fGravity * 0.8f;

    // 更新位置
    m_nX += static_cast<int>(m_fVelocityX);
    m_nY += static_cast<int>(m_fVelocityY);

    // 死亡动画结束后设置为完全死亡状态
    if (m_fDeathTimer >= m_fDeathAnimationTime)
    {
        m_bIsDying = FALSE;
        m_State = MarioState::DEAD;
        TRACE(_T("死亡动画结束，马里奥进入完全死亡状态\n"));
    }
}
// Miku动画更新
void CMario::UpdateMikuAnimation(float deltaTime)
{
    if (m_bIsMoving && m_bIsOnGround)
    {
        // 行走动画
        m_fMikuAnimTimer += deltaTime;
        float frameTime = 0.05f; // 每帧显示时间，可调整来控制动画速度

        if (m_fMikuAnimTimer >= frameTime)
        {
            m_fMikuAnimTimer = 0.0f;
            m_nMikuCurrentFrame = (m_nMikuCurrentFrame + 1) % MIKU_WALK_FRAMES;
        }
    }
    else if (m_bIsJumping || !m_bIsOnGround)
    {
        // 跳跃或下落状态，使用特定帧
        m_nMikuCurrentFrame = 1; // 使用第一帧作为跳跃帧，或者根据需要调整
        m_fMikuAnimTimer = 0.0f;
    }
    else
    {
        // 站立状态
        m_nMikuCurrentFrame = 1; // 使用第一帧作为站立帧
        m_fMikuAnimTimer = 0.0f;
    }
}

// 获取Miku精灵坐标
SSpriteCoord CMario::GetMikuSpriteCoord() const
{
    int frameX = (m_nMikuCurrentFrame+1) * MIKU_FRAME_WIDTH;

    // 假设图集从(0,0)开始横向排列所有帧
    return SSpriteCoord(frameX, 0, MIKU_FRAME_WIDTH, MIKU_FRAME_HEIGHT);
}
// 新的输入处理方法
void CMario::HandleInput(BOOL left, BOOL right, BOOL jump)
{
    // 保存上一帧的跳跃状态
    m_bJumpWasPressed = m_bJumpPressed;
    m_bJumpPressed = jump;

    // 处理左右移动
    if (left && !right)
    {
        // 向左加速
        m_fVelocityX -= m_fAcceleration;
        if (m_fVelocityX < -m_fMaxSpeed)
            m_fVelocityX = -m_fMaxSpeed;
        m_Direction = Direction::LEFT;
    }
    else if (right && !left)
    {
        // 向右加速
        m_fVelocityX += m_fAcceleration;
        if (m_fVelocityX > m_fMaxSpeed)
            m_fVelocityX = m_fMaxSpeed;
        m_Direction = Direction::RIGHT;
    }
    else
    {
        // 没有输入时减速
        if (m_fVelocityX > 0)
        {
            m_fVelocityX -= m_fAcceleration * 1.5f;
            if (m_fVelocityX < 0) m_fVelocityX = 0;
        }
        else if (m_fVelocityX < 0)
        {
            m_fVelocityX += m_fAcceleration * 1.5f;
            if (m_fVelocityX > 0) m_fVelocityX = 0;
        }
    }

    // 处理跳跃（原版马里奥逻辑）
    if (jump)
    {
        // 跳跃键被按下
        if (!m_bJumpWasPressed && m_bCanJump && m_bIsOnGround)
        {
            // 开始跳跃
            StartJump();
        }

        // 跳跃过程中持续按住跳跃键
        if (m_bIsJumping && m_fJumpHoldTime < m_fMaxJumpHoldTime)
        {
            m_fJumpHoldTime += 1.0f / 60.0f; // 假设60fps
        }
    }
    else
    {
        // 跳跃键被释放
        if (m_bIsJumping)
        {
            // 释放跳跃键时，立即开始快速下落
            m_fJumpHoldTime = m_fMaxJumpHoldTime;
        }
    }
}
// 开始跳跃方法
void CMario::StartJump()
{
    m_bIsJumping = TRUE;
    m_bIsOnGround = FALSE;
    m_bCanJump = FALSE;
    m_fJumpHoldTime = 0.0f;
    m_bReachedJumpPeak = FALSE;
    m_fPeakHoldTime = 0.0f;

    // 给予跳跃初始速度（比原版小，因为后续会持续加速）
    m_fVelocityY = m_fJumpForce * 0.7f;

    TRACE(_T("开始跳跃！初始速度: %.2f\n"), m_fVelocityY);
}
// 应用物理效果
// 修改ApplyPhysics方法
void CMario::ApplyPhysics(float deltaTime)
{
    // 保存上一帧的速度用于峰值检测
    m_fPreviousVelocityY = m_fVelocityY;

    // 跳跃逻辑
    if (m_bIsJumping)
    {
        // 跳跃上升阶段
        if (!m_bReachedJumpPeak)
        {
            // 使用较小的跳跃重力
            m_fVelocityY += m_fJumpGravity;

            // 检测是否到达跳跃峰值（速度从负变正）
            if (m_fPreviousVelocityY < 0 && m_fVelocityY >= 0)
            {
                m_bReachedJumpPeak = TRUE;
                m_fPeakHoldTime = 0.0f;
                TRACE(_T("到达跳跃峰值！\n"));
            }

            // 跳跃键持续按住时的额外上升力
            if (m_fJumpHoldTime < m_fMaxJumpHoldTime)
            {
                // 持续给予上升力，模拟原版马里奥的长按跳得高
                float holdFactor = 1.0f - (m_fJumpHoldTime / m_fMaxJumpHoldTime);
                m_fVelocityY += m_fJumpForce * 0.05f * holdFactor;
            }
        }
        else
        {
            // 峰值悬停阶段
            if (m_fPeakHoldTime < m_fMaxPeakHoldTime)
            {
                m_fPeakHoldTime += deltaTime;
                // 在峰值处给予轻微的速度调整，模拟悬停效果
                m_fVelocityY = m_fVelocityY * 0.8f;
            }
            else
            {
                // 悬停结束后正常下落
                m_fVelocityY += m_fFallGravity;
            }
        }

        // 跳跃时间过长或速度过大时强制结束跳跃
        if (m_fJumpHoldTime >= m_fMaxJumpHoldTime && m_fVelocityY > 0)
        {
            // 切换到下落状态
            m_bIsJumping = FALSE;
        }
    }
    else if (!m_bIsOnGround)
    {
        // 正常下落（非跳跃状态）
        m_fVelocityY += m_fFallGravity;
    }

    // 限制最大下落速度
    if (m_fVelocityY > CGameConfig::MARIO_MAX_FALL_SPEED)
    {
        m_fVelocityY = CGameConfig::MARIO_MAX_FALL_SPEED;
    }

    // 限制最大上升速度
    if (m_fVelocityY < m_fJumpForce * 1.2f)
    {
        m_fVelocityY = m_fJumpForce * 1.2f;
    }

    // 更新位置
    m_nX += static_cast<int>(m_fVelocityX);
    m_nY += static_cast<int>(m_fVelocityY);
}
// 碰撞检测与响应
void CMario::CheckCollisions(const std::vector<CRect>& solidRects)
{
    // 死亡状态下不进行碰撞检测
    if (m_bIsDying || m_State == MarioState::DEAD)
        return;

    // 重置地面状态
    BOOL wasOnGround = m_bIsOnGround;
    m_bIsOnGround = FALSE;

    // 先检测垂直碰撞（处理落地）
    CRect marioRect = GetRect();

    for (const auto& rect : solidRects)
    {
        CRect intersection;
        if (intersection.IntersectRect(&marioRect, &rect))
        {
            // 计算重叠
            int overlapTop = marioRect.bottom - rect.top;
            int overlapBottom = rect.bottom - marioRect.top;
            int overlapLeft = marioRect.right - rect.left;
            int overlapRight = rect.right - marioRect.left;

            // 找到最小重叠方向
            int dx = min(overlapLeft, overlapRight);
            int dy = min(overlapTop, overlapBottom);

            if (dy <= dx) // 垂直碰撞
            {
                if (dy == overlapTop && m_fVelocityY >= 0) // 落地
                {
                    // 更精确的落地检测
                    int feetHeight = 4; // 脚部检测区域高度
                    CRect feetCheck(marioRect.left + 4, marioRect.bottom - feetHeight,
                        marioRect.right - 4, marioRect.bottom);

                    CRect feetIntersection;
                    if (feetIntersection.IntersectRect(&feetCheck, &rect))
                    {
                        // 落地成功
                        m_nY = rect.top - m_nHeight;
                        m_fVelocityY = 0;
                        m_bIsOnGround = TRUE;
                        m_bIsJumping = FALSE;
                        m_bCanJump = TRUE; // 重置跳跃能力

                        // 重置跳跃状态
                        m_fJumpHoldTime = 0.0f;
                        m_bReachedJumpPeak = FALSE;
                        m_fPeakHoldTime = 0.0f;
                    }
                }
                else if (dy == overlapBottom && m_fVelocityY < 0) // 撞头
                {
                    m_nY = rect.bottom;
                    m_fVelocityY = 0;

                    // 撞头时立即停止跳跃
                    if (m_bIsJumping)
                    {
                        m_bIsJumping = FALSE;
                        m_bReachedJumpPeak = TRUE; // 强制进入下落
                    }
                }
            }
            else // 水平碰撞
            {
                if (dx == overlapLeft && m_fVelocityX > 0) // 右侧碰撞
                {
                    m_nX = rect.left - m_nWidth;
                    m_fVelocityX = 0;
                }
                else if (dx == overlapRight && m_fVelocityX < 0) // 左侧碰撞
                {
                    m_nX = rect.right;
                    m_fVelocityX = 0;
                }
            }
        }
    }

    // 边缘情况：如果上一帧在地面，这一帧不在，但速度很小，强制认为在地面
    if (wasOnGround && !m_bIsOnGround && abs(m_fVelocityY) < 0.5f)
    {
        // 向下探测几个像素
        CRect probeRect = marioRect;
        probeRect.OffsetRect(0, 3);

        for (const auto& rect : solidRects)
        {
            CRect tmp;
            if (tmp.IntersectRect(&probeRect, &rect))
            {
                m_bIsOnGround = TRUE;
                m_fVelocityY = 0;
                m_nY = rect.top - m_nHeight;
                break;
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

// 设置马里奥的状态（小、大、火焰）并更新大小
void CMario::SetState(MarioState state)
{
    // 保存当前状态的底部Y坐标
    int currentBottom = m_nY + m_nHeight;
    m_State = state;
    UpdateSize(); // 根据新状态更新宽度和高度
    // 调整Y坐标，保持脚部位置不变
    m_nY = currentBottom - m_nHeight;
}
// 根据状态更新马里奥的宽度和高度
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

// 获取脚部碰撞区域
CRect CMario::GetFeetRect() const
{
    // 脚部区域：身体底部的一小块矩形，用于检测是否踩在地面上
    int feetHeight = 8;
    return CRect(m_nX + 5, m_nY + m_nHeight - feetHeight,
        m_nX + m_nWidth - 5, m_nY + m_nHeight);
}

// 获取头部碰撞区域
CRect CMario::GetHeadRect() const
{
    // 头部区域：身体顶部的一小块矩形，用于检测是否撞到上方的物体
    int headHeight = 4;  // 检测区域的高度
    int sideMargin = 8;  // 从两侧缩进的边距，使检测更精确

    if (m_State == MarioState::BIG || m_State == MarioState::FIRE)
    {
        headHeight = 6;
        sideMargin = 10;
    }

    return CRect(m_nX + sideMargin, m_nY,
        m_nX + m_nWidth - sideMargin, m_nY + headHeight);
}
// 获取身体碰撞区域
CRect CMario::GetBodyRect() const
{
    // 身体区域：排除脚部和头部的中间部分，用于检测与墙壁等的碰撞
    return CRect(m_nX + 3, m_nY + 10,
        m_nX + m_nWidth - 3, m_nY + m_nHeight - 8);
}

// 添加死亡触发方法
void CMario::Die()
{
    if (m_bIsDying || m_State == MarioState::DEAD)
        return;

    TRACE(_T("马里奥死亡！当前位置: (%d, %d)\n"), m_nX, m_nY);

    // 设置死亡状态
    m_bIsDying = TRUE;
    m_fDeathTimer = 0.0f;
    m_State = MarioState::DEAD;
    // 保存当前状态用于重生
    // 死亡时给予向上的速度
    m_fVelocityY = m_fDeathJumpVelocity;
    m_fVelocityX = 0; // 水平速度清零

    // 停止所有输入
    m_bInputLeft = FALSE;
    m_bInputRight = FALSE;
    m_bInputJump = FALSE;

    // 播放死亡音效（如果有的话）
    // PlayDeathSound();
}

// 添加重生方法
void CMario::Respawn()
{
    TRACE(_T("马里奥重生\n"));

    // 重置状态
    m_bIsDying = FALSE;
    m_fDeathTimer = 0.0f;
    m_State = MarioState::SMALL; // 重生为小马里奥

    // 重置物理状态
    m_fVelocityX = 0.0f;
    m_fVelocityY = 0.0f;
    m_bIsJumping = FALSE;
    m_bIsOnGround = FALSE;

    // 重置输入状态
    m_bInputLeft = FALSE;
    m_bInputRight = FALSE;
    m_bInputJump = FALSE;

    // 更新大小
    UpdateSize();
	//更新皮肤资源
    UpdateSkinResources();
 
}


// 从高处掉落死亡方法
void CMario::DieFromFall()
{
    if (m_bIsDying || m_State == MarioState::DEAD)
        return;

    TRACE(_T("马里奥掉落死亡！当前位置: (%d, %d)\n"), m_nX, m_nY);

    // 设置死亡状态
    m_bIsDying = TRUE;
    m_fDeathTimer = 0.0f;

    // 掉落死亡时给予更强的向上弹跳力，制造戏剧效果
    m_fVelocityY = m_fDeathJumpVelocity * 1.5f; // 比普通死亡跳得更高
    m_fVelocityX = 0; // 水平速度清零

    // 停止所有输入
    m_bInputLeft = FALSE;
    m_bInputRight = FALSE;
    m_bInputJump = FALSE;

    // 播放死亡音效（如果有的话）
    // PlayDeathSound();
}
// Game/Objects/Mario.cpp
#include <afxwin.h>  // 直接包含MFC头文件
#include "Mario.h"

// 构造函数：初始化马里奥的默认状态
CMario::CMario() : CGameObject(100, 400, CGameConfig::MARIO_SMALL_WIDTH, CGameConfig::MARIO_SMALL_HEIGHT)  // 调用基类构造函数，设置初始位置和大小
{

    // 初始化速度
    m_fVelocityX = 0.0f;
    m_fVelocityY = 0.0f;
    // 初始化皮肤和动画
    m_Skin = MarioSkin::MARIO;
    m_fMikuAnimTimer = 0.0f;
    m_nMikuCurrentFrame = 1;
    // 使用全局配置的物理参数
    m_fAcceleration = CGameConfig::MARIO_ACCELERATION;
    m_fMaxSpeed = CGameConfig::MARIO_MAX_SPEED;
    m_fJumpForce = CGameConfig::MARIO_JUMP_FORCE;
    m_fGravity = CGameConfig::GRAVITY;

    m_Skin = MarioSkin::MARIO; // 默认使用马里奥皮肤
    // 初始化状态
    m_bIsJumping = FALSE;
    m_bIsOnGround = FALSE;
    m_bIsMoving = FALSE;

    m_Direction = Direction::RIGHT; // 默认朝向右
    m_State = MarioState::SMALL; // 默认是小马里奥
    // 初始化输入状态
    m_bInputLeft = FALSE;
    m_bInputRight = FALSE;
    m_bInputJump = FALSE;

    // 初始化跳跃相关
    m_fJumpTime = 0.0f;
    m_fMaxJumpTime = CGameConfig::MARIO_JUMP_MAX_TIME;  // 使用全局配置
    m_bCanJump = TRUE;

    // 初始化死亡状态
    m_bIsDying = FALSE;
    m_fDeathTimer = 0.0f;
    m_fDeathJumpVelocity = -12.0f; // 死亡时向上的弹跳力
    m_fDeathAnimationTime = 2.0f;  // 死亡动画持续2秒
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
void CMario::DrawWithSprite(CDC* pDC, int screenX, int screenY)
{
    if (!m_bVisible) return; // 如果不可见，则不绘制
    if (m_bIsDying || m_State == MarioState::DEAD)
    {
        // 死亡时使用半透明效果
        CDC memDC;
        memDC.CreateCompatibleDC(pDC);
        CBitmap tempBitmap;
        tempBitmap.CreateCompatibleBitmap(pDC, m_nWidth, m_nHeight);
        CBitmap* pOldMemBitmap = memDC.SelectObject(&tempBitmap);

        // 清空临时DC
        memDC.FillSolidRect(0, 0, m_nWidth, m_nHeight, RGB(0, 0, 0));

        // 正常绘制到临时DC
        CResourceManager& resMgr = CResourceManager::GetInstance();
        CBitmap* pBitmap = nullptr;
        SSpriteCoord spriteCoord;

        if (m_Skin == MarioSkin::MIKU)
        {
            pBitmap = resMgr.GetBitmap(CSpriteConfig::GetSpritesheetForMiku());
            spriteCoord = GetMikuSpriteCoord();
        }
        else
        {
            pBitmap = resMgr.GetBitmap(CSpriteConfig::GetSpritesheetForMario());
            // 死亡时使用特定的精灵，比如向下掉落的样子
            spriteCoord = CSpriteConfig::MARIO_SMALL_DEAD; // 需要在SpriteConfig中添加这个坐标
        }

        BOOL flipHorizontal = (m_Direction == Direction::RIGHT);

        CSpriteRenderer::DrawSprite(&memDC, pBitmap, 0, 0,
            m_nWidth, m_nHeight, spriteCoord.x, spriteCoord.y,
            spriteCoord.width, spriteCoord.height, flipHorizontal);

        // 使用半透明方式绘制到屏幕
        BLENDFUNCTION blend = { 0 };
        blend.BlendOp = AC_SRC_OVER;
        blend.BlendFlags = 0;
        blend.SourceConstantAlpha = 128; // 50% 透明度
        blend.AlphaFormat = 0;

        pDC->AlphaBlend(screenX, screenY, m_nWidth, m_nHeight,
            &memDC, 0, 0, m_nWidth, m_nHeight, blend);

        memDC.SelectObject(pOldMemBitmap);
        memDC.DeleteDC();
        tempBitmap.DeleteObject();
    }
    else
    {
        // 正常的绘制逻辑（你原有的代码）
        CResourceManager& resMgr = CResourceManager::GetInstance();
        CBitmap* pBitmap = nullptr;
        SSpriteCoord spriteCoord;
        // 根据皮肤选择图集和精灵坐标
        if (m_Skin == MarioSkin::MIKU)
        {
            // 使用Miku图集
            pBitmap = resMgr.GetBitmap(CSpriteConfig::GetSpritesheetForMiku());

            // 使用新的动画系统获取Miku精灵坐标
            spriteCoord = GetMikuSpriteCoord();
        }
        else
        {
            // 原始马里奥皮肤（保持原有逻辑）
            pBitmap = resMgr.GetBitmap(CSpriteConfig::GetSpritesheetForMario());
            // 根据状态和动作选择精灵（全部使用向右的贴图）
            switch (m_State)
            {
            case MarioState::SMALL:
                if (m_bIsJumping)
                    spriteCoord = CSpriteConfig::MARIO_SMALL_JUMP_RIGHT;
                else if (IsMoving())
                {
                    // 三个行走动作循环
                    DWORD animTime = GetTickCount64() % CGameConfig::MARIO_WALK_SPEED; // 一个完整循环
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
                    // 三个行走动作循环
                    DWORD animTime = GetTickCount64() % CGameConfig::MARIO_WALK_SPEED; // 一个完整循环
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
                    // 三个行走动作循环
                    DWORD animTime = GetTickCount64() % CGameConfig::MARIO_WALK_SPEED; // 一个完整循环
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
        // 判断是否需要水平翻转
        BOOL flipHorizontal = (m_Direction == Direction::LEFT);

        // 使用精灵渲染器绘制，添加翻转参数
        CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX, screenY,
            m_nWidth, m_nHeight, spriteCoord.x, spriteCoord.y,
            spriteCoord.width, spriteCoord.height, flipHorizontal);
    }
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
void CMario::Draw(CDC* pDC)
{
    DrawWithSprite(pDC, m_nX, m_nY);
}
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
        return; // 死亡状态下不执行正常更新
    }
    // 处理输入
    HandleInput(m_bInputLeft, m_bInputRight, m_bInputJump);

    // 应用物理
    ApplyPhysics(deltaTime);
    // 更新Miku动画
    if (m_Skin == MarioSkin::MIKU)
    {
        UpdateMikuAnimation(deltaTime);
    }
    // 更新移动状态 - 基于实际速度
    m_bIsMoving = (abs(m_fVelocityX) > 0.01f);

    // 简单的边界检查（临时）
    if (m_nX < 0)
    {
        m_nX = 0;
        m_fVelocityX = 0;
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
    // 处理左右移动
    if (left && !right)
    {
        m_fVelocityX = -m_fMaxSpeed;
        m_Direction = Direction::LEFT;
    }
    else if (right && !left)
    {
        m_fVelocityX = m_fMaxSpeed;
        m_Direction = Direction::RIGHT;
    }
    else
    {
        // 没有移动输入时应用摩擦力，使马里奥减速停止
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
        m_bCanJump = FALSE; // 防止在空中连续跳跃
        m_fJumpTime = 0.0f;
    }

    // 跳跃键释放时重置跳跃能力
    if (!jump)
    {
        m_bCanJump = TRUE;
    }

    // 跳跃持续时间控制（实现长按跳得更高）
    if (m_bIsJumping && jump && m_fJumpTime < m_fMaxJumpTime)
    {
        m_fVelocityY = m_fJumpForce * (1.0f - m_fJumpTime / m_fMaxJumpTime);
        m_fJumpTime += 1.0f / 60.0f; // 假设60fps
    }
}

// 应用物理效果
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
// 碰撞检测与响应
void CMario::CheckCollisions(const std::vector<CRect>& solidRects)
{
    // 死亡状态下不进行碰撞检测
    if (m_bIsDying || m_State == MarioState::DEAD)
        return;

    // 步骤 1: 在每帧开始时，先假设马里奥不在地面上。
    m_bIsOnGround = FALSE;

    // 步骤 2: 遍历所有实体矩形，进行碰撞检测和位置修正。
    for (const auto& rect : solidRects)
    {
        CRect intersection;
        CRect marioRect = GetRect();

        if (intersection.IntersectRect(&marioRect, &rect))
        {
            // 计算重叠区域的宽度和高度
            int overlapLeft = marioRect.right - rect.left;
            int overlapRight = rect.right - marioRect.left;
            int overlapTop = marioRect.bottom - rect.top;
            int overlapBottom = rect.bottom - marioRect.top;

            // 找到最小的重叠量，以确定碰撞方向
            int dx = min(overlapLeft, overlapRight);
            int dy = min(overlapTop, overlapBottom);

            if (dy <= dx) // 垂直方向碰撞
            {
                if (dy == overlapTop && m_fVelocityY >= 0) // 从上方碰撞（落地）
                {
                    m_nY = rect.top - m_nHeight; // 修正位置到平台上方
                    m_fVelocityY = 0; // 垂直速度清零
                    m_bIsOnGround = TRUE; // 标记在地面上
                    m_bIsJumping = FALSE; // 结束跳跃状态
                }
                else if (dy == overlapBottom && m_fVelocityY < 0) // 从下方碰撞（撞头）
                {
                    m_nY = rect.bottom; // 修正位置到平台下方
                    m_fVelocityY = 0; // 垂直速度清零
                    m_bIsJumping = FALSE; // 结束跳跃状态
                }
            }
            else // 水平方向碰撞
            {
                if (dx == overlapLeft && m_fVelocityX > 0) // 从左侧碰撞
                {
                    m_nX = rect.left - m_nWidth; // 修正位置到平台左侧
                    m_fVelocityX = 0; // 水平速度清零
                }
                else if (dx == overlapRight && m_fVelocityX < 0) // 从右侧碰撞
                {
                    m_nX = rect.right; // 修正位置到平台右侧
                    m_fVelocityX = 0; // 水平速度清零
                }
            }
        }
    }

    // 步骤 3: 地面状态的滞后处理，防止在平台边缘抖动。
    // 如果经过上面的精确碰撞修正后，马里奥仍被判定为“空中”，我们再做一次宽松的检查。
    if (!m_bIsOnGround)
    {
        CRect feetProbe = GetFeetRect();
        // 将探测框向下延伸几个像素，形成一个“缓冲区”。
        feetProbe.OffsetRect(0, 2);

        for (const auto& rect : solidRects)
        {
            CRect tmp;
            if (tmp.IntersectRect(&feetProbe, &rect))
            {
                // 如果探测框接触到了地面，我们就强制认为马里奥在地面上。
                // 注意：这里我们不修正位置，只更新状态标志，以增加稳定性。
                m_bIsOnGround = TRUE;
                m_bIsJumping = FALSE;
                // 如果此时有微小的垂直速度，也将其清零。
                if (m_fVelocityY > 0) {
                    m_fVelocityY = 0;
                }
                // 找到一个接触点就足够了，跳出循环。
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

// 头部碰撞响应
void CMario::OnHeadCollision()
{
    m_fVelocityY = 0; // 停止上升
    m_bIsJumping = FALSE;
}

// 脚部碰撞响应
void CMario::OnFeetCollision(int surfaceY)
{
    m_nY = surfaceY - m_nHeight; // 将马里奥放在平台上
    m_fVelocityY = 0; // 垂直速度清零
    m_bIsOnGround = TRUE; // 标记在地面上
    m_bIsJumping = FALSE; // 结束跳跃
    m_fJumpTime = 0.0f; // 重置跳跃计时器
}

// 左侧碰撞响应
void CMario::OnLeftCollision(int surfaceX)
{
    m_nX = surfaceX; // 将马里奥停在障碍物右侧
    m_fVelocityX = 0; // 水平速度清零
}

// 右侧碰撞响应
void CMario::OnRightCollision(int surfaceX)
{
    m_nX = surfaceX - m_nWidth; // 将马里奥停在障碍物左侧
    m_fVelocityX = 0; // 水平速度清零
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

    // 设置重生位置（可以在外部设置）
    // m_nX = respawnX;
    // m_nY = respawnY;
}


// 新增：从高处掉落死亡方法
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
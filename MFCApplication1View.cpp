// MFCApplication1View.cpp: CMFCApplication1View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"
#include "Game/Core/ResourceIDs.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()
#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG

// CMFCApplication1View 构造/析构

CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: 在此处添加构造代码
		// 初始化游戏变量

    m_nCameraX = 0;
    m_nCameraY = 0;
	m_pOldBitmap = nullptr;
	m_nTimerID = 0;
    // 初始化输入状态
    m_bKeyLeft = FALSE;
    m_bKeyRight = FALSE;
    m_bKeyJump = FALSE;
    // 初始化帧率相关变量
    m_dwLastTime = std::chrono::steady_clock::now();
    m_nFrameCount = 0;
    m_fDeltaTime = 0.016f; // 默认1/60秒
    m_fSmoothedFPS = 60.0f; // 默认60帧
}

CMFCApplication1View::~CMFCApplication1View()
{
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
        // 设置窗口样式 - 去掉最大化和调整大小
    cs.style &= ~WS_MAXIMIZEBOX;
    cs.style &= ~WS_THICKFRAME;
	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View 打印

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCApplication1View 诊断


// CMFCApplication1View 消息处理程序

int CMFCApplication1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	  // 初始化游戏
	InitializeGame();

	// 设置定时器 - 16ms ≈ 60fps
	m_nTimerID = SetTimer(1, 5, nullptr);
	if (m_nTimerID == 0)
	{
		AfxMessageBox(_T("无法创建定时器！"));
		return -1;
	}

	return 0;
}
// 视图销毁时调用
void CMFCApplication1View::OnDestroy()
{
    // 销毁定时器
    if (m_nTimerID != 0)
    {
        KillTimer(m_nTimerID);
        m_nTimerID = 0;
    }

    // 清理游戏资源
    CleanupGame();

    CView::OnDestroy();
}
// 新增：初始化瓦片地图
// 修改 InitializeTileMap 方法
void CMFCApplication1View::InitializeTileMap()
{
    TRACE(_T("=== 开始初始化瓦片地图 ===\n"));

    // 使用新的 LoadLevel 方法加载第一关
    BOOL result = m_TileMap.LoadLevel(1);
    TRACE(_T("地图加载结果: %s\n"), result ? _T("成功") : _T("失败"));
    TRACE(_T("地图尺寸: %dx%d, 瓦片大小: %d\n"),
        m_TileMap.GetWidth(), m_TileMap.GetHeight(), m_TileMap.GetTileSize());

    // 设置马里奥初始位置
    int marioStartX = 5 * CGameConfig::TILE_SIZE;
    int marioStartY = 200; // 适当的位置
    m_Mario.SetPosition(marioStartX, marioStartY);

    TRACE(_T("=== 瓦片地图初始化完成 ===\n"));
}
// 新增：更新摄像机
void CMFCApplication1View::UpdateCamera()
{
    // 简单的摄像机跟随马里奥
    int marioCenterX = m_Mario.GetX() + m_Mario.GetWidth() / 2;

    // 摄像机平滑跟随（使用缓动效果）
    int targetCameraX = marioCenterX - m_nScreenWidth / 2;

    // 添加缓动效果，让摄像机移动更平滑
    m_nCameraX += (targetCameraX - m_nCameraX) * 0.1f;

    // 限制摄像机范围
    if (m_nCameraX < 0) m_nCameraX = 0;
    int maxCameraX = m_TileMap.GetWidth() * m_TileMap.GetTileSize() - m_nScreenWidth;
    if (m_nCameraX > maxCameraX && maxCameraX > 0)
        m_nCameraX = maxCameraX;

    // 垂直方向也做简单跟随（如果需要）
    int marioCenterY = m_Mario.GetY() + m_Mario.GetHeight() / 2;
    int targetCameraY = marioCenterY - m_nScreenHeight / 2;

    // 限制垂直摄像机范围（通常不需要垂直滚动，或者限制在一定范围内）
    if (targetCameraY < 0) targetCameraY = 0;
    int maxCameraY = m_TileMap.GetHeight() * m_TileMap.GetTileSize() - m_nScreenHeight;
    if (targetCameraY > maxCameraY && maxCameraY > 0)
        targetCameraY = maxCameraY;

    // 垂直方向缓动
    m_nCameraY += (targetCameraY - m_nCameraY) * 0.1f;
}
// 初始化游戏
void CMFCApplication1View::InitializeGame()
{
    CClientDC dc(this);

    // 创建内存DC用于双缓冲
    m_memDC.CreateCompatibleDC(&dc);
    m_memBitmap.CreateCompatibleBitmap(&dc, m_nScreenWidth, m_nScreenHeight);
    m_pOldBitmap = m_memDC.SelectObject(&m_memBitmap);
    // 初始化资源
    InitializeResources();

    // 初始化瓦片地图
    InitializeTileMap();
}

// 新增：初始化资源
void CMFCApplication1View::InitializeResources()
{
    TRACE(_T("开始初始化资源...\n"));

    CResourceManager& resMgr = CResourceManager::GetInstance();

    // 使用新的资源加载方法
    BOOL success = resMgr.LoadGameResources();

    TRACE(_T("资源初始化: %s\n"), success ? _T("成功") : _T("失败"));

    // 测试获取贴图 - 使用新的名称
    TRACE(_T("测试获取贴图...\n"));
    CBitmap* testMain  = resMgr.GetBitmap(CSpriteConfig::TILESET_MAIN);
    CBitmap* testMario = resMgr.GetBitmap(CSpriteConfig::TILESET_MARIO);
    CBitmap* testMiku  = resMgr.GetBitmap(CSpriteConfig::TILESET_MIKU);
    TRACE(_T("主贴图集: %p\n"), testMain);
    TRACE(_T("马里奥贴图集: %p\n"), testMario);
    TRACE(_T("初音未来图集: %p\n"), testMiku);
    if (testMain)
    {
        BITMAP bm;
        testMain->GetBitmap(&bm);
        TRACE(_T("主贴图集尺寸: %dx%d\n"), bm.bmWidth, bm.bmHeight);
       
    }
    if (testMario) {
        BITMAP bm;
        testMario->GetBitmap(&bm);
        TRACE(_T("马里奥贴图集尺寸: %dx%d\n"), bm.bmWidth, bm.bmHeight);
    }
    if (testMiku) {
        BITMAP bm;
        testMiku->GetBitmap(&bm);
        TRACE(_T("初音未来贴图集尺寸: %dx%d\n"), bm.bmWidth, bm.bmHeight);
    }
    else
    {
        TRACE(_T("错误: 无法获取主贴图集\n"));
       
    }
}
// 修改RenderGame方法
// 修改 RenderGame 方法
void CMFCApplication1View::RenderGame(CDC* pDC)
{
    // 绘制背景 - 简单的天空色
    pDC->FillSolidRect(0, 0, m_nScreenWidth, m_nScreenHeight, RGB(135, 206, 235));

    // 使用瓦片地图绘制整个关卡（包括瓦片和独立对象）
    m_TileMap.Draw(pDC, m_nCameraX, m_nCameraY);

    // 绘制马里奥
    int marioScreenX = m_Mario.GetX() - m_nCameraX;
    int marioScreenY = m_Mario.GetY() - m_nCameraY;
    m_Mario.DrawWithSprite(pDC, marioScreenX, marioScreenY);

    // 调试模式：绘制碰撞信息
    if (m_bDebugMode)
    {
        DrawDebugCollision(pDC);
    }

    // 绘制调试信息
    DrawDebugInfo(pDC);
}
// 清理游戏资源
void CMFCApplication1View::CleanupGame()
{
    // 恢复旧位图并删除资源
    if (m_pOldBitmap != nullptr)
    {
        m_memDC.SelectObject(m_pOldBitmap);
        m_pOldBitmap = nullptr;
    }

    if (m_memBitmap.GetSafeHandle() != NULL)
    {
        m_memBitmap.DeleteObject();
    }

    if (m_memDC.GetSafeHdc() != NULL)
    {
        m_memDC.DeleteDC();
    }
}

// 定时器消息处理 - 游戏主循环
void CMFCApplication1View::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == m_nTimerID)
    {
        // 计算时间差
        CalculateDeltaTime();

        // 更新游戏逻辑
        UpdateGame();

        // 请求重绘
        Invalidate(FALSE);
    }

    CView::OnTimer(nIDEvent);
}
// 修改 UpdateGame 方法
void CMFCApplication1View::UpdateGame()
{
    // 将输入状态传递给马里奥
    m_Mario.HandleInput(m_bKeyLeft, m_bKeyRight, m_bKeyJump);

    // 更新马里奥状态（使用世界坐标）
    m_Mario.Update(m_fDeltaTime);

    // 更新金币动画
    m_TileMap.UpdateCoins(m_fDeltaTime);
    // 更新摄像机
    UpdateCamera();

    // 使用瓦片地图进行碰撞检测（现在包括独立对象）
    std::vector<CRect> solidObjects = m_TileMap.GetAllCollisionRects();
    m_Mario.CheckCollisions(solidObjects);
    // 检查金币碰撞
    m_TileMap.CheckCoinCollisions(m_Mario.GetRect());
}
// 新增：绘制调试碰撞信息
void CMFCApplication1View::DrawDebugCollision(CDC* pDC)
{
    if (!m_bDebugMode) return;

    // 获取所有碰撞实体（世界坐标）
    std::vector<CRect> solidObjects = m_TileMap.GetSolidTileRects();

    // 创建画笔
    CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
    CPen greenPen(PS_SOLID, 3, RGB(0, 255, 0));
    CPen cyanPen(PS_SOLID, 1, RGB(0, 255, 255));
    CPen yellowPen(PS_SOLID, 3, RGB(255, 255, 0));

    CPen* pOldPen = pDC->SelectObject(&redPen);

    // 1. 绘制所有实体的碰撞框（红色）- 转换为屏幕坐标
    for (const auto& rect : solidObjects)
    {
        CRect screenRect(
            rect.left - m_nCameraX,
            rect.top - m_nCameraY,
            rect.right - m_nCameraX,
            rect.bottom - m_nCameraY
        );

        pDC->MoveTo(screenRect.left, screenRect.top);
        pDC->LineTo(screenRect.right, screenRect.top);
        pDC->LineTo(screenRect.right, screenRect.bottom);
        pDC->LineTo(screenRect.left, screenRect.bottom);
        pDC->LineTo(screenRect.left, screenRect.top);
    }

    // 2. 绘制马里奥的碰撞框（绿色）- 使用屏幕坐标
    pDC->SelectObject(&greenPen);

    CRect marioWorldRect = m_Mario.GetRect();
    CRect marioScreenRect(
        marioWorldRect.left - m_nCameraX,
        marioWorldRect.top - m_nCameraY,
        marioWorldRect.right - m_nCameraX,
        marioWorldRect.bottom - m_nCameraY
    );

    pDC->MoveTo(marioScreenRect.left, marioScreenRect.top);
    pDC->LineTo(marioScreenRect.right, marioScreenRect.top);
    pDC->LineTo(marioScreenRect.right, marioScreenRect.bottom);
    pDC->LineTo(marioScreenRect.left, marioScreenRect.bottom);
    pDC->LineTo(marioScreenRect.left, marioScreenRect.top);

    // 3. 绘制碰撞检测点（青色点）- 使用屏幕坐标
    pDC->SelectObject(&cyanPen);

    CRect marioHead = m_Mario.GetHeadRect();
    CRect marioFeet = m_Mario.GetFeetRect();
    CRect marioBody = m_Mario.GetBodyRect();

    // 转换为屏幕坐标
    CRect headScreen(
        marioHead.left - m_nCameraX, marioHead.top - m_nCameraY,
        marioHead.right - m_nCameraX, marioHead.bottom - m_nCameraY
    );
    CRect feetScreen(
        marioFeet.left - m_nCameraX, marioFeet.top - m_nCameraY,
        marioFeet.right - m_nCameraX, marioFeet.bottom - m_nCameraY
    );
    CRect bodyScreen(
        marioBody.left - m_nCameraX, marioBody.top - m_nCameraY,
        marioBody.right - m_nCameraX, marioBody.bottom - m_nCameraY
    );

    // 绘制头部检测点
    pDC->Ellipse(headScreen.left - 3, headScreen.top - 3,
        headScreen.left + 3, headScreen.top + 3);
    pDC->Ellipse(headScreen.right - 3, headScreen.top - 3,
        headScreen.right + 3, headScreen.top + 3);

    // 绘制脚部检测点
    pDC->Ellipse(feetScreen.left - 3, feetScreen.bottom - 3,
        feetScreen.left + 3, feetScreen.bottom + 3);
    pDC->Ellipse(feetScreen.right - 3, feetScreen.bottom - 3,
        feetScreen.right + 3, feetScreen.bottom + 3);

    // 4. 绘制当前碰撞的实体（黄色高亮）
    pDC->SelectObject(&yellowPen);

    // 检查当前与马里奥碰撞的实体
    for (const auto& rect : solidObjects)
    {
        CRect intersection;
        if (intersection.IntersectRect(&marioWorldRect, &rect))
        {
            // 转换为屏幕坐标
            CRect screenRect(
                rect.left - m_nCameraX,
                rect.top - m_nCameraY,
                rect.right - m_nCameraX,
                rect.bottom - m_nCameraY
            );

            // 高亮显示当前碰撞的实体
            pDC->MoveTo(screenRect.left, screenRect.top);
            pDC->LineTo(screenRect.right, screenRect.top);
            pDC->LineTo(screenRect.right, screenRect.bottom);
            pDC->LineTo(screenRect.left, screenRect.bottom);
            pDC->LineTo(screenRect.left, screenRect.top);

            // 在碰撞实体上显示碰撞深度信息
            CString depthInfo;
            depthInfo.Format(_T("%d,%d"), intersection.Width(), intersection.Height());
            pDC->SetTextColor(RGB(255, 255, 0));
            pDC->SetBkMode(TRANSPARENT);
            pDC->TextOut(screenRect.left + 5, screenRect.top + 5, depthInfo);
        }
    }

    // 恢复原来的画笔
    pDC->SelectObject(pOldPen);
}
// 增强的调试信息显示
// 在调试信息中添加瓦片大小信息
void CMFCApplication1View::DrawDebugInfo(CDC* pDC)
{
    CString strInfo;

    // 基本信息
    pDC->SetTextColor(RGB(255, 255, 255));
    pDC->SetBkMode(TRANSPARENT);

    // 第1行：位置和状态
    CString stateStr;

    // Use Mario's internal flags to determine a more precise action state: standing, walking, jumping, falling
    if (m_Mario.IsJumping())
        stateStr = _T("跳跃");
    else if (!m_Mario.IsOnGround())
        stateStr = _T("下落");
    else if (m_Mario.IsMoving())
    {
        if (m_Mario.GetDirection() == Direction::RIGHT)
            stateStr = _T("向右行走");
        else
            stateStr = _T("向左行走");
    }
    else
        stateStr = _T("站立");
   
    strInfo.Format(_T("世界位置: (%d, %d)  状态: %s "),
        m_Mario.GetX(), m_Mario.GetY(), stateStr);
    pDC->TextOut(10, 10, strInfo);

    // 第2行：屏幕位置和速度
    int screenX = m_Mario.GetX() - m_nCameraX;
    int screenY = m_Mario.GetY() - m_nCameraY;

    strInfo.Format(_T("屏幕位置: (%d, %d)  速度: (%.1f, %.1f)"),
        screenX, screenY,
        m_Mario.GetVelocityX(), m_Mario.GetVelocityY());
    pDC->TextOut(10, 30, strInfo);

    // 第3行：输入状态
    strInfo.Format(_T("输入: %s%s%s"),
        m_bKeyLeft ? _T("←") : _T(" "),
        m_bKeyRight ? _T("→") : _T(" "),
        m_bKeyJump ? _T("↑") : _T(" "));
    pDC->TextOut(10, 50, strInfo);

    // 第4行：游戏状态信息
    strInfo.Format(_T("金币: %d   分数: %d   生命: %d"),
        CGameState::GetInstance().GetCoins(),
        CGameState::GetInstance().GetScore(),
        CGameState::GetInstance().GetLives());
    pDC->TextOut(10, 70, strInfo);

    // 第5行：马里奥状态和皮肤
    CString strMarioState;
    switch (m_Mario.GetState())
    {
    case MarioState::SMALL:
        strMarioState = _T("小马里奥");
        break;
    case MarioState::BIG:
        strMarioState = _T("大马里奥");
        break;
    case MarioState::FIRE:
        strMarioState = _T("火焰马里奥");
        break;
    }

    // 添加皮肤信息显示
    CString skinStr = (m_Mario.GetSkin() == MarioSkin::MARIO) ? _T("马里奥") : _T("初音未来");
    strInfo.Format(_T("皮肤: %s   角色: %s   帧率: %.1f FPS"),
        skinStr, strMarioState, m_fSmoothedFPS);
    pDC->TextOut(10, 90, strInfo);
    // 调试模式特定信息
    if (m_bDebugMode)
    {
        // 调试模式标题
        pDC->SetTextColor(RGB(255, 255, 0)); // 黄色
        pDC->TextOut(10, 100, _T("=== 调试模式 ==="));
        pDC->TextOut(10, 120, _T("按 D 键关闭调试模式"));

        // 碰撞框说明
        pDC->SetTextColor(RGB(255, 0, 0));   // 红色
        pDC->TextOut(10, 140, _T("红色: 实体碰撞框"));

        pDC->SetTextColor(RGB(0, 255, 0));   // 绿色
        pDC->TextOut(10, 160, _T("绿色: 马里奥碰撞框"));

        pDC->SetTextColor(RGB(0, 255, 255)); // 青色
        pDC->TextOut(10, 180, _T("青色: 碰撞检测点"));

        // 实体统计
        strInfo.Format(_T("实体数量: 砖块(%d)  水管(%d)"),
            m_Bricks.size(), m_Pipes.size());
        pDC->SetTextColor(RGB(255, 255, 255));
        pDC->TextOut(10, 200, strInfo);

        // 物理信息
        strInfo.Format(_T("重力: %.1f  最大速度: %.1f"),
            m_Mario.GetGravity(), m_Mario.GetMaxSpeed());
        pDC->TextOut(10, 220, strInfo);

        // 地图信息
        strInfo.Format(_T("地图: %dx%d 瓦片  瓦片大小: %dx%d"),
            m_TileMap.GetWidth(), m_TileMap.GetHeight(),
            m_TileMap.GetTileSize(), m_TileMap.GetTileSize());
        pDC->TextOut(10, 240, strInfo);

        // 摄像机信息
        strInfo.Format(_T("摄像机: (%d, %d)"), m_nCameraX, m_nCameraY);
        pDC->TextOut(10, 260, strInfo);

        // 贴图系统状态
        CResourceManager& resMgr = CResourceManager::GetInstance();
        CBitmap* testBrick = resMgr.GetBitmap(_T("TilesetMain"));
        strInfo.Format(_T("贴图系统: 正常  砖块贴图: %p"), testBrick);
        pDC->SetTextColor(RGB(0, 255, 0)); // 绿色表示正常
        pDC->TextOut(10, 280, strInfo);
    }
    else
    {
        // 非调试模式提示
        pDC->SetTextColor(RGB(200, 200, 200)); // 灰色
        pDC->TextOut(10, 100, _T("按 D 键开启调试模式"));
        pDC->TextOut(10, 120, _T("操作: 方向键移动, 空格跳跃, 1/2/3切换状态"));
    }
}
// 绘制游戏
void CMFCApplication1View::OnDraw(CDC* pDC)
{
    CMFCApplication1Doc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // 渲染游戏到内存DC
    RenderGame(&m_memDC);

    // 将内存DC内容复制到屏幕DC
    pDC->BitBlt(0, 0, m_nScreenWidth, m_nScreenHeight, &m_memDC, 0, 0, SRCCOPY);
}
// 键盘按下事件
void CMFCApplication1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // 设置按键状态，而不是直接调用马里奥的方法
    switch (nChar)
    {
    case VK_LEFT:   // 左箭头
        m_bKeyLeft = TRUE;
        break;
    case VK_RIGHT:  // 右箭头
        m_bKeyRight = TRUE;
        break;
    case VK_UP:     // 上箭头
    case VK_SPACE:  // 空格键
        m_bKeyJump = TRUE;
        break;
    case '1':       // 数字1 - 小马里奥
        m_Mario.SetState(MarioState::SMALL);
        break;
    case '2':       // 数字2 - 大马里奥
        m_Mario.SetState(MarioState::BIG);
        break;
    case '3':       // 数字3 - 火焰马里奥
        m_Mario.SetState(MarioState::FIRE);
        break;
	case 'D':       // 切换调试模式
    case 'd':
        m_bDebugMode = !m_bDebugMode;
		break;
    case 'M':   // 按M键切换Miku皮肤
    case 'm':
    {
        MarioSkin currentSkin = m_Mario.GetSkin();
        if (currentSkin == MarioSkin::MARIO)
            m_Mario.SetSkin(MarioSkin::MIKU);
        else
            m_Mario.SetSkin(MarioSkin::MARIO);
    }
    break;
    }

    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMFCApplication1View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // 清除按键状态
    switch (nChar)
    {
    case VK_LEFT:
        m_bKeyLeft = FALSE;
        break;
    case VK_RIGHT:
        m_bKeyRight = FALSE;
        break;
    case VK_UP:
    case VK_SPACE:
        m_bKeyJump = FALSE;
        break;
    }

    CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

// 计算帧时间差
void CMFCApplication1View::CalculateDeltaTime()
{
    using namespace std::chrono;
    auto currentTime = steady_clock::now();
    duration<float> elapsed = currentTime - m_dwLastTime;
    m_fDeltaTime = elapsed.count();

    // 限制最大deltaTime，避免卡顿导致的大跳帧
    if (m_fDeltaTime > 0.1f)
        m_fDeltaTime = 0.1f;

    m_dwLastTime = currentTime;

    // 帧率平滑处理（移动平均）
    static float frameRates[10] = { 0 };
    static int frameIndex = 0;
    static float frameSum = 0;

    frameSum -= frameRates[frameIndex];
    if (m_fDeltaTime > 0) {
        frameRates[frameIndex] = 1.0f / m_fDeltaTime;
    }
    else {
        frameRates[frameIndex] = 0;
    }
    frameSum += frameRates[frameIndex];
    frameIndex = (frameIndex + 1) % 10;

    // 使用平滑后的帧率
    m_fSmoothedFPS = frameSum / 10;
}


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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "Game/Objects/Mario.h"

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
	ON_WM_CREATE()
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
    m_dwLastTime = GetTickCount();
    m_nFrameCount = 0;
    m_fDeltaTime = 0.033f; // 默认1/30秒
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

	// 设置定时器 - 33ms ≈ 30fps
	m_nTimerID = SetTimer(1, 33, nullptr);
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
void CMFCApplication1View::InitializeTileMap()
{
    // 明确设置新的瓦片大小 - 32x32像素
    TRACE(_T("=== 开始初始化瓦片地图 ===\n"));

    // 创建一个小一点的测试地图，确保变化明显
    // 地图尺寸：30x15个瓦片，每个瓦片32x32像素
        // 使用全局配置的地图尺寸
    BOOL result = m_TileMap.LoadMap(
        CGameConfig::TILE_MAP_WIDTH,
        CGameConfig::TILE_MAP_HEIGHT,
        CGameConfig::TILE_SIZE
    );
    TRACE(_T("地图加载结果: %s\n"), result ? _T("成功") : _T("失败"));
    TRACE(_T("地图尺寸: %dx%d, 瓦片大小: %d\n"),
        m_TileMap.GetWidth(), m_TileMap.GetHeight(), m_TileMap.GetTileSize());

    // 创建地面 - 放在第10行（从0开始）
    // 这样地面在Y=320像素（10*32=320）
     // 创建地面 - 使用全局配置计算位置
    int groundLevel = CGameConfig::TILE_MAP_HEIGHT - 5; // 离底部5行的位置

    for (int x = 0; x < CGameConfig::TILE_MAP_WIDTH; x++)
    {
        for (int y = groundLevel; y < CGameConfig::TILE_MAP_HEIGHT; y++)
        {
            m_TileMap.SetTile(x, y, 1, TRUE, _T("ground"));
        }
    }

    // 设置马里奥初始位置在地面上方
    int marioStartX = 5 * CGameConfig::TILE_SIZE;
    int marioStartY = (groundLevel - 2) * CGameConfig::TILE_SIZE;
    m_Mario.SetPosition(marioStartX, marioStartY);

    // 创建明显的测试平台 - 使用大瓦片

    // 平台1 - 在Y=7行（224像素）
    for (int x = 3; x < 8; x++)
    {
        m_TileMap.SetTile(x, 7, 2, TRUE, _T("brick"));
    }

    // 平台2 - 在Y=5行（160像素）
    for (int x = 10; x < 15; x++)
    {
        m_TileMap.SetTile(x, 5, 2, TRUE, _T("brick"));
    }

    // 问号砖块 - 在Y=3行（96像素）
    m_TileMap.SetTile(7, 3, 3, TRUE, _T("question"));
    m_TileMap.SetTile(8, 3, 3, TRUE, _T("question"));

    // 硬砖块 - 在Y=2行（64像素）
    for (int x = 12; x < 14; x++)
    {
        m_TileMap.SetTile(x, 2, 4, TRUE, _T("hard_brick"));
    }

    // 水管 - 2x3个瓦片
    m_TileMap.SetTile(20, 7, 5, TRUE, _T("pipe"));
    m_TileMap.SetTile(20, 8, 5, TRUE, _T("pipe"));
    m_TileMap.SetTile(20, 9, 5, TRUE, _T("pipe"));
    m_TileMap.SetTile(21, 7, 5, TRUE, _T("pipe"));
    m_TileMap.SetTile(21, 8, 5, TRUE, _T("pipe"));
    m_TileMap.SetTile(21, 9, 5, TRUE, _T("pipe"));

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

    // 加载马里奥贴图
    TRACE(_T("加载马里奥贴图...\n"));
    BOOL marioSmall = resMgr.LoadBitmap(IDB_MARIO_SMALL, _T("MarioSmall"));
    BOOL marioBig = resMgr.LoadBitmap(IDB_MARIO_BIG, _T("MarioBig"));
    BOOL marioFire = resMgr.LoadBitmap(IDB_MARIO_FIRE, _T("MarioFire"));

    TRACE(_T("马里奥贴图加载: 小=%d, 大=%d, 火=%d\n"), marioSmall, marioBig, marioFire);

    // 加载地图贴图
    TRACE(_T("加载地图贴图...\n"));
    BOOL brick = resMgr.LoadBitmap(IDB_BRICK, _T("Brick"));
    BOOL question = resMgr.LoadBitmap(IDB_QUESTION_BLOCK, _T("QuestionBlock"));
    BOOL hardBrick = resMgr.LoadBitmap(IDB_HARD_BRICK, _T("HardBrick"));
    BOOL pipe = resMgr.LoadBitmap(IDB_PIPE, _T("Pipe"));
    BOOL ground = resMgr.LoadBitmap(IDB_GROUND, _T("Ground"));
    BOOL background = resMgr.LoadBitmap(IDB_BACKGROUND, _T("Background"));

    TRACE(_T("地图贴图加载: 砖块=%d, 问号=%d, 硬砖=%d, 水管=%d, 地面=%d, 背景=%d\n"),
        brick, question, hardBrick, pipe, ground, background);

    // 测试获取贴图
    TRACE(_T("测试获取贴图...\n"));
    CBitmap* testBrick = resMgr.GetBitmap(_T("Brick"));
    TRACE(_T("砖块贴图指针: %p\n"), testBrick);

    TRACE(_T("资源初始化完成\n"));
}
// 修改RenderGame方法
void CMFCApplication1View::RenderGame(CDC* pDC)
{
    // 使用贴图绘制背景
    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBgBitmap = resMgr.GetBitmap(_T("Background"));

    if (pBgBitmap)
    {
        // 平铺背景贴图
        CSize bgSize;
        if (CSpriteRenderer::GetBitmapSize(pBgBitmap, bgSize))
        {
            for (int y = 0; y < m_nScreenHeight; y += bgSize.cy)
            {
                for (int x = 0; x < m_nScreenWidth; x += bgSize.cx)
                {
                    CSpriteRenderer::DrawSprite(pDC, pBgBitmap, x, y,
                        0, 0, bgSize.cx, bgSize.cy, FALSE);
                }
            }
        }
    }
    else
    {
        // 备用：单色背景
        pDC->FillSolidRect(0, 0, m_nScreenWidth, m_nScreenHeight, RGB(135, 206, 235));
    }

    // 使用瓦片地图绘制关卡（现在使用贴图）
    m_TileMap.Draw(pDC, m_nCameraX, m_nCameraY);

    // 绘制马里奥（使用贴图）
    int marioScreenX = m_Mario.GetX() - m_nCameraX;
    int marioScreenY = m_Mario.GetY() - m_nCameraY;
    m_Mario.DrawAt(pDC, marioScreenX, marioScreenY);

    // 调试模式：绘制碰撞信息
    if (m_bDebugMode)
    {
        DrawDebugCollision(pDC);
    }

    // 绘制调试信息
    DrawDebugInfo(pDC);

    // 测试：在屏幕左上角绘制一个砖块贴图，确保贴图系统工作
    CBitmap* pTestBrick = resMgr.GetBitmap(_T("Brick"));
    if (pTestBrick)
    {
        CSpriteRenderer::DrawSprite(pDC, pTestBrick, 10, 100, 0, 0, 32, 32, TRUE);
        pDC->TextOut(50, 100, _T("砖块贴图测试"));
    }
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
        // 更新游戏逻辑
        UpdateGame();

        // 请求重绘
        Invalidate(FALSE);
    }

    CView::OnTimer(nIDEvent);
}

// 更新游戏逻辑
void CMFCApplication1View::UpdateGame()
{
    // 将输入状态传递给马里奥
    m_Mario.HandleInput(m_bKeyLeft, m_bKeyRight, m_bKeyJump);

    // 更新马里奥状态（使用世界坐标）
    m_Mario.Update(m_fDeltaTime);

    // 更新摄像机
    UpdateCamera();

    // 使用瓦片地图进行碰撞检测（使用世界坐标）
    std::vector<CRect> solidObjects = m_TileMap.GetSolidTileRects();
    m_Mario.CheckCollisions(solidObjects);
}
// 修改获取碰撞对象的方法
std::vector<CRect> CMFCApplication1View::GetAllSolidObjects() const
{
    std::vector<CRect> solidObjects;

    // 添加地面
    solidObjects.push_back(CRect(0, 500, m_nScreenWidth, 600));

    // 添加所有砖块
    for (const auto& brick : m_Bricks)
    {
        solidObjects.push_back(brick.GetRect());
    }

    // 添加所有水管
    for (const auto& pipe : m_Pipes)
    {
        solidObjects.push_back(pipe.GetRect());
    }

    return solidObjects;
}
// 新增：绘制碰撞检测点
void CMFCApplication1View::DrawCollisionPoints(CDC* pDC, const CRect& marioRect)
{
    // 绘制马里奥周围的检测点
    int points[][2] = {
        {marioRect.left, marioRect.top},      // 左上角
        {marioRect.right, marioRect.top},     // 右上角
        {marioRect.left, marioRect.bottom},   // 左下角
        {marioRect.right, marioRect.bottom},  // 右下角
        {marioRect.CenterPoint().x, marioRect.top},     // 上中点
        {marioRect.CenterPoint().x, marioRect.bottom},  // 下中点
        {marioRect.left, marioRect.CenterPoint().y},    // 左中点
        {marioRect.right, marioRect.CenterPoint().y}    // 右中点
    };

    for (int i = 0; i < 8; i++)
    {
        pDC->Ellipse(points[i][0] - 2, points[i][1] - 2,
            points[i][0] + 2, points[i][1] + 2);
    }
}
// 新增：绘制当前碰撞的实体
void CMFCApplication1View::DrawCurrentCollisions(CDC* pDC, const std::vector<CRect>& solidObjects, const CRect& marioRect)
{
    CPen yellowPen(PS_SOLID, 3, RGB(255, 255, 0));
    pDC->SelectObject(&yellowPen);

    // 检查当前与马里奥碰撞的实体
    for (const auto& rect : solidObjects)
    {
        CRect intersection;
        if (intersection.IntersectRect(&marioRect, &rect))
        {
            // 高亮显示当前碰撞的实体
            pDC->MoveTo(rect.left, rect.top);
            pDC->LineTo(rect.right, rect.top);
            pDC->LineTo(rect.right, rect.bottom);
            pDC->LineTo(rect.left, rect.bottom);
            pDC->LineTo(rect.left, rect.top);

            // 在碰撞实体上显示碰撞深度信息
            CString depthInfo;
            depthInfo.Format(_T("%d,%d"), intersection.Width(), intersection.Height());
            pDC->SetTextColor(RGB(255, 255, 0));
            pDC->SetBkMode(TRANSPARENT);
            pDC->TextOut(rect.left + 5, rect.top + 5, depthInfo);
        }
    }
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
    if (m_Mario.IsOnGround())
        stateStr = _T("地面");
    else if (m_Mario.IsJumping())
        stateStr = _T("跳跃");
    else
        stateStr = _T("空中");

    strInfo.Format(_T("世界位置: (%d, %d)  状态: %s"),
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

    // 第4行：马里奥状态和帧率
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

    strInfo.Format(_T("角色: %s   帧率: %.1f FPS"),
        strMarioState, m_fSmoothedFPS);
    pDC->TextOut(10, 70, strInfo);

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
        CBitmap* testBrick = resMgr.GetBitmap(_T("Brick"));
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
    DWORD dwCurrentTime = GetTickCount64();
    m_fDeltaTime = (dwCurrentTime - m_dwLastTime) / 1000.0f; // 转换为秒

    // 限制最大deltaTime，避免卡顿导致的大跳帧
    if (m_fDeltaTime > 0.1f)
        m_fDeltaTime = 0.1f;

    m_dwLastTime = dwCurrentTime;
    // 帧率平滑处理（移动平均）
    static float frameRates[10] = { 0 };
    static int frameIndex = 0;
    static float frameSum = 0;

    frameSum -= frameRates[frameIndex];
    frameRates[frameIndex] = 1.0f / m_fDeltaTime;
    frameSum += frameRates[frameIndex];
    frameIndex = (frameIndex + 1) % 10;

    // 使用平滑后的帧率
    m_fSmoothedFPS = frameSum / 10;
}

// 新增：获取所有平台的矩形区域
std::vector<CRect> CMFCApplication1View::GetAllPlatformRects() const
{
    std::vector<CRect> platforms;

    // 添加地面
    platforms.push_back(CRect(0, 500, m_nScreenWidth, 600));

    // 添加所有砖块
    for (const auto& brick : m_Bricks)
    {
        platforms.push_back(brick.GetRect());
    }

    // 添加所有水管
    for (const auto& pipe : m_Pipes)
    {
        platforms.push_back(pipe.GetRect());
    }

    return platforms;
}

// 新增：检查马里奥碰撞
void CMFCApplication1View::CheckMarioCollisions()
{
    // 获取所有碰撞平台
    std::vector<CRect> platforms = GetAllPlatformRects();

    // 让马里奥检查碰撞
    m_Mario.CheckCollisions(platforms);

    // 处理砖块的特殊碰撞（顶砖块）
    HandleBrickCollisions();
}

// 新增：处理砖块碰撞
void CMFCApplication1View::HandleBrickCollisions()
{
    // 获取马里奥的头部区域
    CRect marioHead = m_Mario.GetHeadRect();

    // 检查马里奥头部与砖块的碰撞
    for (auto& brick : m_Bricks)
    {
        CRect brickRect = brick.GetRect();
        CRect intersection;

        if (intersection.IntersectRect(&marioHead, &brickRect))
        {
            // 如果马里奥正在上升并且顶到了砖块
            if (m_Mario.IsJumping() && brick.CanBeHitFromBelow())
            {
                brick.OnHitFromBelow();
                // 这里可以添加得分效果等
            }
        }
    }
}
// 新增：绘制坐标网格
void CMFCApplication1View::DrawCoordinateGrid(CDC* pDC)
{
    if (!m_bDebugMode) return;

    CPen gridPen(PS_DOT, 1, RGB(100, 100, 100));
    CPen* pOldPen = pDC->SelectObject(&gridPen);

    // 绘制垂直网格线（每50像素）- 考虑摄像机偏移
    for (int worldX = (m_nCameraX / 50) * 50; worldX < m_nCameraX + m_nScreenWidth; worldX += 50)
    {
        int screenX = worldX - m_nCameraX;
        pDC->MoveTo(screenX, 0);
        pDC->LineTo(screenX, m_nScreenHeight);

        // 显示坐标（世界坐标）
        if (worldX % 100 == 0) // 每100像素显示坐标数字
        {
            CString coord;
            coord.Format(_T("%d"), worldX);
            pDC->SetTextColor(RGB(150, 150, 150));
            pDC->SetBkMode(TRANSPARENT);
            pDC->TextOut(screenX + 2, 10, coord);
        }
    }

    // 绘制水平网格线（每50像素）- 考虑摄像机偏移
    for (int worldY = (m_nCameraY / 50) * 50; worldY < m_nCameraY + m_nScreenHeight; worldY += 50)
    {
        int screenY = worldY - m_nCameraY;
        pDC->MoveTo(0, screenY);
        pDC->LineTo(m_nScreenWidth, screenY);

        // 显示坐标（世界坐标）
        if (worldY % 100 == 0 && worldY > 0) // 每100像素显示坐标数字
        {
            CString coord;
            coord.Format(_T("%d"), worldY);
            pDC->SetTextColor(RGB(150, 150, 150));
            pDC->SetBkMode(TRANSPARENT);
            pDC->TextOut(10, screenY + 2, coord);
        }
    }

    pDC->SelectObject(pOldPen);
}
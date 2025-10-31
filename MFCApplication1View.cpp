
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
	m_nScreenWidth = 800;
	m_nScreenHeight = 600;

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

// 初始化游戏
void CMFCApplication1View::InitializeGame()
{
    CClientDC dc(this);

    // 创建内存DC用于双缓冲
    m_memDC.CreateCompatibleDC(&dc);
    m_memBitmap.CreateCompatibleBitmap(&dc, m_nScreenWidth, m_nScreenHeight);
    m_pOldBitmap = m_memDC.SelectObject(&m_memBitmap);

    // 设置内存DC背景为白色
    m_memDC.FillSolidRect(0, 0, m_nScreenWidth, m_nScreenHeight, RGB(135, 206, 235)); // 天蓝色背景
  
    // 初始化地图元素
    InitializeMap();
}
// 新增：初始化地图
void CMFCApplication1View::InitializeMap()
{
    // 清空所有地图元素
    m_Bricks.clear();
    m_Pipes.clear();

    // 创建简单但有效的测试场景

    // 平台1：长平台 - 测试正常行走和边缘掉落
    for (int i = 0; i < 6; i++)
    {
        m_Bricks.push_back(CBrick(100 + i * 40, 400));
    }

    // 平台2：短平台 - 专门测试边缘掉落
    for (int i = 0; i < 2; i++)
    {
        m_Bricks.push_back(CBrick(400 + i * 40, 350));
    }

    // 平台3：高平台 - 测试跳跃
    for (int i = 0; i < 3; i++)
    {
        m_Bricks.push_back(CBrick(200 + i * 40, 300));
    }

    // 问号砖块
    m_Bricks.push_back(CBrick(350, 250));
    m_Bricks.back().SetBrickType(CBrick::QUESTION);

    // 硬砖块
    m_Bricks.push_back(CBrick(450, 250));
    m_Bricks.back().SetBrickType(CBrick::HARD);

    // 添加水管
    m_Pipes.push_back(CPipe(600, 380, 120));
}

// 修改 RenderGame 方法，绘制所有地图元素
void CMFCApplication1View::RenderGame(CDC* pDC)
{
    // 清除背景（天空）
    pDC->FillSolidRect(0, 0, m_nScreenWidth, m_nScreenHeight, RGB(135, 206, 235));

    // 绘制远处的山（装饰）
    pDC->FillSolidRect(50, 350, 100, 50, RGB(100, 100, 100));
    pDC->FillSolidRect(120, 330, 80, 70, RGB(120, 120, 120));

    // 绘制云朵（装饰）
    pDC->FillSolidRect(200, 100, 60, 20, RGB(255, 255, 255));
    pDC->FillSolidRect(230, 90, 40, 25, RGB(255, 255, 255));
    pDC->FillSolidRect(500, 150, 70, 25, RGB(255, 255, 255));
    pDC->FillSolidRect(530, 140, 50, 30, RGB(255, 255, 255));
    // 绘制地面
    pDC->FillSolidRect(0, 500, m_nScreenWidth, 100, RGB(101, 67, 33));

    // 绘制草地纹理
    for (int i = 0; i < m_nScreenWidth; i += 20)
    {
        pDC->FillSolidRect(i, 500, 10, 5, RGB(0, 150, 0));
    }

    // 绘制所有水管
    for (auto& pipe : m_Pipes)
    {
        pipe.Draw(pDC);
    }

    // 绘制所有砖块
    for (auto& brick : m_Bricks)
    {
        brick.Draw(pDC);
    }

    // 绘制马里奥
    m_Mario.Draw(pDC);

    // 可选：绘制碰撞区域（调试用）
    if (FALSE) // 设置为TRUE可以显示碰撞区域
    {
        CBrush redBrush(RGB(255, 0, 0));
        CBrush greenBrush(RGB(0, 255, 0));
        CBrush blueBrush(RGB(0, 0, 255));

        // 绘制脚部碰撞区域
        CRect feet = m_Mario.GetFeetRect();
        pDC->FrameRect(&feet, &redBrush);

        // 绘制头部碰撞区域
        CRect head = m_Mario.GetHeadRect();
        pDC->FrameRect(&head, &greenBrush);

        // 绘制身体碰撞区域
        CRect body = m_Mario.GetBodyRect();
        pDC->FrameRect(&body, &blueBrush);
    }

    // 绘制调试信息
    CString strInfo;
    CString stateStr;

    if (m_Mario.IsOnGround())
        stateStr = _T("地面");
    else if (m_Mario.IsJumping())
        stateStr = _T("跳跃");
    else
        stateStr = _T("空中");

    strInfo.Format(_T("位置: (%d, %d) - 状态: %s - 按1/2/3切换状态"),
        m_Mario.GetX(), m_Mario.GetY(), stateStr);

    pDC->SetTextColor(RGB(255, 255, 255));
    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(10, 10, strInfo);

    // 显示马里奥状态
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

    pDC->TextOut(10, 30, _T("状态: ") + strMarioState);

    // 显示操作提示
    pDC->TextOut(10, 50, _T("操作: 方向键移动, 空格键跳跃"));
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

    // 更新马里奥状态
    m_Mario.Update(m_fDeltaTime);

    // 更新砖块动画
    for (auto& brick : m_Bricks)
    {
        brick.Update(m_fDeltaTime);
    }
    // 简化的碰撞检测
    std::vector<CRect> solidObjects = GetAllSolidObjects();
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
// 简化的调试碰撞区域绘制
void CMFCApplication1View::DrawDebugCollision(CDC* pDC)
{
    if (!m_bDebugMode) return;

    // 创建画笔
    CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&redPen);

    // 只绘制主要的碰撞框
    CRect marioRect = m_Mario.GetRect();
    pDC->MoveTo(marioRect.left, marioRect.top);
    pDC->LineTo(marioRect.right, marioRect.top);
    pDC->LineTo(marioRect.right, marioRect.bottom);
    pDC->LineTo(marioRect.left, marioRect.bottom);
    pDC->LineTo(marioRect.left, marioRect.top);

    // 恢复原来的画笔
    pDC->SelectObject(pOldPen);

    // 绘制所有实体的碰撞框
    std::vector<CRect> solidObjects = GetAllSolidObjects();
    for (const auto& rect : solidObjects)
    {
        pDC->MoveTo(rect.left, rect.top);
        pDC->LineTo(rect.right, rect.top);
        pDC->LineTo(rect.right, rect.bottom);
        pDC->LineTo(rect.left, rect.bottom);
        pDC->LineTo(rect.left, rect.top);
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
    DWORD dwCurrentTime = GetTickCount();
    m_fDeltaTime = (dwCurrentTime - m_dwLastTime) / 1000.0f; // 转换为秒

    // 限制最大deltaTime，避免卡顿导致的大跳帧
    if (m_fDeltaTime > 0.1f)
        m_fDeltaTime = 0.1f;

    m_dwLastTime = dwCurrentTime;
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

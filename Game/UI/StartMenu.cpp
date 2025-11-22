// Game/UI/StartMenu.cpp

#include <afxwin.h>
#include "StartMenu.h"

CStartMenu::CStartMenu()
    : m_nSelectedLevel(1)
    , m_bStartGame(FALSE)
    , m_fAnimationTime(0.0f)
    , m_nTitleOffsetY(0)
    , m_bHoverStart(FALSE)
{
    for (int i = 0; i < 3; i++)
    {
        m_bHoverLevel[i] = FALSE;
    }
    Initialize();
}

CStartMenu::~CStartMenu()
{
}

void CStartMenu::Initialize()
{
    int screenWidth = CGameConfig::SCREEN_WIDTH;
    int screenHeight = CGameConfig::SCREEN_HEIGHT;

    // 设置关卡按钮位置
    int buttonWidth = 120;
    int buttonHeight = 60;
    int startX = screenWidth / 2 - (buttonWidth * 3 + 40) / 2;
    int startY = 250;

    for (int i = 0; i < 3; i++)
    {
        m_LevelButtons[i] = CRect(
            startX + i * (buttonWidth + 20),
            startY,
            startX + i * (buttonWidth + 20) + buttonWidth,
            startY + buttonHeight
        );
    }

    // 设置开始按钮位置
    m_StartButton = CRect(
        screenWidth / 2 - 80,
        startY + 100,
        screenWidth / 2 + 80,
        startY + 140
    );
}

void CStartMenu::Draw(CDC* pDC)
{
    // 绘制更漂亮的渐变背景
    for (int y = 0; y < CGameConfig::SCREEN_HEIGHT; y++)
    {
        // 从深蓝到紫色的渐变
        int blue = 100 + (y * 100 / CGameConfig::SCREEN_HEIGHT);
        int red = 50 + (y * 50 / CGameConfig::SCREEN_HEIGHT);
        pDC->FillSolidRect(0, y, CGameConfig::SCREEN_WIDTH, 1,
            RGB(red, 60, blue));
    }

    // 绘制星空效果
    pDC->SetPixel(100, 80, RGB(255, 255, 255));
    pDC->SetPixel(200, 120, RGB(255, 255, 200));
    pDC->SetPixel(300, 60, RGB(200, 200, 255));
    pDC->SetPixel(400, 100, RGB(255, 255, 255));
    pDC->SetPixel(500, 140, RGB(255, 200, 200));
    pDC->SetPixel(600, 70, RGB(200, 255, 200));
    pDC->SetPixel(700, 110, RGB(255, 255, 255));

    // 绘制标题
    DrawTitle(pDC);

    // 绘制关卡按钮
    DrawLevelButtons(pDC);

    // 绘制开始按钮
    DrawStartButton(pDC);

    // 绘制操作提示
    pDC->SetTextColor(RGB(255, 255, 255));
    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(20, CGameConfig::SCREEN_HEIGHT - 30,
        _T("点击关卡选择，点击开始游戏"));
}

void CStartMenu::DrawTitle(CDC* pDC)
{
    // 计算标题动画
    m_nTitleOffsetY = static_cast<int>(sin(m_fAnimationTime * 2) * 8);

    // 绘制主标题
    pDC->SetTextColor(RGB(255, 255, 0));
    pDC->SetBkMode(TRANSPARENT);

    CFont titleFont;
    titleFont.CreatePointFont(480, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&titleFont);

    CString title = _T("超级马里奥");
    CSize textSize = pDC->GetTextExtent(title);
    pDC->TextOut(CGameConfig::SCREEN_WIDTH / 2 - textSize.cx / 2,
        80 + m_nTitleOffsetY, title);
    // 主标题（黄色带红边效果）
    pDC->SetTextColor(RGB(255, 220, 0));  // 金黄色

    // 绘制多个偏移的副本来创建描边效果
    pDC->SetTextColor(RGB(255, 0, 0));
    pDC->TextOut(CGameConfig::SCREEN_WIDTH / 2 - textSize.cx / 2 - 1,
        80 + m_nTitleOffsetY - 1, title);
    pDC->TextOut(CGameConfig::SCREEN_WIDTH / 2 - textSize.cx / 2 + 1,
        80 + m_nTitleOffsetY - 1, title);
    pDC->TextOut(CGameConfig::SCREEN_WIDTH / 2 - textSize.cx / 2 - 1,
        80 + m_nTitleOffsetY + 1, title);
    pDC->TextOut(CGameConfig::SCREEN_WIDTH / 2 - textSize.cx / 2 + 1,
        80 + m_nTitleOffsetY + 1, title);

    pDC->SetTextColor(RGB(255, 255, 0));  // 亮黄色
    pDC->TextOut(CGameConfig::SCREEN_WIDTH / 2 - textSize.cx / 2,
        80 + m_nTitleOffsetY, title);

    pDC->SelectObject(pOldFont);

    // 绘制副标题
    CFont subFont;
    subFont.CreatePointFont(180, _T("Arial"));
    pDC->SelectObject(&subFont);

    pDC->SetTextColor(RGB(200, 200, 255));
    CString subTitle = _T("MFC 重制版");
    CSize subSize = pDC->GetTextExtent(subTitle);
    pDC->TextOut(CGameConfig::SCREEN_WIDTH / 2 - subSize.cx / 2,
        150 + m_nTitleOffsetY, subTitle);

    pDC->SelectObject(pOldFont);
}

void CStartMenu::DrawLevelButtons(CDC* pDC)
{
    // 绘制关卡选择标题
    pDC->SetTextColor(RGB(255, 255, 255));
    pDC->SetBkMode(TRANSPARENT);

    CFont font;
    font.CreatePointFont(180, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font);

    CString selectText = _T("选择关卡");
    CSize textSize = pDC->GetTextExtent(selectText);
    pDC->TextOut(CGameConfig::SCREEN_WIDTH / 2 - textSize.cx / 2, 200, selectText);

    pDC->SelectObject(pOldFont);

    // 绘制三个关卡按钮
    for (int i = 0; i < 3; i++)
    {
        CRect rect = m_LevelButtons[i];
        BOOL selected = (i + 1 == m_nSelectedLevel);
        BOOL hover = m_bHoverLevel[i];
        // 按钮颜色 - 更丰富的颜色
        COLORREF bgColor, borderColor, textColor;

        if (selected)
        {
            bgColor = RGB(255, 200, 50);   // 选中状态 - 金色
            borderColor = RGB(255, 255, 100);
            textColor = RGB(0, 0, 0);      // 黑色文字
        }
        else if (hover)
        {
            bgColor = RGB(80, 150, 255);   // 悬停状态 - 亮蓝色
            borderColor = RGB(150, 200, 255);
            textColor = RGB(255, 255, 255); // 白色文字
        }
        else
        {
            bgColor = RGB(70, 90, 180);    // 普通状态 - 深蓝色
            borderColor = RGB(120, 140, 220);
            textColor = RGB(255, 255, 255); // 白色文字
        }

        // 绘制按钮背景（带圆角效果）
        DrawRoundRect(pDC, rect, bgColor, borderColor);

        // 绘制关卡图标
        DrawLevelIcon(pDC, rect, i + 1, selected);

        // 绘制关卡文字
        CString levelText;
        levelText.Format(_T("关卡 %d"), i + 1);
        pDC->SetTextColor(textColor);
        pDC->SetBkMode(TRANSPARENT);

        CFont btnFont;
        btnFont.CreatePointFont(120, _T("Arial"));
        pDC->SelectObject(&btnFont);

        CSize levelSize = pDC->GetTextExtent(levelText);
        pDC->TextOut(rect.left + rect.Width() / 2 - levelSize.cx / 2,
            rect.top + rect.Height() - 25,
            levelText);

        pDC->SelectObject(pOldFont);
    }
}

void CStartMenu::DrawStartButton(CDC* pDC)
{
    CRect rect = m_StartButton;
    BOOL hover = m_bHoverStart;

    // 按钮颜色
    COLORREF bgColor, borderColor;

    if (hover)
    {
        bgColor = RGB(0, 200, 0);      // 悬停状态 - 亮绿色
        borderColor = RGB(100, 255, 100);
    }
    else
    {
        bgColor = RGB(0, 150, 0);      // 普通状态 - 绿色
        borderColor = RGB(0, 200, 0);
    }

    // 绘制按钮背景（带圆角效果）
    DrawRoundRect(pDC, rect, bgColor, borderColor);

    // 绘制开始文字
    CString startText = _T("开始冒险!");
    pDC->SetTextColor(RGB(255, 255, 255));
    pDC->SetBkMode(TRANSPARENT);

    CFont font;
    font.CreatePointFont(140, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font);

    CSize textSize = pDC->GetTextExtent(startText);
    pDC->TextOut(rect.left + rect.Width() / 2 - textSize.cx / 2,
        rect.top + rect.Height() / 2 - textSize.cy / 2,
        startText);

    pDC->SelectObject(pOldFont);

    // 绘制小星星动画
    if (static_cast<int>(m_fAnimationTime * 10) % 2 == 0)
    {
        pDC->SetTextColor(RGB(255, 255, 0));
        pDC->TextOut(rect.left - 20, rect.top + 10, _T("★"));
        pDC->TextOut(rect.right + 5, rect.bottom - 25, _T("★"));
    }
}


BOOL CStartMenu::HandleMouseClick(CPoint point)
{
    // 检查关卡按钮点击
    for (int i = 0; i < 3; i++)
    {
        if (m_LevelButtons[i].PtInRect(point))
        {
            m_nSelectedLevel = i + 1;
            return TRUE;
        }
    }

    // 检查开始按钮点击
    if (m_StartButton.PtInRect(point))
    {
        m_bStartGame = TRUE;
        return TRUE;
    }

    return FALSE;
}

void CStartMenu::Update(float deltaTime)
{
    m_fAnimationTime += deltaTime;

    // 更新悬停状态（这里需要外部提供鼠标位置，暂时不实现）
    // 在实际使用中，可以在视图类中处理鼠标移动事件来更新悬停状态
}
void CStartMenu::DrawInstructions(CDC* pDC)
{
    pDC->SetTextColor(RGB(200, 200, 255));
    pDC->SetBkMode(TRANSPARENT);

    CFont font;
    font.CreatePointFont(100, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font);

    CString instructions = _T("点击关卡选择，点击开始游戏");
    CSize textSize = pDC->GetTextExtent(instructions);
    pDC->TextOut(CGameConfig::SCREEN_WIDTH / 2 - textSize.cx / 2,
        CGameConfig::SCREEN_HEIGHT - 60, instructions);

    pDC->SelectObject(pOldFont);
}

void CStartMenu::DrawCopyright(CDC* pDC)
{
    pDC->SetTextColor(RGB(150, 150, 200));
    pDC->SetBkMode(TRANSPARENT);

    CFont font;
    font.CreatePointFont(80, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font);

    CString copyright = _T("© 2024 超级马里奥 MFC版");
    CSize textSize = pDC->GetTextExtent(copyright);
    pDC->TextOut(CGameConfig::SCREEN_WIDTH / 2 - textSize.cx / 2,
        CGameConfig::SCREEN_HEIGHT - 30, copyright);

    pDC->SelectObject(pOldFont);
}

// 新增：绘制圆角矩形
void CStartMenu::DrawRoundRect(CDC* pDC, CRect rect, COLORREF fillColor, COLORREF borderColor)
{
    // 绘制填充
    CBrush brush(fillColor);
    CBrush* pOldBrush = pDC->SelectObject(&brush);
    CPen pen(PS_SOLID, 2, borderColor);
    CPen* pOldPen = pDC->SelectObject(&pen);

    // 简单的圆角矩形（通过绘制椭圆角实现）
    pDC->RoundRect(rect, CPoint(15, 15));

    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
}

// 新增：绘制关卡图标
void CStartMenu::DrawLevelIcon(CDC* pDC, CRect rect, int level, BOOL selected)
{
    // 在按钮顶部绘制小图标
    int iconSize = 30;
    int iconX = rect.left + rect.Width() / 2 - iconSize / 2;
    int iconY = rect.top + 10;

    COLORREF iconColor;
    switch (level)
    {
    case 1:
        iconColor = RGB(100, 200, 100);  // 绿色
        break;
    case 2:
        iconColor = RGB(200, 150, 50);   // 橙色
        break;
    case 3:
        iconColor = RGB(200, 50, 50);    // 红色
        break;
    default:
        iconColor = RGB(150, 150, 150);
        break;
    }

    if (selected)
    {
        // 选中时图标更亮
        iconColor = RGB(
            GetRValue(iconColor) + 50,
            GetGValue(iconColor) + 50,
            GetBValue(iconColor) + 50
        );
    }

    // 绘制简单的图标（圆形）
    CBrush brush(iconColor);
    CBrush* pOldBrush = pDC->SelectObject(&brush);
    CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
    CPen* pOldPen = pDC->SelectObject(&pen);

    pDC->Ellipse(iconX, iconY, iconX + iconSize, iconY + iconSize);

    // 在图标中绘制关卡数字
    pDC->SetTextColor(RGB(255, 255, 255));
    pDC->SetBkMode(TRANSPARENT);

    CFont font;
    font.CreatePointFont(120, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font);

    CString levelStr;
    levelStr.Format(_T("%d"), level);
    CSize textSize = pDC->GetTextExtent(levelStr);
    pDC->TextOut(iconX + iconSize / 2 - textSize.cx / 2,
        iconY + iconSize / 2 - textSize.cy / 2,
        levelStr);

    pDC->SelectObject(pOldFont);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
}
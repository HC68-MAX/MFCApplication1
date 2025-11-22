#pragma once
// Game/UI/StartMenu.h

#include <afxwin.h>
#include <vector>
#include "../Core/GameConfig.h"

class CStartMenu
{
public:
    CStartMenu();
    ~CStartMenu();

    // 初始化菜单
    void Initialize();

    // 绘制菜单
    void Draw(CDC* pDC);

    // 处理鼠标点击
    BOOL HandleMouseClick(CPoint point);

    // 获取选中的关卡
    int GetSelectedLevel() const { return m_nSelectedLevel; }

    // 检查是否开始游戏
    BOOL ShouldStartGame() const { return m_bStartGame; }

    // 重置开始状态
    void ResetStartState() { m_bStartGame = FALSE; }

    // 更新动画
    void Update(float deltaTime);

private:
    void DrawTitle(CDC* pDC);
    void DrawLevelButtons(CDC* pDC);
    void DrawStartButton(CDC* pDC);
    void DrawRoundRect(CDC* pDC, CRect rect, COLORREF fillColor, COLORREF borderColor);
    void DrawLevelIcon(CDC* pDC, CRect rect, int level, BOOL selected);
    void DrawInstructions(CDC* pDC);
    void DrawCopyright(CDC* pDC);
private:
    int m_nSelectedLevel;    // 选中的关卡 (1-3)
    BOOL m_bStartGame;       // 是否开始游戏

    // 按钮区域
    CRect m_LevelButtons[3]; // 三个关卡按钮
    CRect m_StartButton;     // 开始按钮

    // 动画相关
    float m_fAnimationTime;
    int m_nTitleOffsetY;

    // 按钮状态
    BOOL m_bHoverLevel[3];
    BOOL m_bHoverStart;
};
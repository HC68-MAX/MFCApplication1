#include <afxwin.h>
#include "Flagpole.h"

CFlagpole::CFlagpole() : CGameObject(0, 0, CGameConfig::FLAGPOLE_WIDTH, CGameConfig::FLAGPOLE_HEIGHT)
{
    m_nFlagY = 0;
    m_bFlagDown = FALSE;
}

CFlagpole::CFlagpole(int x, int y) : CGameObject(x, y, CGameConfig::FLAGPOLE_WIDTH, CGameConfig::FLAGPOLE_HEIGHT)
{
    m_nFlagY = 0;
    m_bFlagDown = FALSE;
}

CFlagpole::~CFlagpole()
{
}

void CFlagpole::Update(float deltaTime)
{
}

void CFlagpole::Draw(CDC* pDC)
{
    // 这里的Draw通常不被直接调用，而是由TileMap调用DrawWithSprite
}

BOOL CFlagpole::CheckTouch(const CRect& rect) const
{
    // 简单的矩形碰撞检测
    CRect flagpoleRect = GetRect();
    // 稍微缩小一点碰撞范围，只检测旗杆主体
    flagpoleRect.left += 10;
    flagpoleRect.right -= 10;
    
    CRect intersect;
    return intersect.IntersectRect(&flagpoleRect, &rect);
}
// 绘制旗子：核心逻辑，在这里处理下落的Y偏移
void CFlagpole::DrawFlag(CDC* pDC, int screenX, int screenY)
{
    // 如果旗子不可见，直接返回
    if (!m_bVisible)
        return;

    // 1. 触发下落后，让旗子的Y偏移逐步增加（直到最大距离）
    if (m_bFlagDown && m_nFlagY < m_nMaxFallY)
    {
        m_nFlagY += m_nFallStep; // 每次绘制下落固定步长
        // 防止超出最大下落距离（兜底）
        if (m_nFlagY > m_nMaxFallY)
            m_nFlagY = m_nMaxFallY;
    }

    // 2. 获取主瓦片集（和砖块、旗杆共用，避免贴图缺失）
    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = resMgr.GetBitmap(_T("TilesetMain"));
    if (!pBitmap) // 贴图未找到时打印日志，避免崩溃
    {
        TRACE(_T("警告：TilesetMain 贴图未找到，无法绘制旗子\n"));
        return;
    }
    SSpriteCoord flagCoord= CSpriteConfig::FLAGPOLE_FLAG;
    // 4. 绘制旗子（核心：Y坐标加上下落偏移量）
    CSpriteRenderer::DrawSprite(
        pDC,                // 设备上下文
        pBitmap,            // 瓦片集贴图
        screenX ,           // 旗子的屏幕X坐标
        screenY + m_nFlagY, // 旗子的屏幕Y坐标（加上下落偏移）
        32, 32,             // 旗子的显示宽高（和瓦片一致）
        flagCoord.x, flagCoord.y, // 瓦片集中旗子的坐标
        flagCoord.width, flagCoord.height // 瓦片集中旗子的宽高
    );
}
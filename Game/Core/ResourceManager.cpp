// Game/Core/ResourceManager.cpp
#include <afxwin.h>
#include "ResourceManager.h"
#include "GameConfig.h" 
CResourceManager& CResourceManager::GetInstance()
{
    static CResourceManager instance;
    return instance;
}

CResourceManager::~CResourceManager()
{
    Cleanup();
}

BOOL CResourceManager::LoadBitmap(UINT resourceID, const CString& resourceName)
{
    // 检查是否已加载
    if (m_Bitmaps.find(resourceName) != m_Bitmaps.end())
    {
        return TRUE; // 已存在
    }

    // 创建新位图
    CBitmap* pBitmap = new CBitmap();
    if (!pBitmap->LoadBitmap(resourceID))
    {
        delete pBitmap;
        // 如果资源不存在，创建一个彩色占位符
        return CreatePlaceholderBitmap(resourceName, GetPlaceholderColor(resourceName));
    }

    m_Bitmaps[resourceName] = pBitmap;
    return TRUE;
}

// 新增：创建占位符位图
BOOL CResourceManager::CreatePlaceholderBitmap(const CString& resourceName, COLORREF color)
{
    TRACE(_T("创建占位符位图: %s\n"), resourceName);

    // 使用桌面DC创建兼容位图
    CWindowDC screenDC(NULL); // 使用桌面DC

    // 创建位图
    CBitmap* pBitmap = new CBitmap();
    if (!pBitmap->CreateCompatibleBitmap(&screenDC, 32, 32))
    {
        TRACE(_T("创建兼容位图失败\n"));
        delete pBitmap;
        return FALSE;
    }

    // 创建内存DC来绘制到位图
    CDC memDC;
    if (!memDC.CreateCompatibleDC(&screenDC))
    {
        TRACE(_T("创建内存DC失败\n"));
        delete pBitmap;
        return FALSE;
    }

    // 选择位图到内存DC
    CBitmap* pOldBitmap = memDC.SelectObject(pBitmap);

    // 填充背景色
    CBrush brush(color);
    memDC.FillRect(CRect(0, 0, 32, 32), &brush);

    // 绘制边框
    memDC.DrawEdge(CRect(0, 0, 32, 32), EDGE_RAISED, BF_RECT);

    // 绘制资源名称
    memDC.SetTextColor(RGB(255, 255, 255));
    memDC.SetBkColor(color);
    memDC.TextOut(2, 10, resourceName.Left(4));

    // 恢复并清理
    memDC.SelectObject(pOldBitmap);
    memDC.DeleteDC();

    m_Bitmaps[resourceName] = pBitmap;
    TRACE(_T("占位符位图创建成功: %s\n"), resourceName);
    return TRUE;
}
// 新增：根据资源名称获取占位符颜色
// 更新占位符颜色方法
COLORREF CResourceManager::GetPlaceholderColor(const CString& resourceName)
{
    // 使用全局配置中的颜色常量
    if (resourceName.Find(_T("Mario")) != -1) return CGameConfig::COLOR_MARIO_RED;
    if (resourceName.Find(_T("Brick")) != -1) return CGameConfig::COLOR_BRICK;
    if (resourceName.Find(_T("Question")) != -1) return CGameConfig::COLOR_QUESTION;
    if (resourceName.Find(_T("Hard")) != -1) return CGameConfig::COLOR_HARD_BRICK;
    if (resourceName.Find(_T("Pipe")) != -1) return CGameConfig::COLOR_PIPE;
    if (resourceName.Find(_T("Ground")) != -1) return CGameConfig::COLOR_GROUND;
    if (resourceName.Find(_T("Background")) != -1) return CGameConfig::COLOR_BACKGROUND_;
    if (resourceName.Find(_T("Coin")) != -1) return CGameConfig::COLOR_COIN;
    if (resourceName.Find(_T("Goomba")) != -1) return CGameConfig::COLOR_ENEMY;

    return RGB(200, 200, 200); // 默认浅灰色
}
BOOL CResourceManager::LoadBitmapFromFile(const CString& filePath, const CString& resourceName)
{
    // 注意：MFC直接加载外部文件比较复杂，这里先实现资源ID加载
    // 外部文件加载可以使用GDI+或其他图像库
    return FALSE; // 暂不实现
}

CBitmap* CResourceManager::GetBitmap(const CString& resourceName)
{
    auto it = m_Bitmaps.find(resourceName);
    if (it != m_Bitmaps.end())
    {
        return it->second;
    }
    return nullptr;
}

void CResourceManager::Cleanup()
{
    for (auto& pair : m_Bitmaps)
    {
        if (pair.second)
        {
            pair.second->DeleteObject();
            delete pair.second;
        }
    }
    m_Bitmaps.clear();
}
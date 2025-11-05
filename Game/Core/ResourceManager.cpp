// Game/Core/ResourceManager.cpp
#include <afxwin.h>
#include "ResourceManager.h"
#include "GameConfig.h" 
#include "ResourceIDs.h"
CResourceManager& CResourceManager::GetInstance()
{
    static CResourceManager instance;
    return instance;
}

CResourceManager::~CResourceManager()
{
    Cleanup();
}
// 加载游戏所需的所有资源
BOOL CResourceManager::LoadGameResources()
{
    TRACE(_T("开始加载游戏资源...\n"));

    BOOL success = TRUE;

    // 加载主贴图集
    success = LoadBitmap(IDB_TILESET_MAIN, _T("TilesetMain"));
    TRACE(_T("主贴图集加载: %s\n"), success ? _T("成功") : _T("失败"));

    // 加载马里奥贴图集
    success &= LoadBitmap(IDB_MARIO_SPRITES, _T("MarioSprites"));
    TRACE(_T("马里奥贴图集加载: %s\n"), success ? _T("成功") : _T("失败"));

    if (!success) {
        TRACE(_T("错误: 无法加载主贴图集!\n"));
        // 创建占位符
        CreatePlaceholderBitmap(_T("TilesetMain"), RGB(255, 0, 0)); // 红色占位符
    }

    // 测试获取贴图
    CBitmap* pTest = GetBitmap(_T("TilesetMain"));
    if (pTest)
    {
        BITMAP bm;
        pTest->GetBitmap(&bm);
        TRACE(_T("贴图尺寸: %dx%d\n"), bm.bmWidth, bm.bmHeight);
    }
    else
    {
        TRACE(_T("错误: 获取贴图失败!\n"));
    }
    // 测试获取马里奥贴图
    CBitmap* pMarioTest = GetBitmap(_T("MarioSprites"));
    if (pMarioTest)
    {
        BITMAP bm;
        pMarioTest->GetBitmap(&bm);
        TRACE(_T("马里奥贴图尺寸: %dx%d\n"), bm.bmWidth, bm.bmHeight);
    }
    return success;
}

BOOL CResourceManager::LoadBitmap(UINT resourceID, const CString& resourceName)
{
    // 检查是否已加载
    if (m_Bitmaps.find(resourceName) != m_Bitmaps.end())
    {
        TRACE(_T("贴图已加载: %s\n"), resourceName);
        return TRUE;
    }

    // 创建新位图
    CBitmap* pBitmap = new CBitmap();
    if (!pBitmap->LoadBitmap(resourceID))
    {
        TRACE(_T("错误: 无法加载位图资源 ID=%d, Name=%s\n"), resourceID, resourceName);
        delete pBitmap;
        return FALSE;
    }

    m_Bitmaps[resourceName] = pBitmap;
    TRACE(_T("成功加载贴图: %s (ID=%d)\n"), resourceName, resourceID);
    return TRUE;
}

CBitmap* CResourceManager::GetBitmap(const CString& resourceName)
{
    if (resourceName == _T("Brick")) {
        TRACE(_T("注意: 将Brick请求重定向到TilesetMain\n"));
        return GetBitmap(_T("TilesetMain"));
    }
    auto it = m_Bitmaps.find(resourceName);
    if (it != m_Bitmaps.end())
    {
        return it->second;
    }

    TRACE(_T("警告: 贴图未找到: %s\n"), resourceName);
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
// 新增：创建占位符位图
// 创建占位符位图
BOOL CResourceManager::CreatePlaceholderBitmap(const CString& resourceName, COLORREF color)
{
    TRACE(_T("创建占位符位图: %s\n"), resourceName);

    // 使用桌面DC创建兼容位图
    CWindowDC screenDC(NULL);

    // 创建位图 - 根据资源类型设置不同大小
    CBitmap* pBitmap = new CBitmap();
    int width = 64, height = 64;

    if (resourceName.Find(_T("Background")) != -1) {
        width = 256; height = 256;
    }
    else if (resourceName.Find(_T("Spritesheet")) != -1) {
        width = 512; height = 512;
    }

    if (!pBitmap->CreateCompatibleBitmap(&screenDC, width, height))
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
    memDC.FillRect(CRect(0, 0, width, height), &brush);

    // 绘制边框
    memDC.DrawEdge(CRect(0, 0, width, height), EDGE_RAISED, BF_RECT);

    // 绘制资源名称
    memDC.SetTextColor(RGB(255, 255, 255));
    memDC.SetBkColor(color);
    memDC.SetBkMode(OPAQUE);

    CString displayName = resourceName;
    if (displayName.GetLength() > 12) {
        displayName = displayName.Left(12) + _T("...");
    }
    memDC.TextOut(5, height / 2 - 8, displayName);

    // 恢复并清理
    memDC.SelectObject(pOldBitmap);
    memDC.DeleteDC();

    m_Bitmaps[resourceName] = pBitmap;
    TRACE(_T("占位符位图创建成功: %s (%dx%d)\n"), resourceName, width, height);
    return TRUE;
}

// 根据资源名称获取占位符颜色
COLORREF CResourceManager::GetPlaceholderColor(const CString& resourceName)
{
    if (resourceName.Find(_T("Mario")) != -1) return CGameConfig::COLOR_MARIO_RED;
    if (resourceName.Find(_T("Brick")) != -1) return CGameConfig::COLOR_BRICK;
    if (resourceName.Find(_T("Question")) != -1) return CGameConfig::COLOR_QUESTION;
    if (resourceName.Find(_T("Hard")) != -1) return CGameConfig::COLOR_HARD_BRICK;
    if (resourceName.Find(_T("Pipe")) != -1) return CGameConfig::COLOR_PIPE;
    if (resourceName.Find(_T("Ground")) != -1) return CGameConfig::COLOR_GROUND;
    if (resourceName.Find(_T("Background")) != -1) return CGameConfig::COLOR_BACKGROUND_;
    if (resourceName.Find(_T("Coin")) != -1) return CGameConfig::COLOR_COIN;
    if (resourceName.Find(_T("Goomba")) != -1) return CGameConfig::COLOR_ENEMY;
    if (resourceName.Find(_T("Spritesheet")) != -1) return RGB(100, 100, 200);

    return RGB(200, 200, 200); // 默认浅灰色
}
// 新增：根据资源名称获取占位符颜色
// 更新占位符颜色方法
BOOL CResourceManager::LoadBitmapFromFile(const CString& filePath, const CString& resourceName)
{
    // 注意：MFC直接加载外部文件比较复杂，这里先实现资源ID加载
    // 外部文件加载可以使用GDI+或其他图像库
    return FALSE; // 暂不实现
}




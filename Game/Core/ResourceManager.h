#pragma once
// Game/Core/ResourceManager.h
#pragma once

#include <afxwin.h>
#include <map>
#include <string>
#include "ResourceIDs.h"
class CResourceManager
{
public:
    // 单例模式访问
    static CResourceManager& GetInstance();

    // 新增：加载游戏所需的所有资源
    BOOL LoadGameResources();

    // 资源加载
    BOOL LoadBitmap(UINT resourceID, const CString& resourceName);
    BOOL LoadBitmapFromFile(const CString& filePath, const CString& resourceName);

    // 资源获取
    CBitmap* GetBitmap(const CString& resourceName);
    BOOL  CreatePlaceholderBitmap(const CString& resourceName, COLORREF color);
    // 新增：根据资源名称获取占位符颜色
    COLORREF GetPlaceholderColor(const CString& resourceName);

    // 资源清理
    void Cleanup();

private:
    CResourceManager() = default;
    ~CResourceManager();
    CResourceManager(const CResourceManager&) = delete;
    CResourceManager& operator=(const CResourceManager&) = delete;

private:
    std::map<CString, CBitmap*> m_Bitmaps;
};
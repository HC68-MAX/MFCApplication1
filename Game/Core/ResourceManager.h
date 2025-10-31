#pragma once
// Game/Core/ResourceManager.h
#pragma once

#include <afxwin.h>
#include <map>
#include <string>

class CResourceManager
{
public:
    // 单例模式访问
    static CResourceManager& GetInstance();

    // 资源加载
    BOOL LoadBitmap(UINT resourceID, const CString& resourceName);
    BOOL LoadBitmapFromFile(const CString& filePath, const CString& resourceName);

    // 资源获取
    CBitmap* GetBitmap(const CString& resourceName);

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
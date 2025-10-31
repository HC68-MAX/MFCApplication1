// Game/Core/ResourceManager.cpp
#include <afxwin.h>
#include "ResourceManager.h"

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
        return FALSE;
    }

    m_Bitmaps[resourceName] = pBitmap;
    return TRUE;
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
#pragma once
// Game/Core/ResourceManager.h
#pragma once

#include <afxwin.h>
#include <map>
#include <string>
#define IDB_MARIO_SMALL                130
#define IDB_MARIO_BIG                  131
#define IDB_MARIO_FIRE                 132
#define IDB_BRICK                      133
#define IDB_QUESTION_BLOCK             134
#define IDB_HARD_BRICK                 135
#define IDB_PIPE                       136
#define IDB_GROUND                     137
#define IDB_BACKGROUND                 138
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
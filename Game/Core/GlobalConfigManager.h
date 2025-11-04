// Game/Core/GlobalConfigManager.h
#pragma once

#include <afxwin.h>
#include <map>
#include <vector>
class CGlobalConfigManager
{
public:
    static CGlobalConfigManager& GetInstance();

    // === 配置获取方法 ===
    float GetSpriteScale() const { return m_fSpriteScale; }
    BOOL IsDebugMode() const { return m_bDebugMode; }
    float GetPhysicsMultiplier() const { return m_fPhysicsMultiplier; }
    BOOL IsSoundEnabled() const { return m_bSoundEnabled; }
    int GetMusicVolume() const { return m_nMusicVolume; }

    // === 配置变更通知 ===
    typedef void (*ConfigChangedCallback)(const CString& key, const CString& value);


private:
    CGlobalConfigManager();
    ~CGlobalConfigManager();

    void InitializeDefaults();

private:
    // 配置数据
    float m_fSpriteScale;
    BOOL m_bDebugMode;
    float m_fPhysicsMultiplier;
    BOOL m_bSoundEnabled;
    int m_nMusicVolume;

    // 回调函数列表
    std::vector<ConfigChangedCallback> m_Callbacks;

    // 配置键值映射
    std::map<CString, CString> m_ConfigMap;
};
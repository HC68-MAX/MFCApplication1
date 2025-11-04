// Game/Core/GlobalConfigManager.cpp
#include <afxwin.h>
#include "GlobalConfigManager.h"
#include "GameConfig.h"

CGlobalConfigManager& CGlobalConfigManager::GetInstance()
{
    static CGlobalConfigManager instance;
    return instance;
}

CGlobalConfigManager::CGlobalConfigManager()
{
    InitializeDefaults();
}

CGlobalConfigManager::~CGlobalConfigManager()
{
    // 自动保存配置
  
}

void CGlobalConfigManager::InitializeDefaults()
{
    // 使用GameConfig的默认值
    m_fSpriteScale = 1.0f;
    m_bDebugMode = CGameConfig::DEBUG_SHOW_COLLISION;
    m_fPhysicsMultiplier = 1.0f;
    m_bSoundEnabled = TRUE;
    m_nMusicVolume = 80;
}


// 其他方法实现...
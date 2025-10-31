
// MFCApplication1View.h: CMFCApplication1View 类的接口
//

#pragma once
#include "Game/Objects/Mario.h"
#include "Game/Objects/Pipe.h"
#include "Game/Objects/Brick.h"
#include <vector>
class CMFCApplication1View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication1View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication1View)

// 特性
public:
	CMFCApplication1Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCApplication1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 游戏相关成员变量
protected:
	// 双缓冲绘图
	CDC m_memDC;
	CBitmap m_memBitmap;
	CBitmap* m_pOldBitmap;

	// 游戏循环定时器
	UINT_PTR m_nTimerID;

	// 屏幕尺寸
	int m_nScreenWidth;
	int m_nScreenHeight;
/*
	// 测试用 - 马里奥位置
	int m_nMarioX;
	int m_nMarioY;
	int m_nMarioWidth;
	int m_nMarioHeight;
*/
// 添加新的马里奥对象
	CMario m_Mario;  // 马里奥对象实例

	// 添加帧率计算相关
	DWORD m_dwLastTime;
	int m_nFrameCount;
	float m_fDeltaTime;

	// 输入状态变量
	BOOL m_bKeyLeft;
	BOOL m_bKeyRight;
	BOOL m_bKeyJump;
	BOOL m_bDebugMode;
	// 地图元素容器
	std::vector<CBrick> m_Bricks;
	std::vector<CPipe> m_Pipes;
// 生成的消息映射函数
protected:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()

//游戏相关函数
protected:
	void InitializeGame();
	void InitializeMap();
	void UpdateGame();
	void RenderGame(CDC* pDC);
	void CleanupGame();
	void CalculateDeltaTime();  // 新增：计算帧时间差
	// 新增碰撞检测方法
	void CheckMarioCollisions();
	std::vector<CRect> GetAllPlatformRects() const;
	void HandleBrickCollisions();
	void DrawDebugCollision(CDC* pDC);
	void DrawDebugInfo(CDC* pDC);
	// 新增调试绘制方法
	void DrawCollisionPoints(CDC* pDC, const CRect& marioRect);
	void DrawCurrentCollisions(CDC* pDC, const std::vector<CRect>& solidObjects, const CRect& marioRect);
	void DrawCoordinateGrid(CDC* pDC);
	std::vector<CRect> CMFCApplication1View::GetAllSolidObjects() const;
};

#ifndef _DEBUG  // MFCApplication1View.cpp 中的调试版本
inline CMFCApplication1Doc* CMFCApplication1View::GetDocument() const
   { return reinterpret_cast<CMFCApplication1Doc*>(m_pDocument); }
#endif


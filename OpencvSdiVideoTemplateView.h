
// OpencvSdiVideoTemplateView.h: COpencvSdiVideoTemplateView 클래스의 인터페이스
//

#include "opencv2/opencv.hpp"
#include "CCvImgWrapper.h"

#pragma once


class COpencvSdiVideoTemplateView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	COpencvSdiVideoTemplateView() noexcept;
	DECLARE_DYNCREATE(COpencvSdiVideoTemplateView)

// 특성입니다.
public:
	COpencvSdiVideoTemplateDoc* GetDocument() const;

	CCvImgWrapper m_imgWrapper;

	BOOL		m_bQuit;			// 종료 메시지 발생했을 때 TRUE
	BOOL		m_bStopCurrentJob;	// esc 또는 종료 메시지 발생 시 true
	BOOL		m_bPlay, m_bPause;

	CRectTracker	m_tracker;
// 작업입니다.
public:

	VideoCapture& getVideo();
	CString		getPathName();
	void		setCurrentFrame(Mat& img) { GetDocument()->m_frame = img; }
	Mat& getCurrentFrame() { return GetDocument()->m_frame;  }

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~COpencvSdiVideoTemplateView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool resetVideo();
	int	processPendingMessages();
// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnFileOpen();
	afx_msg void OnPlayFile();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdatePlayFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // OpencvSdiVideoTemplateView.cpp의 디버그 버전
inline COpencvSdiVideoTemplateDoc* COpencvSdiVideoTemplateView::GetDocument() const
   { return reinterpret_cast<COpencvSdiVideoTemplateDoc*>(m_pDocument); }
#endif



// OpencvSdiVideoTemplateView.cpp: COpencvSdiVideoTemplateView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "OpencvSdiVideoTemplate.h"
#endif

#include "OpencvSdiVideoTemplateDoc.h"
#include "OpencvSdiVideoTemplateView.h"
#include "opencv2/opencv.hpp"

#include "opencv2/tracking/tracker.hpp"


#ifdef _AFXDLL
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG
#endif // _AFXDLL

using namespace cv;
using namespace std;

/*
	알려진 문제점

	재생 중에 다른 파일을 오픈하는 경우 OnPlayFile() 함수 내의 MessagePump에서 해당 처리를 하게 되어 스페이스바나 ESC에 계획된 동작이 수행되지 않는 경우가 발생한다.

	이것을 막으려면 쓰레드를 만들어서 비디오 영상을 따로 돌리는 것이 제일 확실한 방법이다.
	하지만 OpenCV를 Thread로 돌리는 경우 어떤 문제가 발생할지 그리고 어떤 생각지 못했던 일이 날지 알 수가 없어 쓰레드 사용은 일단 유보. ( 내가 쓰레드를 잘못다루는 것도 큰 이유)


*/

Ptr<Tracker> m_cvTracker = TrackerCSRT::create();
Rect2d m_roi = Rect2d();

VideoCapture& COpencvSdiVideoTemplateView::getVideo()
{
	COpencvSdiVideoTemplateDoc* pDoc = GetDocument();
	return pDoc->getVideo();
}

CString		COpencvSdiVideoTemplateView::getPathName()
{
	COpencvSdiVideoTemplateDoc* pDoc = GetDocument();
	return pDoc->getPathName();
}

// COpencvSdiVideoTemplateView

IMPLEMENT_DYNCREATE(COpencvSdiVideoTemplateView, CScrollView)

BEGIN_MESSAGE_MAP(COpencvSdiVideoTemplateView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
//	ON_COMMAND(ID_FILE_OPEN, &COpencvSdiVideoTemplateView::OnFileOpen)
	ON_COMMAND(ID_PLAY_FILE, &COpencvSdiVideoTemplateView::OnPlayFile)
	ON_WM_KEYDOWN()
//	ON_WM_KEYUP()
ON_UPDATE_COMMAND_UI(ID_PLAY_FILE, &COpencvSdiVideoTemplateView::OnUpdatePlayFile)
ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &COpencvSdiVideoTemplateView::OnUpdateFileOpen)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// COpencvSdiVideoTemplateView 생성/소멸

COpencvSdiVideoTemplateView::COpencvSdiVideoTemplateView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_bStopCurrentJob = FALSE;

	m_tracker.m_rect.SetRect(0, 0, 0, 0);
	m_tracker.m_nStyle = CRectTracker::hatchedBorder | CRectTracker::resizeOutside;
}

COpencvSdiVideoTemplateView::~COpencvSdiVideoTemplateView()
{
	if (!m_imgWrapper.isEmpty()) m_imgWrapper.destroy();
}

BOOL COpencvSdiVideoTemplateView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}


// COpencvSdiVideoTemplateView 인쇄

BOOL COpencvSdiVideoTemplateView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void COpencvSdiVideoTemplateView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void COpencvSdiVideoTemplateView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// COpencvSdiVideoTemplateView 진단

#ifdef _DEBUG
void COpencvSdiVideoTemplateView::AssertValid() const
{
	CScrollView::AssertValid();
}

void COpencvSdiVideoTemplateView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}


COpencvSdiVideoTemplateDoc* COpencvSdiVideoTemplateView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpencvSdiVideoTemplateDoc)));
	return (COpencvSdiVideoTemplateDoc*)m_pDocument;
}
#endif //_DEBUG

bool COpencvSdiVideoTemplateView::resetVideo()
{
	getVideo().release();
	
	m_bQuit = m_bStopCurrentJob = m_bPlay = m_bPause = FALSE;

	GetDocument()->openVideoFile(getPathName());

	//if (!getVideo().isOpened()) return false;

	//m_sPathName = sPath;
	//videoCap >> m_frame;
//	m_imgWrapper.create(m_frame);

	return true;
}

int COpencvSdiVideoTemplateView::processPendingMessages()
{
	TickMeter tm;
	tm.start();

	CWinApp* pApp = AfxGetApp();
	if (pApp == NULL) return 0;
	MSG msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)) {
		if (msg.message == WM_CLOSE || msg.message == WM_QUIT || msg.message == WM_DESTROY  ) {
			m_bStopCurrentJob = TRUE;
			m_bQuit = TRUE;
			break;
		}
		pApp->PumpMessage();
	}

	tm.stop();

	return tm.getTimeMilli();
}


// COpencvSdiVideoTemplateView 메시지 처리기


//void COpencvSdiVideoTemplateView::OnFileOpen()
//{
//	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, _T("동영상 파일(*.wmv;*.mpg;*.mpeg;*.mov;*.div;*.divx;*.mp4;*.avi)|*.wmv;*.mpg;*.mpeg;*.mov;*.div;*.divx;*.mp4;*.avi;|All Files(*.*)|*.*||"));
//	if (fileDlg.DoModal() != IDOK) return;
//
//	CString sPath = fileDlg.GetPathName();
//
//	if (!openVideoFile(sPath)) {
//		AfxMessageBox(sPath + " 동영상 파일 읽기 실패!!");
//		return;
//	}
//
//	GetDocument()->OnOpenDocument(sPath);
//	InvalidateRect(NULL);
//}

Rect2d CRect2Rect2d(CRect& rt)
{
	return Rect2d(rt.left, rt.top, rt.Width(), rt.Height());
}

void COpencvSdiVideoTemplateView::OnPlayFile()
{
	// 현재 선택 영역을 기반으로 Tracking 시작
//	if ( m_bStopCurrentJob == TRUE) return;


	if (!getVideo().isOpened())
	{
		AfxMessageBox(L"동영상 파일을 먼저 오픈하세요");
		return;
	}

	//m_roi = Rect2d(CRect2Rect2d(m_tracker.m_rect));
	//m_cvTracker->init(getCurrentFrame(), m_roi);

	double fps = getVideo().get(CAP_PROP_FPS);
	if (fps <= 0.0) fps = 30.0;
	int delay = (int)(1000.0 / fps);
	//int srcW = cap.get(CAP_PROP_FRAME_WIDTH);
	//int srcH = cap.get(CAP_PROP_FRAME_HEIGHT);
	//int dstW, dstH;

	CClientDC dc(this);
	Mat img;

	m_bStopCurrentJob = FALSE;
	m_bPlay = TRUE;

	TickMeter tm;
	while (!m_bStopCurrentJob) {

		tm.reset();
		if (!m_bPause) {

			tm.start();
			getVideo() >> img;

			if (img.empty()) break;

			setCurrentFrame( img );

			if (!m_roi.empty()) {
				//m_roi = Rect2d(CRect2Rect2d(m_tracker.m_rect));
				bool ret = m_cvTracker->update(img, m_roi);
				if (ret) {
					rectangle(getCurrentFrame(), m_roi, Scalar(255, 0, 0), 2, 1);
					//				m_tracker.m_rect.SetRect(roi.x, roi.y, roi.x + roi.width, roi.y + roi.height);
									//TRACE("ROI = x=%lf y=%lf w=%lf h=%lf\n", roi.x, roi.y, roi.width, roi.height);
				}
			}

			m_imgWrapper.replace(getCurrentFrame());
			m_imgWrapper.draw(dc, CRect());

			tm.stop();
		}

		//Sleep(delay);
		int nMills = processPendingMessages();
		int timeToSleep = delay - nMills - tm.getTimeMilli();
		if (timeToSleep > 0)
			Sleep(timeToSleep);
	}
//	m_imgWrapper.setImage(m_frame);

	if (!m_bQuit) {

		m_bStopCurrentJob = FALSE;

		getVideo().release();
		resetVideo();
		//Invalidate();
	}
}

// COpencvSdiVideoTemplateView 그리기


void COpencvSdiVideoTemplateView::OnDraw(CDC* pDC)
{
	COpencvSdiVideoTemplateDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (!m_imgWrapper.isEmpty()) {
		m_imgWrapper.draw(*pDC, CRect());
	}

	if (!m_tracker.m_rect.IsRectEmpty()) m_tracker.Draw(pDC);

	if (!m_bPlay || m_bPause) {
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(0, 0, L"Space Key : Play or Pause,  ESC : Stop");
	}
}


void COpencvSdiVideoTemplateView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case VK_SPACE :
		if (m_bPlay)
			// 일시 중지
			m_bPause = !m_bPause;
		else
			OnPlayFile();		
		break;

	case VK_ESCAPE:
		// 재생 중단하고 처음으로
		m_bStopCurrentJob = TRUE;
		break;
	}

	if( !m_bQuit)
		CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void COpencvSdiVideoTemplateView::OnUpdatePlayFile(CCmdUI* pCmdUI)
{
	// video 파일이 열려 있고 재생 중이 아닐 때만 Enable
	BOOL bEnable = getVideo().isOpened() && !m_bPlay;
	// 포즈 중이면 Enable
	bEnable |= m_bPause;

	pCmdUI->Enable( bEnable );
	pCmdUI->SetCheck(m_bPlay);
}


void COpencvSdiVideoTemplateView::OnUpdateFileOpen(CCmdUI* pCmdUI)
{

	// video 파일이 오픈 되어 있지 않을 때나 재생 중이 아닐 때 오픈 가능	// 하지만 MRU가 있어서 이걸로 막아도 소용이 없다.
	BOOL bEnable = !getVideo().isOpened() || !m_bPlay;
//	BOOL bEnable = !m_bPlay;

	pCmdUI->Enable(bEnable);
}


void COpencvSdiVideoTemplateView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	if (getVideo().isOpened()) {
		int srcW = getVideo().get(CAP_PROP_FRAME_WIDTH);
		int srcH = getVideo().get(CAP_PROP_FRAME_HEIGHT);
		//		SetWindowPos(this, 0, 0, srcW, srcH, SWP_NOMOVE);
		SetScrollSizes(MM_TEXT, CSize(srcW, srcH));

		ResizeParentToFit(FALSE);

		m_bQuit = m_bStopCurrentJob = m_bPlay = m_bPause = FALSE;

		//if (!getVideo().isOpened()) return false;

		//m_sPathName = sPath;
		//videoCap >> m_frame;
		m_imgWrapper.create(getCurrentFrame());

		CRect roi;

		// 얼굴 영역 찾고 있으면 얼굴 영역으로 설정 -> 안 하는게 낫다
		//string cccName = "haarcascade_frontalface_default.xml";
		//CascadeClassifier ccc(cccName);
		//if (ccc.empty()) {
		//	TRACE("%s 로드 실패!!\n", cccName.c_str());
		//}
		//else {
		//	vector<Rect> faces;
		//	ccc.detectMultiScale(getCurrentFrame(), faces);

		//	// 영역 크기로 sort
		//	sort(faces.begin(), faces.end(), [](Rect& r1, Rect& r2) {
		//		return r1.area() > r2.area();
		//	});

		//	if (!faces.empty()) {
		//		roi = CRect(faces[0].x, faces[0].y, faces[0].x + faces[0].width, faces[0].y + faces[0].height);
		//		m_tracker.m_rect = roi;
		//	}
		//}

		//if (roi.IsRectEmpty()) {
		//	int cx = srcW / 2;
		//	int cy = srcH / 2;
		//	// 없으면 화면 중앙에
		//	m_tracker.m_rect.SetRect(cx - 50, cy - 50, cx + 50, cy + 50);
		//}

	}
	else {
		SetScrollSizes(MM_TEXT, CSize(100, 100));
		m_tracker.m_rect.SetRectEmpty();
	}
}


void COpencvSdiVideoTemplateView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	m_tracker.GetTrueRect(rect);

	if (m_tracker.HitTest(point) < 0) {
		m_tracker.TrackRubberBand(this, point);
		// 이전 트래커 위치 다시 그림
		InvalidateRect(rect);

		//InvalidateRect(m_tracker.m_rect, FALSE);
		CClientDC dc(this);
		m_tracker.Draw(&dc);
	}
	else {
		m_tracker.Track(this, point);

		// 이전 트래커 위치 다시 그림
		InvalidateRect(rect);

		CClientDC dc(this);
		m_tracker.Draw(&dc);
		InvalidateRect(m_tracker.m_rect, FALSE);
	}

	if (getVideo().isOpened()) {
		//CRect rt(m_tracker.m_rect);
		//Rect2d roi(rt.left, rt.top, rt.Width(), rt.Height());
		m_roi = CRect2Rect2d(m_tracker.m_rect);
		m_cvTracker->init(getCurrentFrame(), m_roi);
		//delete m_cvTracker;
		//m_cvTracker = TrackerCSRT::create();
		//bool ret = 	m_cvTracker->init(getCurrentFrame(), m_roi);
		//if (!ret) {
		//	AfxMessageBox(L"tracker init failed!!!");
		//}
		//m_cvTracker->update(getCurrentFrame(), m_roi);
	}

	CScrollView::OnLButtonDown(nFlags, point);
}


// 트랙커 사용하면 호출되지 않는다!!!
void COpencvSdiVideoTemplateView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonUp(nFlags, point);
}

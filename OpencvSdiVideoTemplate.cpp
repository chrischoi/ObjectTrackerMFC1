
// OpencvSdiVideoTemplate.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "OpencvSdiVideoTemplate.h"
#include "MainFrm.h"

#include "OpencvSdiVideoTemplateDoc.h"
#include "OpencvSdiVideoTemplateView.h"



#include "opencv2/opencv.hpp"
#include <iostream>
#include "opencv2/tracking/tracker.hpp"

using namespace cv;
using namespace std;

string int2str(int a) {
	char buf[100];
	_itoa_s(a, buf, 10);
	return string(buf);
}


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpencvSdiVideoTemplateApp

BEGIN_MESSAGE_MAP(COpencvSdiVideoTemplateApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &COpencvSdiVideoTemplateApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// COpencvSdiVideoTemplateApp 생성

COpencvSdiVideoTemplateApp::COpencvSdiVideoTemplateApp() noexcept
{
	// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("OpencvSdiVideoTemplate.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 COpencvSdiVideoTemplateApp 개체입니다.

COpencvSdiVideoTemplateApp theApp;


// COpencvSdiVideoTemplateApp 초기화

BOOL COpencvSdiVideoTemplateApp::InitInstance()
{
	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	TRACE(L"***********************************\n");
	TRACE ("           hello OpenCV %s \n", CV_VERSION);
	TRACE(L"***********************************\n");

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));
	LoadStdProfileSettings(10);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	// 애플리케이션의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COpencvSdiVideoTemplateDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(COpencvSdiVideoTemplateView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
//	m_pMainWnd->DragAcceptFiles();

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

#if _DEBUG
namespace cv { extern void deleteInitializationMutex(); }
#endif

int COpencvSdiVideoTemplateApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

#if _DEBUG
	// Opencv를 release용으로 빌드하면 빌드가 완료가 안되어서 일단 debug 상태에서만 호출하게 해 놓음
	// 하지만 이거 쓴다고 메모리 릭이 줄진 않는다 --ㅋ
	cv::deleteInitializationMutex();
#endif

	return CWinApp::ExitInstance();
}

// COpencvSdiVideoTemplateApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void COpencvSdiVideoTemplateApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// COpencvSdiVideoTemplateApp 메시지 처리기





void COpencvSdiVideoTemplateApp::OnFileOpen()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, _T("동영상 파일(*.wmv;*.mpg;*.mpeg;*.mov;*.div;*.divx;*.mp4;*.avi)|*.wmv;*.mpg;*.mpeg;*.mov;*.div;*.divx;*.mp4;*.avi;|All Files(*.*)|*.*||"));
	if (fileDlg.DoModal() != IDOK) return;

	CString sPath = fileDlg.GetPathName();

	OpenDocumentFile(sPath);
}

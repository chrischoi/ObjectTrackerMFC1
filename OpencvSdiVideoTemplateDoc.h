
// OpencvSdiVideoTemplateDoc.h: COpencvSdiVideoTemplateDoc 클래스의 인터페이스
//


#pragma once

#include "opencv2/opencv.hpp"

using namespace cv;

class COpencvSdiVideoTemplateDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	COpencvSdiVideoTemplateDoc() noexcept;
	DECLARE_DYNCREATE(COpencvSdiVideoTemplateDoc)

// 특성입니다.
public:
	CString		m_sPathName;			// 오픈한 파일 이름
	cv::VideoCapture m_videoCap;
	cv::Mat		m_frame;


// 작업입니다.
public:
	CString			getPathName() { return m_sPathName;  }
	VideoCapture&	getVideo() { return m_videoCap; }
	bool openVideoFile(CString& sPath);
// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~COpencvSdiVideoTemplateDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};

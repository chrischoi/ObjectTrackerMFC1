#include <windows.h>
#include <afxwin.h>
#include "opencv2/opencv.hpp"

#pragma once

class CCvImgWrapper
{
private:
	BITMAPINFO* m_pBitmapInfo; // Bitmap 정보를 담고 있는 구조체.
	cv::Mat* m_pMat;

public:
	CCvImgWrapper() {
		m_pMat = 0;
		m_pBitmapInfo = 0;
	}

	~CCvImgWrapper() {
		destroy();
	}

	cv::Mat* getMat() { return m_pMat; }

	void replace(cv::Mat& src) { m_pMat = &src; }
	bool isEmpty() { return (m_pMat == NULL);  }
public:
	//	bool create(int w, int h, int bpp);
	bool create(cv::Mat& mat);

	void draw(const CDC& dc, const CRect& rect);
	void destroy() { 
		if (m_pBitmapInfo) m_pBitmapInfo;
		m_pBitmapInfo = 0;
	}
};


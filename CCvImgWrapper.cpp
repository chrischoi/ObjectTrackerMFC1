#include "pch.h"
#include "CCvImgWrapper.h"

bool CCvImgWrapper::create(cv::Mat& mat)
{
	if (m_pBitmapInfo != NULL)
	{
		delete[]m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
	m_pMat = &mat;
	int bpp;
	switch (mat.type())
	{
	case CV_8UC1: bpp = 8;
	case CV_8UC4: bpp = 32;
	default:
		bpp = 24;
	}
	int w = mat.cols;
	int h = mat.rows;

	if (bpp == 8)
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = bpp;
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapInfo->bmiHeader.biSizeImage = 0;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biClrUsed = 0;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	m_pBitmapInfo->bmiHeader.biWidth = w;
	m_pBitmapInfo->bmiHeader.biHeight = -h;

	return true;
}

void CCvImgWrapper::draw(const CDC& dc, const CRect& rt)
{
	if (m_pMat == 0) return;

	CRect rect(rt);
	if (rect.IsRectEmpty())
		rect.SetRect(0, 0, m_pMat->cols, m_pMat->rows);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, 0, 0, m_pMat->cols, m_pMat->rows, m_pMat->data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}


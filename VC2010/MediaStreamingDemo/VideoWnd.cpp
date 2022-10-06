// VideoWnd.cpp : implementation file
//

#include "stdafx.h"
#include "VideoWnd.h"


// VideoWnd

IMPLEMENT_DYNAMIC(VideoWnd, CWnd)

VideoWnd::VideoWnd()
{

}

VideoWnd::~VideoWnd()
{
}


BEGIN_MESSAGE_MAP(VideoWnd, CWnd)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// VideoWnd message handlers



BOOL VideoWnd::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(rc, RGB(0,0,0));
	return TRUE;
//	return CWnd::OnEraseBkgnd(pDC);
}

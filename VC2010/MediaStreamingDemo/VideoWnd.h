#pragma once


// VideoWnd

class VideoWnd : public CWnd
{
	DECLARE_DYNAMIC(VideoWnd)

public:
	VideoWnd();
	virtual ~VideoWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};



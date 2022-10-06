
// MediaStreamingDemoDlg.h : header file
//

#pragma once

#include "VideoWnd.h"
#include "MediaCapture.h"
#include "afxwin.h"

enum CaptureState {
	Stop,
	Start,
	Pause
};


// CMediaStreamingDemoDlg dialog
class CMediaStreamingDemoDlg : public CDialogEx
{
// Construction
public:
	CMediaStreamingDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MEDIASTREAMINGDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	MediaCapture mMediaCapture;
	VideoWnd mVideoWnd;
	int mWidth;
	int mHeight;
	CaptureState mState;

// Implementation
private:
	void updateResolutions(const WCHAR* device);
	bool hasSize(DWORD size);
	void updateCaptureUI();

protected:
	HICON m_hIcon;
	LRESULT OnRTMPDisconnect(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CComboBox uiVideoDevices;
	CComboBox uiVideoSizes;
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnCbnSelchangeVideoDeviceCombo();
	afx_msg void OnCbnSelchangeVideoSizeCombo();
	CString mFileName;
	int mVideoFPS;
	int mVideoBitrate;
	CComboBox uiAudioDevices;
	CComboBox uiSamplingFrequency;
	CComboBox uiAudioBitrate;
	CComboBox uiH264Profile;
	CComboBox uiVideoQuality;
	int mMinGop;
	int mRTMPOutputWidth;
	int mRTMPOutputHeight;
	CButton mChkCustomResolution;
	afx_msg void OnBnClickedCheckcustomres();
	CButton mChkLowLatency;
	CComboBox uiCRF;
	CButton mChkBGR;
	afx_msg void OnBnClickedChecklowlatency();
};


// MediaStreamingDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MediaStreamingDemo.h"
#include "MediaStreamingDemoDlg.h"
#include "afxdialogex.h"

#include "DirectShowHelper.h"

#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMediaStreamingDemoDlg dialog




CMediaStreamingDemoDlg::CMediaStreamingDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMediaStreamingDemoDlg::IDD, pParent)
	, mFileName(_T("rtmp://localhost:1935/live/myStream"))
	, mVideoFPS(25)
	, mMinGop(25)
	, mVideoBitrate(1000000)
	, mState(CaptureState::Stop)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	mRTMPOutputWidth = 640;
	mRTMPOutputHeight = 480;
}

void CMediaStreamingDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIDEO_DEVICE_COMBO, uiVideoDevices);
	DDX_Control(pDX, IDC_VIDEO_SIZE_COMBO, uiVideoSizes);
	DDX_Text(pDX, IDC_EDIT_FILE_NAME, mFileName);
	DDX_Text(pDX, IDC_EDIT_RTMPFPS, mVideoFPS);
	DDX_Text(pDX, IDC_EDIT_VIDEO_BITRATE, mVideoBitrate);
	DDX_Control(pDX, IDC_COMBO_AUDIO_DEVICE, uiAudioDevices);
	DDX_Control(pDX, IDC_COMBO_SAMPLING_FREQUENCY, uiSamplingFrequency);
	DDX_Control(pDX, IDC_COMBO_AUDIO_BITRATE, uiAudioBitrate);
	DDX_Control(pDX, IDC_COMBO_H264_PROFILE, uiH264Profile);
	DDX_Control(pDX, IDC_COMBO_QUALITY, uiVideoQuality);
	DDX_Text(pDX, IDC_EDITMINGOPLEN, mMinGop);
	DDX_Text(pDX, IDC_EDIT_OUTPUTWIDTH, mRTMPOutputWidth);
	DDX_Text(pDX, IDC_EDIT_OUTPUTHEIGHT, mRTMPOutputHeight);
	DDX_Control(pDX, IDC_CHECKCUSTOMRES, mChkCustomResolution);
	DDX_Control(pDX, IDC_CHECKLOWLATENCY, mChkLowLatency);
	DDX_Control(pDX, IDC_COMBOCRF, uiCRF);
	DDX_Control(pDX, IDC_CHECKBGR, mChkBGR);
}

BEGIN_MESSAGE_MAP(CMediaStreamingDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BUTTON, &CMediaStreamingDemoDlg::OnBnClickedStartButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CMediaStreamingDemoDlg::OnBnClickedStopButton)
	ON_CBN_SELCHANGE(IDC_VIDEO_DEVICE_COMBO, &CMediaStreamingDemoDlg::OnCbnSelchangeVideoDeviceCombo)
	ON_CBN_SELCHANGE(IDC_VIDEO_SIZE_COMBO, &CMediaStreamingDemoDlg::OnCbnSelchangeVideoSizeCombo)
	ON_BN_CLICKED(IDC_CHECKCUSTOMRES, &CMediaStreamingDemoDlg::OnBnClickedCheckcustomres)
	ON_BN_CLICKED(IDC_CHECKLOWLATENCY, &CMediaStreamingDemoDlg::OnBnClickedChecklowlatency)
	ON_MESSAGE(WM_RTMP_DISCONNECT, OnRTMPDisconnect)
END_MESSAGE_MAP()


// CMediaStreamingDemoDlg message handlers

BOOL CMediaStreamingDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CRect rc;
	GetDlgItem(IDC_VIDEO_PLACEHOLDER)->GetWindowRect(rc);
	ScreenToClient(rc);

	mVideoWnd.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rc, this, IDC_VIDEO_PLACEHOLDER);
	//mPreviewWnd = &mVideoWnd;

	DirectShowHelper dsHelper;
	std::vector<std::wstring> devices;
	dsHelper.enumerateDevices(CLSID_VideoInputDeviceCategory, devices);

	int i;

	for (i = 0; i <= 51; i++) {
		CString str1;
		str1.Format(L"%d",i);
		uiCRF.AddString(str1);
	}
	uiCRF.SetCurSel(26);

	for (i = 0; i < devices.size(); i++) {
		uiVideoDevices.AddString(devices[i].c_str());
	}

	if (uiVideoDevices.GetCount()) {
		uiVideoDevices.SetCurSel(0);
		CString curCam;
		uiVideoDevices.GetLBText(uiVideoDevices.GetCurSel(), curCam);
		updateResolutions(curCam.GetBuffer());
	}

	devices.clear();
	dsHelper.enumerateDevices(CLSID_AudioInputDeviceCategory, devices);

	for (i = 0; i < devices.size(); i++) {
		uiAudioDevices.AddString(devices[i].c_str());
	}

	if (uiAudioDevices.GetCount()) {
		uiAudioDevices.SetCurSel(0);
	}

	uiH264Profile.SetCurSel(1);
	uiSamplingFrequency.SetCurSel(0);
	uiAudioBitrate.SetCurSel(0);
	uiVideoQuality.SetCurSel(3);

	this->mChkLowLatency.SetCheck(1);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMediaStreamingDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMediaStreamingDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMediaStreamingDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CMediaStreamingDemoDlg::hasSize(DWORD size)
{
	for (int i = 0; i < uiVideoSizes.GetCount(); i++) {
		if (uiVideoSizes.GetItemData(i) == size)
			return true;
	}

	return false;
}

void CMediaStreamingDemoDlg::updateResolutions(const WCHAR* device)
{
	DirectShowHelper dsHelper;
	std::vector<AM_MEDIA_TYPE*> mediaTypes;

	uiVideoSizes.ResetContent();

	dsHelper.enumerateMediaTypes(device, CLSID_VideoInputDeviceCategory, mediaTypes);
	for (int i = 0; i < mediaTypes.size(); i++) {
		AM_MEDIA_TYPE* mt = mediaTypes[i];
		if (mt) {
			if (mt->majortype == MEDIATYPE_Video /*&& mt->subtype == MEDIASUBTYPE_RGB24*/ && mt->cbFormat == sizeof(VIDEOINFOHEADER)) {
				VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*) mt->pbFormat;
				if (vih) {
					std::wstringstream ss;
					ss << vih->bmiHeader.biWidth << "x" << vih->bmiHeader.biHeight;
					std::wstring size = ss.str();

					DWORD data = vih->bmiHeader.biWidth;
					data <<= 16;
					data |= vih->bmiHeader.biHeight;

					if (!hasSize(data)) {
						uiVideoSizes.AddString(size.c_str());
						uiVideoSizes.SetItemData(uiVideoSizes.GetCount() - 1, (DWORD_PTR)data);
					}
				}
			}
			dsHelper.deleteMediaType(mt);
		}
	}

	if (uiVideoSizes.GetCount()) {
		uiVideoSizes.SetCurSel(0);
		OnCbnSelchangeVideoSizeCombo();
	}
}

void CMediaStreamingDemoDlg::updateCaptureUI()
{
	CString text;
	switch (mState) {
	case CaptureState::Stop:
		text = L"Start";
		break;
	case CaptureState::Start:
		text = L"Pause";
		break;
	case CaptureState::Pause:
		text = L"Resume";
		break;
	}

	GetDlgItem(IDC_START_BUTTON)->SetWindowText(text);
}

void CMediaStreamingDemoDlg::OnBnClickedStartButton()
{
	if (mState == CaptureState::Stop) {
		UpdateData();

		// video
		H264Profile profile[] = {Base_Profile, Main_Profile, High_Profile};
		VideoBitrateMode mode[] = {VBR, CBR};

		CString curCam;
		uiVideoDevices.GetLBText(uiVideoDevices.GetCurSel(), curCam);
		mMediaCapture.setVideoDevice(curCam.GetBuffer());
		mMediaCapture.setWidth(mWidth);
		mMediaCapture.setHeight(mHeight);
		mMediaCapture.setFramerate(mVideoFPS);
		mMediaCapture.setVideoBitrate(mVideoBitrate);
		mMediaCapture.setDisplayWindow(mVideoWnd.m_hWnd);
		mMediaCapture.setVideoProfile(profile[uiH264Profile.GetCurSel()]);
		mMediaCapture.setVideoQuality(uiVideoQuality.GetCurSel() + 1);
		mMediaCapture.setFPS(this->mVideoFPS);
		mMediaCapture.setMinGOPlength(this->mMinGop);
		mMediaCapture.setNotificationWindow(this->GetSafeHwnd());

		if(this->mChkBGR.GetCheck()==1)
			mMediaCapture.setUseBGR(true);
		else
			mMediaCapture.setUseBGR(false);
	
		if(this->mChkLowLatency.GetCheck()==1)
			mMediaCapture.setUseLowlatencyMode(true);
		else
			mMediaCapture.setUseLowlatencyMode(false);

			CString strCRF;
		
		uiCRF.GetLBText(uiCRF.GetCurSel(), strCRF);
	
		mMediaCapture.setCRFvalue(_ttoi(strCRF));

		
		if(mChkCustomResolution.GetCheck()==1)
		{
			
			mMediaCapture.setUseCustomResolution(true);
			mMediaCapture.setCustomResolution(this->mRTMPOutputWidth,this->mRTMPOutputHeight);
		
		}
		else
			mMediaCapture.setUseCustomResolution(false);

	


		// audio
		SamplingFrequency frequency[] = {Audio_48000, Audio_44100};
		AudioBitrate aubitrate[] = {Bitrate_96Kbps, Bitrate_128Kbps, Bitrate_160Kbps, Bitrate_192Kbps};
		CString curMic;
		uiAudioDevices.GetLBText(uiAudioDevices.GetCurSel(), curMic);
		mMediaCapture.setAudioDevice(curMic.GetBuffer());
		mMediaCapture.setSamplingFrequency(frequency[uiSamplingFrequency.GetCurSel()]);
		mMediaCapture.setAudioBitrate(aubitrate[uiAudioBitrate.GetCurSel()]);

		mMediaCapture.setFileName(mFileName.GetBuffer());
		
		if (mMediaCapture.start())
			mState = CaptureState::Start;
		else
			AfxMessageBox(mMediaCapture.getLastError().c_str());

		updateCaptureUI();
	}
	else if (mState == CaptureState::Start) {

		
		mMediaCapture.pause();
		mState = CaptureState::Pause;
		updateCaptureUI();
	}
	else if (mState == CaptureState::Pause) {
		mMediaCapture.resume();
		mState = CaptureState::Start;
		updateCaptureUI();
	}
}


void CMediaStreamingDemoDlg::OnBnClickedStopButton()
{
	mMediaCapture.stop();
	mState = CaptureState::Stop;
	updateCaptureUI();
}


void CMediaStreamingDemoDlg::OnCbnSelchangeVideoDeviceCombo()
{
	CString curCam;
	uiVideoDevices.GetLBText(uiVideoDevices.GetCurSel(), curCam);
	updateResolutions(curCam.GetBuffer());
}


void CMediaStreamingDemoDlg::OnCbnSelchangeVideoSizeCombo()
{
	if (uiVideoSizes.GetCurSel() >= 0) {
		DWORD data = (DWORD)uiVideoSizes.GetItemData(uiVideoSizes.GetCurSel());
		mWidth = (data >> 16) & 0xFFFF;
		mHeight = data & 0xFFFF;
	}

	CRect rc;

	GetDlgItem(IDC_VIDEO_PLACEHOLDER)->GetWindowRect(rc);

	if (rc.Width() > mWidth && rc.Height() > mHeight) {
		CRect rc2(0, 0, mWidth, mHeight);
		rc2.OffsetRect(rc.left + (rc.Width() - rc2.Width()) / 2, rc.top + (rc.Height() - rc2.Height()) / 2);
		rc = rc2;
	}
/*	else {
		double ratio;
		int dx = mWidth - rc.Width();
		int dy = mHeight - rc.Height();
		if (dx > dy) {
			ratio = mWidth / (double)mHeight;
			rc.DeflateRect(0, (rc.Height() - (rc.Height() / ratio)) / 2);
		}
		else {
			ratio = mHeight / (double)mWidth;
			rc.DeflateRect((rc.Width() - (rc.Width() / ratio)) / 2, 0);
		}
	}
*/
	ScreenToClient(rc);
	mVideoWnd.MoveWindow(rc);
}


void CMediaStreamingDemoDlg::OnBnClickedCheckcustomres()
{
	// TODO: Add your control notification handler code here
	if(this->mChkCustomResolution.GetCheck()==1)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_OUTPUTWIDTH))->EnableWindow(true);
		((CEdit*)GetDlgItem(IDC_EDIT_OUTPUTHEIGHT))->EnableWindow(true);
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_OUTPUTWIDTH))->EnableWindow(false);
		((CEdit*)GetDlgItem(IDC_EDIT_OUTPUTHEIGHT))->EnableWindow(false);
	
	}


}


void CMediaStreamingDemoDlg::OnBnClickedChecklowlatency()
{
	// TODO: Add your control notification handler code here
	if(this->mChkLowLatency.GetCheck()==1)
		((CComboBox*)GetDlgItem(IDC_COMBOCRF))->EnableWindow(true);
	else
			((CComboBox*)GetDlgItem(IDC_COMBOCRF))->EnableWindow(false);
}


LRESULT CMediaStreamingDemoDlg::OnRTMPDisconnect(WPARAM wParam, LPARAM lParam)
{
	
	AfxMessageBox(_T("RTMP Connection disconnect"));

	return 0;
}
#pragma once

#include <dshow.h>
#include <string>
#include "..\RTMPLiveStreamFilter\RTMPConstant.h"

#define WM_RTMP_DISCONNECT (WM_APP + 1)

class MediaCapture
{
public:
	MediaCapture(void);
	~MediaCapture(void);

public:
	// devices
	void setVideoDevice(std::wstring deviceName);
	std::wstring getVideoDevice();
	void setAudioDevice(std::wstring deviceName);
	std::wstring getAudioDevice();

	// event notifications
	void setNotificationWindow(HWND wnd);

	// output file
	void setFileName(std::wstring fileName);
	std::wstring getFileName();

	// video display
	void setDisplayWindow(HWND wnd);
	
	void setUseLowlatencyMode(bool bvalue);
	void setUseBGR(bool bvalue);
	void setCRFvalue(int ivalue);
	void setFPS(int fps);
	void setMinGOPlength(int ikeyint_min);
	void setUseCustomResolution(bool bValue);
	void setCustomResolution(int iWidth, int iHeight);



	// capture parameters
	void setWidth(int width);
	int getWidth();
	void setHeight(int height);
	int getHeight();
	void setFramerate(int rate);
	int getFramerate();
	void setSamplingFrequency(SamplingFrequency frequency);
	SamplingFrequency getSamplingFrequency();
	
	// codec parameters
	void setVideoBitrate(int bitrate);
	int getVideoBitrate();
	void setVideoBitrateMode(VideoBitrateMode mode);
	VideoBitrateMode getVideoBitrateMode();
	void setVideoProfile(H264Profile profile);
	H264Profile getVideoProfile();
	void setVideoQuality(int q);
	int getVideoQuality();
	void setAudioBitrate(AudioBitrate bitrate);
	AudioBitrate getAudioBitrate();

	// operations
	bool start();
	void pause();
	void resume();
	void stop();

	const std::wstring getLastError();

private:
	void cleanup();
	void setClippingWindow();
	VideoFormat getVideoFormat(GUID subtype);

private:
	HANDLE _exiting;
	HANDLE _thread;

	std::wstring mVideoDevice;
	std::wstring mAudioDevice;
	std::wstring mFileName;
	HWND mDisplayWindow;
	HWND mnotificationWindow;

	int mFps;
	bool mUseCustomResolution;
	bool mUseLowlatencyMode;
	bool mUseBGR;
	int mCRFValue;
	int mCustomWidth;
	int mCustomHeight;

	int mkeyint_min;
	int mWidth;
	int mHeight;
	int mFramerate;
	SamplingFrequency mSamplingFrequency;
	int mVideoBitrate;
	VideoBitrateMode mVideoBitrateMode;
	AudioBitrate mAudioBitrate;
	H264Profile mH264Profile;
	int mVideoQuality;
	std::wstring mLastError;
	VideoFormat mSelectedVideoFormat;

	// dshow stuff
	DWORD mGraphReg;
	CComPtr<IGraphBuilder>		mGraphBuilder;
	CComPtr<IMediaControl>		mMediaCtrl;
	CComPtr<IMediaEvent>		mMediaEvent;
	CComPtr<IBaseFilter>		mVideoSource;
	CComPtr<IBaseFilter>		mAudioSource;
	CComPtr<IBaseFilter>		mVideoRenderer;
	CComPtr<IBaseFilter>		mAudioRenderer;
	CComPtr<IBaseFilter>		mMediaStreamingFilter;
	CComPtr<IBaseFilter>		mTeeFilter;

private:
	static DWORD notificationThreadProc(LPVOID lpParam);
	void setLastError(WCHAR* text);
	DWORD notificationThread();
};


#include "StdAfx.h"
#include "MediaCapture.h"

//#include <streams.h>

#include "DirectShowHelper.h"
#include <vector>
#include <sstream>

#include <InitGuid.h>
#include "..\RTMPLiveStreamFilter\RTMPStreamingFilter.h"

#ifndef __D3DRM_H__
#define __D3DRM_H__
#endif

#include <d3d9.h>
#include <vmr9.h>
#include <wmsdkidl.h>




//#include <qedit.h>

using namespace std;

const LONGLONG MILLISECONDS = (1000);            // 10 ^ 3
const LONGLONG NANOSECONDS = (1000000000);       // 10 ^ 9
const LONGLONG UNITS = (NANOSECONDS / 100);      // 10 ^ 7

MediaCapture::MediaCapture(void)
	: mFramerate(30)
	, mSamplingFrequency(SamplingFrequency::Audio_48000)
	, mVideoBitrate(1000000)
	, mVideoBitrateMode(VideoBitrateMode::VBR)
	, mAudioBitrate(AudioBitrate::Bitrate_96Kbps)
	, mH264Profile(H264Profile::Main_Profile)
	, mSelectedVideoFormat(VideoFormat::NONE)
	, mVideoQuality(4)
	, mFps(25)
	, mkeyint_min(25)

{
	_exiting = CreateEvent(NULL, TRUE, FALSE, NULL);

}

MediaCapture::~MediaCapture(void)
{
	stop();
		CloseHandle(_exiting);
}

bool MediaCapture::start()
{
	DirectShowHelper dsHelper;
	HRESULT hr;

	do {
		CComPtr<IPin> inPin;
		CComPtr<IPin> outPin;
		CComPtr<IVMRFilterConfig9> filterConfig;
		CComPtr<IVMRWindowlessControl9> windowlessCtrl;
		CComPtr<IRTMPStreamingConfig> streamingFilter;

		CRect rc;

		// create GraphBuilder
		hr = dsHelper.createGraphBuilder(&mGraphBuilder);
		if (FAILED(hr)) {
			setLastError(L"ERROR. GraphBuilder");
			break;
		}

		// add graph builder to ROT
#ifdef _DEBUG
		hr = dsHelper.AddToRot(mGraphBuilder, &mGraphReg);
		if (FAILED(hr)) {
			setLastError(L"ERROR. Adding ROT");
			break;
		}
#endif
		// media control
		hr = mGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&mMediaCtrl);
		if (FAILED(hr)) {
			setLastError(L"ERROR. MediaControl");
			break;
		}

		// media event
		hr = mGraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&mMediaEvent);
		if (FAILED(hr)) {
			setLastError(L"ERROR. MediaEvent");
			break;
		}

		if (!mVideoDevice.empty()) {
			// VideoSource: create video source filter
			hr = dsHelper.createFilter(mVideoDevice.c_str(), CLSID_VideoInputDeviceCategory, &mVideoSource);
			if (FAILED(hr)) {
				setLastError(L"ERROR. Webcam");
				break;
			}

			// VideoSource: set media format
			std::vector<AM_MEDIA_TYPE*> mediaTypes;
			dsHelper.enumerateMediaTypes(mVideoDevice.c_str(), CLSID_VideoInputDeviceCategory, mediaTypes);
			for (int i = 0; i < mediaTypes.size(); i++) {
				AM_MEDIA_TYPE* mt = mediaTypes[i];
				if (mt) {
					VideoFormat vf = getVideoFormat(mt->subtype);
					if (mt->majortype == MEDIATYPE_Video 
						&& vf != VideoFormat::NONE
						&& mt->cbFormat == sizeof(VIDEOINFOHEADER)) {
						VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*) mt->pbFormat;
						if (vih && vih->bmiHeader.biWidth == mWidth && vih->bmiHeader.biHeight == mHeight) {
							if (mFramerate > 0)
								vih->AvgTimePerFrame = UNITS / mFramerate;
							
							if (vf < mSelectedVideoFormat) {
								dsHelper.setFilterFormat(mt, mVideoSource);
								mSelectedVideoFormat = vf;
							}
						}
					}
				}
				dsHelper.deleteMediaType(mt);
			}

			if (mSelectedVideoFormat == VideoFormat::NONE) {
				setLastError(L"ERROR. Video format");
				break;
			}

			// VideoSource: add filter to graph
			hr = mGraphBuilder->AddFilter(mVideoSource, L"Video Source");
			if (FAILED(hr)) {
				setLastError(L"ERROR. Add video capture source");
				break;
			}

			// VideoRenderer: create renderer filter
			hr = dsHelper.createFilter(CLSID_VideoMixingRenderer9, &mVideoRenderer);
			if (FAILED(hr)) {
				setLastError(L"ERROR. VideoRenderer@1");
				break;
			}

			// VideoRenderer: add renderer to graph
			hr = mGraphBuilder->AddFilter(mVideoRenderer, L"Video Renderer");
			if (FAILED(hr)) {
				setLastError(L"ERROR. VideoRenderer@2");
				break;
			}

			// VideoRenderer: get filter config interface
			hr = mVideoRenderer->QueryInterface(IID_IVMRFilterConfig9, (void**)&filterConfig);
			if (FAILED(hr)) {
				setLastError(L"ERROR. VideoRenderer@3");
				break;
			}

			// VideoRenderer: set rendering mode
			hr = filterConfig->SetRenderingMode(VMRMode_Windowless);
			if (FAILED(hr)) {
				setLastError(L"ERROR. VideoRenderer@4");
				break;
			}

			filterConfig.Release();
			setClippingWindow();
		}

		if (!mAudioDevice.empty()) {
			// AudioSource: create audio source filter
			hr = dsHelper.createFilter(mAudioDevice.c_str(), CLSID_AudioInputDeviceCategory, &mAudioSource);
			if (FAILED(hr)) {
				setLastError(L"ERROR. Microphone");
				break;
			}

			// AudioSource: set media format
			std::vector<AM_MEDIA_TYPE*> mediaTypes;
			dsHelper.enumerateMediaTypes(mAudioDevice.c_str(), CLSID_AudioInputDeviceCategory, mediaTypes);
			for (int i = 0; i < mediaTypes.size(); i++) {
				AM_MEDIA_TYPE* mt = mediaTypes[i];
				if (mt) {
					if (mt->majortype == MEDIATYPE_Audio && 
						(mt->subtype == MEDIASUBTYPE_PCM) && 
						mt->cbFormat == sizeof(WAVEFORMATEX)) {
						WAVEFORMATEX* wfx = (WAVEFORMATEX*) mt->pbFormat;
						if (wfx && 
							wfx->wFormatTag == WAVE_FORMAT_PCM && 
							wfx->nSamplesPerSec == getSamplingFrequency() &&
							wfx->nChannels == 2 &&
							wfx->wBitsPerSample == 16) {
							dsHelper.setFilterFormat(mt, mAudioSource);
						}
					}
				}
				dsHelper.deleteMediaType(mt);
			}

			// AudioSource: add filter to graph
			hr = mGraphBuilder->AddFilter(mAudioSource, L"Audio Source");
			if (FAILED(hr)) {
				setLastError(L"ERROR. Add audio capture source");
				break;
			}
		}

		// MediaStreaming: create media streaming filter

		hr = dsHelper.createFilter(CLSID_ViscomsoftRTMPStreamingFilter, &mMediaStreamingFilter);
		if (FAILED(hr)) {
			setLastError(L"ERROR. MediaStreaming@2");
			break;
		}

		// MediaStreaming: set properties
		hr = mMediaStreamingFilter->QueryInterface(IID_IRTMPStreamingConfig, (void**)&streamingFilter);
		if (FAILED(hr)) {
			setLastError(L"ERROR. MediaStreaming@3");
			break;
		}

		streamingFilter->setRTMPUrl(getFileName().c_str());

		streamingFilter->setLowlatencyMode(mUseLowlatencyMode);
		streamingFilter->setBGR(mUseBGR);
		streamingFilter->setCRF(mCRFValue);

		streamingFilter->setVideoBitrate(getVideoBitrate());
		streamingFilter->setAudioSamplesPerSecond(getSamplingFrequency());
		streamingFilter->setAudioBitrate(getAudioBitrate());
		streamingFilter->setH264Profile(getVideoProfile());
		streamingFilter->setVideoQuality(getVideoQuality());
		streamingFilter->setFPS(mFps);
		streamingFilter->setMinGOPlength(mkeyint_min);
		streamingFilter->setUseCustomResolution(mUseCustomResolution);
		streamingFilter->setCustomResolution(mCustomWidth,mCustomHeight);
		

		// MediaStreaming: add media streaming filter to graph
		hr = mGraphBuilder->AddFilter(mMediaStreamingFilter, L"Media Streaming");
		if (FAILED(hr)) {
			setLastError(L"ERROR. MediaStreaming@4");
			break;
		}

		// Tee: create tee filter
		hr = dsHelper.createFilter(CLSID_SmartTee, &mTeeFilter);
		if (FAILED(hr)) {
			setLastError(L"ERROR. TeeFilter");
			break;
		}

		// Tee: add tee filter to graph
		hr = mGraphBuilder->AddFilter(mTeeFilter, L"Tee");
		if (FAILED(hr)) {
			setLastError(L"ERROR. TeeFilter@2");
			break;
		}

		// Connection: connect video source -> tee
		hr = dsHelper.connectFilters(mGraphBuilder, mVideoSource, mTeeFilter);
		if (FAILED(hr)) {
			setLastError(L"ERROR. Connect video source -> tee");
			break;
		}

		// Connection: connect tee -> mediastreaming
		hr = dsHelper.connectFilters(mGraphBuilder, mTeeFilter, mMediaStreamingFilter);
		if (FAILED(hr)) {
			setLastError(L"ERROR. Connect tee -> mediastreaming");
			break;
		}

		// Connection: connect tee -> video renderer
		hr = dsHelper.connectFilters(mGraphBuilder, mTeeFilter, mVideoRenderer);
		if (FAILED(hr)) {
			setLastError(L"ERROR. Connect tee -> video renderer");
			break;
		}

		// Connection: connect audio source -> mediastreaming
		hr = dsHelper.connectFilters(mGraphBuilder, mAudioSource, mMediaStreamingFilter);
		if (FAILED(hr)) {
			setLastError(L"ERROR. Connect audio source -> mediastreaming");
			break;
		}


		// run graph
		hr = mMediaCtrl->Run();
		if (FAILED(hr)) {
			setLastError(L"ERROR. Start streaming filter");
			break;
		}

		ResetEvent(_exiting);
		DWORD id;
		_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)notificationThreadProc, this, 0, &id);


		return true;
	} while (0);

	cleanup();
	return false;
}

DWORD MediaCapture::notificationThreadProc(LPVOID lpParam)
{
	MediaCapture *obj = (MediaCapture*)lpParam;
	return obj->notificationThread();
}

DWORD MediaCapture::notificationThread()
{
	HANDLE  graphEvent;
	mMediaEvent->GetEventHandle((OAEVENT*)&graphEvent);

	HANDLE events[] = {_exiting, graphEvent};
	
	long evCode;
	LONG_PTR param1;
	LONG_PTR  param2;


	while (1) {
		DWORD waitResult = WaitForMultipleObjects(ARRAYSIZE(events), events, FALSE, INFINITE);

		if (waitResult == WAIT_OBJECT_0)
			break;

		if (waitResult == WAIT_OBJECT_0 + 1) {
			while (SUCCEEDED(mMediaEvent->GetEvent(&evCode, &param1, &param2, 0))) {
				mMediaEvent->FreeEventParams(evCode, param1, param2);

				if (evCode == EC_RTMP_DISCONNECT) {
					
					if (mnotificationWindow != NULL)
						PostMessage(mnotificationWindow, WM_RTMP_DISCONNECT, 0, 0);
				}
			}
		}
	}

	return 0;
}

void MediaCapture::pause()
{
	CComPtr<IRTMPStreamingConfig> streamingFilter;
	if (mMediaStreamingFilter) {
		HRESULT hr = mMediaStreamingFilter->QueryInterface(IID_IRTMPStreamingConfig, (void**)&streamingFilter);
		if (SUCCEEDED(hr)) {
			streamingFilter->pause();
		}
	}
}

void MediaCapture::resume()
{
	CComPtr<IRTMPStreamingConfig> streamingFilter;
	if (mMediaStreamingFilter) {
		HRESULT hr = mMediaStreamingFilter->QueryInterface(IID_IRTMPStreamingConfig, (void**)&streamingFilter);
		if (SUCCEEDED(hr)) {
			streamingFilter->resume();
		}
	}
}

void MediaCapture::stop()
{

	if (mMediaCtrl.p)
		mMediaCtrl->Stop();
	cleanup();
}

void MediaCapture::cleanup()
{
	if (mGraphReg != 0) {
		DirectShowHelper ds;
		ds.RemoveFromRot(mGraphReg);
		mGraphReg = 0;
	}

	mGraphBuilder.Release();
	mMediaCtrl.Release();
	mMediaEvent.Release();
	mVideoSource.Release();
	mAudioSource.Release();
	mVideoRenderer.Release();
	mMediaStreamingFilter.Release();
	mTeeFilter.Release();
}

void MediaCapture::setFileName(wstring fileName)
{
	mFileName = fileName;
}

wstring MediaCapture::getFileName()
{
	return mFileName;
}

void MediaCapture::setDisplayWindow(HWND wnd)
{
	mDisplayWindow = wnd;
}

void MediaCapture::setVideoDevice(wstring deviceName)
{
	mVideoDevice = deviceName;
}

wstring MediaCapture::getVideoDevice()
{
	return mVideoDevice;
}

void MediaCapture::setFPS(int fps)
{
	mFps = fps;
}

void MediaCapture::setUseCustomResolution(bool bValue)
{
	mUseCustomResolution = bValue;

}

void MediaCapture::setCustomResolution(int iWidth, int iHeight)
{
	mCustomWidth = iWidth;
	mCustomHeight = iHeight;
}



void MediaCapture::setMinGOPlength(int ikeyint_min)
{
	mkeyint_min= ikeyint_min;
}

void MediaCapture::setWidth(int width)
{
	mWidth = width;
}

int MediaCapture::getWidth()
{
	return mWidth;
}

void MediaCapture::setHeight(int height)
{
	mHeight = height;
}

int MediaCapture::getHeight()
{
	return mHeight;
}

void MediaCapture::setFramerate(int rate)
{
	mFramerate = rate;
}
void MediaCapture::setUseLowlatencyMode(bool bvalue)
{
	mUseLowlatencyMode = bvalue;
}
void MediaCapture::setCRFvalue(int ivalue)
{	
	mCRFValue = ivalue;
}
void MediaCapture::setUseBGR(bool bvalue)
{
	mUseBGR = bvalue;
}



int MediaCapture::getFramerate()
{
	return mFramerate;
}

void MediaCapture::setVideoBitrate(int bitrate)
{
	mVideoBitrate = bitrate;
}

int MediaCapture::getVideoBitrate()
{
	return mVideoBitrate;
}

void MediaCapture::setVideoBitrateMode(VideoBitrateMode mode)
{
	mVideoBitrateMode = mode;
}

VideoBitrateMode MediaCapture::getVideoBitrateMode()
{
	return mVideoBitrateMode;
}

void MediaCapture::setVideoProfile(H264Profile profile)
{
	mH264Profile = profile;
}

H264Profile MediaCapture::getVideoProfile()
{
	return mH264Profile;
}

void MediaCapture::setVideoQuality(int q)
{
	mVideoQuality = q;
}

int MediaCapture::getVideoQuality()
{
	return mVideoQuality;
}

void MediaCapture::setAudioDevice(wstring deviceName)
{
	mAudioDevice = deviceName;
}

wstring MediaCapture::getAudioDevice()
{
	return mAudioDevice;
}

void MediaCapture::setSamplingFrequency(SamplingFrequency frequency)
{
	mSamplingFrequency = frequency;
}

SamplingFrequency MediaCapture::getSamplingFrequency()
{
	return mSamplingFrequency;
}

void MediaCapture::setAudioBitrate(AudioBitrate bitrate)
{
	mAudioBitrate = bitrate;
}

AudioBitrate MediaCapture::getAudioBitrate()
{
	return mAudioBitrate;
}

void MediaCapture::setLastError(WCHAR* text)
{
	mLastError = text;
}

void MediaCapture::setNotificationWindow(HWND wnd)
{
	mnotificationWindow = wnd;
}

const wstring MediaCapture::getLastError()
{
	return mLastError;
}

void MediaCapture::setClippingWindow()
{
	CComPtr<IVMRWindowlessControl9> windowlessCtrl;
	RECT rc;
	HRESULT hr;

	if (mVideoRenderer) {
		hr = mVideoRenderer->QueryInterface(IID_IVMRWindowlessControl9, (void**)&windowlessCtrl);
		if (FAILED(hr)) {
			setLastError(L"ERROR. VideoRenderer@5");
			return;
		}
	}

	if (windowlessCtrl) {
		windowlessCtrl->SetVideoClippingWindow(mDisplayWindow);
		GetClientRect(mDisplayWindow, &rc);
		windowlessCtrl->SetVideoPosition(0, &rc);

		windowlessCtrl->SetAspectRatioMode(VMR9ARMode_LetterBox);

		windowlessCtrl.Release();
	}
}

VideoFormat MediaCapture::getVideoFormat(GUID subtype)
{
	if (subtype == MEDIASUBTYPE_YUY2)
		return VideoFormat::YUY2;
	if (subtype == MEDIASUBTYPE_YV12)
		return VideoFormat::YV12;
	if (subtype == MEDIASUBTYPE_IYUV)
		return VideoFormat::IYUV;
	if (subtype == MEDIASUBTYPE_NV12)
		return VideoFormat::NV12;
	if (subtype == WMMEDIASUBTYPE_I420)
		return VideoFormat::I420;
	if (subtype == MEDIASUBTYPE_RGB24)
		return VideoFormat::RGB24;

	return VideoFormat::NONE;
}
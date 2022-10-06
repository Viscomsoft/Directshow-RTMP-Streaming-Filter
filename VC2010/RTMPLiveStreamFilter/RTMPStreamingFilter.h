#pragma once

#include "RTMPConstant.h"

// {64673138-3884-4D97-B524-E2748A861C00}
DEFINE_GUID(IID_IRTMPStreamingConfig, 
0x64673138, 0x3884, 0x4d97, 0xb5, 0x24, 0xe2, 0x74, 0x8a, 0x86, 0x1c, 0x0);

DECLARE_INTERFACE_(IRTMPStreamingConfig, IUnknown)
{
	STDMETHOD(setLowlatencyMode) (bool bvalue);

	STDMETHOD(setCRF)(int ivalue);

	STDMETHOD(setBGR) (bool bvalue);
	// File name
	STDMETHOD(setRTMPUrl) (const wchar_t* url);

	STDMETHOD(setLicenseKey) (int iKey);

	// Video bitrate
	STDMETHOD(setVideoBitrate) (int bitrate);

	// Video profile
	STDMETHOD(setH264Profile) (H264Profile profile);

	// Video quality
	STDMETHOD(setVideoQuality) (int quality);

	// Video aspect ratio
	STDMETHOD(setPixelAspectRatio) (int numerator, int denominator);

	// Audio sampling frequency
	STDMETHOD(setAudioSamplesPerSecond) (SamplingFrequency frequency);

	// Audio bitrate
	STDMETHOD(setAudioBitrate) (AudioBitrate bitrate);

	
	STDMETHOD(setFPS) (int iFPS);

	STDMETHOD(setUseCustomResolution) (bool bValue);

	STDMETHOD(setCustomResolution) (int iWidth, int iHeight);

	STDMETHOD(setMinGOPlength) (int ikeyint_min);

	// Start
	//STDMETHOD(start) ();

	// Pause
	STDMETHOD(pause) ();

	// Resume
	STDMETHOD(resume) ();

	// Stop
	//STDMETHOD(stop) ();

	// Version
	STDMETHOD(getVersion) (unsigned short* version);
};

// {2ADDBC12-F933-41F5-BB1E-DF62ED7BB8B9}
//DEFINE_GUID(CLSID_MediaStreamingFilter, 
//0x2addbc12, 0xf933, 0x41f5, 0xbb, 0x1e, 0xdf, 0x62, 0xed, 0x7b, 0xb8, 0xb9);

DEFINE_GUID(CLSID_ViscomsoftRTMPStreamingFilter, 
	0x2addbc12, 0xf933, 0x41f5, 0xbb, 0x1e, 0xdf, 0x62, 0xed, 0x7b, 0xb8, 0xb9);

#define EC_RTMP_DISCONNECT (EC_USER + 100)
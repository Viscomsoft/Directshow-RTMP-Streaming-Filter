#pragma once

enum VideoFormat {
	I420,
	IYUV,
	YV12,
	NV12,
	YUY2,
	RGB24,

	NONE
};

enum H264Profile {
	Base_Profile,
	Main_Profile,
	High_Profile
};

enum SamplingFrequency {
	Audio_48000 = 48000,
	Audio_44100 = 44100
};

enum AudioBitrate {
	Bitrate_96Kbps  =  96000,
	Bitrate_128Kbps = 128000,
	Bitrate_160Kbps = 160000,
	Bitrate_192Kbps = 192000
};

enum VideoBitrateMode {
	VBR,
	CBR
};

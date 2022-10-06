
using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using DirectShowLib;
using System.Drawing;

namespace WindowsFormsApplication1
{
	/// <summary> Summary description for MainForm. </summary>
	internal class Capture: IDisposable
	{
        #region Member variables

        /// <summary> graph builder interface. </summary>
		private IFilterGraph2 m_FilterGraph = null;
        IMediaControl m_mediaCtrl = null;

        /// <summary> Set by async routine when it captures an image </summary>
        private bool m_bRunning = false;
        public string m_strURL;
//#if DEBUG
        DsROTEntry m_rot = null;
        //#endif

        #endregion

        [Guid("64673138-3884-4D97-B524-E2748A861C00"),
        InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]

      
        interface IRTMPStreamingConfig
        {

            void setLowlatencyMode([In] bool bvalue);
            void setCRF([In] int ivalue);
            void setBGR([In] bool bvalue);
            void setRTMPUrl([In] string url);
            void setLicenseKey([In] int iKey);
            void setVideoBitrate([In] int bitrate);
            void setH264Profile([In] int profile);
            void setVideoQuality([In] int quality);
            void setPixelAspectRatio([In] int numerator, [In] int denominator);
            void setAudioSamplesPerSecond([In] int frequency);
            void setAudioBitrate([In] int bitrate);
            void setFPS([In] int iFPS);
            void setUseCustomResolution([In] bool bValue);
            void setCustomResolution([In] int iWidth, int iHeight);
            void setMinGOPlength([In] int ikeyint_min);
            void pause();
            void resume();
         
         
        }

        /// <summary> release everything. </summary>
        public void Dispose()
        {
            GC.SuppressFinalize(this);
            CloseInterfaces();
        }

        ~Capture()
        {
            Dispose();
        }

        /// <summary>
        /// Create capture object
        /// </summary>
        /// <param name="iDeviceNum">Zero based index of capture device</param>
        public Capture(int iDeviceNum, int iAudioDeviceNum, string strURL, IntPtr windowHandle, Rectangle videoWndRect, int iWidth, int iHeight, int iH264Profile, int iQuality, int iVideoBitrate, int iAudioSampleRate, int iAudioBitrate, int iFPS, int iMinGop, bool bUseCustomResolution, int iCustomWidth, int iCustomHeight, bool bUseBGR, bool bUseLowlatencyMode, int iCRF)
        {
            DsDevice [] capDevices;
            DsDevice[] capAudioDevices;

            m_strURL = strURL;

            // Get the collection of video devices
            capDevices = DsDevice.GetDevicesOfCat( FilterCategory.VideoInputDevice );

            if (iDeviceNum + 1 > capDevices.Length)
            {
                throw new Exception("No video capture devices found at that index!");
            }

            capAudioDevices = DsDevice.GetDevicesOfCat(FilterCategory.AudioInputDevice);

            if (iAudioDeviceNum + 1 > capAudioDevices.Length)
            {
                throw new Exception("No audio capture devices found at that index!");
            }

            try
            {
                // Set up the capture graph
                SetupGraph(capDevices[iDeviceNum], capAudioDevices[iAudioDeviceNum], windowHandle, videoWndRect, iWidth, iHeight, iH264Profile, iQuality, iVideoBitrate, iAudioSampleRate, iAudioBitrate, iFPS, iMinGop, bUseCustomResolution,iCustomWidth,iCustomHeight,bUseBGR,bUseLowlatencyMode, iCRF);

                m_bRunning = false;
            }
            catch
            {
                Dispose();
                throw;
            }
        }


        // Start the capture graph
        public void Start()
        {
            if (!m_bRunning)
            {
                int hr = m_mediaCtrl.Run();
                //Marshal.ThrowExceptionForHR( hr );

                m_bRunning = true;
            }
        }

        // Pause the capture graph.
        // Running the graph takes up a lot of resources.  Pause it when it
        // isn't needed.
        public void Pause()
        {
            if (m_bRunning)
            {
                IMediaControl mediaCtrl = m_FilterGraph as IMediaControl;

                int hr = mediaCtrl.Pause();
          
                m_bRunning = false;
            }
        }

        /// <summary> build the capture graph. </summary>
        /// 
        private bool SetVideoCaptureParameters(ICaptureGraphBuilder2 capGraph, IBaseFilter captureFilter, Guid mediaSubType, int iWidth, int iHeight)
        {
            /* The stream config interface */
            object streamConfig;

            /* Get the stream's configuration interface */
            int hr = capGraph.FindInterface(PinCategory.Capture,
                                            MediaType.Video,
                                            captureFilter,
                                            typeof(IAMStreamConfig).GUID,
                                            out streamConfig);

            DsError.ThrowExceptionForHR(hr);

            var videoStreamConfig = streamConfig as IAMStreamConfig;

            /* If QueryInterface fails... */
            if (videoStreamConfig == null)
            {
                throw new Exception("Failed to get IAMStreamConfig");
            }

            ///* Make the VIDEOINFOHEADER 'readable' */
            var videoInfo = new VideoInfoHeader();

            int iCount = 0, iSize = 0;
            videoStreamConfig.GetNumberOfCapabilities(out iCount, out iSize);

            IntPtr TaskMemPointer = Marshal.AllocCoTaskMem(iSize);


            AMMediaType pmtConfig = null;
            for (int iFormat = 0; iFormat < iCount; iFormat++)
            {
                IntPtr ptr = IntPtr.Zero;

                videoStreamConfig.GetStreamCaps(iFormat, out pmtConfig, TaskMemPointer);

                videoInfo = (VideoInfoHeader)Marshal.PtrToStructure(pmtConfig.formatPtr, typeof(VideoInfoHeader));

                if (videoInfo.BmiHeader.Width == iWidth && videoInfo.BmiHeader.Height == iHeight)
                {

                    ///* Setup the VIDEOINFOHEADER with the parameters we want */
                    //videoInfo.AvgTimePerFrame = (1000000000 /100) / 25;

                    if (mediaSubType != Guid.Empty)
                    {
                        int fourCC = 0;
                        byte[] b = mediaSubType.ToByteArray();
                        fourCC = b[0];
                        fourCC |= b[1] << 8;
                        fourCC |= b[2] << 16;
                        fourCC |= b[3] << 24;

                        videoInfo.BmiHeader.Compression = fourCC;
                 
                    }

                    /* Copy the data back to unmanaged memory */
                    Marshal.StructureToPtr(videoInfo, pmtConfig.formatPtr, true);

                    hr = videoStreamConfig.SetFormat(pmtConfig);
                    break;
                }

            }

            /* Free memory */
            Marshal.FreeCoTaskMem(TaskMemPointer);
            DsUtils.FreeAMMediaType(pmtConfig);

            if (hr < 0)
                return false;

            return true;
        }

        private void SetupGraph(DsDevice dev, DsDevice audiodev, IntPtr windowHandle, Rectangle videoWndRect, int iWidth, int iHeight, int iH264Profile, int iQuality, int iVideoBitrate, int iAudioSampleRate, int iAudioBitrate, int iFPS, int iMinGop, bool bUseCustomResolution, int iCustomWidth, int iCustomHeight, bool bUseBGR, bool bUseLowlatencyMode, int iCRF)
		{
            int hr;

            IBaseFilter capFilter = null;
            IBaseFilter audiocapFilter = null;
             ICaptureGraphBuilder2 capGraph = null;
            IBaseFilter m_RTMPFilter = null;
            IBaseFilter vmr9Filter = null;
            IBaseFilter mTeeFilter = null;

            // Get the graphbuilder object
            m_FilterGraph = (IFilterGraph2)new FilterGraph();

//#if DEBUG
            m_rot = new DsROTEntry( m_FilterGraph );
//#endif

            try
            {
                // Get the ICaptureGraphBuilder2
                capGraph = (ICaptureGraphBuilder2) new CaptureGraphBuilder2();

                // Start building the graph
                hr = capGraph.SetFiltergraph( m_FilterGraph );
                Marshal.ThrowExceptionForHR( hr );

                // Add the capture device to the graph
                hr = m_FilterGraph.AddSourceFilterForMoniker(dev.Mon, null, dev.Name, out capFilter);
                Marshal.ThrowExceptionForHR( hr );

                hr = m_FilterGraph.AddSourceFilterForMoniker(audiodev.Mon, null, audiodev.Name, out audiocapFilter);
                Marshal.ThrowExceptionForHR(hr);


                SetVideoCaptureParameters(capGraph, capFilter, Guid.Empty, iWidth, iHeight);

                Guid guid = new Guid("2ADDBC12-F933-41F5-BB1E-DF62ED7BB8B9");
                Type comtype = Type.GetTypeFromCLSID(guid);
                m_RTMPFilter = (IBaseFilter)Activator.CreateInstance(comtype);
                m_FilterGraph.AddFilter(m_RTMPFilter, "RTMP Streaming");




                IRTMPStreamingConfig mystreamingFilterConfig = (IRTMPStreamingConfig)m_RTMPFilter;
                

                 mystreamingFilterConfig.setRTMPUrl(m_strURL);
                 mystreamingFilterConfig.setFPS(iFPS);
                 mystreamingFilterConfig.setMinGOPlength(iMinGop);

                 mystreamingFilterConfig.setBGR(bUseBGR);
                 mystreamingFilterConfig.setLowlatencyMode(bUseLowlatencyMode);
                 mystreamingFilterConfig.setCRF(iCRF);
                mystreamingFilterConfig.setVideoBitrate(iVideoBitrate);

                // must enter 48000 or 44100,otherwise cannot connect to server
               mystreamingFilterConfig.setAudioSamplesPerSecond(44100); //48000

               mystreamingFilterConfig.setAudioBitrate(3);//96000
               mystreamingFilterConfig.setH264Profile(iH264Profile); //Base_Profile
               mystreamingFilterConfig.setVideoQuality(iQuality);  //0-9
               mystreamingFilterConfig.setUseCustomResolution(bUseCustomResolution);
               mystreamingFilterConfig.setCustomResolution(iCustomWidth, iCustomHeight);

                vmr9Filter = (IBaseFilter)new VideoMixingRenderer9();
                m_FilterGraph.AddFilter(vmr9Filter, "Video Renderer");

                IVMRFilterConfig9 FilterConfig = (IVMRFilterConfig9)vmr9Filter;
                FilterConfig.SetRenderingMode(VMR9Mode.Windowless);



                IVMRWindowlessControl9 windowlessCtrl = (IVMRWindowlessControl9)vmr9Filter;
                windowlessCtrl.SetVideoClippingWindow(windowHandle);
                windowlessCtrl.SetVideoPosition(null, DsRect.FromRectangle(videoWndRect));
                windowlessCtrl.SetAspectRatioMode(VMR9AspectRatioMode.LetterBox);


               mTeeFilter = (IBaseFilter)new SmartTee();
                m_FilterGraph.AddFilter(mTeeFilter, "Tee");


                    hr = capGraph.RenderStream(PinCategory.Preview, MediaType.Video, capFilter, null, mTeeFilter);
                hr = capGraph.RenderStream(null,MediaType.Video, mTeeFilter, null, vmr9Filter);


                hr = capGraph.RenderStream(PinCategory.Capture, MediaType.Video, capFilter, null, m_RTMPFilter);



               hr = capGraph.RenderStream(PinCategory.Capture, MediaType.Audio, audiocapFilter, null, m_RTMPFilter);

             Marshal.ThrowExceptionForHR( hr );

                
                m_mediaCtrl = m_FilterGraph as IMediaControl;
            }
            finally
            {
                if (capFilter != null)
                {
                    Marshal.ReleaseComObject(capFilter);
                    capFilter = null;
                }

                if (audiocapFilter != null)
                {
                    Marshal.ReleaseComObject(audiocapFilter);
                    audiocapFilter = null;

                }

                if (m_RTMPFilter != null)
                {
                    Marshal.ReleaseComObject(m_RTMPFilter);
                    m_RTMPFilter = null;

                }
             
                if (capGraph != null)
                {
                    Marshal.ReleaseComObject(capGraph);
                    capGraph = null;
                }
            }
        }

       

        /// <summary> Shut down capture </summary>
		private void CloseInterfaces()
		{
            int hr;

            try
			{
				if( m_mediaCtrl != null )
				{
                    // Stop the graph
                    hr = m_mediaCtrl.Stop();
                    m_bRunning = false;
				}
			}
			catch
			{
			}

//#if DEBUG
            // Remove graph from the ROT
            if ( m_rot != null )
            {
                m_rot.Dispose();
                m_rot = null;
            }
//#endif

            if (m_FilterGraph != null)
            {
                Marshal.ReleaseComObject(m_FilterGraph);
                m_FilterGraph = null;
            }
        }
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using DirectShowLib;
using System.Runtime.InteropServices;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            textBox1.Text = "rtmp://localhost:1935/live/myStream";

            //enumerate Video Input filters and add them to comboBox1
            foreach (DsDevice ds in DsDevice.GetDevicesOfCat(FilterCategory.VideoInputDevice))
            {
                cboVideoDevice.Items.Add(ds.Name);

            }

            if (cboVideoDevice.Items.Count > 0)
                cboVideoDevice.SelectedIndex = 0;

            foreach (DsDevice ds in DsDevice.GetDevicesOfCat(FilterCategory.AudioInputDevice))
            {
                cboAudioDevice.Items.Add(ds.Name);
            }

            if (cboAudioDevice.Items.Count > 0)
                cboAudioDevice.SelectedIndex = 0;


            cboprofile.Items.Add("Base");
            cboprofile.Items.Add("Main");
            cboprofile.Items.Add("High");
            cboprofile.SelectedIndex = 1;


            cboquality.Items.Add("Best");
            cboquality.Items.Add("2");
            cboquality.Items.Add("3");
            cboquality.Items.Add("4");
            cboquality.Items.Add("5");
            cboquality.Items.Add("6");
            cboquality.Items.Add("7");
            cboquality.Items.Add("8");
            cboquality.Items.Add("9");
            cboquality.Items.Add("Poor");
            cboquality.SelectedIndex = 0;


            cboaudiosample.Items.Add("48000");
            cboaudiosample.Items.Add("44100");
            cboaudiosample.SelectedIndex = 0;

            cboaudiobitrate.Items.Add("96000");
            cboaudiobitrate.Items.Add("128000");
            cboaudiobitrate.Items.Add("1600000");
            cboaudiobitrate.Items.Add("1920000");
            cboaudiobitrate.SelectedIndex = 0;

            for (int i = 0; i <= 51; i++)
                cboCRF.Items.Add(i.ToString());
            cboCRF.SelectedIndex = 26;
        }
        Capture cam = null;

        private void GetAllAvailableResolution(DsDevice vidDev)
        {
            try
            {
                int hr, bitCount = 0;

                IBaseFilter sourceFilter = null;

                var m_FilterGraph2 = new FilterGraph() as IFilterGraph2;
                hr = m_FilterGraph2.AddSourceFilterForMoniker(vidDev.Mon, null, vidDev.Name, out sourceFilter);
                var pRaw2 = DsFindPin.ByCategory(sourceFilter, PinCategory.Capture, 0);
                //var AvailableResolutions = new List<string>();

                VideoInfoHeader v = new VideoInfoHeader();
                IEnumMediaTypes mediaTypeEnum;
                hr = pRaw2.EnumMediaTypes(out mediaTypeEnum);

                AMMediaType[] mediaTypes = new AMMediaType[1];
                IntPtr fetched = IntPtr.Zero;
                hr = mediaTypeEnum.Next(1, mediaTypes, fetched);

                while (fetched != null && mediaTypes[0] != null)
                {
                    Marshal.PtrToStructure(mediaTypes[0].formatPtr, v);
                    if (v.BmiHeader.Size != 0 && v.BmiHeader.BitCount != 0)
                    {
                        if (v.BmiHeader.BitCount > bitCount)
                        {
                            //AvailableResolutions.Clear();
                            cboVideoFormat.Items.Clear();
                            bitCount = v.BmiHeader.BitCount;
                        }

                        int iResult = cboVideoFormat.FindString(v.BmiHeader.Width + "x" + v.BmiHeader.Height);
                        if (iResult == -1)
                            cboVideoFormat.Items.Add(v.BmiHeader.Width + "x" + v.BmiHeader.Height);
                    }
                    hr = mediaTypeEnum.Next(1, mediaTypes, fetched);
                }
                //return AvailableResolutions;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                //   return new List<string>();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int iwidth = cboVideoFormat.Text.IndexOf("x");

            if (iwidth == -1)
            {
                MessageBox.Show("Video Format Error");
                return;
            }
            string strWidth = cboVideoFormat.Text.Substring(0, iwidth);

            string strHeight = cboVideoFormat.Text.Substring(iwidth + 1);

            Cursor.Current = Cursors.WaitCursor;


            if (cam == null)
            {
              

                cam = new Capture(cboVideoDevice.SelectedIndex, cboAudioDevice.SelectedIndex, textBox1.Text, pictureBox1.Handle, pictureBox1.ClientRectangle, Int32.Parse(strWidth), Int32.Parse(strHeight), cboprofile.SelectedIndex, cboquality.SelectedIndex, Int32.Parse(txtvideobitrate.Text), cboaudiosample.SelectedIndex, cboaudiobitrate.SelectedIndex, Int32.Parse(txtfps.Text), Int32.Parse(txtmingop.Text), chkusecustomRTMP.Checked, Int32.Parse(txtcustomwidth.Text), Int32.Parse(txtcustomheight.Text), chkuseBGR.Checked, chkuseLowLatency.Checked, Int32.Parse(cboCRF.Text));
                cam.Start();
                button1.Text = "Stop";

            }
            else
            {
                button1.Text = "Start";


                // Pause the recording
                cam.Pause();

                // Close it down
                cam.Dispose();
                cam = null;
            }

            Cursor.Current = Cursors.Default;
        }

        private void cboVideoDevice_SelectedIndexChanged(object sender, EventArgs e)
        {
            cboVideoFormat.Items.Clear();
            int videodeviceindex = cboVideoDevice.SelectedIndex;

            if (videodeviceindex != -1)
            {
                int i = 0;

                foreach (DsDevice ds in DsDevice.GetDevicesOfCat(FilterCategory.VideoInputDevice))
                {

                    if (i == videodeviceindex)
                    {
                        GetAllAvailableResolution(ds);
                        if (cboVideoFormat.Items.Count > 0)
                            cboVideoFormat.SelectedIndex = 0;
                    }
                    i++;
                }


            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (cam != null)
            {
                cam.Pause();

                // Close it down
                cam.Dispose();
                cam = null;
            }
        }

        private void chkusecustomRTMP_CheckedChanged(object sender, EventArgs e)
        {
            if (chkusecustomRTMP.Checked)
            {
                txtcustomwidth.Enabled = true;
                txtcustomheight.Enabled = true;

            }
            else
            {
                txtcustomwidth.Enabled = false;
                txtcustomheight.Enabled = false;

            }
        }

        private void chkuseLowLatency_CheckedChanged(object sender, EventArgs e)
        {
            if (chkuseLowLatency.Checked)
            {
                cboCRF.Enabled = true;
            }
            else
                cboCRF.Enabled = false;
         
        }
    }
}

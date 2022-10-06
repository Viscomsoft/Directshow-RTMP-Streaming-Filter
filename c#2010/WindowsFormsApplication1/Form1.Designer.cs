namespace WindowsFormsApplication1
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.cboquality = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.cboaudiobitrate = new System.Windows.Forms.ComboBox();
            this.label8 = new System.Windows.Forms.Label();
            this.cboaudiosample = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.cboprofile = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.txtvideobitrate = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.cboVideoFormat = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.cboAudioDevice = new System.Windows.Forms.ComboBox();
            this.cboVideoDevice = new System.Windows.Forms.ComboBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.txtfps = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.txtmingop = new System.Windows.Forms.TextBox();
            this.chkusecustomRTMP = new System.Windows.Forms.CheckBox();
            this.label12 = new System.Windows.Forms.Label();
            this.txtcustomwidth = new System.Windows.Forms.TextBox();
            this.txtcustomheight = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.chkuseBGR = new System.Windows.Forms.CheckBox();
            this.chkuseLowLatency = new System.Windows.Forms.CheckBox();
            this.cboCRF = new System.Windows.Forms.ComboBox();
            this.label14 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // cboquality
            // 
            this.cboquality.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboquality.FormattingEnabled = true;
            this.cboquality.Location = new System.Drawing.Point(144, 244);
            this.cboquality.Name = "cboquality";
            this.cboquality.Size = new System.Drawing.Size(326, 26);
            this.cboquality.TabIndex = 39;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(9, 241);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(71, 18);
            this.label9.TabIndex = 38;
            this.label9.Text = "Quality";
            // 
            // cboaudiobitrate
            // 
            this.cboaudiobitrate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboaudiobitrate.FormattingEnabled = true;
            this.cboaudiobitrate.Location = new System.Drawing.Point(144, 375);
            this.cboaudiobitrate.Name = "cboaudiobitrate";
            this.cboaudiobitrate.Size = new System.Drawing.Size(326, 26);
            this.cboaudiobitrate.TabIndex = 37;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(14, 375);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(125, 18);
            this.label8.TabIndex = 36;
            this.label8.Text = "Audio bitrate";
            // 
            // cboaudiosample
            // 
            this.cboaudiosample.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboaudiosample.FormattingEnabled = true;
            this.cboaudiosample.Location = new System.Drawing.Point(144, 327);
            this.cboaudiosample.Name = "cboaudiosample";
            this.cboaudiosample.Size = new System.Drawing.Size(326, 26);
            this.cboaudiosample.TabIndex = 35;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(12, 327);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(116, 18);
            this.label7.TabIndex = 34;
            this.label7.Text = "Audio Sample";
            // 
            // cboprofile
            // 
            this.cboprofile.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboprofile.FormattingEnabled = true;
            this.cboprofile.Location = new System.Drawing.Point(144, 202);
            this.cboprofile.Name = "cboprofile";
            this.cboprofile.Size = new System.Drawing.Size(326, 26);
            this.cboprofile.TabIndex = 33;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(9, 201);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(71, 18);
            this.label6.TabIndex = 32;
            this.label6.Text = "Profile";
            // 
            // txtvideobitrate
            // 
            this.txtvideobitrate.Location = new System.Drawing.Point(144, 164);
            this.txtvideobitrate.Name = "txtvideobitrate";
            this.txtvideobitrate.Size = new System.Drawing.Size(322, 28);
            this.txtvideobitrate.TabIndex = 31;
            this.txtvideobitrate.Text = "1000000";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(9, 165);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(125, 18);
            this.label5.TabIndex = 30;
            this.label5.Text = "Video Bitrate";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 280);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(116, 18);
            this.label4.TabIndex = 29;
            this.label4.Text = "Audio Device";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(9, 122);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(116, 18);
            this.label3.TabIndex = 28;
            this.label3.Text = "Video Fomrat";
            // 
            // cboVideoFormat
            // 
            this.cboVideoFormat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboVideoFormat.FormattingEnabled = true;
            this.cboVideoFormat.Location = new System.Drawing.Point(144, 116);
            this.cboVideoFormat.Name = "cboVideoFormat";
            this.cboVideoFormat.Size = new System.Drawing.Size(326, 26);
            this.cboVideoFormat.TabIndex = 27;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(116, 18);
            this.label2.TabIndex = 26;
            this.label2.Text = "Video Device";
            // 
            // cboAudioDevice
            // 
            this.cboAudioDevice.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboAudioDevice.FormattingEnabled = true;
            this.cboAudioDevice.Location = new System.Drawing.Point(144, 283);
            this.cboAudioDevice.Name = "cboAudioDevice";
            this.cboAudioDevice.Size = new System.Drawing.Size(326, 26);
            this.cboAudioDevice.TabIndex = 25;
            // 
            // cboVideoDevice
            // 
            this.cboVideoDevice.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboVideoDevice.FormattingEnabled = true;
            this.cboVideoDevice.Location = new System.Drawing.Point(144, 56);
            this.cboVideoDevice.Name = "cboVideoDevice";
            this.cboVideoDevice.Size = new System.Drawing.Size(326, 26);
            this.cboVideoDevice.TabIndex = 24;
            this.cboVideoDevice.SelectedIndexChanged += new System.EventHandler(this.cboVideoDevice_SelectedIndexChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(490, 56);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(525, 399);
            this.pictureBox1.TabIndex = 23;
            this.pictureBox1.TabStop = false;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(144, 14);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(704, 28);
            this.textBox1.TabIndex = 22;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(80, 18);
            this.label1.TabIndex = 21;
            this.label1.Text = "RTMP URL";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(704, 482);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(219, 80);
            this.button1.TabIndex = 20;
            this.button1.Text = "Start";
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(16, 424);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(35, 18);
            this.label10.TabIndex = 40;
            this.label10.Text = "FPS";
            // 
            // txtfps
            // 
            this.txtfps.Location = new System.Drawing.Point(144, 424);
            this.txtfps.Name = "txtfps";
            this.txtfps.Size = new System.Drawing.Size(322, 28);
            this.txtfps.TabIndex = 41;
            this.txtfps.Text = "25";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(16, 465);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(134, 18);
            this.label11.TabIndex = 42;
            this.label11.Text = "Min GOP length";
            // 
            // txtmingop
            // 
            this.txtmingop.Location = new System.Drawing.Point(144, 465);
            this.txtmingop.Name = "txtmingop";
            this.txtmingop.Size = new System.Drawing.Size(322, 28);
            this.txtmingop.TabIndex = 43;
            this.txtmingop.Text = "25";
            // 
            // chkusecustomRTMP
            // 
            this.chkusecustomRTMP.AutoSize = true;
            this.chkusecustomRTMP.Location = new System.Drawing.Point(12, 515);
            this.chkusecustomRTMP.Margin = new System.Windows.Forms.Padding(2);
            this.chkusecustomRTMP.Name = "chkusecustomRTMP";
            this.chkusecustomRTMP.Size = new System.Drawing.Size(268, 22);
            this.chkusecustomRTMP.TabIndex = 44;
            this.chkusecustomRTMP.Text = "Use Custom RTMP Resolution";
            this.chkusecustomRTMP.UseVisualStyleBackColor = true;
            this.chkusecustomRTMP.CheckedChanged += new System.EventHandler(this.chkusecustomRTMP_CheckedChanged);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(310, 515);
            this.label12.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(53, 18);
            this.label12.TabIndex = 45;
            this.label12.Text = "Width";
            // 
            // txtcustomwidth
            // 
            this.txtcustomwidth.Location = new System.Drawing.Point(380, 510);
            this.txtcustomwidth.Margin = new System.Windows.Forms.Padding(2);
            this.txtcustomwidth.Name = "txtcustomwidth";
            this.txtcustomwidth.Size = new System.Drawing.Size(76, 28);
            this.txtcustomwidth.TabIndex = 46;
            this.txtcustomwidth.Text = "640";
            // 
            // txtcustomheight
            // 
            this.txtcustomheight.Location = new System.Drawing.Point(549, 510);
            this.txtcustomheight.Margin = new System.Windows.Forms.Padding(2);
            this.txtcustomheight.Name = "txtcustomheight";
            this.txtcustomheight.Size = new System.Drawing.Size(76, 28);
            this.txtcustomheight.TabIndex = 48;
            this.txtcustomheight.Text = "480";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(479, 515);
            this.label13.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(62, 18);
            this.label13.TabIndex = 47;
            this.label13.Text = "Height";
            // 
            // chkuseBGR
            // 
            this.chkuseBGR.AutoSize = true;
            this.chkuseBGR.Location = new System.Drawing.Point(10, 561);
            this.chkuseBGR.Name = "chkuseBGR";
            this.chkuseBGR.Size = new System.Drawing.Size(520, 22);
            this.chkuseBGR.TabIndex = 49;
            this.chkuseBGR.Text = "Use BGR set to true or false if you see discoloration ";
            this.chkuseBGR.UseVisualStyleBackColor = true;
            // 
            // chkuseLowLatency
            // 
            this.chkuseLowLatency.AutoSize = true;
            this.chkuseLowLatency.Checked = true;
            this.chkuseLowLatency.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkuseLowLatency.Location = new System.Drawing.Point(10, 601);
            this.chkuseLowLatency.Name = "chkuseLowLatency";
            this.chkuseLowLatency.Size = new System.Drawing.Size(169, 22);
            this.chkuseLowLatency.TabIndex = 50;
            this.chkuseLowLatency.Text = "Use Low latency";
            this.chkuseLowLatency.UseVisualStyleBackColor = true;
            this.chkuseLowLatency.CheckedChanged += new System.EventHandler(this.chkuseLowLatency_CheckedChanged);
            // 
            // cboCRF
            // 
            this.cboCRF.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboCRF.FormattingEnabled = true;
            this.cboCRF.Location = new System.Drawing.Point(296, 602);
            this.cboCRF.Name = "cboCRF";
            this.cboCRF.Size = new System.Drawing.Size(121, 26);
            this.cboCRF.TabIndex = 51;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(245, 605);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(35, 18);
            this.label14.TabIndex = 52;
            this.label14.Text = "CRF";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1050, 728);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.cboCRF);
            this.Controls.Add(this.chkuseLowLatency);
            this.Controls.Add(this.chkuseBGR);
            this.Controls.Add(this.txtcustomheight);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.txtcustomwidth);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.chkusecustomRTMP);
            this.Controls.Add(this.txtmingop);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.txtfps);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.cboquality);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.cboaudiobitrate);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.cboaudiosample);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.cboprofile);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.txtvideobitrate);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cboVideoFormat);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cboAudioDevice);
            this.Controls.Add(this.cboVideoDevice);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "RTMP Streaming to Facebook Live, Youtube Live, Wowza Media Server ";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cboquality;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox cboaudiobitrate;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ComboBox cboaudiosample;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox cboprofile;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox txtvideobitrate;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cboVideoFormat;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cboAudioDevice;
        private System.Windows.Forms.ComboBox cboVideoDevice;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox txtfps;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox txtmingop;
        private System.Windows.Forms.CheckBox chkusecustomRTMP;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox txtcustomwidth;
        private System.Windows.Forms.TextBox txtcustomheight;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.CheckBox chkuseBGR;
        private System.Windows.Forms.CheckBox chkuseLowLatency;
        private System.Windows.Forms.ComboBox cboCRF;
        private System.Windows.Forms.Label label14;
    }
}


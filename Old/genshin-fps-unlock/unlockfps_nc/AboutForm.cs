﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace unlockfps_nc
{
    public partial class AboutForm : Form
    {
        public AboutForm ( )
        {
            InitializeComponent ( );
        }

        private void LinkLabelSource_LinkClicked ( object sender, LinkLabelLinkClickedEventArgs e )
        {
            OpenLink ( "https://github.com/elbadcode/OpenGenshin" );
        }

        private void LinkLabelIssues_LinkClicked ( object sender, LinkLabelLinkClickedEventArgs e )
        {
            OpenLink ( "https://github.com/34736384/genshin-fps-unlock" );
        }

        private void OpenLink ( string url )
        {
            var psi = new ProcessStartInfo
            {
                FileName = url,
                UseShellExecute = true
            };

            Process.Start ( psi );
        }

        private void LabelTitle_Click ( object sender, EventArgs e )
        {

        }

        private void label1_Click ( object sender, EventArgs e )
        {

        }

        private void label2_Click ( object sender, EventArgs e )
        {

        }
    }
}
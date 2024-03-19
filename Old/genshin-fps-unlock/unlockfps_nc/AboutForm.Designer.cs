namespace unlockfps_nc
{
    partial class AboutForm
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
        private void InitializeComponent ( )
        {
            LabelTitle = new Label ( );
            LabelDescription = new Label ( );
            LinkLabelSource = new LinkLabel ( );
            label1 = new Label ( );
            linkLabel1 = new LinkLabel ( );
            label2 = new Label ( );
            SuspendLayout ( );
            // 
            // LabelTitle
            // 
            LabelTitle.Font = new Font ( "Segoe UI", 9F, FontStyle.Bold, GraphicsUnit.Point,    0 );
            LabelTitle.Location = new Point ( 12, 9 );
            LabelTitle.Name = "LabelTitle";
            LabelTitle.Size = new Size ( 320, 19 );
            LabelTitle.TabIndex = 0;
            LabelTitle.Text = "Open Genshin 0.9";
            LabelTitle.TextAlign = ContentAlignment.TopCenter;
            LabelTitle.Click +=  LabelTitle_Click ;
            // 
            // LabelDescription
            // 
            LabelDescription.Location = new Point ( 12, 28 );
            LabelDescription.Name = "LabelDescription";
            LabelDescription.Size = new Size ( 320, 20 );
            LabelDescription.TabIndex = 1;
            LabelDescription.Text = "This program is free and open source";
            LabelDescription.TextAlign = ContentAlignment.MiddleCenter;
            // 
            // LinkLabelSource
            // 
            LinkLabelSource.LinkArea = new LinkArea ( 8, 46 );
            LinkLabelSource.Location = new Point ( 12, 48 );
            LinkLabelSource.Name = "LinkLabelSource";
            LinkLabelSource.Size = new Size ( 320, 20 );
            LinkLabelSource.TabIndex = 2;
            LinkLabelSource.TabStop = true;
            LinkLabelSource.Text = "Source: https://github.com/elbadcode/OpenGenshin";
            LinkLabelSource.TextAlign = ContentAlignment.MiddleCenter;
            LinkLabelSource.UseCompatibleTextRendering = true;
            LinkLabelSource.UseMnemonic = false;
            LinkLabelSource.LinkClicked +=  LinkLabelSource_LinkClicked ;
            // 
            // label1
            // 
            label1.Font = new Font ( "Segoe UI", 9F );
            label1.Location = new Point ( 12, 115 );
            label1.Name = "label1";
            label1.Size = new Size ( 320, 19 );
            label1.TabIndex = 4;
            label1.Text = "Forked from Genshin FPS Unlock 3.0.3";
            label1.TextAlign = ContentAlignment.TopCenter;
            label1.Click +=  label1_Click ;
            // 
            // linkLabel1
            // 
            linkLabel1.LinkArea = new LinkArea ( 10, 56 );
            linkLabel1.Location = new Point ( 12, 134 );
            linkLabel1.Name = "linkLabel1";
            linkLabel1.Size = new Size ( 331, 28 );
            linkLabel1.TabIndex = 2;
            linkLabel1.TabStop = true;
            linkLabel1.Text = "Original: https://github.com/34736384/genshin-fps-unlock";
            linkLabel1.TextAlign = ContentAlignment.MiddleCenter;
            linkLabel1.UseCompatibleTextRendering = true;
            linkLabel1.LinkClicked +=  LinkLabelSource_LinkClicked ;
            // 
            // label2
            // 
            label2.Location = new Point ( 23, 39 );
            label2.Name = "label2";
            label2.Size = new Size ( 320, 95 );
            label2.TabIndex = 5;
            label2.Text = "Full credit info for included submodules and additional usage instructions on GitHub";
            label2.TextAlign = ContentAlignment.MiddleCenter;
            label2.Click +=  label2_Click ;
            // 
            // AboutForm
            // 
            AutoScaleDimensions = new SizeF ( 7F, 15F );
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size ( 344, 171 );
            Controls.Add ( label1 );
            Controls.Add ( linkLabel1 );
            Controls.Add ( LinkLabelSource );
            Controls.Add ( LabelDescription );
            Controls.Add ( LabelTitle );
            Controls.Add ( label2 );
            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MinimizeBox = false;
            Name = "AboutForm";
            StartPosition = FormStartPosition.CenterParent;
            Text = "About";
            ResumeLayout ( false );
        }

        #endregion

        private Label LabelTitle;
        private Label LabelDescription;
        private LinkLabel LinkLabelSource;
        private Label label1;
        private LinkLabel linkLabel1;
        private Label label2;
    }
}
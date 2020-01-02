namespace viiidem_customlauncher
{
    partial class Form1
    {
        /// <summary>
        /// Wymagana zmienna projektanta.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Wyczyść wszystkie używane zasoby.
        /// </summary>
        /// <param name="disposing">prawda, jeżeli zarządzane zasoby powinny zostać zlikwidowane; Fałsz w przeciwnym wypadku.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Kod generowany przez Projektanta formularzy systemu Windows

        /// <summary>
        /// Metoda wymagana do obsługi projektanta — nie należy modyfikować
        /// jej zawartości w edytorze kodu.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.button5 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.IsSplitterFixed = true;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.splitContainer1.Panel1.Controls.Add(this.richTextBox1);
            this.splitContainer1.Panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.splitContainer1_Panel1_Paint);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.splitContainer1.Panel2.Controls.Add(this.label3);
            this.splitContainer1.Panel2.Controls.Add(this.button5);
            this.splitContainer1.Panel2.Controls.Add(this.button4);
            this.splitContainer1.Panel2.Controls.Add(this.label1);
            this.splitContainer1.Panel2.Controls.Add(this.label2);
            this.splitContainer1.Panel2.Controls.Add(this.button3);
            this.splitContainer1.Panel2.Controls.Add(this.button2);
            this.splitContainer1.Panel2.Controls.Add(this.button1);
            this.splitContainer1.Size = new System.Drawing.Size(932, 610);
            this.splitContainer1.SplitterDistance = 460;
            this.splitContainer1.TabIndex = 0;
            // 
            // richTextBox1
            // 
            this.richTextBox1.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.richTextBox1.ForeColor = System.Drawing.SystemColors.Window;
            this.richTextBox1.Location = new System.Drawing.Point(12, 247);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(385, 202);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = "";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.ForeColor = System.Drawing.Color.DarkRed;
            this.label3.Location = new System.Drawing.Point(12, 39);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(308, 104);
            this.label3.TabIndex = 5;
            this.label3.Text = resources.GetString("label3.Text");
            // 
            // button5
            // 
            this.button5.BackgroundImage = global::viiidem_customlauncher.Properties.Resources.blue_off;
            this.button5.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.button5.Font = new System.Drawing.Font("Nina", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.button5.ForeColor = System.Drawing.SystemColors.Control;
            this.button5.Location = new System.Drawing.Point(639, 83);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(142, 51);
            this.button5.TabIndex = 4;
            this.button5.Text = "DEMASTER OPTIONS";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.MouseEnter += new System.EventHandler(this.buttonCallback_blueOn);
            this.button5.MouseLeave += new System.EventHandler(this.buttonCallback_blueOff);
            // 
            // button4
            // 
            this.button4.BackgroundImage = global::viiidem_customlauncher.Properties.Resources.blue_off;
            this.button4.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.button4.Font = new System.Drawing.Font("Nina", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.button4.ForeColor = System.Drawing.SystemColors.Control;
            this.button4.Location = new System.Drawing.Point(639, 30);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(142, 51);
            this.button4.TabIndex = 3;
            this.button4.Text = "FORCE GAME FILES RE-UNPACK";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            this.button4.MouseEnter += new System.EventHandler(this.buttonCallback_blueOn);
            this.button4.MouseLeave += new System.EventHandler(this.buttonCallback_blueOff);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.ForeColor = System.Drawing.SystemColors.ControlDark;
            this.label1.Location = new System.Drawing.Point(12, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(109, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Demaster version: %d";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.Transparent;
            this.label2.ForeColor = System.Drawing.SystemColors.ButtonShadow;
            this.label2.Location = new System.Drawing.Point(12, 13);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(109, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Launcher version: %d";
            // 
            // button3
            // 
            this.button3.BackgroundImage = global::viiidem_customlauncher.Properties.Resources.silver_off;
            this.button3.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.button3.Font = new System.Drawing.Font("Nina", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.button3.ForeColor = System.Drawing.SystemColors.Control;
            this.button3.Location = new System.Drawing.Point(787, 83);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(142, 51);
            this.button3.TabIndex = 2;
            this.button3.Text = "EXIT";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.exitButton);
            this.button3.MouseEnter += new System.EventHandler(this.buttonCallback_silver);
            this.button3.MouseLeave += new System.EventHandler(this.buttonCallback_silverOff);
            // 
            // button2
            // 
            this.button2.BackgroundImage = global::viiidem_customlauncher.Properties.Resources.silver_off;
            this.button2.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.button2.Font = new System.Drawing.Font("Nina", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.button2.ForeColor = System.Drawing.SystemColors.Control;
            this.button2.Location = new System.Drawing.Point(491, 83);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(142, 51);
            this.button2.TabIndex = 1;
            this.button2.Text = "FFVIII OPTIONS";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.MouseEnter += new System.EventHandler(this.buttonCallback_silver);
            this.button2.MouseLeave += new System.EventHandler(this.buttonCallback_silverOff);
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.DarkOrange;
            this.button1.BackgroundImage = global::viiidem_customlauncher.Properties.Resources.orange_off;
            this.button1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.button1.Font = new System.Drawing.Font("Nina", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.button1.ForeColor = System.Drawing.SystemColors.Control;
            this.button1.Location = new System.Drawing.Point(328, 30);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(157, 104);
            this.button1.TabIndex = 0;
            this.button1.Text = "PLAY";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.MouseEnter += new System.EventHandler(this.buttonCallback_SetOrange);
            this.button1.MouseLeave += new System.EventHandler(this.buttonCallback_setOrangeOff);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(932, 610);
            this.Controls.Add(this.splitContainer1);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "FINAL FANTASY VIII Remastered LAUNCHER - Demaster unofficial patch by Maki";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Label label3;
    }
}


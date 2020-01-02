using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace viiidem_customlauncher
{
    public partial class zzzWatcher : Form
    {
        string outDirectory;
        public zzzWatcher(string outDirectory)
        {
            this.outDirectory = outDirectory;
            InitializeComponent();
            

        }


        Thread thread;
        private void zzzWatcher_Shown(object sender, EventArgs e)
        {
            thread = new Thread(new ThreadStart(StartThr));
            thread.Start();
            //this.Close();
        }

        public void StartThr()
        {
            zzz.UnpackFile("main.zzz", outDirectory, this);
            zzz.UnpackFile("other.zzz", outDirectory, this);
            File.Create($"{outDirectory}\\data\\disk\\wrong_disk");
            this.Invoke((MethodInvoker)(() =>
            {
                this.Close();
            }));
        }

        public void UpdateStatus(int mainIndex, int otherIndex, int mainLen, int otherLen, string currentFile)
        {
            label1.Invoke((MethodInvoker)(() =>
            {
                label1.Text = $"main.zzz- ({(float)mainIndex / mainLen * 100.0}%)";
            }));
            label2.Invoke((MethodInvoker)(() =>
            {
                label2.Text = $"other.zzz- ({(float)otherIndex / otherLen * 100.0}%)";
            }));

            if (otherLen == 1)
            {
                label3.Invoke((MethodInvoker)(() =>
                {
                    label3.Text = $"{mainIndex}\\{mainLen}";
                }));
                progressBar1.Invoke((MethodInvoker)(() =>
                {
                    progressBar1.Maximum = mainLen;
                    progressBar1.Value = mainIndex;
                }));
                progressBar2.Invoke((MethodInvoker)(() =>
                {
                    progressBar2.Maximum = mainLen * 2;
                    progressBar2.Value = mainIndex;
                }));
            }
            else
            {
                label3.Invoke((MethodInvoker)(() =>
                {
                    label3.Text = $"{otherIndex}\\{otherLen}";
                }));
                progressBar1.Invoke((MethodInvoker)(() =>
                {
                    progressBar1.Maximum = otherLen;
                    progressBar1.Value = otherIndex;
                }));
                progressBar2.Invoke((MethodInvoker)(() =>
                {
                    progressBar2.Maximum = otherLen * 2;
                    progressBar2.Value = otherIndex+otherLen;
                }));
            }

            label4.Invoke((MethodInvoker)(() =>
            {
                label4.Text = $"Current file: {currentFile}";
            }));
        }

        private void zzzWatcher_FormClosing(object sender, FormClosingEventArgs e)
        {
            thread.Abort();
        }
    }
}

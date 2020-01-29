using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Diagnostics;

namespace viiidem_customlauncher
{
    public partial class Form1 : Form
    {
        Bitmap logoBmp;
        public Form1()
        {
            ServicePointManager.Expect100Continue = true;
            ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls12;
            InitializeComponent();

            CheckUnpacked();
        }

        string gitStatus = string.Empty;

        bool bAlreadyUpdated = false;
        private void CheckUpdates()
        {
            bAlreadyUpdated = false;
            if(!File.Exists("demaster.conf"))
            {
                CreateNewConfFile();
                ForceUpdate();
                bAlreadyUpdated = true;
            }
            if (!File.Exists("ff8_demaster.dll"))
            {
                ForceUpdate();
                bAlreadyUpdated = true;
            }
            else if(new FileInfo("ff8_demaster.dll").Length == 0)
            {
                ForceUpdate();
                bAlreadyUpdated = true;
            }
            string[] confReader = File.ReadAllLines("demaster.conf");
            if(confReader.Length>5)
            {
                ForceUpdate();
                bAlreadyUpdated = true;
            }
            else
            {
                CreateNewConfFile();
                ForceUpdate();
                bAlreadyUpdated = true;
            }

        }

        private void ForceUpdate()
        {
            if (bAlreadyUpdated)
                return;
            richTextBox1.AppendText("Downloading ff8_demaster.dll from https://github.com/MaKiPL/FF8_demastered/raw/master/ff8_demaster.dll \n");
            DownloadFile("https://github.com/MaKiPL/FF8_demastered/raw/master/ff8_demaster.dll");
            if (File.Exists("ff8_demaster.dll"))
            {
                if (new FileInfo("ff8_demaster.dll").Length == 0)
                    MessageBox.Show("Update ERROR! Please download manually from github!");
                else
                    MessageBox.Show($"Succesfully downloaded newest version from Github!");
            }
            else
                    MessageBox.Show("Update ERROR! Please download manually from github!");
        }

        private void CreateNewConfFile()
        {
            richTextBox1.AppendText("Downloading demaster.conf from: https://raw.githubusercontent.com/MaKiPL/FF8_demastered/master/demaster.conf \n");
            DownloadFile("https://raw.githubusercontent.com/MaKiPL/FF8_demastered/master/demaster.conf");
        }

        private void CheckUnpacked()
        {
            bool bUnpackedAllSuccess = true;
            //to test if basically unpacked we basically test three files- audio.dat; 
            //get some music because why not, also check disk1 and if wrong_disk belongs for additional file
            if (!Directory.Exists("DEMASTER_EXP"))
                bUnpackedAllSuccess = false;
            else
            {
                if (!File.Exists("DEMASTER_EXP\\data\\disk\\disk1"))
                    bUnpackedAllSuccess = false;
                if (!File.Exists("DEMASTER_EXP\\data\\disk\\wrong_disk"))
                    bUnpackedAllSuccess = false;
                if (!File.Exists("DEMASTER_EXP\\data\\sound\\audio.dat"))
                    bUnpackedAllSuccess = false;
                if (!File.Exists("DEMASTER_EXP\\textures\\null.png"))
                    bUnpackedAllSuccess = false;
            }
            if (!bUnpackedAllSuccess)
                if (MessageBox.Show("We detected you don't have unpacked files to DEMASTER_EXP catalogue. We can do it automatically. Just click 'Yes'. If not, you can always do it in the menu. This message will disappear when the files will be detected as unpacked", "Confirm files re-unpack", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                    zzz.UnpackAll("DEMASTER_EXP");
        }

        private void DownloadFile(string uri)
        {
            try
            {
                using (WebClient wc = new WebClient())
                {
                    wc.DownloadFile(uri, Path.GetFileName(uri));
                }
            }
            catch(Exception e)
            {
                MessageBox.Show(e.Message);
            }
        }

        private void exitButton(object sender, EventArgs e)
        {
            Close();
        }

        private void buttonCallback_silver(object sender, EventArgs e)
        {
            (sender as Button).BackgroundImage = Properties.Resources.silver_on;
        }

        private void buttonCallback_silverOff(object sender, EventArgs e)
        {
            (sender as Button).BackgroundImage = Properties.Resources.silver_off;
        }

        private void buttonCallback_blueOn(object sender, EventArgs e)
        {
            (sender as Button).BackgroundImage = Properties.Resources.blue_on;
        }

        private void buttonCallback_blueOff(object sender, EventArgs e)
        {
            (sender as Button).BackgroundImage = Properties.Resources.blue_off;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if(MessageBox.Show("Are you sure?", "Confirm files re-unpack", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                zzz.UnpackAll("DEMASTER_EXP");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            string path = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            path = Path.Combine(path, "My Games", "FINAL FANTASY VIII Remastered", "Steam");
            Process.Start("explorer.exe", path);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            using (DemasterOpt dopt = new DemasterOpt())
                if (!dopt.IsDisposed)
                    dopt.ShowDialog();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            richTextBox1.AppendText("Opening url with default browser: https://github.com/MaKiPL/FF8_demastered/blob/master/ff8_demaster.dll \n");
            System.Diagnostics.Process.Start("https://github.com/MaKiPL/FF8_demastered/blob/master/ff8_demaster.dll");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            richTextBox1.AppendText("Trying auto-update...\n");
            CheckUpdates();
        }
    }
}

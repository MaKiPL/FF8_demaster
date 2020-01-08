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
        VanillaOptions opt = null;
        public Form1(bool bOffline, bool bForcePlay)
        {
            if (bForcePlay)
                LaunchGame();
            ServicePointManager.Expect100Continue = true;
            ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls12;
            if(!bOffline)
                GetOfficialAssets();

            InitializeComponent();
            opt = new VanillaOptions();
            opt.ReRead();

            if(!bOffline)
                CheckUpdates();
            CheckUnpacked();


            if(File.Exists("bg_top.jpg"))
            {
                FileInfo fi = new FileInfo("bg_top.jpg");
                if (fi.Length != 0)
                {
                    splitContainer1.Panel1.BackgroundImage = Image.FromFile("bg_top.jpg");
                    splitContainer1.Panel1.BackgroundImageLayout = ImageLayout.Stretch;
                }
            }
            if (File.Exists("logo_top.png"))
            {
                FileInfo fi = new FileInfo("logo_top.png");
                if (fi.Length != 0)
                {
                    logoBmp = new Bitmap("logo_top.png");
                }
            }

        }

        string gitStatus = string.Empty;

        private void GetOfficialAssets()
        {
            try
            {
                gitStatus = DownloadString("https://raw.githubusercontent.com/MaKiPL/FF8_demastered/master/status");
                if (!File.Exists("bg_top.jpg") || new FileInfo("bg_top.jpg").Length == 0)
                    DownloadFile("https://ffviiiremastered.square-enix-games.com/images/home/bg_top.jpg");
                if (!File.Exists("logo_top.png") || new FileInfo("logo_top.png").Length == 0)
                    DownloadFile("https://ffviiiremastered.square-enix-games.com/images/home/logo_top.png");
            }
            catch
            {
                ;
            }
        }

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
                    string getVersion = confReader[1].TrimStart(';');
                    string serverVersion = gitStatus.Split('\n')[0];
                if (getVersion != serverVersion)
                {
                    ForceUpdate();
                    bAlreadyUpdated = true;
                    confReader[1] = $";{serverVersion}";
                    File.WriteAllLines("demaster.conf", confReader);
                }
                }
                else
                {
                    CreateNewConfFile();
                    ForceUpdate();
                bAlreadyUpdated = true;
                }

            if(!string.IsNullOrWhiteSpace(gitStatus))
            {
                string[] op = gitStatus.Split('\n');
                label1.Text = $"demaster version: {op[0]}";
                label2.Text = $"launcher version: {op[1]}";
                richTextBox1.Text = op[2].Replace("\\n", Environment.NewLine);
            }

        }

        private void ForceUpdate()
        {
            if (bAlreadyUpdated)
                return;
            DownloadFile("https://github.com/MaKiPL/FF8_demastered/raw/master/ff8_demaster.dll");
            if (File.Exists("ff8_demaster.dll"))
            {
                if (new FileInfo("ff8_demaster.dll").Length == 0)
                    MessageBox.Show("Update ERROR! Please download manually from github!");
                else
                    MessageBox.Show($"Succesfully updated to {gitStatus.Split('\n')[0]}");
            }
            else
                    MessageBox.Show("Update ERROR! Please download manually from github!");
        }

        private void CreateNewConfFile()
        {
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
            catch
            {

            }
        }

        private string DownloadString(string uri)
        {
            string localn;
            using (WebClient wc = new WebClient() {CachePolicy = new System.Net.Cache.RequestCachePolicy(System.Net.Cache.RequestCacheLevel.NoCacheNoStore) })
            {
                localn=wc.DownloadString(uri);
            }
            return localn;
        }

        private void splitContainer1_Panel1_Paint(object sender, PaintEventArgs e)
        {
            logoBmp.MakeTransparent();
            e.Graphics.DrawImage(logoBmp, 0,0, logoBmp.Width/2, logoBmp.Height/2);
        }

        private void exitButton(object sender, EventArgs e)
        {
            Close();
        }

        private void buttonCallback_SetOrange(object sender, EventArgs e)
        {
            (sender as Button).BackgroundImage = Properties.Resources.orange;
        }

        private void buttonCallback_setOrangeOff(object sender, EventArgs e)
        {
            (sender as Button).BackgroundImage = Properties.Resources.orange_off;
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
            opt.ReRead();
                opt.ShowDialog();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            LaunchGame();
        }

        private void LaunchGame()
        {
            string text = "FFVIII.exe";
            bool result = true;
            ProcessStartInfo processStartInfo = new ProcessStartInfo(text);
            processStartInfo.UseShellExecute = true;
            processStartInfo.ErrorDialog = true;
            processStartInfo.ErrorDialogParentHandle = base.Handle;
            try
            {
                if (!File.Exists(text))
                {
                    string message = new FileNotFoundException().Message;
                }
                Process process;
                if (opt != null)
                {
                    if (opt.language == "JP")
                    {
                        processStartInfo.Arguments = "jp";
                        process = Process.Start(processStartInfo);
                    }
                    else
                        process = Process.Start(processStartInfo);
                }
                else
                    process = Process.Start(processStartInfo);
                process.WaitForExit();
            }
            catch
            {
                ;
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            using(DemasterOpt dopt = new DemasterOpt())
            {
                dopt.ShowDialog();
            }
        }
    }
}

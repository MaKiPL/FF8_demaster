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



namespace viiidem_customlauncher
{
    public partial class Form1 : Form
    {
        Bitmap logoBmp;
        public Form1()
        {
            ServicePointManager.Expect100Continue = true;
            ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls12;
            GetOfficialAssets();

            CheckUpdates();
            CheckUnpacked();


            InitializeComponent();
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
                if (!File.Exists("bg_top.jpg") || new FileInfo("bg_top.jpg").Length == 0)
                    DownloadFile("https://ffviiiremastered.square-enix-games.com/images/home/bg_top.jpg");
                if (!File.Exists("logo_top.png") || new FileInfo("logo_top.png").Length == 0)
                    DownloadFile("https://ffviiiremastered.square-enix-games.com/images/home/logo_top.png");
                gitStatus = DownloadString("https://raw.githubusercontent.com/MaKiPL/FF8_demastered/loli/status");

            }
            catch
            {
                ;
            }
        }

        private void CheckUpdates()
        {
            if(!File.Exists("demaster.conf"))
            {
                CreateNewConfFile();
            }

        }

        private void CreateNewConfFile()
        {
            //download from github
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
            using(WebClient wc = new WebClient())
            {
                wc.DownloadFile(uri, Path.GetFileName(uri));
            }
        }

        private string DownloadString(string uri)
        {
            string localn;
            using (WebClient wc = new WebClient())
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
    }
}

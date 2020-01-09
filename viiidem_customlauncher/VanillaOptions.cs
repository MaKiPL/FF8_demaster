using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Steamworks;

namespace viiidem_customlauncher
{
    public partial class VanillaOptions : Form
    {
        int width, height, mastervol, bgmvol, screenmode, display, cameraspeed, brightness, antialiaslevel, firstboot;

        public string language = "EN";
        string configPath;

        //this is write command
        private void button1_Click(object sender, EventArgs e)
        {
            width = int.Parse(textBox2.Text);
            height = int.Parse(textBox3.Text);
            mastervol = trackBar1.Value;
            bgmvol = trackBar1.Value;
            screenmode = comboBox3.SelectedIndex;
            string getDisplayNumber = comboBox4.SelectedItem.ToString().Substring(1,1);
            display = int.Parse(getDisplayNumber);
            cameraspeed = (int)numericUpDown2.Value;
            brightness = (int)numericUpDown1.Value;
            antialiaslevel = comboBox5.SelectedIndex;
            firstboot = 0;
            language = ReverseGetLangIdx(comboBox1.SelectedIndex);

            using (StreamWriter sw = new StreamWriter(configPath, false, Encoding.GetEncoding("Shift_JIS")))
            {
                sw.WriteLine($"width\t\t\t{width}");
                sw.WriteLine($"height\t\t\t{height}");
                sw.WriteLine($"language\t\t{language}");
                sw.WriteLine($"mastervol\t\t{mastervol}");
                sw.WriteLine($"bgmvol\t\t\t{bgmvol}");
                sw.WriteLine($"screenmode\t\t{screenmode}");
                sw.WriteLine($"display\t\t\t{display}");
                sw.WriteLine($"cameraspeed\t\t{cameraspeed}");
                sw.WriteLine($"brightness\t\t{brightness}");
                sw.WriteLine($"antialiaslevel\t{antialiaslevel}");
                sw.WriteLine($"firstboot\t{firstboot}");
            }

            this.Close();
        }

        internal void ReRead()
        {

            Init();

            //read here
            if (!File.Exists(configPath))
                return;
            string[] configFile = File.ReadAllLines(configPath, Encoding.GetEncoding("Shift_JIS"));
            foreach (string line in configFile)
            {
                string[] lineSplitted = line.Split('\t');
                string name = lineSplitted[0];
                string param = lineSplitted.Last();
                switch (name)
                {
                    case "width":
                        width = int.Parse(param);
                        break;
                    case "height":
                        height = int.Parse(param);
                        break;
                    case "language":
                        language = param;
                        break;
                    case "mastervol":
                        mastervol = int.Parse(param);
                        break;
                    case "bgmvol":
                        bgmvol = int.Parse(param);
                        break;
                    case "screenmode":
                        screenmode = int.Parse(param);
                        break;
                    case "display":
                        display = int.Parse(param);
                        break;
                    case "cameraspeed":
                        cameraspeed = int.Parse(param);
                        break;
                    case "brightness":
                        brightness = int.Parse(param);
                        break;
                    case "antialiaslevel":
                        antialiaslevel = int.Parse(param);
                        break;
                    case "firstboot":
                        firstboot = int.Parse(param);
                        break;
                }
            }
            comboBox1.SelectedIndex = getLangIdx(language);
            if (mastervol > 100 || mastervol < 0)
                mastervol = 100; //safe 
            trackBar1.Value = mastervol;
            if (brightness < 0 || brightness > 100)
                brightness = 50;
            numericUpDown1.Value = brightness;

            var screens = Screen.AllScreens;

            comboBox4.Items.Clear();
            for (int i = 0; i < screens.Length; i++)
                comboBox4.Items.Add($"[{i}]{(screens[i].Primary ? " PRIMARY" : "")}");

            if (display > comboBox4.Items.Count)
                display = 0;
            comboBox4.SelectedIndex = display;
            if (antialiaslevel > comboBox5.Items.Count || antialiaslevel < 0)
                antialiaslevel = 0;
            comboBox5.SelectedIndex = antialiaslevel;


            DEVMODE vDevMode = new DEVMODE();
            int __i = 0;
            resolutions = new List<res>();
            while (EnumDisplaySettings(null, __i, ref vDevMode))
            {
                resolutions.Add(new res()
                {
                    width = vDevMode.dmPelsWidth,
                    height = vDevMode.dmPelsHeight,
                    bpp = vDevMode.dmBitsPerPel,
                    freq = vDevMode.dmDisplayFrequency
                });
                __i++;
            }
            foreach (var vidMode in resolutions)
                comboBox2.Items.Add($"{vidMode.width}x{vidMode.height}x{vidMode.bpp}x{vidMode.freq}");

            for (int i = 0; i < comboBox2.Items.Count; i++)
            {
                if (i == resolutions.Count)
                    break;
                res _res = resolutions[i];
                if (width == _res.width && height == _res.height)
                {
                    comboBox2.SelectedIndex = i;
                    break;
                }
            }

            if (screenmode < 0 || screenmode > comboBox3.Items.Count)
                screenmode = 0;
            comboBox3.SelectedIndex = screenmode;
            if (cameraspeed < 0 || cameraspeed > 8)
                cameraspeed = 4;
            numericUpDown2.Value = cameraspeed;
        }

        private string ReverseGetLangIdx(int idx)
        {
            switch (idx)
            {
                case 0:
                    return "JP";
                case 1:
                    return "EN";
                case 2:
                    return "FR";
                case 3:
                    return "IT";
                case 4:
                    return "DE";
                case 5:
                    return "ES";
                default:
                    goto case 0;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            textBox1.Text = (sender as TrackBar).Value.ToString();
        }


        public void Init()
        {
            bool bDefaultValue = false;
            AppId_t applicationId = (AppId_t)1026680u;
            try
            {
                if (!SteamAPI.Init())
                {
                    MessageBox.Show("There was an error with SteamApi Init. You might have issues with config. Using first folder instead in MyGames");
                    bDefaultValue = true;
                }
            }
            catch
            {
                MessageBox.Show("There was an error with SteamApi Init. You might have issues with config. Using first folder instead in MyGames");
                bDefaultValue = true;
            }
            string steamplayer = null;
            if (!bDefaultValue)
            {
                CSteamID player = SteamApps.GetAppOwner();
                steamplayer = player.m_SteamID.ToString();
                string lang = SteamUtils.GetSteamUILanguage();
            }
            string path = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            if (!bDefaultValue)
                configPath = path = Path.Combine(path, "My Games", "FINAL FANTASY VIII Remastered", "Steam", steamplayer, "config.txt");
            else
            {
                configPath = path = Path.Combine(path, "My Games", "FINAL FANTASY VIII Remastered", "Steam");
                var paths = Directory.EnumerateDirectories(path).ToList();
                if (paths.Count != 0)
                    configPath = path = paths[0];
                else
                    configPath = path = Path.Combine(path, "0");
                configPath = path = Path.Combine(path, "config.txt");
            }
            if (!Directory.Exists(Path.GetDirectoryName(path)))
                Directory.CreateDirectory(Path.GetDirectoryName(path));
            if (!File.Exists(path))
            {
                File.WriteAllLines(path, new string[]
                {
                    "width\t\t\t1280",
                    "height\t\t\t768",
                    "language\t\tEN",
                    "mastervol\t\t100",
                    "bgmvol\t\t\t100",
                    "screenmode\t\t2",
                    "display\t\t\t1",
                    "cameraspeed\t\t4",
                    "brightness\t\t50",
                    "antialiaslevel\t1",
                    "firstboot\t1"
                }, Encoding.GetEncoding("Shift_JIS"));
            }
        }

        public VanillaOptions()
        {
            InitializeComponent();
        }

        //pinvoke.net

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        public struct DISPLAY_DEVICE
        {
            [MarshalAs(UnmanagedType.U4)]
            public int cb;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string DeviceName;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string DeviceString;
            [MarshalAs(UnmanagedType.U4)]
            public DisplayDeviceStateFlags StateFlags;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string DeviceID;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string DeviceKey;
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            res selectedRes = resolutions[(sender as ComboBox).SelectedIndex];
            textBox2.Text = selectedRes.width.ToString();
            textBox3.Text = selectedRes.height.ToString();
        }

        [Flags()]
        public enum DisplayDeviceStateFlags : int
        {
            /// <summary>The device is part of the desktop.</summary>
            AttachedToDesktop = 0x1,
            MultiDriver = 0x2,
            /// <summary>The device is part of the desktop.</summary>
            PrimaryDevice = 0x4,
            /// <summary>Represents a pseudo device used to mirror application drawing for remoting or other purposes.</summary>
            MirroringDriver = 0x8,
            /// <summary>The device is VGA compatible.</summary>
            VGACompatible = 0x10,
            /// <summary>The device is removable; it cannot be the primary display.</summary>
            Removable = 0x20,
            /// <summary>The device has more display modes than its output devices support.</summary>
            ModesPruned = 0x8000000,
            Remote = 0x4000000,
            Disconnect = 0x2000000
        }

        [DllImport("user32.dll")]
        static extern bool EnumDisplayDevices(string lpDevice, uint iDevNum, ref DISPLAY_DEVICE lpDisplayDevice, uint dwFlags);

        struct res
        {
            public int width;
            public int height;
            public int bpp;
            public int freq;


        }

        List<res> resolutions;

        //stack
        [DllImport("user32.dll")]
        public static extern bool EnumDisplaySettings(
              string deviceName, int modeNum, ref DEVMODE devMode);
        const int ENUM_CURRENT_SETTINGS = -1;

        const int ENUM_REGISTRY_SETTINGS = -2;

        [StructLayout(LayoutKind.Sequential)]
        public struct DEVMODE
        {

            private const int CCHDEVICENAME = 0x20;
            private const int CCHFORMNAME = 0x20;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 0x20)]
            public string dmDeviceName;
            public short dmSpecVersion;
            public short dmDriverVersion;
            public short dmSize;
            public short dmDriverExtra;
            public int dmFields;
            public int dmPositionX;
            public int dmPositionY;
            public ScreenOrientation dmDisplayOrientation;
            public int dmDisplayFixedOutput;
            public short dmColor;
            public short dmDuplex;
            public short dmYResolution;
            public short dmTTOption;
            public short dmCollate;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 0x20)]
            public string dmFormName;
            public short dmLogPixels;
            public int dmBitsPerPel;
            public int dmPelsWidth;
            public int dmPelsHeight;
            public int dmDisplayFlags;
            public int dmDisplayFrequency;
            public int dmICMMethod;
            public int dmICMIntent;
            public int dmMediaType;
            public int dmDitherType;
            public int dmReserved1;
            public int dmReserved2;
            public int dmPanningWidth;
            public int dmPanningHeight;

        }

        int getLangIdx(string lang)
        {
            switch (lang)
            {
                case "JP":
                    return 0;
                case "EN":
                    return 1;
                case "FR":
                    return 2;
                case "IT":
                    return 3;
                case "DE":
                    return 4;
                case "ES":
                    return 5;

            }
            return 1;
        }
    }
}

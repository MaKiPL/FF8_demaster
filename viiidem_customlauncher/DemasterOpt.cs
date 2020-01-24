using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace viiidem_customlauncher
{
    public partial class DemasterOpt : Form
    {
        const string header = "; FF8 Remastered: Demaster patch config file";
        string version;
        public DemasterOpt()
        {
            InitializeComponent();
            if(!File.Exists("demaster.conf"))
            {
                nodata();
            }
            void nodata()
            {
                MessageBox.Show("demaster.conf is empty or missing, Click [Download Update (Auto)] to download the default version.");
                //MessageBox.Show("demaster.conf not found, restart launcher or download current release from github");
                Close();
            }
            string[] demContent = File.ReadAllLines("demaster.conf");
            version = demContent[1];
            for(int i = 2; i<demContent.Length; i+=2)
            {
                if (string.IsNullOrWhiteSpace(demContent[i])) continue;
                string[] feature = demContent[i].Split('=');
                if (feature.Length != 2) continue;
                string descr = demContent[i + 1].TrimStart(';');
                dataGridView1.Rows.Add($"{feature[0]}", descr, feature[1][0] == '1' ? true : false);
            }
            if (dataGridView1.Rows.Count == 0)
                nodata();
                
        }

        private void DemasterOpt_FormClosing(object sender, FormClosingEventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            using (StreamWriter sw = new StreamWriter("demaster.conf"))
            {
                sw.WriteLine(header);
                sw.WriteLine(version);
                foreach (DataGridViewRow tableEntry in dataGridView1.Rows)
                {
                    string optionName = tableEntry.Cells[0].Value as string;
                    string descr = tableEntry.Cells[1].Value as string;
                    bool bEnabled = (bool)tableEntry.Cells[2].Value;
                    sw.WriteLine($"{optionName}={(bEnabled ? "1" : "0")}");
                    sw.WriteLine($";{descr}");
                }
            }
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}

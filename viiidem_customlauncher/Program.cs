using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace viiidem_customlauncher
{
    static class Program
    {
        /// <summary>
        /// Główny punkt wejścia dla aplikacji.
        /// </summary>
        [STAThread]
        static int Main(string[] args)
        {
            if (args.Length > 0)
                switch (args[0].ToLower().Trim())
                {
                    case "-u":
                    case "--unpack":
                        if (args.Length <= 1)
                        {
                            MessageBox.Show("Path to FFVIII:Remastered root folder was not provided.", "Error",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return -1;
                        }
                        if (!Directory.Exists(args[1]))
                        {
                            MessageBox.Show("Path to FFVIII:Remastered root folder does not exist.", "Error",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return -1;
                        }
                        zzz.UnpackAllDetached(args[1], "DEMASTER_EXP");
                        MessageBox.Show("FFVIII: Remastered unpacked successfully.", "Success", MessageBoxButtons.OK,
                            MessageBoxIcon.Information);
                        return 0;
                }
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
            return 0;
        }
    }
}

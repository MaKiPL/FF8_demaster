using System;
using System.Collections.Generic;
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
        static void Main(string[] args)
        {
            bool bOffline = false;
            bool bForcePlay = false;
            if (args.Length > 0)
            {
                for(int i = 0; i<args.Length; i++)
                {
                    switch(args[i])
                    {
                        case "-offline":
                            bOffline = true;
                            break;
                        case "-forceplay"
                            :bForcePlay = true;break;
                    }
                }
            }
                
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1(bOffline,bForcePlay));
        }
    }
}

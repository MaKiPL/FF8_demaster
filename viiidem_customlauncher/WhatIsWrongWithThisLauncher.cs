using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace viiidem_customlauncher
{
    class WhatIsWrongWithThisLauncher
    {
        public static void Tellme(string why)
        {
            using (FileStream fs = new FileStream("launcher_unfucker.txt", FileMode.Append, FileAccess.Write))
            {
                using (StreamWriter sw = new StreamWriter(fs))
                {
                    sw.WriteLine($"{DateTime.UtcNow - Process.GetCurrentProcess().StartTime.ToUniversalTime()}|{why}");
                }
            }
        }
    }
}

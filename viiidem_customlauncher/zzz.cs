using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace viiidem_customlauncher
{
    /// <summary>
    /// zzz is called zzz because sleep is fun!
    /// </summary>
    class zzz
    {
        public static void UnpackAll(string outDirectory)
        {
            localIndex = 0;
            localIndex2 = 0;
            if(!File.Exists("main.zzz") || !File.Exists("other.zzz"))
            {
                MessageBox.Show(
                    "Either main.zzz or other.zzz was not found in current directory!\nDid you put this launcher in your Final Fantasy VIII Remastered root directory?",
                    "Auto unpacking error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if (!Directory.Exists(outDirectory))
                Directory.CreateDirectory(outDirectory);

            zzzWatcher zzzWatcher = new zzzWatcher(outDirectory);
            zzzWatcher.ShowDialog();


        }

        private struct zzzEntry
        {
            public uint stringLen;
            public string string_;
            public UInt64 offset;
            public uint size;
        }

        static int localIndex = 0;
        static int localIndex2 = 0;

        public static void UnpackFile(string path, string outDirectory, zzzWatcher watcher)
        {
            List<zzzEntry> zzzzzzzEntries = new List<zzzEntry>();
            
            using(FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read))
            {
                using(BinaryReader br = new BinaryReader(fs))
                {
                    uint filecnt = br.ReadUInt32();
                    for(int i = 0; i<filecnt; i++)
                    {
                        zzzEntry zentr = new zzzEntry() { stringLen = br.ReadUInt32() };
                        zentr.string_ = new string(br.ReadChars((int)zentr.stringLen));
                        zentr.offset = br.ReadUInt64();
                        zentr.size = br.ReadUInt32();
                        zzzzzzzEntries.Add(zentr);
                    }

                    foreach(var lst in zzzzzzzEntries)
                    {
                        if(path == "other.zzz")
                            watcher.UpdateStatus(localIndex, localIndex2, localIndex, zzzzzzzEntries.Count, lst.string_);
                        else
                            watcher.UpdateStatus(localIndex, localIndex2, zzzzzzzEntries.Count, 1, lst.string_);
                        string outPath = Path.Combine(outDirectory, lst.string_);
                        string dirPath = Path.GetDirectoryName(outPath);
                        if (!Directory.Exists(dirPath))
                            Directory.CreateDirectory(dirPath);
                        fs.Seek((long)lst.offset, SeekOrigin.Begin);
                        byte[] localb = br.ReadBytes((int)lst.size);
                        File.WriteAllBytes(outPath, localb);
                        if (path == "main.zzz")
                            localIndex++;
                        else
                            localIndex2++;
                    }
                }
            }
        }
    }
}

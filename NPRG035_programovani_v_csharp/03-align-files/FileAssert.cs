using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace task3.tests;

public static class FileTools
{
    public static bool AreEqual(string expected, string actual)
    {
        byte[] file1 = File.ReadAllBytes(expected);
        byte[] file2 = File.ReadAllBytes(actual);
        if (file1.Length == file2.Length)
        {
            for (int i = 0; i < file1.Length; i++)
            {
                if (file1[i] != file2[i])
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    
}

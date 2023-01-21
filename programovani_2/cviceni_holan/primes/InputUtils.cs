using System.Text;
using System.Collections.Generic;
using System;
using System.IO;


namespace cs
{
    public class InputUtils
    {
        private TextReader inputStream;
        
        public InputUtils(TextReader input) {
            this.inputStream = input;
        }
        public int ReadNumber() {

            char x = ' ';
            StringBuilder s = new StringBuilder();

            bool negative;

            do {
                negative = x == '-';
                x = (char) inputStream.Read();
            }
            while( !Char.IsNumber(x)  );
            

            do {
                s.Append(x);
                x = (char) inputStream.Read();
            }
            while(Char.IsNumber(x));

            int ret = Int32.Parse(s.ToString());
            return negative ? -ret : ret;
        }
    }
}
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Pexeso
{
    public partial class PexesoCardView : UserControl
    {
        public PexesoCardView() : base()
        {
            InitializeComponent();
        }


        private bool turned;
        public bool Turned
        {
            get { return turned; }
            set
            {
                turned = value;
                back.Visible = !turned;
                front.Visible = turned;
            }
        }

        public PexesoCardView(string frontText, int width, int height, int margin) : this()
        {

            this.Width = width;
            this.Height = height;
            this.Margin = new Padding(margin);

            front.Text = frontText;
            front.Click += (s, e) => this.OnClick(e);
            back.Click += (s, e) => this.OnClick(e);

            Turned = false;
        }

        public bool Hidden { get; private set; } = false;

        public void Hide()
        {
            this.Hidden = true;
            this.front.Visible = false;
            this.back.Visible = false;
            this.BackColor = Color.Yellow;
        }

        private void front_Click(object sender, EventArgs e)
        {
            // nic
        }
    }
}

namespace Pexeso
{
    partial class PexesoCardView
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.front = new System.Windows.Forms.Label();
            this.back = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // front
            // 
            this.front.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.front.AutoSize = true;
            this.front.Font = new System.Drawing.Font("Garamond", 31.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.front.Location = new System.Drawing.Point(0, 0);
            this.front.Name = "front";
            this.front.Size = new System.Drawing.Size(99, 60);
            this.front.TabIndex = 0;
            this.front.Text = "XX";
            this.front.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.front.Click += new System.EventHandler(this.front_Click);
            // 
            // back
            // 
            this.back.AutoSize = true;
            this.back.Dock = System.Windows.Forms.DockStyle.Fill;
            this.back.Font = new System.Drawing.Font("Garamond", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.back.Location = new System.Drawing.Point(0, 0);
            this.back.Name = "back";
            this.back.Size = new System.Drawing.Size(96, 34);
            this.back.TabIndex = 1;
            this.back.Text = "Pexeso";
            this.back.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // PexesoCardView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(192)))), ((int)(((byte)(192)))));
            this.Controls.Add(this.back);
            this.Controls.Add(this.front);
            this.Name = "PexesoCardView";
            this.Size = new System.Drawing.Size(100, 100);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Label front;
        private Label back;
    }
}

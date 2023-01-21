namespace Pexeso
{
    partial class PexesoView
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PexesoView));
            this.pexesoPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.scoreLabel = new System.Windows.Forms.Label();
            this.winPanel = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.newGameButton = new System.Windows.Forms.Button();
            this.winScore = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.winPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // pexesoPanel
            // 
            this.pexesoPanel.Location = new System.Drawing.Point(12, 57);
            this.pexesoPanel.Name = "pexesoPanel";
            this.pexesoPanel.Size = new System.Drawing.Size(650, 584);
            this.pexesoPanel.TabIndex = 1;
            // 
            // scoreLabel
            // 
            this.scoreLabel.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.scoreLabel.AutoSize = true;
            this.scoreLabel.Font = new System.Drawing.Font("Segoe UI", 16F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.scoreLabel.Location = new System.Drawing.Point(311, 9);
            this.scoreLabel.Name = "scoreLabel";
            this.scoreLabel.Size = new System.Drawing.Size(32, 37);
            this.scoreLabel.TabIndex = 2;
            this.scoreLabel.Text = "0";
            // 
            // winPanel
            // 
            this.winPanel.BackColor = System.Drawing.Color.Transparent;
            this.winPanel.Controls.Add(this.label4);
            this.winPanel.Controls.Add(this.label3);
            this.winPanel.Controls.Add(this.label2);
            this.winPanel.Controls.Add(this.newGameButton);
            this.winPanel.Controls.Add(this.winScore);
            this.winPanel.Controls.Add(this.label1);
            this.winPanel.Location = new System.Drawing.Point(12, 12);
            this.winPanel.Name = "winPanel";
            this.winPanel.Size = new System.Drawing.Size(650, 473);
            this.winPanel.TabIndex = 3;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Segoe UI", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.label4.Location = new System.Drawing.Point(443, 205);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(94, 46);
            this.label4.TabIndex = 5;
            this.label4.Text = "tahů.";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Segoe UI", 16F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.label3.Location = new System.Drawing.Point(209, 294);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(344, 37);
            this.label3.TabIndex = 4;
            this.label3.Text = "Klaním se Vaší sloní pamětí.";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Segoe UI", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.label2.Location = new System.Drawing.Point(53, 205);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(225, 46);
            this.label2.TabIndex = 3;
            this.label2.Text = "A to pouze za";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // newGameButton
            // 
            this.newGameButton.Font = new System.Drawing.Font("Segoe UI", 32F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.newGameButton.Location = new System.Drawing.Point(299, 362);
            this.newGameButton.Name = "newGameButton";
            this.newGameButton.Size = new System.Drawing.Size(328, 86);
            this.newGameButton.TabIndex = 2;
            this.newGameButton.Text = "Nová hra";
            this.newGameButton.UseVisualStyleBackColor = true;
            this.newGameButton.Click += new System.EventHandler(this.newGame);
            // 
            // winScore
            // 
            this.winScore.AutoSize = true;
            this.winScore.Font = new System.Drawing.Font("Segoe UI", 60F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.winScore.Location = new System.Drawing.Point(273, 161);
            this.winScore.Name = "winScore";
            this.winScore.Size = new System.Drawing.Size(175, 133);
            this.winScore.TabIndex = 1;
            this.winScore.Text = "XX";
            this.winScore.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Segoe UI", 32F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.label1.Location = new System.Drawing.Point(3, 76);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(628, 72);
            this.label1.TabIndex = 0;
            this.label1.Text = "Pane doktore, vyhrál jste!";
            // 
            // PexesoView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(669, 648);
            this.Controls.Add(this.winPanel);
            this.Controls.Add(this.scoreLabel);
            this.Controls.Add(this.pexesoPanel);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "PexesoView";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "Pexeso (Zdeněk Tomis)";
            this.Load += new System.EventHandler(this.PexesoWindow_Load);
            this.winPanel.ResumeLayout(false);
            this.winPanel.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private FlowLayoutPanel pexesoPanel;
        private Label scoreLabel;
        private Panel winPanel;
        private Label winScore;
        private Label label1;
        private Button newGameButton;
        private Label label2;
        private Label label3;
        private Label label4;
    }
}
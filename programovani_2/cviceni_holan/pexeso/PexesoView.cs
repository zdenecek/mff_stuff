using System.Diagnostics;

namespace Pexeso
{
    public partial class PexesoView : Form
    {
        private PexesoViewModel viewModel;
        private int pexesoMargin = 3;


        public PexesoView()
        {
            InitializeComponent();
            viewModel = new PexesoViewModel(this);
        }

        //public void SetScore(int score) => this.scoreLabel.Text = score.ToString();

        private void PexesoWindow_Load(object sender, EventArgs e)
        {
            winScore.DataBindings.Add("Text", viewModel.Model, "Score");
            scoreLabel.DataBindings.Add("Text", viewModel.Model, "Score");

            newGame();
        }

        public void SwitchToWinScreen()
        {
            Debug.WriteLine("Finished");
            pexesoPanel.Visible = false;
            winPanel.Visible = true;
        }

        private void newGame(object sender, EventArgs e) => newGame();
        private void newGame()
        {
            viewModel.Start();
            pexesoPanel.Visible = true;
            winPanel.Visible = false;
        }

        public void createCardButtons(PexesoCard[,] cards)
        {
            pexesoPanel.Controls.Clear();

            int cols = cards.GetLength(0);
            int rows = cards.GetLength(1);
            int width = pexesoPanel.Width / cols - pexesoMargin * 2;
            int height = pexesoPanel.Height / rows - pexesoMargin * 2;

            for (int r = 0; r < cards.GetLength(0); r++)
            {
                for (var c = 0; c < cards.GetLength(1); c++)
                {
                    var card = cards[r, c];

                    var cardView = new PexesoCardView(card.Value.ToString(), width, height, pexesoMargin);

                    cardView.Text = card.Value.ToString();
                    cardView.Click += (sender, args) => viewModel.cardClicked(card, cardView);


                    card.CardTurned += (sender, args) =>
                    {
                        if (args.Card.Turned) cardView.Turned = true;
                    };

                    viewModel.CardsAreToBeHidden += () =>
                    {
                        if (card.Removed && !cardView.Hidden)
                            cardView.Hide();
                        else if (card.Turned == false)
                            cardView.Turned = false;
                    };

                    this.pexesoPanel.Controls.Add(cardView);
                }
            }
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }
    }

}
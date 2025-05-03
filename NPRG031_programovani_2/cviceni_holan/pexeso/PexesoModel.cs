using System;
using System.Diagnostics;

namespace Pexeso;



public class PexesoModel
{
    public event CardEventHandler? CardTurned;
    public event CardEventHandler? CardRemoved;
    public event EventHandler? ScoreChanged;
    public event EventHandler? GameWon;

    public readonly PexesoCard[,] cards;
   
    public int Score
    {
        get { return score; }
        set { score = value;
            ScoreChanged?.Invoke(this, new EventArgs());
        }
    }

    public GameState GameState
    {
        get { return gameState; }
        set { gameState = value;
            if (gameState == GameState.Finished) GameWon?.Invoke(this, new EventArgs());
        }
    }

    private GameState gameState;
    private List<PexesoCard> turnedCards;
    private readonly int width, height, cardCount;
    private readonly Random generator;
    private int score = 0;
    private int remainingCards;


    public PexesoModel(int height, int width, Random generator)
    {
        this.cards = new PexesoCard[height, width];
        this.cardCount = height * width;
        this.width = width;
        this.height = height;

        if (cardCount % 2 != 0) throw new ArgumentException("Invalid width and height, odd card count");

        this.generator = generator;
    }

    public void StartGame()
    {
        remainingCards = cardCount;
        turnedCards = new List<PexesoCard>();
        gameState = GameState.Running;
        Score = 0;

        MakeCards();
    }

    private void MakeCards()
    {

        var cards = new List<int>();
        var realCount = cardCount / 2;
        for (int i = 0; i < realCount; i++)
        {
            cards.Add(i + 1);
            cards.Add(i + 1);
        }

        var shuffled = cards.OrderBy(x => generator.Next()).ToList();


        for (int r = 0; r < height; r++)
        {
            for (int c = 0; c < width; c++)
            {
                var id = shuffled.ElementAt(c + r * width);
                var card = new PexesoCard(id, r, c);
                card.CardTurned += (sender, args) => CardTurned?.Invoke(this, args);
                card.CardRemoved += (sender, args) => CardRemoved?.Invoke(this, args);
                this.cards[r, c] = card;
            }
        }
    }

    public void Turn(PexesoCard card)
    {
        if (gameState == GameState.Finished || turnedCards.Count == 2 || card.Removed || card.Turned) return;
        turnedCards.Add(card);

        Score += 1;
        card.Turned = true;

        this.Evaluate();
    }

    private bool evaluating = false;

    private void Evaluate()
    {
        if (turnedCards.Count != 2 || evaluating) return;
        evaluating = true;
        if (turnedCards[0].Value == turnedCards[1].Value)
        {
            turnedCards.ForEach(x => x.Removed = true);
            remainingCards -= 2;
        }
        else
            turnedCards.ForEach(x => x.Turned = false);
        turnedCards.Clear();

        if(remainingCards == 0)
            GameState = GameState.Finished;

        evaluating = false;
    }
}

public enum GameState
{
    Running,
    Finished
}

public delegate void CardEventHandler(object sender, CardEventArgs args);

public class CardEventArgs : EventArgs
{
    public readonly PexesoCard Card;

    public CardEventArgs(PexesoCard card)
    {
        Card = card;
    }
}


public class PexesoCard
{
    public readonly int Value;

    public event CardEventHandler? CardTurned;
    public event CardEventHandler? CardRemoved;


    private bool turned;
    private bool removed;

    public bool Removed
    {
        get { return removed; }
        set { removed = value;
            CardRemoved?.Invoke(this, new CardEventArgs(this));
        }
    }


    public bool Turned
    {
        get { return turned; }
        set
        {
            turned = value;
            Debug.WriteLine($"Card Turned: {Row}:{Column} {value}");
            CardTurned?.Invoke(this, new CardEventArgs(this));
        }
    }


    public readonly int Row, Column;

    public PexesoCard(int value, int row, int col)
    {
        Row = row;
        Column = col;
        Value = value;
        turned = false;
        removed = false;
    }
}

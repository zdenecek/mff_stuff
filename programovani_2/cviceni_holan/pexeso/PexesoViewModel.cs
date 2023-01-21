using System;

namespace Pexeso;
public class PexesoViewModel
{
    private readonly PexesoView view;
    public PexesoModel Model { get; private set; }


    public event Action? CardsAreToBeHidden;

    public PexesoViewModel(PexesoView view)
	{
        this.view = view;
        Model = new PexesoModel(6, 6, new Random( /* 12 */));

        view.Click += (s, e) => CardsAreToBeHidden?.Invoke();
        Model.GameWon += (s, e) => view.SwitchToWinScreen();
    }

    public void Start()
    {
        Model.StartGame();
        view.createCardButtons(Model.cards);
    }

    public void cardClicked(PexesoCard card, PexesoCardView cardView)
    {
        CardsAreToBeHidden?.Invoke();
        Model.Turn(card);
    }
}

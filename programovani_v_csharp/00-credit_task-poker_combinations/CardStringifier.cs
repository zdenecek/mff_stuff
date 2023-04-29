using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CreditTask;

static class CardStringifier
{
    public static char ValueToString(CardValue value)
    {
        return value switch
        {
            CardValue.Two => '2',
            CardValue.Three => '3',
            CardValue.Four => '4',
            CardValue.Five => '5',
            CardValue.Six => '6',
            CardValue.Seven => '7',
            CardValue.Eight => '8',
            CardValue.Nine => '9',
            CardValue.Ten => 'X',
            CardValue.Jack => 'J',
            CardValue.Queen => 'Q',
            CardValue.King => 'K',
            CardValue.Ace => 'A',
            CardValue.AceLower => 'A',
            _ => throw new ArgumentOutOfRangeException("Card value", value, null)
        };
    }

    public static CardValue ValueFromString(char value)
    {
        return value switch
        {
            '2' => CardValue.Two,
            '3' => CardValue.Three,
            '4' => CardValue.Four,
            '5' => CardValue.Five,
            '6' => CardValue.Six,
            '7' => CardValue.Seven,
            '8' => CardValue.Eight,
            '9' => CardValue.Nine,
            'X' => CardValue.Ten,
            'J' => CardValue.Jack,
            'Q' => CardValue.Queen,
            'K' => CardValue.King,
            'A' => CardValue.Ace,
            _ => throw new ArgumentOutOfRangeException("Card value", value, null)
        };
    }

    public static char SuitToString(Suit suit)
    {
        return suit switch
        {
            Suit.Spades => 's',
            Suit.Hearts => 'h',
            Suit.Diamonds => 'd',
            Suit.Clubs => 'c',
            _ => throw new ArgumentOutOfRangeException("Card suit", suit, null)
        };
    }

    public static Suit SuitFromString(char suit)
    {
        return suit switch
        {
            's' => Suit.Spades,
            'h' => Suit.Hearts,
            'd' => Suit.Diamonds,
            'c' => Suit.Clubs,
            _ => throw new ArgumentOutOfRangeException("Card suit", suit, null)
        };
    }

    public static string ToString(Card card)
    {
            return $"{SuitToString(card.Suit)}{ValueToString(card.Value)}";
    }

    public static Card FromString(string value)
    {
        if (value.Length != 2) throw new ArgumentException("Invalid card: '" + value + "'");

        var suit = SuitFromString(value[0]);
        var cardValue = ValueFromString(value[1]);

        return new Card(cardValue, suit);
    }
}

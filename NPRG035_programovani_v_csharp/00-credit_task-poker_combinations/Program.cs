using System;
using System.Collections.Generic;
using System.IO;

namespace CreditTask;

class Program
{
    static void Main(string[] args)
    {
        if (args.Length != 1)
        {
            Console.WriteLine("Usage: Program.exe file");
            return;
        }

        try
        {
            using var reader = new StreamReader(args[0]);

            var line = reader.ReadLine();
            if (line is null) throw new FormatException();
            int playersc = int.Parse(line);

            if (playersc < 2) throw new FormatException(); 

            var players = new List<Player>();
            for(int i = 0; i < playersc; ++i)
            {
                players.Add(new Player($"Player {i + 1}"));
            }

            run(players, reader);
            
            while(!reader.EndOfStream)
            {
                var emptyLine = reader.ReadLine();
                if (emptyLine is not "") throw new Exception("Invalid format");
                Console.WriteLine("");
                run(players, reader);
            }

        }
        catch (Exception e) when (e is FileNotFoundException or AccessViolationException or IOException)
        {
            Console.WriteLine("File error");
        }
        catch (Exception e) when (e is FormatException)
        {
            Console.WriteLine("Invalid player count");
        }
        catch (Exception e) when (e is ArgumentException or ArgumentOutOfRangeException or ArgumentNullException)
        {
            Console.WriteLine(e.Message);
        }
        catch (Exception e)
        {
            Console.WriteLine("Error: " + e.Message);
        }
    }

    static void run(List<Player> players, StreamReader reader)
    {
        
        bool started = false;
        
        foreach (var player in players)
        {
            player.Cards.Clear();
            player.Combination = null;

            var line = reader.ReadLine();

            if (line is null || line.Trim() == "")
            {
                if (started) throw new Exception( "Invalid input: empty line");
                else return;
            }
            started = true;

            foreach (string s in line.Trim().Split(new char[] { }, StringSplitOptions.RemoveEmptyEntries))
            {
               player.Cards.Add(CardStringifier.FromString(s));
            }
            
            
        }

        PokerEvaluator evaluator = new();

        try
        {
            evaluator.Evaluate(players);
        }
        catch
        {
            throw;
        }
    }
}
namespace task3tests;
using task3;

using Microsoft.VisualStudio.TestTools.UnitTesting;

[TestClass]
public class WordReaderTest
{

    [TestMethod]
    [DeploymentItem(@"test1.txt", "tests")]
    public void TestReadingWords()
    {
        var reader = new WordReader(@"tests\test1.txt");

        int tokenCount = reader.Read().ToArray().Length;

        Assert.AreEqual(tokenCount, 14);
    }

    [TestMethod]
    [DeploymentItem(@"test2.txt", "tests")]
    public void TestParagraphBreak()
    {
        var reader = new WordReader(@"tests\test2.txt");

        var tokens = reader.Read().ToArray();

        Assert.AreEqual(tokens.Length, 16);
        Assert.IsTrue(tokens.ElementAt(tokens.Length - 2) is ParagraphBreakToken);
    }
}
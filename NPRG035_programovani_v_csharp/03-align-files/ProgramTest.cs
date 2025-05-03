namespace task3tests;
using task3;

using Microsoft.VisualStudio.TestTools.UnitTesting;
using task3.tests;

[TestClass]
public class ProgramTest
{

    [TestMethod]
    [DeploymentItem("base.in")]
    [DeploymentItem("base.out")]
    public void TestBase()
    {
        Program.Main(new string[] {  "base.in", "0.out", "40" });
        Assert.IsTrue(FileTools.AreEqual("0.out","base.out"));
    }

    [TestMethod]
    [DeploymentItem("01.in")]
    [DeploymentItem("ex01.out")]
    public void Test1()
    {
        Program.Main("--highlight-spaces 01.in 1.out 17".Split(" "));
        Assert.IsTrue(FileTools.AreEqual("ex01.out", "1.out"));
    }

    [TestMethod]
    [DeploymentItem("01.in")]
    [DeploymentItem(" ex02.out")]
    public void Test2()
    {
        Program.Main("--highlight-spaces 01.in 01.in 01.in 2.out 17".Split(" "));
        Assert.IsTrue(FileTools.AreEqual("ex02.out", "2.out"));
    }

    [TestMethod]
    [DeploymentItem("01.in")]
    [DeploymentItem("ex08.out")]
    public void Test3()
    {
        Program.Main("--highlight-spaces xx.in xx.in xx.in 01.in xx.in xx.in 3.out 80".Split(" "));
        Assert.IsTrue(FileTools.AreEqual("ex08.out", "3.out"));
    }

    [TestMethod]
    [DeploymentItem("01.in")]
    [DeploymentItem("ex12.out")]
    public void Test4()
    {
        Program.Main("01.in 01.in 01.in 4.out 17".Split(" "));
        Assert.IsTrue(FileTools.AreEqual("ex12.out", "4.out"));
    }

}
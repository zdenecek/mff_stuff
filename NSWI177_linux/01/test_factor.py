import unittest
from factor import primeFactors


# doesn't test file manipulation

class TestFactor(unittest.TestCase):

    def test_zero(self):
        self.assertRaises(ValueError, primeFactors, 0)

    def test_one(self):
        self.assertEqual([1], list(primeFactors(1)))

    def test_negative(self):
        self.assertRaises(ValueError, primeFactors, -20)

    def test_example(self):
        self.assertEqual([2,2,3], list(primeFactors(12)))


if __name__ == '__main__':

    unittest.main()
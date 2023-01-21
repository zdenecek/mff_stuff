You are participating in a competition where you have one hundred opponents. The competition will take place in k stages. In each stage you can eliminate some number of opponents (always at least one) and you will receive a reward for eliminating them.

The reward for eliminating 'v' out of 'p' current opponents is

`100,000 * v / p`

rounded down to the nearest integer.

For example, eliminating 50 out of 100 opponents in the first stage earns you 50,000. Eliminating 30 of 50 opponents in the second stage, you will earn 100,000 \* 30/50 = 60,000, and your total reward so far is 110,000. By eliminating the last 20 opponents in the last stage you will earn 100,000 and your total reward will be 210,000.

Write a program that calculates and prints the maximum possible reward for a given number of stages. On the second output line, write the number of opponents to eliminate in each individual stage.

**Example:**

Input:

```
3
```

Output:

```
280000
90 9 1
```
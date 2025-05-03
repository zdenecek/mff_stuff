
def pay(coins, amount, coin_memory = []):
 
 new_coins = list(coins)
 for coin in coins:
  new_amount = amount - coin
  
  
  if new_amount == 0:    
   yield coin_memory + [coin]
  elif new_amount > 0: 
   yield from pay(new_coins, new_amount, coin_memory + [coin])
  new_coins.pop(0)
  
  
input()

coins = list(map(int, input().split()))
amount = int(input())

for p in pay(coins, amount):
 print(" ".join(map(str, p)))


        
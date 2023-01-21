
def int_to_str_with_base(num, base):
    if num == 0:
        return '0'
    
    s = ''
    n = abs(num)

    while(n != 0):
        s = str(n % base) + s
        n = n // base

    s = s if num > 0 else "-" + s

    return s



inp = input().split()

n1 = int(inp[1], int(inp[0]))
n2 = int(inp[3], int(inp[2]))
p_base = int(inp[4])

to_print = [n1 + n2, n1 - n2, n1 * n2, n1 // n2]

for val in to_print:
    print(int_to_str_with_base(val, p_base))



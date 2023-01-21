def DruhaNeboPata(a,b,c,d,e,f):
    
    # v nejhorsim pripade 7
    ####################
    # Použijeme algoritmus na určení druheho nejtezsiho:
    # nalezneme nejtezsiho pomoci peti porovnani
    # na druheho nejtezsiho mame dva kandidaty, bud u a e/f, nebo z a x/y a a/b/c/d
    # celkem 6 nebo 7 porovnani, v nejhoršim případě 7, když je  u težší než z.
    #     p
    #    / \
    #    u  \
    #   / \  \
    #  /   \  \
    #  x   y   z
    # / \ / \ / \
    # a b c d e f
    # Využití možnosti vrátit i druhý nejlehčí jsem zatím nenalezl...

    # JeTezsi = lambda x, y: x > y

    vratTezsi = lambda i, j: i if JeTezsi(i ,j) else j

    x = vratTezsi(a, b)
    y = vratTezsi(c, d) 
    z = vratTezsi(e, f)
    u = vratTezsi(x, y)
    if vratTezsi(u, z) == u:
        if u == x:
            return vratTezsi(y, z)
        else: # u == y
            return vratTezsi(x, z)
    else:
        if z == e:
            return vratTezsi(f, u)
        else:
            return vratTezsi(u, e)


# print(DruhaNeboPata(*map(int, input().split())))
def DruhaNeboPata(a,b,c,d,e,f):
    
    JeTezsi = lambda x, y: x > y

    if JeTezsi(a,b):
        a,b=b,a

    if JeTezsi(c,d):
        c,d = d,c

    if JeTezsi(e,f):
        e,f = f,e

    if JeTezsi(b,d):
        b,d = d,b
        a,c = c,a

    if JeTezsi(d,f):
        if JeTezsi(b,c):
            b,c = c,b
        if JeTezsi(c,f):
            return c
        else:
            return f
    else:
        if JeTezsi(d,e):
            return d
        else:
            return e


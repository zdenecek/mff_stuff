class Prvek:
    def __init__(self, x, dalsi):
        self.x = x
        self.dalsi = dalsi

    # def __repr__(self) -> str:
    #     return str(self.x)

def VytiskniLSS( p ):
    print( "LSS:", end=" " )
    while p!=None:
        print( p.x, end=" " )
        p = p.dalsi
    print(".")

def NactiLSS():
    """cte cisla z radku, dokud nenacte prazdny radek"""
    prvni = None
    posledni = None
    r = input()
    while r!="":
        radek = r.split()
        if len(radek)==0: # protoze ten test r!="" v RCDX neukoncil cyklus!
            break
        for s in radek:
            p = Prvek(int(s),None)
            if prvni==None:
                prvni = p
            else:
                posledni.dalsi = p
            posledni = p
        r = input()
    return prvni

#################################################

def UnionDestruct(a,b):
    """ destruktivni prunik dvou usporadanych seznamu
    * nevytvari zadne nove prvky, vysledny seznam bude poskladany z prvku puvodnich seznamu,
    * vysledek je MNOZINA, takze se hodnoty neopakuji """

    def iterator(root: Prvek):
        current = root
        while current != None:
            yield current
            current = current.dalsi

    a, b = iterator(a), iterator(b)

    nxt = lambda x: next(x, None)

    current_a, current_b = nxt(a), nxt(b)

    new_root = None
    last_added = None

    while(current_a is not None or current_b is not None):
        if current_a is None:
            to_add = current_b
            current_b = nxt(b)
        elif current_b is None:
            to_add = current_a
            current_a = nxt(a)
        elif(current_a.x == current_b.x):
            to_add = current_a
            current_b = nxt(b)
            current_a = nxt(a)
        elif(current_a.x > current_b.x):
            to_add = current_b
            current_b = nxt(b)
        else:
            to_add = current_a
            current_a = nxt(a)

        ## adding
        if new_root == None:
            new_root = to_add
        else:
            last_added.dalsi = to_add
        last_added = to_add

    if last_added is not None:
        last_added.dalsi = None            



    return new_root


#################################################

VytiskniLSS( UnionDestruct( NactiLSS(), NactiLSS() ) )

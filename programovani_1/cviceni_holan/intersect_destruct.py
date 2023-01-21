class Prvek:
    def __init__(self, x, dalsi):
        self.x = x
        self.dalsi = dalsi

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

def IntersectionDestruct(a, b):
    """ destruktivni prunik dvou usporadanych seznamu
    * nevytvari zadne nove prvky, vysledny seznam bude poskladany z prvku puvodnich seznamu,
    * vysledek je MNOZINA, takze se hodnoty neopakuji """

    # sem doplnte kod funkce, dalsi casti zdrojoveho kodu NEMENTE

    # for code cleanlines we use an iterator closure and call next on it
    def iterator(root: Prvek):
        current = root
        while current != None:
            yield current
            current = current.dalsi

    a, b = iterator(a), iterator(b)

    # instead of StopIteration we can check for None
    # redefine next to return None if StopIteration should occur
    nxt = lambda x: next(x, None)

    current_a, current_b = nxt(a), nxt(b)

    new_root = None
    last_added = None

    while(current_a is not None and current_b is not None):
        if(current_a.x == current_b.x):
            if(last_added is None):
                # adding first element
                new_root = last_added = current_a
            else:
                last_added.dalsi = last_added = current_a

            # unlink our added a but move to next in a before removing reference
            current_a.dalsi, current_a  = None, nxt(a)

            current_b = nxt(b)
            
        elif(current_a.x > current_b.x):
            current_b = nxt(b)
        else:
            current_a = nxt(a)



    return new_root

#################################################

VytiskniLSS( IntersectionDestruct( NactiLSS(), NactiLSS() ) )

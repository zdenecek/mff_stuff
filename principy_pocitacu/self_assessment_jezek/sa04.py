from numpy import uint8


cislo = 5 

print(cislo, type(cislo))

cislo = 256 

print(cislo, type(cislo))

cislo = 0x4FE 

print(cislo, type(cislo))

cislo = 251 
cislo = uint8(cislo) 

print(cislo, type(cislo))

cislo = 256 
cislo = uint8(cislo) 

print(cislo, type(cislo))

cislo = 0x4FE 
cislo = uint8(cislo)

print(cislo, type(cislo))
#!/opt/local/bin/python
from random import *
from math import sqrt,pi
from sys import argv
inside=0
n=int(argv[1])
for i in range(0,n):
    x=random()
    y=random()
    if sqrt(x*x+y*y)<=1:
        inside+=1
pi_prime=4.0*inside/n
print pi_prime, pi-pi_prime
        

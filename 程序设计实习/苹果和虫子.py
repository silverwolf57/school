from math import ceil
n,x,y = input().split()
n = int(n)
x = int(x)
y = int(y)
print(n-ceil(y/x))
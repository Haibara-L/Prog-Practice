import math
if __name__=="__main__":
    n=int(input())
    sq=int(math.sqrt(n))
    row=None
    col=None
    for i in range(sq+1,0,-1):
        if n%i==0:
            row=i
            col=n//i
            break
        
    print(2*row+2*col)
    
if __name__=="__main__":
    x,y,op=input().split()
    x=int(x)
    y=int(y)
    if op=='+':
        print(x+y)
    elif op=='-':
        print(x-y)
    elif op=='*':
        print(x*y)
    elif op=='/':
        if y==0:
            print("Divided by zero!")
        else:
            print(x//y)
    else:
        print("Invalid operator!")
            
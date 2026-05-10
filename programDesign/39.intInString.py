if __name__=="__main__":
    mlist=input().split()
    res=0
    for ms in mlist:
        res+=int(ms[:2])
    print(res)
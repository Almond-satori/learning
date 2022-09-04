import os
i = 1
while i < 20000:
    print("page num:"+str(i)+"\n")
    # val = os.system('./ '+str(2000)+' '+str(i))
    val = os.system('./TLB_test '+str(i)+' '+str(2000))
    i *= 2
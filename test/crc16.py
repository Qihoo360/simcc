def crc16(x):
    b=0xA001
    a=0xFFFF
    for byte in x:
        a=a^ord(byte)
        for i in range(8):
            last=a%2
            a=a>>1
            if last==1:a=a^b
    aa='0'*(6-len(hex(a)))+hex(a)[2:]
    ll,hh=int(aa[:2],16),int(aa[2:],16)
    return hh+ll*256

print crc16('hello')
print crc16('world')

from tools import CRT, FastModExp, Fast_Power_Mod
from  Montgomery import ext_gcd
from Barrett import Barrett
from random import randint


def RSA_keygen(p: int, q: int, n: int) -> (int, int):       # 密钥生成
    e = randint(n // 5, n - 1)                              # 下界为n/5，避免小指数攻击
    ruler = (p - 1)*(q - 1)                                 # 计算欧拉函数
    while (ext_gcd(e, ruler)[2] != 1):                      # 与欧拉函数互素
        e = randint(n // 5, n - 1)
    d = ext_gcd(e, ruler)[0]                                # 计算e模欧拉函数的逆元
    if(d<0):
        d+=ruler
    return d, e


def RSA_enc(m: int, e: int, n: int) -> int:                 # 使用快速模幂求m^e mod n
    res = Fast_Power_Mod(m, e, n)
    return res


def RSA_dec(c: int, d: int, n: int) -> int:                 # 使用快速模幂求c^d mod n
    res = Fast_Power_Mod(c, d, n)
    return res


def RSA_CRT_dec(c: int, d: int, p: int, q: int, n: int) -> int:     # CRT加速解密
    c1 = Barrett(c,p)                                               # c1 = c mod p
    c2 = Barrett(c,q)                                               # c2 = c mod q
    d1 = Barrett(d,p-1)                                             # d1 = d mod p-1
    d2 = Barrett(d,q-1)                                             # d2 = d mod q-1
    m1 = Fast_Power_Mod(c1, d1, p)                                  # m1 = c1^d1 mod p
    m2 = Fast_Power_Mod(c2, d2, q)                                  # m2 = c2^d2 mod q
    res = CRT([m1, m2], [p, q])                                     # CRT求解明文
    return res


if __name__ == "__main__":

    p=161972213979815669782159457908136009507495980245895815149802492695468520680999734626875009287058908846132921794790690321279880609219355137868328489431240449437161425029639326289037965491192840352143233229633964174519251866759936198579602707223514199946490568836797528810253439384813154366574892215101215243011
    q=110339209700448177278555193422970718504194158855101845010349998392794132437936080686885135823084387607262454044374605341010745148429633319791333230371421766256391915492656510887719700519228714057862721370410510210060588348180512367030224867725988116894319202105210222986339930474972630202375232839414551798073
    n=17871886083964745021502678986694457093887098653830818474184118157747286465030641888478575717132059405120964974631241751288076540809288649503134945035530283241963398426905434659446200067456586379978492008213550075183253936808150856547928594303643713025021813920874097919776754085967497648647973964879489630668726515814295088985202269406307315939098773800525289915106982930339599765762231520309554367412795020761309089658602281747835970703670681261712039930162830178909821974492237081759305021090108738439150768068126095010541086138585039981026964534802814141945162454978110703002828473267228624393471100906605696517803

    m=10086

    print(f"p长度:{p.bit_length()}bit\nq长度:{q.bit_length()}bit\nn:长度{n.bit_length()}bit")
    print(f"加密明文为:{m}")
    print(f"生成密钥......")
    d,e=RSA_keygen(p,q,n)                           # 密钥生成
    print(f"公钥e:{e}\n私钥d:{d}")
    ciphertext=RSA_enc(m,e,n)                       # 加密
    print(f"加密结果:\n{ciphertext}")

    plaintext=RSA_dec(ciphertext,d,n)               # RSA解密
    if(plaintext==m):
        print(f"解密结果:{plaintext}")
    else:
        print("解密失败")

    plaintext=RSA_CRT_dec(ciphertext,d,p,q,n)       # RSA
    if(plaintext==m):
        print(f"CRT解密结果:{plaintext}")
    else:
        print("解密失败")


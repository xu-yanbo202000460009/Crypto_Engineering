from time import time

def ext_gcd(m: int, n: int) -> (int, int, int):  # 拓展欧几里得算法
    """
    input:
        m,n : 要求最大公因数的两个数
    output:
        x,y,gcd(m,n)满足mx+ny=gcd(m,n)
    """
    if (n == 0):  # 模数为0，最大公因数即为m
        return 1, 0, m
    x0, x1 = 1, 0  # 初始化参数
    y0, y1 = 0, 1
    r0, r1 = m, n
    while (r1 != 0):
        q = r0 // r1
        r0, r1 = r1, r0 - q * r1  # 辗转相除
        x0, x1 = x1, x0 - q * x1
        y0, y1 = y1, y0 - q * y1
    return x0, y0, r0


def reduce(t: int, r: int, n_inv: int, n: int) -> (int):    # Montgomery转换
    """
    input:
        t : t mod n in Montgomery_mod   # 需要被转换的值
        r : 2^k # 用于代替n的模数
        n_inv : n * n_inv = -1 mod n    # 模数n的逆元的相反数
        n : module  # 模数n

    output:
        REDC(T,R,N',N) = T * R' mod N  (R * R' = 1 mod n)   # Montgomery转换结果，R‘为R的逆元
    """
    if (t < 0 or t > r * n - 1):                # t属于[0,n-1]，否则需要更大的r
        return None

    mod_mask = r - 1                            # 模r取余数对应的二进制掩码，例如模8(1000)的掩码是(111)
    m = ((t & mod_mask) * n_inv) & mod_mask     # m = ((T mod R)N') mod R 取模被转化成与操作

    bitlen_r = r.bit_length() - 1               # 除8(1000)等于向右移3位
    t = (t + m * n) >> bitlen_r                 # (T+mN)/R 除法被转化成移位操作

    if (t < n):
        res = t
    else:
        res = t - n

    if (res < 0 or res > n):                    # 转换后结果还在n的剩余系中
        return None
    else:
        return res


def Montgomery_mul_mod(a: int, b: int, n: int) -> (int):    # Montgomery模乘算法
    """
    input:
            Integer a,b
            module n
            注： 实际上下面的 r2 = r * r mod n 、n的逆元的相反数n_inv都可以进行预计算，后续计算其它模n乘法则无需再算，
            这里为了说明算法每次运行函数时都重算，实际上这样做不会提升效率，因为计算r * r mod n的复杂度实际上与计算a*b mod n复杂度相同，
            因此实际使用或者测试性能时应预计算二者后作为参数传入此函数而不是在函数内部计算。
    output:
            a*b mod n
    """
    bit_len = max(a.bit_length(), b.bit_length(), n.bit_length())   # 转换后的新模式应为a、b和n三者中最大的，也可以直接用n的二进制长度
    exp_len = (bit_len + 7) // 8 * 8                                # 新模数的二进制位数

    r = 2 << exp_len     # r = 2 ** exp_len作为新模数
    r2 = r * r % n       # 虽然也是一个模乘，但由于在模数n确定后，bit_len也确定了。因此r也被确定，实际上这个值可以进行预计算，这样在后续计算模n的乘法时无需再次计算，速度会大大提升

    x, y, g = ext_gcd(r, n)     # 计算模n的逆元

    if(g!=1):           # 由于r一定为偶数(2^k)，而n一定是奇数(素数)，因此二者一定互素
        return None

    n_inv = (-y) % r    # n的逆元的相反数

    aR = reduce(a * r2, r, n_inv, n)    # aR = a * r2 * r^(-1) mod n = a*r^2*r(-1) mod n = a*r mod n
    bR = reduce(b * r2, r, n_inv, n)    # bR = b * r2 * r^(-1) mod n = b*r^2*r(-1) mod n = b*r mod n
    abR = reduce(aR * bR, r, n_inv, n)  # abR = aR*bR * r^(-1) mod n = ab*r^2*r(-1) mod n = ab*r mod n
    return reduce(abR, r, n_inv, n)     # ab = abR * r^(-1) mod n = ab*r*r(-1) mod n = ab mod n


def Montgomery_mod(a: int, n: int) -> (int):    # Montgomery算法取余
    """
    input:
            Integer a
            module n
            注：与 Montgomery_mul_mod 中相同，此处的 r1 = r mod n和 n的逆元的相反数n_inv都可以进行预计算
    output:
            a mod n in Montgomery_mod
    """

    bit_len = max(a.bit_length(), n.bit_length())   # 转换后的新模式应为a、n中最大的，也可以直接用n的二进制长度
    exp_len = (bit_len + 7) // 8 * 8    # 新模数的二进制位数

    r = 2 << exp_len                    # r = 2 ** exp_len作为新模数
    r1 = r % n                          # 虽然是对n取模，但是可以预计算

    x, y, g = ext_gcd(r, n)             # 计算模n的逆元

    if(g!=1):                           # 由于r一定为偶数(2^k)，而n一定是奇数(素数)，因此二者一定互素
        return None

    n_inv = (-y) % r                    # n的逆元的相反数

    return reduce(a * r1, r, n_inv, n)      # a mod n = a * r1 * r^(-1) mod n = a*r*r(-1) mod n = a mod n

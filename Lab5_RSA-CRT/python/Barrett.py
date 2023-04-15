def Barrett(x: int, m: int) -> (int):
    """
    input:
        x           : 需要取模的值
        m           : 模数 m
    output:
        x mod m
    """
    # 预计算值
    k = m.bit_length()              # 模数二进制位数
    new_mod = 1 << k                # 2^k
    mod_mask = (new_mod<<1) - 1     # 用于取模的掩码值，pow(2, k + 1) - 1
    mu = new_mod**2 // m            # pow(2, 2 * k) // m

    q1 = x >> (k - 1)               # x / 2^(k-1)
    q2 = q1 * mu
    q3 = q2 >> (k + 1)              # q2 / 2^(k+1)

    r1 = x & mod_mask               # x % 2^k
    r2 = (q3 * m) & mod_mask        # q3*m % 2^k
    r = r1 - r2

    if(r<0):
        r+= mod_mask + 1            # + 2^(k+1)
    while(r >= m):
        r -= m

    return r
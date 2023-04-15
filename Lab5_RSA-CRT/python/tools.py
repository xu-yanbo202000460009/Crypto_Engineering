from Barrett import Barrett
from Montgomery import Montgomery_mul_mod,ext_gcd


def CRT(list_r: list, list_m: list) -> int:                     # 中国剩余定理
    """
    input:
        list_r : 余数
        list_m : 模数
    output:
        同余式的解
    """
    list_r = [Barrett(r,m) for (r, m) in zip(list_r, list_m)]   # 计算 r%m，确保所有的r都在既约系
    res, M = 0, 1
    for m in list_m:                                            # 所有模数的乘积
        M *= m
    for (r, m) in zip(list_r, list_m):
        res = (res + M // m * ext_gcd(M // m, m)[0] * r) % M    # 计算所有的MM’r并求和,
    return res                                                  # M'为使用拓展Euclid计算的M模m的逆元


def FastModExp(a: int, p: int, k: int) -> int:                  # 分治算法计算模乘
    """
    input:
        a : 底数
        p : 指数
        k : 模数
    output:
        a^p mod k
    """
    if (p == 1):
        return Barrett(a,k)                                         # 递归出口为a%k
    elif (p % 2 == 1):
        return Barrett(FastModExp(Barrett(a,k), p - 1, k) * a,k)    # 如果是奇数则计算FastModExp(a%k, p - 1, k) * a % k
    else:
        return FastModExp(Montgomery_mul_mod(a,a,k), p // 2, k)     # 偶数则计算FastModExp((a*a%k), p // 2, k)


def Fast_Power_Mod(a: int, p: int, k: int) -> int:              # 快速幂计算模乘
    """
    input:
        a : 底数
        p : 指数
        k : 模数
    output:
        a^p mod k
    """
    res = 1
    while (p > 0):
        if (p % 2):
            res = Montgomery_mul_mod(res,a,k)                   # 奇数则模乘底数
        a = Montgomery_mul_mod(a,a,k)                           # 偶数只平方底数
        p = p >> 1
    return res

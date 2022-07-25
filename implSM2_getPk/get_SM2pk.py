from gmssl import sm2, func
import random
import math


def SM2_Mulyipoint(k, P, a, p):  # 返回值R = k*P , P为椭圆曲线上的一点 , k为正整数
    k_b = bin(k).replace('0b', '')  # 按2^i分层逐层运算
    i = len(k_b) - 1
    R = P
    if i > 0:
        k = k - 2 ** i
        while i > 0:
            R = SM2_Pluspoint(R, R, a, p)
            i -= 1
        if k > 0:
            R = SM2_Pluspoint(R, SM2_Mulyipoint(k, P, a, p), a, p)
    return R


def SM2_Pluspoint(P, Q, a, p):  # 返回值R = P+Q , P、Q为椭圆曲线上的两点 , 加法运算为定义在椭圆曲线上的加法
    if (math.isinf(P[0]) or math.isinf(P[1])) and (~math.isinf(Q[0]) and ~math.isinf(Q[1])):  # OP = P
        R = Q
    elif (~math.isinf(P[0]) and ~math.isinf(P[1])) and (math.isinf(Q[0]) or math.isinf(Q[1])):  # PO = P
        R = P
    elif (math.isinf(P[0]) or math.isinf(P[1])) and (math.isinf(Q[0]) or math.isinf(Q[1])):  # OO = O
        R = [float('inf'), float('inf')]
    else:
        if P != Q:
            l = SM2__Mod_Decimal(Q[1] - P[1], Q[0] - P[0], p)
        else:
            l = SM2__Mod_Decimal(3 * P[0] ** 2 + a, 2 * P[1], p)
        x = SM2_Mod(l ** 2 - P[0] - Q[0], p)
        y = SM2_Mod(l * (P[0] - x) - P[1], p)
        R = [x, y]
    return R


def SM2_Mod(a, b):  # 摸运算
    if math.isinf(a):
        return float('inf')
    else:
        return a % b


def SM2__Mod_Decimal(n, d, b):  # 返回值x = n*d^{-1} mod b
    if d == 0:
        x = float('inf')
    elif n == 0:
        x = 0
    else:
        a = bin(b - 2).replace('0b', '')
        y = 1
        i = 0
        while i < len(a):  # n/d = x mod b => x = n*d^(b-2) mod b
            y = (y ** 2) % b  # 快速指数运算
            if a[i] == '1':
                y = (y * d) % b
            i += 1
        x = (y * n) % b
    return x


def key_gen(a, p, n, G):  # SM2密钥对的生成

    sk = random.randint(1, n - 2)
    pk = SM2_Mulyipoint(sk, G, a, p)
    return sk, pk


def gcd(a, b):
    return a if b == 0 else gcd(b, a % b)


def get_(a, b):
    if b == 0:
        return 1, 0
    x1, y1 = get_(b, a % b)
    x, y = y1, x1 - a // b * y1
    return x, y


def get_inverse(a, p):
    if gcd(a, p) == 1:
        x, y = get_(a, p)
        return x % p
    return 1


def Legendre(n, p):  # 欧拉准则判断是否为二次剩余
    return pow(n, (p - 1) // 2, p)


def Tonelli_Shanks(n, p):  # Tonelli-Shanks算法求二次剩余
    assert Legendre(n, p) == 1
    if p % 4 == 3:
        return pow(n, (p + 1) // 4, p)
    q = p - 1
    s = 0
    while q % 2 == 0:
        q = q // 2
        s += 1
    for z in range(2, p):
        if Legendre(z, p) == p - 1:
            c = pow(z, q, p)
            break
    r = pow(n, (q + 1) // 2, p)
    t = pow(n, q, p)
    m = s
    if t % p == 1:
        return r
    else:
        i = 0
        while t % p != 1:
            temp = pow(t, 2 ** (i + 1), p)
            i += 1
            if temp % p == 1:
                b = pow(c, 2 ** (m - i - 1), p)
                r = r * b % p
                c = b * b % p
                t = t * c % p
                m = i
                i = 0
        return r


if __name__ == '__main__':
    p = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
    a = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC
    b = 0x28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93
    n = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123
    Gx = 0x32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7
    Gy = 0xBC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0
    G = [Gx, Gy]
    [sk, pk] = key_gen(a, p, n, G)
    sk_bytes = hex(sk)[2:].upper()
    pk_bytes = (hex(pk[0])[2:] + hex(pk[1])[2:]).upper()
    print("pk=", pk)

    sm2_crypt = sm2.CryptSM2(public_key=pk_bytes, private_key=sk_bytes)
    data = b"111"  # bytes类型
    random_hex_str = func.random_hex(sm2_crypt.para_len)  # 签名所需的随机数k
    sign = sm2_crypt.sign(data, random_hex_str)
    assert sm2_crypt.verify(sign, data)
    r = sign[0:64]
    s = sign[64:]
    print("r=", r)
    print("s=", s)
    r = int(r, 16)
    s = int(s, 16)
    E = data.hex()  # gmssl库中实现SM2签名时e直接由消息转化为16进制字符串
    e = int(E, 16)
    par1 = get_inverse(r + s, n)
    kG_x = SM2_Mod(r - e, n)
    temp = SM2_Mod(kG_x ** 3 + a * kG_x + b, p)
    kG_y = Tonelli_Shanks(temp, p)
    kG = [kG_x, kG_y]
    sG = SM2_Mulyipoint(s, G, a, p)
    print("kG=", kG)
    print("sG=", sG)
    _sG = [sG[0], -sG[1]]
    print("_sG=", _sG)
    par2 = SM2_Pluspoint(kG, _sG, a, p)
    PA = SM2_Mulyipoint(par1, par2, a, p)
    print("get SM2 publickey:", PA)  # 与pk比较是否一致，可能不一致，因为利用二次剩余求kG的纵坐标时求出的是两个二次剩余中随机的一个，多调试几次会有一致的时候
    if PA == pk:
        print("成功从签名得到了公钥信息")

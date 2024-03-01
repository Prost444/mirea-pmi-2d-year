import math

def GF_Sub(X, Y, P):
    res = X - Y
    return res + P if res > X else res

def GF_Add(X, Y, P):
    return GF_Sub(X, P - Y, P)

def trunc_log2(x):
    return 0 if x <= 1 else 1 + trunc_log2(x // 2)

def GF_Mul64(X, Y, P):
    BITS = trunc_log2(P) + 64
    invP2 = (2 << BITS) // P  # double the invP value
    invP = (invP2 + 1) // 2  # rounded invP value
    extra = 1 - (invP2 & 1)  # 1 if invP was rounded down, 0 otherwise

    res = X * Y
    res -= ((res + extra) * invP) >> BITS * P
    return res % P

def GF_Mul32(X, Y, P):
    estInvP = ((1 << 63) // P) << 1
    invP32 = estInvP // P if estInvP * P > (estInvP + 1) * P else (estInvP + 1) // P

    res = X * Y
    res -= ((res + (res >> 32) * invP32) >> 32) * P
    return res % P if res >= P else res

def GF_Mul(X, Y, P):
    if isinstance(X, int) and isinstance(Y, int) and isinstance(P, int):
        if P == 0x10001:
            res = X * Y
            if res == 0 and X != 0 and Y != 0:
                return 1  # 65536*65536
            res = (res & 0xFFFF) - (res >> 16)
            return res + P if res < 0 else res
        elif P == 0xFFFFFFFF:
            res = (X + Y) + ((X + Y) >> 32)
            return res % P
    return GF_Mul32(X, Y, P) if P < 2**32 else GF_Mul64(X, Y, P)

def GF_Pow(X, N, P):
    res = 1
    while N:
        if N & 1:
            res = GF_Mul(res, X, P)
        X = GF_Mul(X, X, P)
        N //= 2
    return res

def GF_Root(N, P):
    if P == 0x10001:
        main_root = 3 if P == 0x10001 else 19  # root of power P-1 in the GF(P)
        return GF_Pow(main_root, (P - 1) // N, P)
    elif P == 0xFFFFFFFF:
        main_root = 7
        return GF_Pow(main_root, 65536 // N, P)
    elif P == 0xFFFFFFFFFFFFFFFF:
        main_root = 7
        return GF_Pow(main_root, (65536 * 3 * 5 * 17449) // N, P)

def GF_Inv(X, P):
    return GF_Pow(X, 0xFFFFFFFFFFFFFFFF if P == 0xFFFFFFFFFFFFFFFF else 0xFFFFFFFF if P == 0xFFFFFFFF else P - 2, P)

def GF_Div(X, Y, P):
    InvY = GF_Inv(Y, P)
    return GF_Mul(X, InvY, P)

def GF_Normalize(X, P):
    return X % P


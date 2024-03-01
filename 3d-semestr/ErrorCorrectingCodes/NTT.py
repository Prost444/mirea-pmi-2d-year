from GF import *


def NTT2(f0, f1, P):
    u, v = f0, f1
    f0 = (u + v) % P
    f1 = (u - v) % P

def NTT3(f0, f1, f2, P, InvNTT):
    root = GF_Root(3, P) if not InvNTT else GF_Inv(GF_Root(3, P), P)
    root1 = root
    root2 = (root1 * root1) % P
    const_1 = (root1 + root2) // 2
    const_2 = (root1 - root2) // 2

    u = f1 + f2
    v = f1 - f2
    f1_f2 = u

    u = (u * const_1) % P
    v = (v * const_2) % P
    u = (f0 + u) % P

    f0 = (f0 + f1_f2) % P
    f1 = (u + v) % P
    f2 = (u - v) % P

def NTT4(f0, f1, f2, f3, P, InvNTT):
    root = GF_Root(4, P) if not InvNTT else GF_Inv(GF_Root(4, P), P)
    root1 = root

    NTT2(f0, f2, P)
    NTT2(f1, f3, P)
    f3 = (f3 * root1) % P
    NTT2(f0, f1, P)
    NTT2(f2, f3, P)
    f1, f2 = f2, f1

def NTT6(f0, f1, f2, f3, f4, f5, P, InvNTT):
    NTT3(f0, f2, f4, P, InvNTT)
    NTT3(f3, f5, f1, P, InvNTT)

    NTT2(f0, f3, P)
    NTT2(f2, f5, P)
    NTT2(f4, f1, P)

def NTT9(f0, f1, f2, f3, f4, f5, f6, f7, f8, P, InvNTT):
    root = GF_Root(9, P) if not InvNTT else GF_Inv(GF_Root(9, P), P)
    root1 = root
    root2 = (root1 * root1) % P
    root4 = (root2 * root2) % P

    NTT3(f0, f3, f6, P, InvNTT)
    NTT3(f1, f4, f7, P, InvNTT)
    NTT3(f2, f5, f8, P, InvNTT)

    f4 = (f4 * root1) % P
    f5 = (f5 * root2) % P
    f7 = (f7 * root2) % P
    f8 = (f8 * root4) % P

    NTT3(f0, f1, f2, P, InvNTT)
    NTT3(f3, f4, f5, P, InvNTT)
    NTT3(f6, f7, f8, P, InvNTT)

    f1, f3 = f3, f1
    f2, f6 = f6, f2
    f5, f7 = f7, f5

def NTT2D(data, N, SIZE, P):
    for i in range(N):
        for k in range(SIZE):
            NTT2(data[i][k], data[i + N][k], P)

def NTT3D(data, N, SIZE, P, InvNTT):
    for i in range(N):
        for k in range(SIZE):
            NTT3(data[i][k], data[i + N][k], data[i + 2 * N][k], P, InvNTT)

def NTT4D(data, N, SIZE, P, InvNTT):
    for i in range(N):
        for k in range(SIZE):
            NTT4(data[i][k], data[i + N][k], data[i + 2 * N][k], data[i + 3 * N][k], P, InvNTT)

def NTT6D(data, N, SIZE, P, InvNTT):
    for i in range(N):
        for k in range(SIZE):
            NTT6(data[i][k], data[i + N][k], data[i + 2 * N][k], data[i + 3 * N][k], data[i + 4 * N][k],
                 data[i + 5 * N][k], P, InvNTT)

def NTT9D(data, N, SIZE, P, InvNTT):
    for i in range(N):
        for k in range(SIZE):
            NTT9(data[i][k], data[i + N][k], data[i + 2 * N][k], data[i + 3 * N][k], data[i + 4 * N][k],
                 data[i + 5 * N][k], data[i + 6 * N][k], data[i + 7 * N][k], data[i + 8 * N][k], P, InvNTT)

def RecursiveNTT_Steps(data, FirstN, N, SIZE, roots, P):
    N //= 2
    if N >= FirstN:
        RecursiveNTT_Steps(data, FirstN, N, SIZE, roots + 1, P)
        RecursiveNTT_Steps(data + N, FirstN, N, SIZE, roots + 1, P)

    root = roots[0]
    root_i = 1
    for i in range(N):
        block1 = data[i]
        block2 = data[i + N]
        for k in range(SIZE):
            u = block1[k]
            v = GF_Mul(block2[k], root_i, P)
            block1[k] = GF_Add(u, v, P)
            block2[k] = GF_Sub(u, v, P)
        root_i = GF_Mul(root_i, root, P)

def IterativeNTT_Steps(data, FirstN, LastN, SIZE, root_ptr, P):
    for N in range(FirstN, LastN, FirstN):
        root = root_ptr.pop()
        for x in range(0, LastN, 2 * N):
            # first cycle optimized for root_i==1
            block1 = data[x]
            block2 = data[x + N]
            for k in range(SIZE):
                u = block1[k]
                v = block2[k]  # optimized for root_i==1
                block1[k] = GF_Add(u, v, P)
                block2[k] = GF_Sub(u, v, P)

            # remaining cycles with root_i!=1
            root_i = root  # first root of power 2N of 1
            for i in range(1, N):
                block1 = data[x + i]
                block2 = data[x + i + N]
                for k in range(SIZE):
                    u = block1[k]
                    v = GF_Mul(block2[k], root_i, P)
                    block1[k] = GF_Add(u, v, P)
                    block2[k] = GF_Sub(u, v, P)
                root_i = GF_Mul(root_i, root, P)  # next root of power 2N of 1

def revbin_permute(data, n):
    if n <= 2:
        return
    mr = 0  # the reversed 0
    for m in range(1, n):
        l = n
        while mr + l >= n:
            l >>= 1
        mr = (mr & (l - 1)) + l

        if mr > m:
            data[m], data[mr] = data[mr], data[m]

def IterativeNTT(data, N, SIZE, root_ptr, P):
    revbin_permute(data, N)
    IterativeNTT_Steps(data, 1, N, SIZE, root_ptr, P)

def TransposeMatrix(data, R, C):
    if R == C:
        for r in range(R):
            for c in range(r):
                data[r * C + c], data[c * R + r] = data[c * R + r], data[r * C + c]
    else:
        tmp = [0] * (R * C)
        for r in range(R):
            for c in range(C):
                tmp[c * R + r] = data[r * C + c]
        data[:] = tmp

def MFA_NTT(data, N, SIZE, InvNTT, P):
    L2Cache = 96 * 1024  # part of L2 cache owned by each CPU core/thread

    R = 1
    while R * R < N:
        R *= 2

    if R * SIZE * 4 > L2Cache:
        R = 1
        while R * R * R < N:
            R *= 2
    C = N // R

    root = GF_Root(N, P)
    if InvNTT:
        root = GF_Inv(root, P)
    roots = [root_i for root_i in range(root, 1, root)]
    roots.reverse()

    if N < 4 or N * SIZE * 4 < L2Cache:
        IterativeNTT(data, N, SIZE, roots, P)
        return

    TransposeMatrix(data, R, C)

    for c in range(C):
        IterativeNTT(data[c * R:(c + 1) * R], R, SIZE, roots, P)

        if c:
            root_c = pow(roots[0], c, P)
            root_rc = root_c
            for r in range(1, R):
                block = data[r + c * R]
                for k in range(SIZE):
                    block[k] = GF_Mul(block[k], root_rc, P)
                root_rc = GF_Mul(root_rc, root_c, P)

    TransposeMatrix(data, C, R)

    for i in range(0, N, C):
        if R >= C:
            IterativeNTT(data[i:i + C], C, SIZE, roots, P)
        else:
            MFA_NTT(data[i:i + C], C, SIZE, InvNTT, P)

    TransposeMatrix(data, R, C)

def Slow_NTT(data, N, SIZE, InvNTT, P):
    outdata = [[0] * SIZE for _ in range(N)]

    root = GF_Root(N, P)
    if InvNTT:
        root = GF_Inv(root, P)

    dw = 1
    for i in range(N):
        for k in range(SIZE):
            t = 0
            w = 1

            for x in range(N):
                tmp = GF_Mul(w, data[x * SIZE + k], P)
                t = GF_Add(t, tmp, P)
                w = GF_Mul(w, dw, P)

            outdata[i][k] = t

        dw = GF_Mul(dw, root, P)

    for i in range(N):
        for k in range(SIZE):
            data[i * SIZE + k] = outdata[i][k]

def Generic_NTT(data, N, SIZE, InvNTT, P):
    # R = max. 2^n power of N
    R = 1
    while R * R < N:
        R *= 2
    # C = N/R
    C = N // R

    # Employ PFA with R*C elements
    roots = [GF_Root(R * C, P) for _ in range(R * C)]
    if InvNTT:
        roots = [GF_Inv(root, P) for root in roots]

    # MFA_NTT handles R-sized NTTs
    MFA_NTT(data, R * C, SIZE, InvNTT, P)
    # PFA_NTT handles C-sized NTTs
    #PFA_NTT(data, R, C, SIZE, roots, InvNTT, P)
import numpy as np
from NTT import *

P = 0xFFF00001

def encode(N, SIZE):
    data0 = np.arange(N*SIZE) % P

    data = np.split(data0, N)

    MFA_NTT(data, N, SIZE, True, P)

    root_2N = GF_Root(2*N)
    inv_N = GF_Inv(N)
    for i in range(N):
        root_i = GF_Mul(inv_N, GF_Pow(root_2N,i))
        block = data[i]
        for k in range(SIZE):
            block[k] = GF_Mul(block[k], root_i)

    MFA_NTT(data, N, SIZE, False, P)




if __name__ == "__main__":
    N = 1<<19
    SIZE = 2052
    encode(N,SIZE)
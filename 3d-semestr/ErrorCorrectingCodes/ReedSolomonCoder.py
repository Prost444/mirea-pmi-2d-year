import array
import math
import random


################### INIT and stuff ###################

try:
    bytearray
    _bytearray = bytearray
except NameError:
    def _bytearray(obj = 0, encoding = "latin-1"):
        '''Simple pure-python bytearray replacement if not implemented'''
        # always use Latin-1 and not UTF8 because Latin-1 maps the first 256 characters to their bytevalue equivalents. UTF8 may mangle your data (particularly at vale 128)
        if isinstance(obj, str):
            obj = [ord(ch) for ch in obj.encode(encoding)]
        elif isinstance(obj, int):
            obj = [0] * obj
        return array.array("B", obj)


class ReedSolomonError(Exception):
    pass

gf_exp = _bytearray([1] * 512) 
gf_log = _bytearray(256)
field_charac = int(2**8 - 1)

def rwh_primes1(n):
    ''' Returns a list of primes < n '''
    n_half = int(n/2)
    sieve = [True] * n_half
    for i in range(3,int(math.pow(n,0.5))+1,2):
        if sieve[int(i/2)]:
            sieve[int((i*i)/2)::i] = [False] * int((n-i*i-1)/(2*i)+1)
    return array.array('i', [2] + [2*i+1 for i in range(1,n_half) if sieve[i]])

def find_prime_polys(generator=2, c_exp=8, fast_primes=False, single=False):
    '''Compute the list of prime polynomials for the given generator and galois field characteristic exponent.'''
    root_charac = 2
    field_charac = int(root_charac**c_exp - 1)
    field_charac_next = int(root_charac**(c_exp+1) - 1)

    if fast_primes:
        prim_candidates = rwh_primes1(field_charac_next)
        prim_candidates = array.array('i', [x for x in prim_candidates if x > field_charac])
    else:
        prim_candidates = array.array('i', range(field_charac+2, field_charac_next, root_charac)) 
    
    correct_primes = array.array('i', [])
    for prim in prim_candidates: 
        seen = _bytearray(field_charac+1) 
        conflict = False
        
        x = 1
        for i in range(field_charac):
            x = gf_mult_noLUT(x, generator, prim, field_charac+1)

            if x > field_charac or seen[x] == 1:
                conflict = True
                break
            else:
                seen[x] = 1

        if not conflict: 
            correct_primes.append(prim)
            if single: return array.array('i', [prim])
    return correct_primes 

def init_tables(prim=0x11d, generator=2, c_exp=8):
    global _bytearray
    if c_exp <= 8:
        _bytearray = bytearray
    else:
        def _bytearray(obj = 0, encoding = "latin-1"):
            '''Fake bytearray replacement, supporting int values above 255'''
            if isinstance(obj, str):
                obj = obj.encode(encoding)
                if isinstance(obj, str):
                    obj = [ord(chr) for chr in obj]
                elif isinstance(obj, bytes):
                    obj = [int(chr) for chr in obj]
                else:
                    raise(ValueError, "Type of object not recognized!")
            elif isinstance(obj, int):
                obj = [0] * obj
            elif isinstance(obj, bytes):
                obj = [int(b) for b in obj]
            return array.array("i", obj)

    global gf_exp, gf_log, field_charac
    field_charac = int(2**c_exp - 1)
    gf_exp = _bytearray(field_charac * 2)
    gf_log = _bytearray(field_charac+1)

    x = 1
    for i in range(field_charac):
        gf_exp[i] = x
        gf_log[x] = i
        x = gf_mult_noLUT(x, generator, prim, field_charac+1)

    for i in range(field_charac, field_charac * 2):
        gf_exp[i] = gf_exp[i - field_charac]

    return gf_log, gf_exp, field_charac

def gf_add(x, y):
    '''Add two galois field integers'''
    return x ^ y

def gf_sub(x, y):
    '''Subtract two galois field integers'''
    return x ^ y

def gf_neg(x):
    '''Negate one galois field integer (does nothing)'''
    return x

def gf_inverse(x):
    '''Inverse of a galois field integer'''
    return gf_exp[field_charac - gf_log[x]]

def gf_mul(x, y):
    '''Multiply two galois field integers'''
    if x == 0 or y == 0:
        return 0
    return gf_exp[(gf_log[x] + gf_log[y]) % field_charac]

def gf_div(x, y):
    '''Divide x by y galois field integers'''
    if y == 0:
        raise ZeroDivisionError()
    if x == 0:
        return 0
    return gf_exp[(gf_log[x] + field_charac - gf_log[y]) % field_charac]

def gf_pow(x, power):
    '''Power of x galois field integer'''
    return gf_exp[(gf_log[x] * power) % field_charac]

def gf_mult_noLUT_slow(x, y, prim=0):
    '''Standard carry-less multiplication + modular reduction using an irreducible prime polynomial.'''

    def cl_mult(x,y):
        '''Bitwise carry-less multiplication on integers'''
        z = 0
        i = 0
        while (y>>i) > 0:
            if y & (1<<i):
                z ^= x<<i
            i += 1
        return z

    def bit_length(n):
        '''Compute the position of the most significant bit (1) of an integer. Equivalent to int.bit_length()'''
        bits = 0
        while n >> bits: bits += 1
        return bits
 
    def cl_div(dividend, divisor=None):
        '''Bitwise carry-less long division on integers and returns the remainder'''
        dl1 = bit_length(dividend)
        dl2 = bit_length(divisor)
        if dl1 < dl2:
            return dividend
        for i in range(dl1-dl2,-1,-1):
            if dividend & (1 << i+dl2-1):
                dividend ^= divisor << i
        return dividend
 
    ### Main GF multiplication routine ###
 
    result = cl_mult(x,y)
    if prim > 0:
        result = cl_div(result, prim)
 
    return result

def gf_mult_noLUT(x, y, prim=0, field_charac_full=256, carryless=True):
    '''using Russian Peasant Multiplication algorithm (faster than the standard multiplication + modular reduction).'''
    r = 0
    while y:
        if y & 1: r = r ^ x if carryless else r + x
        y = y >> 1
        x = x << 1
        if prim > 0 and x & field_charac_full: x = x ^ prim

    return r


################### GALOIS FIELD POLYNOMIALS MATHS ###################

def gf_poly_scale(p, x):
    '''Scale a galois field polynomial with a factor x (an integer)'''
    out = _bytearray(len(p))
    for i in range(len(p)):
        out[i] = gf_mul(p[i], x)
    return out

def gf_poly_add(p, q):
    '''Add two galois field polynomials'''
    q_len = len(q)
    r = _bytearray( max(len(p), q_len) )
    r[len(r)-len(p):len(r)] = p
    for i in range(q_len):
        r[i + len(r) - q_len] ^= q[i]
    return r

def gf_poly_mul(p, q):
    '''Multiply two polynomials, inside Galois Field (but the procedure is generic). Optimized function by precomputation of log.'''
    r = _bytearray(len(p) + len(q) - 1)
    lp = [gf_log[p[i]] for i in range(len(p))]
    for j in range(len(q)):
        qj = q[j]
        if qj != 0:
            lq = gf_log[qj]
            for i in range(len(p)):
                if p[i] != 0:
                    r[i + j] ^= gf_exp[lp[i] + lq]
    return r

def gf_poly_mul_simple(p, q):
    '''Multiply two polynomials, inside Galois Field'''
    r = _bytearray(len(p) + len(q) - 1)
    for j in range(len(q)):
        for i in range(len(p)):
            r[i + j] ^= gf_mul(p[i], q[j]) 
    return r

def gf_poly_neg(poly):
    '''Returns the polynomial with all coefficients negated.'''
    return poly

def gf_poly_div(dividend, divisor):
    '''Fast polynomial division by using Extended Synthetic Division and optimized for GF(2^p).'''
    msg_out = _bytearray(dividend)
    divisor_len = len(divisor)
    for i in range(len(dividend) - (divisor_len-1)):
        coef = msg_out[i]
        if coef != 0:
            for j in range(1, divisor_len):
                if divisor[j] != 0:
                    msg_out[i + j] ^= gf_mul(divisor[j], coef)

    separator = -(divisor_len-1)
    return msg_out[:separator], msg_out[separator:]

def gf_poly_square(poly):
    '''Linear time implementation of polynomial squaring.'''
    length = len(poly)
    out = _bytearray(2*length - 1)
    for i in range(length-1):
        p = poly[i]
        k = 2*i
        if p != 0:
            out[k] = gf_exp[2*gf_log[p]]
    out[2*length-2] = gf_exp[2*gf_log[poly[length-1]]]
    if out[0] == 0: out[0] = 2*poly[1] - 1
    return out

def gf_poly_eval(poly, x):
    '''Evaluates a polynomial in GF(2^p) given the value for x using Horner's scheme'''
    y = poly[0]
    for i in range(1, len(poly)):
        y = gf_mul(y, x) ^ poly[i]
    return y


################### REED-SOLOMON ENCODING ###################

def rs_generator_poly(nsym, fcr=0, generator=2):
    '''Generate an irreducible generator polynomial.'''
    g = _bytearray([1])
    for i in range(nsym):
        g = gf_poly_mul(g, [1, gf_pow(generator, i+fcr)])
    return g

def rs_generator_poly_all(max_nsym, fcr=0, generator=2):
    '''Generate all irreducible generator polynomials up to max_nsym.'''
    g_all = [[1]] * max_nsym
    for nsym in range(max_nsym):
        g_all[nsym] = rs_generator_poly(nsym, fcr, generator)
    return g_all

def rs_simple_encode_msg(msg_in, nsym, fcr=0, generator=2, gen=None):
    '''Simple Reed-Solomon encoding'''
    global field_charac
    if (len(msg_in) + nsym) > field_charac: raise ValueError("Message is too long (%i when max is %i)" % (len(msg_in)+nsym, field_charac))
    if gen is None: gen = rs_generator_poly(nsym, fcr, generator)

    _, remainder = gf_poly_div(msg_in + _bytearray(len(gen)-1), gen)
    msg_out = msg_in + remainder
    return msg_out

def rs_encode_msg(msg_in, nsym, fcr=0, generator=2, gen=None):
    '''Reed-Solomon main encoding function, using polynomial division.'''
    global field_charac
    if (len(msg_in) + nsym) > field_charac: raise ValueError("Message is too long (%i when max is %i)" % (len(msg_in)+nsym, field_charac))
    if gen is None: gen = rs_generator_poly(nsym, fcr, generator)

    msg_in = _bytearray(msg_in)
    msg_out = _bytearray(msg_in) + _bytearray(len(gen)-1) 
    lgen = _bytearray([gf_log[gen[j]] for j in range(len(gen))])

    msg_in_len = len(msg_in)
    gen_len = len(gen)

    for i in range(msg_in_len):
        coef = msg_out[i]
        if coef != 0:
            lcoef = gf_log[coef]

            for j in range(1, gen_len):
                msg_out[i + j] ^= gf_exp[lcoef + lgen[j]]
    msg_out[:msg_in_len] = msg_in
    return msg_out


################### REED-SOLOMON DECODING ###################

def inverted(msg):
    return _bytearray(reversed(msg))

def rs_calc_syndromes(msg, nsym, fcr=0, generator=2):
    '''Computes the syndromes polynomial using Fourrier Transform (Chien search being the inverse).'''
    return [0] + [gf_poly_eval(msg, gf_pow(generator, i+fcr)) for i in range(nsym)]

def rs_correct_errata(msg_in, synd, err_pos, fcr=0, generator=2):
    '''Forney algorithm, computes the values (error magnitude) to correct the input message.'''
    global field_charac
    msg = _bytearray(msg_in)
    coef_pos = _bytearray(len(msg) - 1 - p for p in err_pos)
    err_loc = rs_find_errata_locator(coef_pos, generator)
    err_eval = inverted(rs_find_error_evaluator(inverted(synd), err_loc, len(err_loc)-1))

    X = _bytearray(len(coef_pos))
    for i in range(len(coef_pos)):
        l = field_charac - coef_pos[i]
        X[i] = gf_pow(generator, -l)

    E = _bytearray(len(msg)) 
    X_len = len(X)
    for i, Xi in enumerate(X):

        Xi_inv = gf_inverse(Xi)

        err_loc_prime = 1
        for j in range(X_len):
            if j != i:
                err_loc_prime = gf_mul(err_loc_prime, gf_sub(1, gf_mul(Xi_inv, X[j])))

        if err_loc_prime == 0:
            raise ReedSolomonError("Decoding failed: Forney algorithm could not properly detect where the errors are located (errata locator prime is 0).")

        y = gf_poly_eval(inverted(err_eval), Xi_inv)
        y = gf_mul(gf_pow(Xi, 1-fcr), y)
        
        magnitude = gf_div(y, err_loc_prime) 
        E[err_pos[i]] = magnitude

    msg = gf_poly_add(msg, E) 
    return msg

def rs_find_error_locator(synd, nsym, erase_loc=None, erase_count=0):
    '''Find error/errata locator and evaluator polynomials with Berlekamp-Massey algorithm'''
    if erase_loc: 
        err_loc = _bytearray(erase_loc)
        old_loc = _bytearray(erase_loc)
    else:
        err_loc = _bytearray([1])
        old_loc = _bytearray([1])
        synd_shift = 0
    if len(synd) > nsym: synd_shift = len(synd) - nsym

    for i in range(nsym-erase_count):
        if erase_loc:
            K = erase_count+i+synd_shift
        else:
            K = i+synd_shift

        delta = synd[K]
        for j in range(1, len(err_loc)):
            delta ^= gf_mul(err_loc[-(j+1)], synd[K - j])
        old_loc = old_loc + _bytearray([0])

        if delta != 0:
            if len(old_loc) > len(err_loc):
                new_loc = gf_poly_scale(old_loc, delta)
                old_loc = gf_poly_scale(err_loc, gf_inverse(delta))
                err_loc = new_loc
            err_loc = gf_poly_add(err_loc, gf_poly_scale(old_loc, delta))

    for i, x in enumerate(err_loc):  
        if x != 0:
            err_loc = err_loc[i:]
            break
    errs = len(err_loc) - 1
    if (errs-erase_count) * 2 + erase_count > nsym:
        raise ReedSolomonError("Too many errors to correct")

    return err_loc

def rs_find_errata_locator(e_pos, generator=2):
    '''Compute the errors/errata locator polynomial from the errors/errata positions.'''
    e_loc = [1]
    for i in e_pos:
        e_loc = gf_poly_mul( e_loc, gf_poly_add(_bytearray([1]), [gf_pow(generator, i), 0]) )
    return e_loc

def rs_find_error_evaluator(synd, err_loc, nsym):
    '''Compute the error polynomial from the syndrome and the error/erasures/errata locator Sigma.'''
    remainder = gf_poly_mul(synd, err_loc)
    remainder = remainder[len(remainder)-(nsym+1):]

    return remainder

def rs_find_errors(err_loc, nmess, generator=2):
    '''Find the roots by smart bruteforce trial.'''
    err_pos = []
    for i in range(nmess):
        if gf_poly_eval(err_loc, gf_pow(generator, i)) == 0:
            err_pos.append(nmess - 1 - i)
        errs = len(err_loc) - 1 
    if len(err_pos) != errs:
        raise ReedSolomonError("Too many (or few) errors found by Chien Search for the errata locator polynomial!")
    return _bytearray(err_pos)

def rs_forney_syndromes(synd, pos, nmess, generator=2):
    erase_pos_reversed = _bytearray(nmess-1-p for p in pos) 

    fsynd = _bytearray(synd[1:])
    for i in range(len(pos)):
        x = gf_pow(generator, erase_pos_reversed[i])
        for j in range(len(fsynd) - 1):
            fsynd[j] = gf_mul(fsynd[j], x) ^ fsynd[j + 1]
    return fsynd

def rs_correct_msg(msg_in, nsym, fcr=0, generator=2, erase_pos=None, only_erasures=False):
    '''Reed-Solomon main decoding function'''
    global field_charac
    if len(msg_in) > field_charac:
        raise ValueError("Message is too long (%i when max is %i)" % (len(msg_in), field_charac))

    msg_out = _bytearray(msg_in)
    if erase_pos is None:
        erase_pos = _bytearray()
    else:
        if isinstance(erase_pos, list):
            erase_pos = _bytearray(erase_pos)
        for e_pos in erase_pos:
            msg_out[e_pos] = 0
    if len(erase_pos) > nsym: raise ReedSolomonError("Too many erasures to correct")
    synd = rs_calc_syndromes(msg_out, nsym, fcr, generator)
    if max(synd) == 0:
        return msg_out[:-nsym], msg_out[-nsym:], erase_pos

    if only_erasures:
        err_pos = _bytearray()
    else:
        fsynd = rs_forney_syndromes(synd, erase_pos, len(msg_out), generator)
        err_loc = rs_find_error_locator(fsynd, nsym, erase_count=len(erase_pos))
        err_pos = rs_find_errors(inverted(err_loc), len(msg_out), generator)
        if err_pos is None:
            raise ReedSolomonError("Could not locate error")

    msg_out = rs_correct_errata(msg_out, synd, erase_pos + err_pos, fcr, generator)
    synd = rs_calc_syndromes(msg_out, nsym, fcr, generator)
    if max(synd) > 0:
        raise ReedSolomonError("Could not correct message")
    return msg_out[:-nsym], msg_out[-nsym:], erase_pos + err_pos

def rs_correct_msg_nofsynd(msg_in, nsym, fcr=0, generator=2, erase_pos=None, only_erasures=False):
    '''Reed-Solomon main decoding function, without using the modified Forney syndromes'''
    global field_charac
    if len(msg_in) > field_charac:
        raise ValueError("Message is too long (%i when max is %i)" % (len(msg_in), field_charac))

    msg_out = _bytearray(msg_in)
    if erase_pos is None:
        erase_pos = _bytearray()
    else:
        if isinstance(erase_pos, list):
            erase_pos = _bytearray(erase_pos)
        for e_pos in erase_pos:
            msg_out[e_pos] = 0
    if len(erase_pos) > nsym: raise ReedSolomonError("Too many erasures to correct")
    synd = rs_calc_syndromes(msg_out, nsym, fcr, generator)
    if max(synd) == 0:
        return msg_out[:-nsym], msg_out[-nsym:], []

    erase_loc = None
    erase_count = 0
    if erase_pos:
        erase_count = len(erase_pos)
        msg_out_len = len(msg_out)
        erase_pos_reversed = [msg_out_len-1-eras for eras in erase_pos]
        erase_loc = rs_find_errata_locator(erase_pos_reversed, generator=generator)

    if only_erasures:
        err_loc = inverted(erase_loc)
    else:
        err_loc = rs_find_error_locator(synd, nsym, erase_loc=erase_loc, erase_count=erase_count)
        err_loc = inverted(err_loc)
    
    err_pos = rs_find_errors(err_loc, len(msg_out), generator) 
    if err_pos is None:
        raise ReedSolomonError("Could not locate error")

    msg_out = rs_correct_errata(msg_out, synd, err_pos, fcr=fcr, generator=generator)
    synd = rs_calc_syndromes(msg_out, nsym, fcr, generator)
    if max(synd) > 0:
        raise ReedSolomonError("Could not correct message")
    return msg_out[:-nsym], msg_out[-nsym:], erase_pos + err_pos

def rs_check(msg, nsym, fcr=0, generator=2):
    '''Returns true if the message + ecc has no error of false otherwise'''
    return ( max(rs_calc_syndromes(msg, nsym, fcr, generator)) == 0 )


#===================================================================================================
# API
#===================================================================================================
class ReedSolomonCoder(object):
    def __init__(self, nsym=10, nsize=255, fcr=0, prim=0x11d, generator=2, c_exp=8, single_gen=True):

        if nsize > 255 and c_exp <= 8:  
            c_exp = int(math.log(2 ** (math.floor(math.log(nsize) / math.log(2)) + 1), 2))
        if c_exp != 8 and prim == 0x11d:
            prim = find_prime_polys(generator=generator, c_exp=c_exp, fast_primes=True, single=True)[0]
            if nsize == 255:
                nsize = int(2**c_exp - 1)
        if nsym >= nsize:
            raise ValueError('ECC symbols must be strictly less than the total message length (nsym < nsize).')

        self.nsym = nsym 
        self.nsize = nsize
        self.fcr = fcr
        self.prim = prim
        self.generator = generator
        self.c_exp = c_exp
        
        self.gf_log, self.gf_exp, self.field_charac = init_tables(prim, generator, c_exp)
        if single_gen:
            self.gen = {}
            self.gen[nsym] = rs_generator_poly(nsym, fcr=fcr, generator=generator)
        else:
            self.gen = rs_generator_poly_all(nsize, fcr=fcr, generator=generator)

    def chunk(self, data, chunk_size):
        '''Split a long message into chunks'''
        for i in range(0, len(data), chunk_size):
            chunk = data[i:i+chunk_size]
            yield chunk

    def encode(self, data, nsym=None):
        '''Encode a message (ie, add the ecc symbols) using Reed-Solomon.'''
        global gf_log, gf_exp, field_charac
        gf_log, gf_exp, field_charac = self.gf_log, self.gf_exp, self.field_charac
        nsize, fcr, generator = self.nsize, self.fcr, self.generator

        if not nsym:
            nsym = self.nsym
        gen = self.gen[nsym]

        if isinstance(data, str):
            data = _bytearray(data)

        chunk_size = int(nsize - nsym)
        total_chunks = int(math.ceil(float(len(data)) / float(chunk_size)))

        enc = _bytearray(total_chunks * nsize) 
        for i in range(0, total_chunks):
            enc[i*nsize:(i+1)*nsize] = rs_encode_msg(data[i*chunk_size:(i+1)*chunk_size], nsym, fcr=fcr, generator=generator, gen=gen)
        return enc

    def decode(self, data, nsym=None, erase_pos=None, only_erasures=False):
        '''Repair a message, whatever its size is, by using chunking.'''
        global gf_log, gf_exp, field_charac
        gf_log, gf_exp, field_charac = self.gf_log, self.gf_exp, self.field_charac

        if isinstance(data, str):
            data = _bytearray(data)
        if isinstance(erase_pos, list):
            erase_pos = _bytearray(erase_pos)

        if not nsym:
            nsym = self.nsym
        nsize = self.nsize
        fcr = self.fcr
        generator = self.generator

        chunk_size = nsize
        total_chunks = int(math.ceil(float(len(data)) / float(chunk_size)))
        nmes = int(nsize-nsym)

        dec = _bytearray(total_chunks * nmes)
        dec_full = _bytearray(total_chunks * nsize)
        errata_pos_all = _bytearray()
        for i in range(0, total_chunks):
            if erase_pos is not None:
                e_pos = [x for x in erase_pos if x < nsize]
                erase_pos = [x - nsize for x in erase_pos if x >= nsize]
            else:
                e_pos = _bytearray()
            rmes, recc, errata_pos = rs_correct_msg(data[i*chunk_size:(i+1)*chunk_size], nsym, fcr=fcr, generator=generator, erase_pos=e_pos, only_erasures=only_erasures)
            dec[i*nmes:(i+1)*nmes] = rmes
            dec_full[i*nsize:(i+1)*nsize] = rmes
            dec_full[i*nsize + nmes:(i+1)*nsize + nmes] = recc
            errata_pos_all.extend(errata_pos)
        return dec, dec_full, errata_pos_all

    def check(self, data, nsym=None):
        '''Check if a message+ecc stream is not corrupted.'''
        if not nsym:
            nsym = self.nsym
        if isinstance(data, str):
            data = _bytearray(data)

        nsize = self.nsize
        fcr = self.fcr
        generator = self.generator

        chunk_size = nsize
        total_chunks = int(math.ceil(float(len(data)) / float(chunk_size)))

        check = [False] * total_chunks
        for i in range(0, total_chunks):
            check[i] = rs_check(data[i*chunk_size:(i+1)*chunk_size], nsym, fcr=fcr, generator=generator)
        return check
    
    def set_errors(self, encoded, nsym=None):
        '''Randomly set errors in the encoded message.'''
        if not nsym:
            nsym = self.nsym
        if not isinstance(encoded, _bytearray):
            raise ValueError("Input must be a bytearray.")
        
        num_errors = nsym//2
        
        error_positions = random.sample(range((len(encoded)-nsym) * 8), num_errors)
        
        for pos in error_positions:
            byte_pos = pos // 8
            bit_pos = pos % 8
            encoded[byte_pos] ^= (1 << bit_pos)
        
        return encoded

    def singletest(self, inp=None):
        print("\nКодирование алгоритмом Рида-Соломона!\n")
        if inp:
            source = bytearray(inp, "latin1")
        else:
            source = bytearray(input('\nУкажите текст для кодирования/декодирования: '), "latin1")
        print('\nСтепень поля кодирования: {0}'.format(self.c_exp))
        print('\nРазрешенное число ошибок: {0}'.format(self.nsym))
        encoded = self.encode(source)
        print('\nЗакодированные данные: ', *encoded, sep='')
        decoded, _, _ = self.decode(encoded)
        print('\nРезультат декодирования: {0}'.format(decoded.decode("latin1")))
        encoded_with_error = self.set_errors(encoded)
        print('\nДопускаем ошибки в закодированных данных: ', *encoded_with_error, sep='')
        newdecoded, _, erroself = self.decode(encoded_with_error)
        print('\nДопущены ошибки в символах:', *erroself[::-1])
        print('\nРезультат декодирования ошибочных данных без исправления ошибок: {0}'.format(encoded_with_error.decode("latin1")))
        print('\nРезультат декодирования ошибочных данных с исправлением ошибок: {0}'.format(newdecoded.decode("latin1")))

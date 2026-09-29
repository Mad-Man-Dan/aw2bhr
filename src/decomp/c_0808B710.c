#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808B710.
 * sub_0808B710 @ 0x0808B710
 */

/* sinf. Argument reduction to [-pi/2, pi/2] by a round-to-nearest multiple of
 * pi computed in double, then the odd Taylor/minimax polynomial in x^2 with
 * double coefficients and a float result. The eight separate `z` terms are
 * eight separate __extendsfdf2 calls -- agbcc does not CSE a library call, so
 * the six stack doubles and `sub sp, #0x30` are a direct readout of the
 * nesting depth. The `(float)` cast on the polynomial is load-bearing: without
 * it `x * poly` is a double multiply and the tail becomes __muldf3/__truncdfsf2
 * instead of __mulsf3/__addsf3, which is 20 bytes. Coefficients are the ROM's
 * own 13-significant-digit decimals, not exact rationals. */

float sinf(float x)
{
    int n;
    float z;

    n = x / 3.141592653589793 + (x >= 0.0f ? 0.5 : -0.5);
    x = x - n * 3.141592653589793;
    if (sub_0808BB0C(x) > 1.1920929e-07)
    {
        z = x * x;
        x = x + x * (float)(z * (-0.1666666666667
              + z * (0.008333333333333
              + z * (-0.0001984126984127
              + z * (2.755731922399e-06
              + z * (-2.505210838544e-08
              + z * (1.605904383682e-10
              + z * (-7.64716373182e-13
              + z * 2.811457254346e-15))))))));
    }
    if (n & 1)
        return -x;
    return x;
}
asm(".global sub_0808B710\n.thumb_set sub_0808B710, sinf\n");

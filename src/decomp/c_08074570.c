#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074570.
 * sub_08074570 @ 0x08074570
 */

/* Walks an 8-byte-stride record array until the leading byte is 7 or 8, and
 * returns the record it stopped on -- its caller RunMapEventRecords does
 * `adds r4, r0, #0` right after the `bl`, which is what fixes the return.
 *
 * The `return p;` inside the loop is load-bearing and `break;` does NOT reach
 * this shape. With `break` the two exits stay distinct, GCC leaves the whole
 * condition in one bottom test block entered by an unconditional `b`, and the
 * function is 16 bytes; with the early `return` the in-loop exit cross-jumps
 * into the function's single tail, `duplicate_loop_exit_test` copies the
 * `*p != 7` test (load and all) into the preheader, and the ROM's 20-byte
 * guard-then-rotated-loop shape falls out. Writing the condition as
 * `while (*p != 7 && *p != 8)` instead is worse still: GCC folds it to the
 * range test `(u8)(*p - 7) <= 1` and there is only one compare left. */

u8 * SkipToMapEventFireRecord(u8 * p)
{
    while (*p != 7)
    {
        if (*p == 8)
            return p;

        p += 8;
    }

    return p;
}
asm(".global sub_08074570\n.thumb_set sub_08074570, SkipToMapEventFireRecord\n");

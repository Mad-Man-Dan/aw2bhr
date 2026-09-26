#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D40C.
 * sub_0802D40C @ 0x0802D40C, sub_0802D41C @ 0x0802D41C, sub_0802D42C @ 0x0802D42C, sub_0802D43C @ 0x0802D43C, sub_0802D44C @ 0x0802D44C
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* One of four wrappers (0802D40C/41C/42C/43C) passing consecutive ids
 * 0xC9A..0xC9D. Each is >255, so agbcc has no `movs #imm8` for it and the pool
 * word is forced by the value alone -- no symbol and no type is involved. The
 * four differ ONLY in that word, which is why they cannot say anything about
 * sub_0802D35C's parameter width; see its comment in unknown-functions.h.
 */

void sub_0802D40C(void)
{
    sub_0802D35C(0xC9A);
}

/* See sub_0802D40C: same wrapper, next id. */

void sub_0802D41C(void)
{
    sub_0802D35C(0xC9B);
}

/* See sub_0802D40C: same wrapper, next id. */

void sub_0802D42C(void)
{
    sub_0802D35C(0xC9C);
}

/* See sub_0802D40C: same wrapper, next id. */

void sub_0802D43C(void)
{
    sub_0802D35C(0xC9D);
}

/* Family F001 forwarder, 12 bytes:
 *     push {lr}
 *     bl   <callee>
 *     pop  {r0}
 *     bx   r0
 * The `pop {r0}` fixes THIS function as void: it overwrites whatever
 * the callee returned, so nothing about the callee is visible from
 * here. Everything below was read off the callee's own body instead.
 *
 * The callee reads no argument register before writing it, so there
 * is no parameter to pass through either.
 */
void sub_0802D44C(void)
{
    sub_0802D3B0();
}

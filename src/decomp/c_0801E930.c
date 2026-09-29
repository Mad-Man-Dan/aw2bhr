#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E930.
 * sub_0801E930 @ 0x0801E930, sub_0801E950 @ 0x0801E950, sub_0801E968 @ 0x0801E968
 */

/* Forwards all four of its own parameters and appends a 64-bit zero and -1.
 *
 * The two `movs #0` into r4 and r5 are NOT a missed CSE -- they are the two
 * halves of one DImode constant, which lives in a register PAIR and cannot be
 * collapsed. Declared as two `int` zeros, agbcc keeps one register and then
 * derives -1 from it as `subs r4, #1`, which is 4 bytes short of the ROM's
 * `movs #1; rsbs`. The longer sequence is the evidence: gcc only spends it
 * when no live zero is available to subtract from. */
int sub_0801E930(int a, int b, int c, int d)
{
    return QueueSpriteRequest(a, b, c, d, 0, -1);
}

/* A two-call nest, and the ABSENCE of a narrowing between the calls is the
 * whole content of it: `adds r1, r0, #0` moves GetObjectListCount's result straight
 * into HideOamObjects's second argument with no `lsls/lsrs` pair. agbcc
 * re-narrows a narrow-returning callee at EVERY call site, so that gap proves
 * GetObjectListCount returns `int` -- which is why its promoted definition in
 * src/decomp/c_0801E334.c was retyped this wave. Its own body cannot say:
 * `return *p;` on a `u16 *` is one `ldrh` either way. */
void sub_0801E950(int a, u16 *p)
{
    HideOamObjects(a, GetObjectListCount(p));
}

/* Dispatches the handler selected by the current record: looks up
 * gUnknown_03001470[i].unk26, uses that to index the 0x4c-byte
 * gUnknown_0200E438, latches the handler in gUnknown_03000550 and calls it.
 *
 * The two scalings are readable straight off the arithmetic and confirm both
 * strides independently: `lsls #1; adds; lsls #5` is *3*32 = 0x60, the size of
 * struct Unk03001470, and the explicit `muls` by 0x4c is struct Unk0200E438's.
 *
 * `bl _call_via_r2` is TWO arguments -- r0 and r1 are occupied, so r2 is the
 * first free scratch. gUnknown_0300054E is re-read for that first argument
 * rather than reused from the index computation, so it is a genuine second
 * load and the global is not const. The handler is called through the register
 * the store used, not re-loaded from gUnknown_03000550, which is ordinary
 * store-forwarding and rules volatile out. */
void sub_0801E968(void)
{
    gUnknown_03000550 = (void (*)(s16, void *))
        gUnknown_0200E438[gUnknown_03001470[gUnknown_0300054E].unk26].unk44;
    gUnknown_03000550(gUnknown_0300054E, &gUnknown_03000548);
}

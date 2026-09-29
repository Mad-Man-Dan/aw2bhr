#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035B00.
 * sub_08035B00 @ 0x08035B00, sub_08035B3C @ 0x08035B3C
 */

#include "proc.h"
/* gUnknown_0849CD88's row is nine words and this indexes it FLAT: the ROM's
 * `(unk36 * 9 + k) * 4` is one multiply, where `gUnknown_0849CD88[unk36]`
 * followed by a word subscript would compute `unk36 * 0x24` and add `k * 4`
 * separately.
 *
 * Both binding locals are load-bearing, and both are the c_080355CC.c rules:
 *  - `tbl` as its own statement, or the table's pool `ldr` lands AFTER the
 *    GetPlayerCoCountry call instead of in the callee-saved register the ROM keeps it
 *    in across the call.
 *  - `k` as its own statement, or agbcc reassociates the `- 1` into the
 *    relocation addend and emits `.word gUnknown_0849CD88-0x4`, losing the
 *    `subs r0, #1`. */
struct Unk35B3CProc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x36);
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
};

/* `* 2` and not `<< 1`: the ROM's `lsls #0x11; asrs #0x10` is the doubling
 * FUSED with the s16 return conversion, which is what shorten_binary_op does
 * for a MULT_EXPR and does not do for a shift. Both arms return, so they are
 * written in ROM-block order with the `== 1` arm second (the c_08017CF0.c
 * rule). */
s16 GetMoveSlideSpeed(u16 i)
{
    if (gPlaySt.animOpts == 1)
        return gUnknown_0849CD88[i].unk18;
    else
        return gUnknown_0849CD88[i].unk18 * 2;
}
asm(".global sub_08035B00\n.thumb_set sub_08035B00, GetMoveSlideSpeed\n");

u8 *GetMoveSlideGraphicsPointer(ProcPtr procArg)
{
    struct Unk35B3CProc *proc = procArg;
    u8 **tbl;
    int k;

    tbl = (u8 **)gUnknown_0849CD88;
    k = GetPlayerCoCountry(proc->unk37 + 1) - 1;

    return tbl[proc->unk36 * 9 + k];
}
asm(".global sub_08035B3C\n.thumb_set sub_08035B3C, GetMoveSlideGraphicsPointer\n");

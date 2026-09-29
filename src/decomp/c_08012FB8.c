#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012FB8.
 * sub_08012FB8 @ 0x08012FB8, sub_08012FE8 @ 0x08012FE8, sub_08013008 @ 0x08013008
 */

/* Creates a gUnknown_08489354 slot, runs a caller-supplied hook on it, then
 * stashes two caller words at +0x20 and +0x24. sub_08012FE8 is the reader of
 * +0x20 and calls it, so the first stashed word is itself a function pointer.
 *
 * `bl _call_via_r5` does NOT mean five arguments. The register-index rule
 * counts arguments only when gcc is free to pick a scratch register; here the
 * pointer arrives as a parameter and must survive the sub_080152EC call, so it
 * is forced into a callee-saved register and the index says nothing.
 *
 * UNPROVED, and byte-neutral either way: whether the hook takes the new slot
 * as an argument. r0 still holds sub_080152EC's result at the `bl` because
 * nothing overwrote it, which is equally consistent with `hook(p)` and with a
 * nullary `hook()`. Spelled with the argument because that is what the value
 * being live there suggests; there is no oracle for it. */
struct Unk8012FB8
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
};
/* Calls the slot's own +0x20 hook and, when it reports false, tears the slot
 * down through ClearSlotScriptCallback. The +0x20 slot is the one sub_08012FB8 fills.
 *
 * `bl _call_via_r0` is a nullary indirect call, and `lsls #0x18; cmp #0` after
 * it is the u8 truth test agbcc emits for a narrow-returning callee -- so the
 * hook returns a byte, not an int.
 *
 * gUnknown_03001FBC is a declared s16 and is read here with a bare `ldrb`:
 * that is the s16 -> u8 conversion to ClearSlotScriptCallback's declared parameter, which
 * on little-endian is just the low byte, not evidence of a second type. */
struct Unk8012FE8
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ u8 (*unk20)(void);
};
/* Not a proc: +0x24 is read with a whole-word `ldr` and called, and in
 * PROC_HEADER that offset is the s16 proc_sleepTime. So this is a plain struct
 * with a callback at +0x24.
 *
 * `bl _call_via_r0` is a NULLARY indirect call -- the trampoline's register
 * index counts the arguments, and gcc puts the pointer in the first free
 * scratch register. gUnknown_03002F1C is the one-shot latch SetSlotScriptFlag sets
 * to 1; this consumes it and clears it in the same breath. */
struct Unk08013008
{
    /* 0x00 */ u8 filler_00[0x24];
    /* 0x24 */ void (*unk24)(void);
};

void sub_08012FB8(void (*hook)(void *), void *a, void *b)
{
    struct Unk8012FB8 *p = (struct Unk8012FB8 *)sub_080152EC(gUnknown_08489354, 0);

    hook(p);

    p->unk20 = a;
    p->unk24 = b;
}

void sub_08012FE8(struct Unk8012FE8 *proc)
{
    if (!proc->unk20())
        ClearSlotScriptCallback(gUnknown_03001FBC);
}

void sub_08013008(struct Unk08013008 *p)
{
    if (gUnknown_03002F1C != 0)
    {
        gUnknown_03002F1C = 0;
        p->unk24();
    }
}

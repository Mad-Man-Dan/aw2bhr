#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080670F8.
 * sub_080670F8 @ 0x080670F8
 */

#include "hardware.h"
/* Applies a four-entry table to the four BG-control shadows through the same
 * three setters c_080688E4.c uses one at a time. The argument is declared
 * `const u8 *` in unknown-functions.h; the accesses are `ldr` at +0, +4 and
 * +0xc of a 0x10-byte stride, so the real shape is a four-element array of
 * 16-byte records and the byte pointer is the caller-side spelling of it.
 * Nothing reaches +8, which stays filler.
 *
 * The shadows are visited BG0..BG3 -- gUnknown_03002B6C, gUnknown_03001FE8,
 * gUnknown_030030B4, gUnknown_0300251C -- each parked in r4 across its own
 * three calls, so each address is one pool word with three uses. */

struct Unk80670F8
{
    /* 0x00 */ u32 unk00;
    /* 0x04 */ u32 unk04;
    /* 0x08 */ u32 filler_08;
    /* 0x0c */ u32 unk0c;
};

void ApplyBgControlTable(const u8 * a1)
{
    const struct Unk80670F8 * cfg = (const struct Unk80670F8 *)a1;

    SetBgCntChrBlock((struct Unk8012C30 *)&gUnknown_03002B6C, cfg[0].unk00);
    SetBgCntTilemapBlock((struct Unk8012C30 *)&gUnknown_03002B6C, cfg[0].unk04);
    SetBgCntScreenSize((struct Unk8012C30 *)&gUnknown_03002B6C, cfg[0].unk0c);

    SetBgCntChrBlock((struct Unk8012C30 *)&gUnknown_03001FE8, cfg[1].unk00);
    SetBgCntTilemapBlock((struct Unk8012C30 *)&gUnknown_03001FE8, cfg[1].unk04);
    SetBgCntScreenSize((struct Unk8012C30 *)&gUnknown_03001FE8, cfg[1].unk0c);

    SetBgCntChrBlock((struct Unk8012C30 *)&gUnknown_030030B4, cfg[2].unk00);
    SetBgCntTilemapBlock((struct Unk8012C30 *)&gUnknown_030030B4, cfg[2].unk04);
    SetBgCntScreenSize((struct Unk8012C30 *)&gUnknown_030030B4, cfg[2].unk0c);

    SetBgCntChrBlock((struct Unk8012C30 *)&gUnknown_0300251C, cfg[3].unk00);
    SetBgCntTilemapBlock((struct Unk8012C30 *)&gUnknown_0300251C, cfg[3].unk04);
    SetBgCntScreenSize((struct Unk8012C30 *)&gUnknown_0300251C, cfg[3].unk0c);
}
asm(".global sub_080670F8\n.thumb_set sub_080670F8, ApplyBgControlTable\n");

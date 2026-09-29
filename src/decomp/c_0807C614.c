#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807C614.
 * sub_0807C614 @ 0x0807C614
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
/* WAVE 53 (W53-C). MATCHED. Needs "rodata": ["0x081D933C"] in its
 * data/promoted.json entry.
 *
 * A screen setup, and its second half is one repeated expression: the byte
 * `gUnknown_030058E0[DivRem(unk52 + k, gUnknown_03005948[unk58]) + unk5c]` for
 * k = 0, 1, 2, handed to four different consumers.
 *
 * gUnknown_081D933C IS NOT A GLOBAL. It is the -fforce-addr .rodata word this
 * unit emits for &gUnknown_030058E0, and the ROM reaches the array through it
 * exactly as agbcc does here: `ldr rN, =<pool word>` once in the CpuFastSet
 * loop's preheader (an LICM hoist -- it is NOT authored, and trying to spell it
 * costs an attempt), then `ldr rM, [rN]` at each of the three references.
 * Spelling it as a `u8 *` variable adds a third `ldr` at every use. See the
 * note in include/unknown-globals.h.
 *
 * `movs r0, #0x20; rsbs r0, r0, #0; ands` is ~0x1F, a bitfield CLEAR of
 * BgCnt.tm_block (5 bits at bit 8 = the low 5 bits of byte 1), not an
 * arithmetic negate; `movs #0xd; rsbs` is the same trick for ~0x0C =
 * chr_block. The clearing half is the only thing that discriminates a bitfield
 * from a scalar `|=`, and both appear here.
 *
 * The CpuSet fill source is an ARRAY, not a `u16` scalar, and that is a real
 * byte-level distinction rather than a style choice. With `u16 c; CpuSet(&c,
 * ...)` agbcc puts the address in r0 and the stored 0 in r1, r0 then survives
 * to the call and the `mov r0, sp` in front of `bl CpuSet` disappears -- two
 * bytes, which also drops the pool's alignment pad, for four. With an array
 * the roles swap to `mov r1, sp; movs r0, #0` and the argument copy comes
 * back. Only "array" is proved; the extent is not.
 *
 * IsBlockWarRoomSelectionActive returns `int` (src/decomp/c_0807F8E4.c). The `lsls #24;
 * lsrs #24` after the `bl` is this caller storing the result in a u8, which is
 * the reading include/unknown-functions.h already records for sub_0807C9EC.
 *
 * ApplyPaletteExt's first argument really is the literal 8 at the third call.
 * That is what the ROM sets up (`movs r0, #8` with no preceding producer of
 * r0), it is byte-exact, and it is left as a cast rather than retyping a
 * prototype that ~40 matched call sites agree on. Flagged as not understood.
 */

struct Unk807C614
{
    /* 00 */ STRUCT_PAD(0x00, 0x52);
    /* 52 */ u16 unk52;
    /* 54 */ STRUCT_PAD(0x54, 0x58);
    /* 58 */ int unk58;
    /* 5c */ int unk5c;
    /* 60 */ STRUCT_PAD(0x60, 0x64);
    /* 64 */ s16 unk64;
};

void WarRoomMapSelected_0807C615(struct Unk807C614 *proc)
{
    int i;
    u8 v;
    u16 clear[2];

    SetupMenuScreenBgs(proc);
    SyncAllBgTilemaps();

    gUnknown_0300251C.bits.tm_block = 0x1b;

    StartScrollingBackdrop(proc);

    gUnknown_03001FE8.bits.size = 2;
    gUnknown_03001FE8.bits.chr_block = 1;

    gUnknown_03002F18 = 0xff38;

    clear[0] = 0;
    CpuSet(clear, (void *)((gUnknown_03001FE8.bits.tm_block << 11) + 0x06000000), 0x01000800);

    Decompress(gUnknown_0823468C, (void *)((gUnknown_03001FE8.bits.chr_block << 14) + 0x06000000));
    Decompress(gUnknown_0823456C, gBG1TilemapBuffer);
    ApplyPaletteExt((u16 *)8, 0, 0x20);

    BG_EnableSyncBG1();

    Decompress(gUnknown_082346D0, gUnknown_0200FC50);

    for (i = 0; i < 8; i++)
        CpuFastSet(gUnknown_0200FC50 + i * 0x400, (void *)(0x06010000 + i * 0x100), 0x40);

    ApplyPaletteExt(gUnknown_08234AD0, 0x200, 0x20);

    sub_0801F114();
    InitTilePool(1, (void *)0x06010000, 0x3e4, 0x1c);
    InitTilePool(2, (void *)0x06010000, 0x344, 0x1d);

    LoadTilePoolGraphic(0x3e);
    LoadTilePoolGraphic(0x3f);
    LoadTilePoolGraphic(0x40);
    LoadTilePoolGraphic(0x41);
    LoadTilePoolGraphic(0x42);
    LoadTilePoolGraphic(0x43);
    LoadTilePoolGraphic(0x44);
    LoadTilePoolGraphic(0x48);
    LoadTilePoolGraphic(0x49);
    LoadTilePoolGraphic(0x4a);
    LoadTilePoolGraphic(0x4b);
    LoadTilePoolGraphic(0x4c);
    LoadTilePoolGraphic(0x4d);
    LoadTilePoolGraphic(0x54);
    LoadTilePoolGraphic(0x56);
    LoadTilePoolGraphic(0x57);
    LoadTilePoolGraphic(0x58);
    LoadTilePoolGraphic(0x5f);
    LoadTilePoolGraphic(0x4f);
    LoadTilePoolGraphic(0x67);
    LoadTilePoolGraphic(0x62);
    LoadTilePoolGraphic(0x65);

    LoadBg1WindowFrame(1);

    ApplyWindowFramePalette(gUnknown_03005958[proc->unk58], 8);

    v = IsBlockWarRoomSelectionActive();

    if (v != 0)
    {
        for (i = 0; i < proc->unk64; i++)
            LoadCoNameGraphic(gUnknown_030058D4[i], i * 0xc + 0x3c0);

        LoadCoNameGraphic(gUnknown_030058E0[DivRem(proc->unk52, gUnknown_03005948[proc->unk58]) + proc->unk5c],
                     proc->unk64 * 0xc + 0x3c0);
    }
    else
    {
        proc->unk52 = 0;
        LoadCoNameGraphic(gUnknown_030058E0[0], 0x3c0);
    }

    LoadCoFullBodyAndPalette(gUnknown_030058E0[DivRem(proc->unk52, gUnknown_03005948[proc->unk58]) + proc->unk5c], 0x40, 1);
    LoadCoFace(gUnknown_030058E0[DivRem(proc->unk52, gUnknown_03005948[proc->unk58]) + proc->unk5c], (void *)0x06013000, 0x12);
    LoadCoFace(gUnknown_030058E0[DivRem(proc->unk52 + 1, gUnknown_03005948[proc->unk58]) + proc->unk5c], (void *)0x06013480, 0x13);
    LoadCoFace(gUnknown_030058E0[DivRem(proc->unk52 + 2, gUnknown_03005948[proc->unk58]) + proc->unk5c], (void *)0x06013900, 0x14);
    LoadCoFace(gUnknown_030058E0[DivRem(proc->unk52, gUnknown_03005948[proc->unk58]) + proc->unk5c], (void *)0x06014200, 0x16);
    LoadCoFace(gUnknown_030058E0[DivRem(proc->unk52 + 1, gUnknown_03005948[proc->unk58]) + proc->unk5c], (void *)0x06014680, 0x17);
    LoadCoFace(gUnknown_030058E0[DivRem(proc->unk52 + 2, gUnknown_03005948[proc->unk58]) + proc->unk5c], (void *)0x06014B00, 0x18);
}

asm(".global sub_0807C614\n.thumb_set sub_0807C614, WarRoomMapSelected_0807C615\n");

#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080355CC.
 * sub_080355CC @ 0x080355CC
 */

#include "proc.h"
/* Shares its proc with sub_080359A4, which is handed the same pointer and
 * reads +0x30, +0x35, +0x42 and +0x44 out of it. +0x42/+0x44 are SIGNED --
 * sub_080359A4 reads both with `ldrsh` -- and the `strh` here is identical
 * either way.
 *
 * Three spellings are load-bearing:
 *   - `GetMoveSlidePaletteRow(...) * 0x1000` and not `<< 12`. The shift lets combine
 *     drop the s16 return's sign extension (the result is truncated by the
 *     `strh` anyway) and comes out `lsls #12`; the multiply keeps it and gives
 *     the ROM's `lsls #0x10; asrs #4`.
 *   - `tbl = gUnknown_0810EA60;` as its own statement. The palette base has to
 *     be materialised BEFORE the gPlayers index chain, which is where
 *     agbcc puts it only when it is a statement of its own.
 *   - `pal = ... - 1;` as its own statement, and gUnknown_0810EA60 declared
 *     `[][16]`. Fold the `- 1` into the subscript and agbcc reassociates it
 *     into the relocation addend, losing the `subs r4, #1`.
 */
struct Unk355CCProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ struct Unk0801C210 *unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x01];
    /* 35 */ u8 unk35;
    /* 36 */ u8 unk36;
    /* 37 */ u8 unk37;
    /* 38 */ s8 unk38;
    /* 39 */ u8 unk39;
    /* 3a */ s8 unk3a;
    /* 3b */ u8 filler_3b[0x03];
    /* 3e */ u16 unk3e;
    /* 40 */ u16 unk40;
    /* 42 */ s16 unk42;
    /* 44 */ s16 unk44;
    /* 46 */ u8 filler_46[0x02];
    /* 48 */ void *unk48;
};

ProcPtr CreateMoveSlide(u16 x, u16 y, u16 a3, u16 a4)
{
    struct Unk355CCProc *proc;
    void *gfx;
    int slot;
    int pal;
    u16 (*tbl)[16];

    slot = FindFreeMoveSlideSlot();
    if (slot == -1)
        return NULL;

    gUnknown_03003124[slot] = 1;

    gfx = GetUnitSpriteFormat(a4);
    proc = Proc_Start(ProcScr_SelectUnit, PROC_TREE_5);
    proc->unk2c = AP_Create(gfx, 2, 1);
    proc->unk3a = slot;
    proc->unk48 = AllocMoveSlideGfxBuffer();
    proc->unk30 = 0;
    proc->unk35 = 1;
    proc->unk42 = x << 4;
    proc->unk44 = y << 4;

    if (a3 == 0 || a3 == 2)
        proc->unk38 = 1;
    else
        proc->unk38 = 0;

    proc->unk3e = 0;
    proc->unk39 = 0;
    proc->unk36 = a4;
    proc->unk40 = GetMoveSlideSpeed(proc->unk36);
    proc->unk37 = a3;

    AP_SwitchAnimation(proc->unk2c, proc->unk38);
    proc->unk2c->unk24 = proc->unk48;
    proc->unk2c->unk22 = (proc->unk3a * 9 + 0x29A) | (GetMoveSlidePaletteRow(proc->unk3a) * 0x1000);

    tbl = gUnknown_0810EA60;
    pal = gPlayers[proc->unk37 + 1].teamColor - 1;
    ApplyPaletteExt(tbl[pal],
                    (u16)(0x20 * (GetMoveSlidePaletteRow(proc->unk3a) + 0x10)), 0x20);

    sub_080359A4(proc);
    Decompress(GetMoveSlideGraphicsPointer(proc), proc->unk48);
    return proc;
}
asm(".global sub_080355CC\n.thumb_set sub_080355CC, CreateMoveSlide\n");

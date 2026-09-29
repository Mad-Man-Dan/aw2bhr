#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078D40.
 * sub_08078D40 @ 0x08078D40, sub_08078D80 @ 0x08078D80
 */

#include "hardware.h"
#include "proc.h"
/* A fade-in that steps on every OTHER frame: a private frame counter at +0x68
 * gates the blend coefficient through DivRem(counter, 2).
 *
 * gUnknown_081D92E0 is NOT a global -- it is a third `-fforce-addr`
 * address-constant word holding 0x03001FFC, like 081D92D8/DC on FadeOutBrightness_Loop
 * and FadeInBrightness_Loop. The global is gUnknown_03001FFC, the volatile BLDY shadow.
 * Needs "rodata": ["0x081D92E0"] in promoted.json.
 *
 * +0x68 is `s16`: the increment is `ldrh`/`strh`, but the value handed to
 * DivRem is re-read `ldrsh` off a zero index register, which is a SIGNED
 * halfword load and not something a `u16` field produces. */

struct Unk8078D40
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk_64;
    /* 0x66 */ u8 filler_66[0x02];
    /* 0x68 */ s16 unk_68;
};

void FadeToBlackFrom6_Loop(struct Unk8078D40 *proc)
{
    proc->unk_68++;

    if (DivRem(proc->unk_68, 2))
        gUnknown_03001FFC++;

    if (gUnknown_03001FFC == 0x10)
        Proc_Break(proc);
}
asm(".global sub_08078D40\n.thumb_set sub_08078D40, FadeToBlackFrom6_Loop\n");

/* A screen setup. The first Decompress destination is the BG char-base idiom
 * already written out verbatim in src/decomp/c_08065990.c line for line --
 * `(gUnknown_0300251C.bits.chr_block << 14) + 0x06000000`, where the
 * `lsls #0x1c; lsrs #0x1e` pair is the bitfield read and `movs #0xc0;
 * lsls #0x13` is 0x06000000. That file decompresses the SAME blob
 * (gUnknown_0822FEF0) to the same place, so this is one shared statement and
 * not a rederivation. */

void StartScrollingBackdrop(ProcPtr parent)
{
    Decompress(gUnknown_0822FEF0, (void *)((gUnknown_0300251C.bits.chr_block << 14) + 0x06000000));
    Decompress(gUnknown_0822F9AC, gBG3TilemapBuffer);
    BG_EnableSyncBG3();
    ApplyPaletteExt(gUnknown_082344CC, 0x20, 0xa0);
    Proc_Start(gUnknown_08615CA0, parent);
}
asm(".global sub_08078D80\n.thumb_set sub_08078D80, StartScrollingBackdrop\n");

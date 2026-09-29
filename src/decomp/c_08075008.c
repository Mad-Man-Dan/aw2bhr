#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075008.
 * sub_08075008 @ 0x08075008, sub_08075058 @ 0x08075058
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* The +0x34 counter is UNSIGNED -- the ROM shifts it with `lsrs`, and an `s32`
 * field gives `asrs` for the same `>> 3`.
 *
 * The jitter lookup MUST be its own local. Left inline in PutSprite's third
 * argument agbcc emits the two coordinate `ldrsh`s first and the table `ldr`
 * after them; bound to `d` the table address loads first and the two `ldrsh`s
 * fall into r2/r3 rather than reusing r0, exactly as the ROM has it. Same
 * lever as c_080763C0.c's `src`. */
struct Unk8075008
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ s16 unk2c;
    /* 0x2e */ s16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 filler_32[0x2];
    /* 0x34 */ u32 unk34;
};
#include "proc.h"
/* The per-entry child spawner of the ProcScr_WM_MoveScope menu proc: sub_0807519C
 * calls this once per frame-group and keeps the returned proc in the ten-entry
 * pointer table at +0x3c of its own proc, which is why this starter RETURNS the
 * proc where the rest of the family drops it. The parameter widths are read off
 * that call site rather than off this body -- agbcc's PROMOTE_MODE zero-extends
 * every sub-word parameter, so the four uniform `lsls #0x10; lsrs #0x10` pairs
 * here carry no signedness at all; the caller's `asrs` on arguments 2 and 3 is
 * what makes them s16. */
struct Unk08075058
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 filler_32[0x2];
    /* 0x34 */ int unk34;
};

void WM_DrawDifficultyStars_IDLE_08075009(struct Unk8075008 *proc)
{
    int d = gUnknown_0861436C[(proc->unk34 >> 3) & 3];

    PutSprite(1, proc->unk2c, proc->unk2e + d,
              gUnknown_081CC4E8, gUnknown_0861433C[proc->unk30]);
    proc->unk34++;
}

void *StartDifficultyStar(ProcPtr parent, u16 a2, s16 a3, s16 a4, u16 a5)
{
    struct Unk08075058 *proc = Proc_Start(ProcScr_WM_DrawDifficultyStars, parent);

    proc->unk2c = a3;
    proc->unk2e = a4;
    proc->unk2a = a2;
    proc->unk30 = a5;
    proc->unk34 = 0;

    return proc;
}
asm(".global sub_08075058\n.thumb_set sub_08075058, StartDifficultyStar\n");

asm(".global sub_08075008\n.thumb_set sub_08075008, WM_DrawDifficultyStars_IDLE_08075009\n");

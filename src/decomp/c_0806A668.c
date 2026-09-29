#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A668.
 * sub_0806A668 @ 0x0806A668, sub_0806A680 @ 0x0806A680, sub_0806A6B8 @ 0x0806A6B8
 */

struct Unk806A668
{
    /* 0x00 */ u8 filler_00[0x44];
    /* 0x44 */ u16 unk44;
};
#include "proc.h"
struct Unk806A680
{
    /* 0x00 */ u8 filler_00[0x44];
    /* 0x44 */ u16 unk44;
};
struct Unk806A6B8
{
    /* 0x00 */ u8 filler_00[0x44];
    /* 0x44 */ u16 unk44;
};

/* Loads the frame budget the MeteorImpactFlash_RedTintLoop / MeteorImpactFlash_FadeBackLoop loops count down. */
void MeteorImpactFlash_Init(struct Unk806A668 *proc)
{
    ColFadeToWhite(1);
    proc->unk44 = 8;
}
asm(".global sub_0806A668\n.thumb_set sub_0806A668, MeteorImpactFlash_Init\n");

/* Steps the palette ramp once per unpaused frame. The `lsls #0x10; cmp #0`
 * after the store is the u16 counter being tested at its own width, i.e. the
 * decrement and the test are one expression. */
void MeteorImpactFlash_RedTintLoop(struct Unk806A680 *proc)
{
    if (!(gGameClock & 1))
    {
        StepPaletteRedTint();
        if (--proc->unk44 == 0)
        {
            proc->unk44 = 0x20;
            Proc_Break(proc);
        }
    }
}
asm(".global sub_0806A680\n.thumb_set sub_0806A680, MeteorImpactFlash_RedTintLoop\n");

/* The MeteorImpactFlash_RedTintLoop twin, with a different pair of per-frame calls and no
 * counter reload before the break. */
void MeteorImpactFlash_FadeBackLoop(struct Unk806A6B8 *proc)
{
    if (!(gGameClock & 1))
    {
        sub_080718F0();
        EnablePaletteSync();
        if (--proc->unk44 == 0)
            Proc_Break(proc);
    }
}
asm(".global sub_0806A6B8\n.thumb_set sub_0806A6B8, MeteorImpactFlash_FadeBackLoop\n");

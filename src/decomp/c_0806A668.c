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

/* Loads the frame budget the sub_0806A680 / sub_0806A6B8 loops count down. */
void sub_0806A668(struct Unk806A668 *proc)
{
    ColFadeToWhite(1);
    proc->unk44 = 8;
}

/* Steps the palette ramp once per unpaused frame. The `lsls #0x10; cmp #0`
 * after the store is the u16 counter being tested at its own width, i.e. the
 * decrement and the test are one expression. */
void sub_0806A680(struct Unk806A680 *proc)
{
    if (!(gGameClock & 1))
    {
        sub_0806A5B8();
        if (--proc->unk44 == 0)
        {
            proc->unk44 = 0x20;
            Proc_Break(proc);
        }
    }
}

/* The sub_0806A680 twin, with a different pair of per-frame calls and no
 * counter reload before the break. */
void sub_0806A6B8(struct Unk806A6B8 *proc)
{
    if (!(gGameClock & 1))
    {
        sub_080718F0();
        EnablePaletteSync();
        if (--proc->unk44 == 0)
            Proc_Break(proc);
    }
}

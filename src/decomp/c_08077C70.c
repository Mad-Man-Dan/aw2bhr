#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08077C70.
 * sub_08077C70 @ 0x08077C70
 */

#include "proc.h"
#include "hardware.h"
/* Keep the scroll in step, and break the proc on ANY key -- the test is
 * `gpKeySt->pressed != 0` against the whole mask, not a single bit, which is the
 * "press anything to continue" shape. */
struct Unk77C70Proc
{
    /* 00 */ u8 filler_00[0x44];
    /* 44 */ int unk44;
};

void sub_08077C70(struct Unk77C70Proc *proc)
{
    sub_08077620(0, 0xA8 - gUnknown_0300064C);

    if (gpKeySt->pressed != 0)
    {
        proc->unk44 = 0;
        PlayMusicOrSfx2(0x66);
        Proc_Break(proc);
    }
}

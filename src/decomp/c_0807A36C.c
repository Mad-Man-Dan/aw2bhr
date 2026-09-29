#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807A36C.
 * sub_0807A36C @ 0x0807A36C
 */

#include "proc.h"
#include "hardware.h"

/* Draw, then start a child proc while A is held. sub_08043C28's fifth argument
 * is a u8 on the stack, which is what the `sub sp, #4` pays for; the other four
 * are literals, and 0xB000 is `movs #0xb0; lsls #8`.
 *
 * `gpKeySt->pressed & 1` and not `pressed`: the read is `ldrh [r0, #4]`, and +4 is
 * the held mask. */
void ResultsScreen_WaitForClose_Loop(ProcPtr proc)
{
    sub_08043C28(0x40, 0xA0, 0xB000, 0, 0);

    if (gpKeySt->pressed & 1)
        Proc_Start(gUnknown_08615E08, proc);
}
asm(".global sub_0807A36C\n.thumb_set sub_0807A36C, ResultsScreen_WaitForClose_Loop\n");

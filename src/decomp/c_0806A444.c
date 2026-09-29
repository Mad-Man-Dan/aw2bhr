#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A444.
 * sub_0806A444 @ 0x0806A444
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Breaks every proc running ProcScr_IntroT0. Proc_BreakEachOpenCoded is Proc_BreakEach
 * open-coded over sProcArray rather than the proc tree; `pop {r0}; bx r0` makes
 * this void, so the call is a bare statement.
 */

void IntroT3_0806A445(void)
{
    Proc_BreakEachOpenCoded(ProcScr_IntroT0);
}

asm(".global sub_0806A444\n.thumb_set sub_0806A444, IntroT3_0806A445\n");

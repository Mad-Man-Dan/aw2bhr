#include "global.h"
#include "proc.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080358AC.
 * sub_080358AC @ 0x080358AC, sub_080358B8 @ 0x080358B8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* Family F001 forwarder, 12 bytes:
 *     push {lr}
 *     bl   <callee>
 *     pop  {r0}
 *     bx   r0
 * The `pop {r0}` fixes THIS function as void: it overwrites whatever
 * the callee returned, so nothing about the callee is visible from
 * here. Everything below was read off the callee's own body instead.
 *
 * One pointer passed through. StartMoveSlideMoveSound reads r0 first thing
 * (`adds r4, r0, #0`) and dereferences +0x36, past PROC_HEADER's 0x29
 * bytes; its other caller BeginMoveSlidePath treats the same object as a proc.
 */
void DesignRoomPlaceUnit_080358AD(ProcPtr proc)
{
    StartMoveSlideMoveSound(proc);
}

/* Family F001 forwarder, 12 bytes:
 *     push {lr}
 *     bl   <callee>
 *     pop  {r0}
 *     bx   r0
 * The `pop {r0}` fixes THIS function as void: it overwrites whatever
 * the callee returned, so nothing about the callee is visible from
 * here. Everything below was read off the callee's own body instead.
 *
 * One pointer passed through, same shape as DesignRoomPlaceUnit_080358AD twelve bytes above:
 * PlayMoveSlideStopSfxNoStep opens `adds r3, r0, #0` and reads +0x36.
 */
void DesignRoomPlaceUnit_CB_080358B9(ProcPtr proc)
{
    PlayMoveSlideStopSfxNoStep(proc);
}

asm(".global sub_080358B8\n.thumb_set sub_080358B8, DesignRoomPlaceUnit_CB_080358B9\n"
    ".global sub_080358AC\n.thumb_set sub_080358AC, DesignRoomPlaceUnit_080358AD\n");

struct ProcCmd CONST_DATA ProcScr_DesignRoomPlaceUnit[] =
{
    PROC_ONEND(DesignRoomPlaceUnit_CB_080358B9),
    PROC_SLEEP(1),
    PROC_CALL(DesignRoomPlaceUnit_080358AD),
    PROC_SLEEP(8),
    PROC_END,
};

asm(".global gUnknown_0849BDB8\n.set gUnknown_0849BDB8, ProcScr_DesignRoomPlaceUnit\n");

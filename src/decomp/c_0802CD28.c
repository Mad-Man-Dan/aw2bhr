#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802CD28.
 * sub_0802CD28 @ 0x0802CD28, sub_0802CD54 @ 0x0802CD54, sub_0802CD78 @ 0x0802CD78
 */

#include "proc.h"

/* IntelMenu_Status with a Proc_Start instead of the fourth teardown call. See that
 * function for why the second parameter is u8 -- the word `str` into
 * gUnknown_030040F0 is what makes the PROMOTE_MODE shift pair live.
 *
 * `movs r1, #3` is PROC_TREE_3 passed as the parent, the root-index convention
 * src/proc.c documents, and the returned proc is discarded.
 */

void MapMenu_Co(int a1, u8 a2)
{
    gUnknown_030040F0 = a2;
    PushMenu();
    CloseTopMenu();
    SetMapStateResumeCursor();
    Proc_Start(gUnknown_0849AA68, PROC_TREE_3);
}
asm(".global sub_0802CD28\n.thumb_set sub_0802CD28, MapMenu_Co\n");

/* Stash the u8 second argument in the word global gUnknown_030040F0, then run
 * four teardown calls. MapMenu_Co and IntelMenu_Rules are the same function with
 * a different fifth statement.
 *
 * This is the discriminating case for the parameter width: the argument's only
 * use is a WORD `str`, so PROMOTE_MODE's `lsls r1,#0x18; lsrs r1,#0x18` is live
 * and appears. An `int` parameter would store r1 directly. Contrast
 * sub_080344F0, whose u8-or-int argument feeds a `strb` and so cannot be told
 * apart at all.
 *
 * gUnknown_030040F0 is already declared u32 and the `str` agrees. The first
 * parameter is never read; its count is a floor taken from the second one's
 * register index.
 */

void IntelMenu_Status(int a1, u8 a2)
{
    gUnknown_030040F0 = a2;
    PushMenu();
    CloseTopMenu();
    SetMapStateResumeCursor();
    StartIntelStatusScreen();
}
asm(".global sub_0802CD54\n.thumb_set sub_0802CD54, IntelMenu_Status\n");

/* IntelMenu_Status with a sub_080152EC install instead of the fourth teardown call,
 * carrying the literal 0 second argument every other sub_080152EC wrapper in
 * this block passes. See IntelMenu_Status for why the second parameter is u8.
 *
 * sub_080152EC's returned slot is discarded, so the call is a bare statement.
 */

void IntelMenu_Rules(int a1, u8 a2)
{
    gUnknown_030040F0 = a2;
    PushMenu();
    CloseTopMenu();
    SetMapStateResumeCursor();
    sub_080152EC(gUnknown_0849ADD0, 0);
}
asm(".global sub_0802CD78\n.thumb_set sub_0802CD78, IntelMenu_Rules\n");

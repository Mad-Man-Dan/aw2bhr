#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044D70.
 * sub_08044D70 @ 0x08044D70
 */

#include "proc.h"
/* Starts a blocking helper, fills its five consecutive bytes at +0x2c..+0x30,
 * and then starts the caller's own script AS A CHILD OF IT -- r1 at the second
 * Proc_Start is the blocking proc, not this function's own parent argument.
 *
 * The five bytes are written through ONE walking address (`adds r0,#0x2c` then
 * four `adds r0,#1`). The EIGHTH parameter is `u8` -- it is the only one
 * narrowed in the prologue -- while the seventh is an `int` read `ldrb` out of
 * its stack slot at the use; see the corrected declaration. */
struct Unk084A08ECProc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u8 unk2c;
    /* 0x2d */ u8 unk2d;
    /* 0x2e */ u8 unk2e;
    /* 0x2f */ u8 unk2f;
    /* 0x30 */ u8 unk30;
};
struct Unk08044D70Child
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ void *unk4c;
    /* 0x50 */ void *unk50;
};

void StartCoPowerDamageHealScript(const struct ProcCmd *a1, void *a2, void *a3, int a4, int a5,
                  int a6, int a7, u8 a8, ProcPtr a9)
{
    struct Unk084A08ECProc *p;
    struct Unk08044D70Child *q;

    p = Proc_StartBlocking(gUnknown_084A08EC, a9);

    p->unk2c = a4;
    p->unk2d = a5;
    p->unk2e = a6;
    p->unk2f = a7;
    p->unk30 = a8;

    q = Proc_Start(a1, p);

    q->unk4c = a2;
    q->unk50 = a3;
}
asm(".global sub_08044D70\n.thumb_set sub_08044D70, StartCoPowerDamageHealScript\n");

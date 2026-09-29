#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017688.
 * sub_08017688 @ 0x08017688
 */

/*
 * StartResumeScript -- start the gUnknown_0848A1EC script with `a` as its argument.
 *
 * sub_080152EC takes a gUnknown_03001470 slot for the script, and the slot's
 * .unk1e carries `a` through to it. Nothing checks the result: if every slot
 * were in use this would write through NULL.
 *
 * Why the C looks odd: `a` is declared u16 rather than being an int cast at the
 * use. With a cast the narrowing folds away, because the halfword store
 * truncates for free, and the original narrows the parameter on entry.
 */
void StartResumeScript(u16 a)
{
    sub_080152EC(gUnknown_0848A1EC, 0)->unk1e = a;
}
asm(".global sub_08017688\n.thumb_set sub_08017688, StartResumeScript\n");

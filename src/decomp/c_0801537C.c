#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801537C.
 * sub_0801537C @ 0x0801537C, sub_080153B8 @ 0x080153B8
 */

/* The remover for the gUnknown_03001470 list: scan all 30 slots for the one
 * whose .unk00 word holds this blob, tear it down and report its index, or -1
 * off the end of the scan.
 *
 * `(s8)i` under an `int` return rather than an `s8` return -- see the long note
 * on this declaration in include/unknown-functions.h. Both spellings emit the
 * same `lsls #0x18; asrs #0x18`; the `int` side was chosen because it is the
 * only one that makes `return sub_0801537C(g);` in CloseTopMenu valid C.
 */
int sub_0801537C(const void *a)
{
    u8 i;

    for (i = 0; i < 30; i++)
    {
        if (gUnknown_03001470[i].unk00 == (u32)a)
        {
            EndSlotScriptAt(i);

            return (s8)i;
        }
    }

    return -1;
}

/* sub_0801537C keyed on the SLOT ADDRESS instead of on the blob it holds --
 * the two functions are the same scan and differ in one instruction, the `ldr
 * r0, [r0]` that sub_0801537C spends dereferencing .unk00 and this one does
 * not. That missing load is the whole four-byte size difference between them.
 */
int sub_080153B8(struct Unk03001470 *a)
{
    u8 i;

    for (i = 0; i < 30; i++)
    {
        if (&gUnknown_03001470[i] == a)
        {
            EndSlotScriptAt(i);

            return (s8)i;
        }
    }

    return -1;
}

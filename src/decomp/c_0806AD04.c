#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806AD04.
 * sub_0806AD04 @ 0x0806AD04
 */

#include "proc.h"
/* Starts one gUnknown_0858175C child per enabled roster row and seeds it: the
 * row id, a 2-bit mode, a 0x180-wide column chosen by the caller's parity, and
 * the row's count rendered as up to three decimal digits, most significant
 * first, into the child's +0x2c byte array (0xff = blank).
 *
 * THE ONE NON-OBVIOUS SPELLING. The 2-bit read at +0x02 is `ldrb [.,#2]`
 * followed by `lsls #0x1e; lsrs #0x1e`, while the 14-bit read at the SAME
 * offset is `ldrh [.,#2]; lsrs #2`. That pair looks exactly like a
 * `u16 lo : 2; u16 hi : 14;` bitfield and it CANNOT be one -- see the Bitfields
 * chapter of docs/agbcc-codegen.md, which this function is the worked example
 * for. Briefly: agbcc gives any struct containing a bitfield 4-byte size and
 * alignment, which takes gUnknown_0202F214's element from 4 bytes to 8 and the
 * index from `lsls #2` to `lsls #3`; and the one layout that keeps the stride
 * loads the enclosing word, giving `ldr` rather than `ldrb`. Nor is it `& 3`,
 * which is `movs #3; ands`. The `(u32)` is load-bearing -- as `int` the second
 * shift is `asrs`.
 *
 * gUnknown_0202F214's declared `u16 unk02` is left ALONE. This is a fact about
 * the access, not about the struct. NOTE for whoever takes BuildCreditsMissionList: that
 * function is this one's WRITER and its stores genuinely want the bitfield
 * declaration, which would break the two reads here. See work/BuildCreditsMissionList/.
 *
 * The digit loop's exit test is at the TOP and its backward branch is on a
 * DIFFERENT variable, which is a `while` whose body ends in a conditional
 * break, not a do/while: `bne` targets the `cmp r4, #0`, so falling out of the
 * `v == 0` test leaves the loop without re-testing i. */

struct Unk806AD04
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
};
struct Unk806AD04Sub
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u8 unk2c[0x04];
    /* 0x30 */ u8 filler_30[0x1c];
    /* 0x4c */ int unk4c;
    /* 0x50 */ u16 unk50;
    /* 0x52 */ u16 unk52;
};

void StartCreditsMissionLine(ProcPtr a1)
{
    struct Unk806AD04 *proc = a1;
    struct Unk806AD04Sub *sub;
    int i;
    int v;

    if (gUnknown_085C77A0[gUnknown_0202F214[proc->unk2a].unk00].unk16 != 0)
    {
        sub = Proc_Start(gUnknown_0858175C, proc);
        sub->unk2a = gUnknown_0202F214[proc->unk2a].unk00;
        sub->unk4c = (u32)(u8)gUnknown_0202F214[proc->unk2a].unk02 << 30 >> 30;
        sub->unk52 = (proc->unk2c & 1) * 0x180;
        v = gUnknown_0202F214[proc->unk2a].unk02 >> 2;

        for (i = 0; i <= 2; i++)
            sub->unk2c[i] |= 0xff;

        i = 3;
        while (i != 0)
        {
            i--;
            sub->unk2c[i] = DivRem(v, 10);
            v = Div(v, 10);
            if (v == 0)
                break;
        }

        sub->unk50 = (proc->unk2c & 1) + 1;
        proc->unk2a++;
        proc->unk2c++;
    }
}
asm(".global sub_0806AD04\n.thumb_set sub_0806AD04, StartCreditsMissionLine\n");

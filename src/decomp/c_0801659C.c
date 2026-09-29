#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801659C.
 * sub_0801659C @ 0x0801659C
 */

/* The float path-mover state of src/decomp/c_080161B4.c, one command further
 * on. The struct is the same object as that file's `struct Unk1470Path` with
 * its filler_44[8] split into two floats; the tag is renamed here only so the
 * two declarations can coexist if promotion appends this function to
 * c_080161B4.c (0x0801659C is contiguous with SlotOp_MoveY). Merging them is
 * byte-neutral -- only a member's START OFFSET reaches the address
 * arithmetic. */
struct Unk1659CPath
{
    /* 0x00 */ u8 filler_00[0x04];
    /* 0x04 */ const void *unk04;
    /* 0x08 */ u8 filler_08[0x30];
    /* 0x38 */ s16 unk38;
    /* 0x3a */ u8 filler_3a[0x02];
    /* 0x3c */ float unk3c;
    /* 0x40 */ float unk40;
    /* 0x44 */ float unk44;
    /* 0x48 */ float unk48;
    /* 0x4c */ float unk4c;
    /* 0x50 */ float unk50;
    /* 0x54 */ float unk54;
    /* 0x58 */ float unk58;
    /* 0x5c */ int unk5c;
};
struct Unk1659CWrap /* not a real object: see c_080161B4.c */
{
    /* 0x00 */ struct Unk1659CPath unk00[30];
};
#define gPath659C (((struct Unk1659CWrap *)gUnknown_03001470)->unk00)

/* One command of the gUnknown_03001470[a].unk04 script stream: an EASE, i.e.
 * SlotOp_MoveAccelerated's smooth move with the acceleration solved for rather than
 * supplied. The four floats behind the command word are a destination offset
 * (x, y), a constant x velocity and a y acceleration; the frame count falls
 * out of the x leg (`(dstX - x) / vx`, made positive), and the y velocity is
 * then chosen so that the ease lands exactly on dstY:
 *     vy = -ay * (n - 1) / 2 + (dstY - y) / n
 * The body of StepSlotSpriteMotion is INLINED here rather than called, with one extra
 * step: when the counter retires, the slot is snapped to the exact
 * destination with a second SetSlotSpritePosition.
 *
 * `-gPath659C[a].unk58` and NOT `-q[3]`, even though the two are the same
 * value: reading the member back makes agbcc keep the just-stored value in a
 * callee-saved register (`ldr r3,[q,#0xc]; mov r8,r3; str r3,[dst]`, then
 * `mov r0,r8` at the negate), where naming `q[3]` again re-loads it through q
 * and shifts two instructions. Probed both ways, and a `float v = q[3]` local
 * is a third, also-wrong shape -- it hoists the load ABOVE the destination
 * address computation.
 *
 * `n` is s16: __fixsfsi's result is kept as `lsl #16; lsr #16` and re-read
 * `lsl #16; asr #16`, and the negate arm re-truncates. The 0x0808E538 pool
 * word is agbcc's -fforce-addr .rodata address constant holding 0x03001470
 * (dereferenced in baserom.gba), so the honest spelling below reproduces it
 * and promotion must carry the rodata entry; the SECOND reference to the same
 * global, in the inlined StepSlotSpriteMotion half, gets an ordinary direct pool word
 * instead, and that too falls out of naming the symbol both times. */
bool8 SlotOp_MoveEase(u8 a)
{
    s16 x, y;
    const float *q;
    s16 n;
    int m;

    q = *(const float **)gPath659C[a].unk04;
    if (gPath659C[a].unk38 == 0)
    {
        GetSlotSpritePosition(a, &x, &y);
        gPath659C[a].unk3c = x;
        gPath659C[a].unk40 = y;
        gPath659C[a].unk44 = q[0] + gPath659C[a].unk3c;
        gPath659C[a].unk48 = q[1] + gPath659C[a].unk40;
        gPath659C[a].unk4c = q[2];
        gPath659C[a].unk54 = 0;
        gPath659C[a].unk58 = q[3];
        n = (gPath659C[a].unk44 - gPath659C[a].unk3c) / gPath659C[a].unk4c;
        if (n < 0)
            n = -n;
        gPath659C[a].unk50 = -gPath659C[a].unk58 * (n - 1) * 0.5f
                           + (gPath659C[a].unk48 - gPath659C[a].unk40) / n;
        gPath659C[a].unk5c = n;
        gPath659C[a].unk38++;
    }

    gPath659C[a].unk3c += gPath659C[a].unk4c;
    gPath659C[a].unk40 += gPath659C[a].unk50;
    gPath659C[a].unk4c += gPath659C[a].unk54;
    gPath659C[a].unk50 += gPath659C[a].unk58;
    SetSlotSpritePosition(a, gPath659C[a].unk3c, gPath659C[a].unk40);
    m = gPath659C[a].unk5c--;
    if (m == 0)
    {
        gPath659C[a].unk38 = 0;
        gPath659C[a].unk04 = (const u8 *)gPath659C[a].unk04 + 8;
        SetSlotSpritePosition(a, gPath659C[a].unk44, gPath659C[a].unk48);
    }
    return FALSE;
}
asm(".global sub_0801659C\n.thumb_set sub_0801659C, SlotOp_MoveEase\n");

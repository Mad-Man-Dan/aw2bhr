#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080647BC.
 * sub_080647BC @ 0x080647BC, sub_0806486C @ 0x0806486C, sub_08064918 @ 0x08064918
 */

void RuleValue_DrawFunds(struct Unk08580934_Obj *obj)
{
    u8 digits[3];
    int n = 0;

    SplitDecimalDigits((obj->unk48 + 2) * 5, &digits[0], &digits[1], &digits[2]);

    if (digits[1] == 1)
        n = 1;

    DrawOamObject(digits[1] + 0x55, (obj->unk28 + n) & 0x1FF,
                 (obj->unk2a + 0xC) & 0xFF, 0, 0);
    DrawOamObject(digits[2] + 0x55, (obj->unk28 + 8) & 0x1FF,
                 (obj->unk2a + 0xC) & 0xFF, 0, 0);
    DrawOamObject(0x55, (obj->unk28 + 0x10) & 0x1FF,
                 (obj->unk2a + 0xC) & 0xFF, 0, 0);
    DrawOamObject(0x55, (obj->unk28 + 0x18) & 0x1FF,
                 (obj->unk2a + 0xC) & 0xFF, 0, 0);
}
asm(".global sub_080647BC\n.thumb_set sub_080647BC, RuleValue_DrawFunds\n");

void RuleValue_DrawTimeLimit(struct Unk08580934_Obj *obj)
{
    u8 digits[3];
    int n = 0;

    if (obj->unk48 == 0)
    {
        DrawOamObject(0xCB, (obj->unk28 + 8) & 0x1FF,
                     (obj->unk2a + 0xC) & 0xFF, 0, 0);
    }
    else
    {
        SplitDecimalDigits(obj->unk48 + 4, &digits[0], &digits[1], &digits[2]);

        if (digits[1] == 1)
            n = 1;

        if (digits[1] != 0xFF)
            DrawOamObject(digits[1] + 0x55, (obj->unk28 + 8 + n) & 0x1FF,
                         (obj->unk2a + 0xC) & 0xFF, 0, 0);

        DrawOamObject(digits[2] + 0x55, (obj->unk28 + 0x10) & 0x1FF,
                     (obj->unk2a + 0xC) & 0xFF, 0, 0);
    }
}
asm(".global sub_0806486C\n.thumb_set sub_0806486C, RuleValue_DrawTimeLimit\n");

void RuleValue_DrawCaptureLimit(struct Unk08580934_Obj *obj)
{
    u8 digits[3];
    int n = 0;

    if (obj->unk48 == 0)
    {
        DrawOamObject(0xCB, (obj->unk28 + 8) & 0x1FF,
                     (obj->unk2a + 0xC) & 0xFF, 0, 0);
    }
    else
    {
        SplitDecimalDigits(obj->unk48 + gUnknown_08580934->unk15 - 1,
                     &digits[0], &digits[1], &digits[2]);

        if (digits[1] == 1)
            n = 1;

        if (digits[1] != 0xFF)
            DrawOamObject(digits[1] + 0x55, (obj->unk28 + 8 + n) & 0x1FF,
                         (obj->unk2a + 0xC) & 0xFF, 0, 0);

        DrawOamObject(digits[2] + 0x55, (obj->unk28 + 0x10) & 0x1FF,
                     (obj->unk2a + 0xC) & 0xFF, 0, 0);
    }
}
asm(".global sub_08064918\n.thumb_set sub_08064918, RuleValue_DrawCaptureLimit\n");

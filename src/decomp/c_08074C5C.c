#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074C5C.
 * sub_08074C5C @ 0x08074C5C, sub_08074C70 @ 0x08074C70
 */

/* The `movs r0, #0` ahead of the first compare, and the second compare being
 * on the copy in r0 rather than on r1, are what say the original had a result
 * variable seeded to 0 and a nested clamp. Writing it as two early returns
 * (`if (v < 0) return 0; if (v > 0xc0) return 0xc0; return v;`) mutates the
 * parameter in place instead and loses both the copy and the r1 temp.
 */
int GetWorldMapCameraXCentered(int arg)
{
    int v = arg - 0x78;
    int result = 0;

    if (v >= 0)
    {
        result = v;

        if (result > 0xc0)
            result = 0xc0;
    }

    return result;
}
asm(".global sub_08074C5C\n.thumb_set sub_08074C5C, GetWorldMapCameraXCentered\n");

/* Twin of GetWorldMapCameraXCentered with the offset and the cap swapped (0x50 / 0x60). */
int GetWorldMapCameraYCentered(int arg)
{
    int v = arg - 0x50;
    int result = 0;

    if (v >= 0)
    {
        result = v;

        if (result > 0x60)
            result = 0x60;
    }

    return result;
}
asm(".global sub_08074C70\n.thumb_set sub_08074C70, GetWorldMapCameraYCentered\n");

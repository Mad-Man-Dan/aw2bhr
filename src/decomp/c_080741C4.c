#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080741C4.
 * sub_080741C4 @ 0x080741C4, sub_0807420C @ 0x0807420C
 */

#include "hardware.h"

/* A subset of SoundRoomMusicPage_Init's screen-init run, which is where every type here
 * comes from -- same two Decompress calls, same palette, same tail.  The one
 * thing that differs is the first destination: SoundRoomMusicPage_Init hard-codes
 * 0x06008000 where this computes the char base out of the BG3 control shadow.
 * `ldr` + `lsls #0x1c; lsrs #0x1e` is the bitfield read of `bits.chr_block`
 * whatever the container, and `<< 0xe` scales it by the 16 KB char block.
 *
 * Wave 53 (W53-A) RETYPES this from `void LoadBg3Backdrop(void)`. The body reads
 * no argument register, so nullary was the honest body-side reading, but the
 * caller-side evidence is decisive the other way: NameEntry_Init sets r0, r1 and
 * r2 to 0 with three separate `movs #0` immediately before the `bl`. An
 * argument already in the right register costs nothing, but a literal 0 never
 * does, so there are three parameters and this body ignores all three. The
 * three unused ints change nothing here -- re-verified byte-for-byte with
 * trymatch, 72 bytes, relocs match -- and AddBg3AutoScrollHook in this file is
 * untouched.
 */
void LoadBg3Backdrop(int a1, int a2, int a3)
{
    Decompress(gUnknown_0823A3D4,
               (void *)(0x06000000 + (gUnknown_0300251C.bits.chr_block << 14)));
    Decompress(gUnknown_08239FA4, gBG3TilemapBuffer);
    ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
    BG_EnableSyncBG3();
}
asm(".global sub_080741C4\n.thumb_set sub_080741C4, LoadBg3Backdrop\n");

/* Bg3AutoScroll_Loop is registered by ADDRESS and so is cast to `void *`, which is
 * the AddVBlankHook convention unknown-functions.h already records.
 */
void AddBg3AutoScrollHook(void)
{
    CpuCopyAuto(gUnknown_0812B29C, (void *)0x06001F00, 0x100);
    AddVBlankHook((void *)Bg3AutoScroll_Loop);
}
asm(".global sub_0807420C\n.thumb_set sub_0807420C, AddBg3AutoScrollHook\n");

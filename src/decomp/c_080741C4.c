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
 * Wave 53 (W53-A) RETYPES this from `void sub_080741C4(void)`. The body reads
 * no argument register, so nullary was the honest body-side reading, but the
 * caller-side evidence is decisive the other way: NameEntry_Init sets r0, r1 and
 * r2 to 0 with three separate `movs #0` immediately before the `bl`. An
 * argument already in the right register costs nothing, but a literal 0 never
 * does, so there are three parameters and this body ignores all three. The
 * three unused ints change nothing here -- re-verified byte-for-byte with
 * trymatch, 72 bytes, relocs match -- and sub_0807420C in this file is
 * untouched.
 */
void sub_080741C4(int a1, int a2, int a3)
{
    Decompress(gUnknown_0823A3D4,
               (void *)(0x06000000 + (gUnknown_0300251C.bits.chr_block << 14)));
    Decompress(gUnknown_08239FA4, gBG3TilemapBuffer);
    ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
    sub_08013B1C();
}

/* sub_08037F1C is registered by ADDRESS and so is cast to `void *`, which is
 * the sub_08011B34 convention unknown-functions.h already records.
 */
void sub_0807420C(void)
{
    sub_08011C68(gUnknown_0812B29C, (void *)0x06001F00, 0x100);
    sub_08011B34((void *)sub_08037F1C);
}

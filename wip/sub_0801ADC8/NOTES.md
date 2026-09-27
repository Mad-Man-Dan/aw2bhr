
## Wave 94 (W94-A) - 76.26% -> 77.34%, size-exact

Renamed the wave-93 volatile temporary `new_var` to `keptFlags` (it holds
`unk20[i] | 8`). Byte-neutral.

Run 1: 76.26 -> 76.80. The mutation wraps the FIRST wipe-and-retry loop in
`do { ... } while (0)`. The inner `break` still binds to the `for (k ...)`
loop, so it is equivalent. Reformatted onto separate lines, which is also
byte-neutral.

Run 2: 76.80 -> 77.34. The mutation moves `unk00[i] = -1` ahead of
`unk10[i] = 0xff` and binds the `-1` to a local (renamed `erased`). The two
stores are to different members of the same struct with no call between them,
so the reorder is equivalent.

Vesly's fork also carries a draft for this function; it compiles to 64.57%,
below ours, so it was not adopted.

### Residual

556/556, 77.34%, first difference +0xa. Unchanged in kind: register allocation
and block placement around the two duplicated scan loops.

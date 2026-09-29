# Rename plan

This document tracks proposed struct and function renames. It is a planning
artifact only: entries here do not authorize or imply source changes.

## Goals and constraints

- Make names clearer while preserving the repository's byte-matching goal.
- Treat `asm/` as the immutable reference; do not rename or edit its contents.
- Record evidence and all known references before approving a rename.
- A rename is complete only after the affected build output is verified against
  the canonical ROM, when implementation is eventually authorized.

## Naming conventions to confirm

Current conventions visible in the tree (to use as a starting point for review):

- Struct/type names use `struct` tags in PascalCase (`Unit`, `UnitType`,
  `BattleUnit`); names should describe the represented object, not just its
  address. Unknown records retain address-derived tags such as
  `Unk802C57C` until their role is well enough established.
- Readable functions use PascalCase verbs (`GetUnitCombatClassColumn`,
  `CreateUnitAt`). Address names (`sub_XXXXXXXX`) remain the stable fallback.
  Existing address symbols can be kept as linker aliases with `.thumb_set`;
  see `src/named.md` and `include/xenesis-names.h` for the repo's precedent.
- Existing proc/resource prefixes (`ProcScr_`, `g`) and game vocabulary should
  be preserved where they carry established meaning. Prefer the repo's
  evidence-backed name over an external name when the latter is narrower or
  conflicts with observed behavior (`src/named.md` records examples).
- No semantic name should imply more certainty than the evidence supports.
  In particular, a field or type label supplied by a ROM-editor definition is
  a lead, not independent proof; `include/unit.h` documents where that source
  agrees or conflicts with in-tree observations.

## Proposed renames

Add one row per symbol. Keep proposals in `Proposed` until reviewed together.

| Kind | Current name | Proposed name | Evidence / meaning | References and dependencies | Confidence | Status |
|---|---|---|---|---|---|---|
| Struct | `struct UnitType` | — | The 0x5c per-unit-type record has many evidence-backed fields; the record's overall domain is clear, but no better whole-record name has been established yet. | `include/unit.h`, all declarations and uses of `gUnknown_085D5ABC`; changing the tag would touch shared headers and promoted functions. | High that current name is serviceable; no replacement proposed | Hold |
| Field | `UnitType.unitClass` | Keep | AW2-InfiniteCOs names the gathered field at offset `0x18` `GatherUnitClass` and defines its values as Soldier, Vehicle, Plane, Copter, and Naval; its unit records assign those categories explicitly. This independently supports the existing `unitClass` field name. The value domain should not be conflated with the separate 5/6/7 bonus-table columns returned by `sub_080432E0`. | `include/unit.h`; AW2-InfiniteCOs `common/definitions_unit.asm` (`GatherUnitClass`, `Class*`), `common/macros_unit.asm` (`setUnitClass`), and `units/aw2/*.asm` | High | Keep |
| Field | `UnitType.unk1b` | `aiUnitType` (candidate) | AW2-InfiniteCOs places `setUnitAIType` at offset `0x1b`, with values named `AICaptureUnit`, `AIIndirectCombat`, `AIDirectCombat`, and carrier roles. The current tree independently observes values 1..6 used as categories by unit-list builders, consistent with an AI classification field, but needs a complete value-to-unit map before choosing the exact semantic label. | `include/unit.h`; `src/decomp/c_0805*.c`; AW2-InfiniteCOs `common/macros_unit.asm` lines 118-122 and `common/definitions_unit.asm` lines 53-60 | Medium | Research |
| Field | `UnitType.unk1d` | `aiUnitTargetMask` (candidate) | AW2-InfiniteCOs labels offset `0x1d` "AI Unit Classification" and writes target-domain bits (`AITargetGround`, `AITargetAir`, `AITargetSea`, `AITargetSub`). The current tree observes this byte used as a mask against a map plane. This is promising evidence for a target-category mask, but audit the in-tree consumers and whether values combine before adopting the name. | `include/unit.h`; `src/decomp/c_08058BB4.c` (`sub_08058BB4` and `sub_08058C54`); AW2-InfiniteCOs `common/macros_unit.asm` lines 130-134 and `common/definitions_unit.asm` lines 89-93 | Medium | Research |
| Global table | `gUnknown_085D3DD0` | `gCOStatsAbilities` (candidate) | CO-indexed data table, one 0x104-byte record per CO. AW2-InfiniteCOs calls its corresponding table `COStatsAbilitiesDatatable` and defines each CO stats record as 260 bytes, exactly 0x104. This strongly supports a CO stats/abilities table name; its address-specific Subroutine List entry is contradicted by both repositories' data-table use. | `include/co.h`; table references; AW2-InfiniteCOs `main-generated.asm`, `common/definitions_romallocation.asm` (`coData_Length = 260`), `datatables/hookcounitstats.asm` | Medium-high | Proposed |
| Struct | `struct Unk085D3DD0` | `struct COStatsAbilities` (candidate) | CO-indexed record (0x104 bytes) with three power-state subrecords and observed CO bonuses. The InfiniteCOs label `COStatsAbilitiesDatatable` for the equivalent 260-byte-per-CO table independently supports this broad name. Xenesis's Subroutine List entry calls `0x085D3DD0` a routine checking allegiance/HQ type, but that conflicts with direct data-table use. | `include/co.h`, `include/unit.h`, `gUnknown_085D3DD0` users and prototypes; InfiniteCOs `main-generated.asm`, `common/definitions_romallocation.asm`; Xenesis `AW2 Subroutine List.txt` line 365 and `AW2 Datasheet.txt` line 5952 | Medium-high | Proposed |
| Struct | `struct Unk085D3DD0Entry` | — | 0x44-byte per-power subrecord. It contains observed luck, capture, combat bonus, movement, and other data; avoid naming it only for the bonus rows. | `include/co.h`; `struct Unk085D3DD0::power[]` users | Medium | Research |
| Field | `Unk085D3DD0Entry.unk24` | `pUnitStatBonus` (candidate) | Eight pointers to four-s16 bonus vectors; family F049 reads vector elements at offsets 0, 2, 4, and 6 for attack, defence, movement, and range. AW2-InfiniteCOs explicitly describes its per-unit table as an alternative to the original per-unit-class boost table, corroborating the class-oriented nature of the original data but not the meanings of all eight slots. Its layout differs, so keep the name generic to the observed pointer array. | `include/co.h`; readers in `src/unit.c`; AW2-InfiniteCOs `datatables/hookcounitstats.asm`, `common/macros_co.asm` lines 566-577, and `cos/aw2/max.asm` lines 26-31 | Medium | Research |
| Function | `GetUnitCombatClassColumn` | `GetCoBonusUnitClassColumn` (candidate) | At `0x080432E0`, maps `UnitType.minRange` values 0, 1, and >1 to indices 7, 5, and 6. Four CO bonus readers use the result as an index into the `unk24` rows of `struct Unk085D3DD0Entry`. Xenesis's datasheet independently identifies `0x080432E0` as a common subroutine in that CO stat-loading path, and describes the 5/6 results as direct/indirect classes; it does not document the 0-to-7 case. The proposed name makes the table-specific use explicit without treating `minRange` itself as a class field. | `src/unit.c`; `.thumb_set sub_080432E0`; bonus readers in `src/unit.c`; `include/co.h`; Xenesis `AW2 Datasheet.txt` lines 2728-2748 and 3518-3527 | Medium-high | Proposed |
| Function | `GetCoAttackBonus` (`sub_080430B0`) | Keep | The older transcription supplies useful parameter meanings: `coId`, `coPowerState`, and `unitType`. These match the current table indexing and `gPlayers[].coMode` model. Existing name already states the returned quantity. | `src/unit.c`; `include/co.h`; aliases/prototypes in `include/xenesis-names.h`; family F049 peers at `0x08043120`, `0x08043190`, and `0x08043200` | High | Keep; parameter names are a cleanup candidate |
| Global | `gUnknown_030032D8` | — | `RunMapStateMachine`'s top-level per-frame state selector, a 21-case switch. Case 13 is the free-cursor handler documented in the player-phase section below. Not otherwise audited this pass. | `src/decomp/c_080345C8.c` | Low | Research |
| Function | `sub_0802DCB4` | `MapCursorIdle` (candidate) | `RunMapStateMachine` case 13's handler (via the one-line forwarder `sub_0802DC2C`). Per-frame free-cursor state: reads `gpKeySt->pressed` and dispatches Start/Select/L/R/A/B, falling back to `sub_0802A7C4()` (info-panel refresh) plus `sub_0802776C(0)` (cursor move/scroll, still asm) when nothing else fires. Every branch matches a user-playtested behavior; see the player-phase notes below for the button-by-button mapping. | `src/decomp/c_0802DCB4.c`, `c_0802DC2C.c`, `c_080345C8.c` | High for role; name is a candidate | Proposed |
| Function | `sub_0802DC2C` | Keep / fold | One-line forwarder to `sub_0802DCB4`, no independent behavior. | `src/decomp/c_0802DC2C.c` | High | Keep |
| Function | `sub_0802E4B4` | `MapCursor_OnPressA` (candidate) | A-button handler, called by `sub_0802DCB4` with the cursor's map cell (x, y). `sub_080242B0(x,y)` true -> `sub_0802D5E8(x,y)` and return — matches "A on a controlled base/port/airport opens the deployment screen." Empty tile or `gUnknown_030040D8->unk01 & 1` set -> `sub_0802D458()` (map menu) and return — matches "A on an empty tile hides PPSWC and opens the map menu." Otherwise starts movement selection (`sub_080202A4`, `sub_08022990(x,y,0)`, ...) — matches "A on a unit starts movement selection." The `unk01 & 1` gate is a plausible but UNCONFIRMED match for "units you do not control"; see open question. | `src/decomp/c_0802E4B4.c` | Medium-high for role; `unk01` gate unconfirmed | Proposed |
| Function | `sub_0802E2D0` | `MapCursor_OnPressB` (candidate) | B-button handler; returns `u8` so `sub_0802DCB4` can fall through to ordinary cursor movement when it returns 0. When a targeted unit's capability flags `a`/`b` (from `sub_08041FE0`/`sub_0804203C`) are both false: SFX `sub_0803B4DC(0x68)`, returns 0 (falls through, no effect). When `b` is set: calls `sub_080201E0(unit->unk02, unit->unk03, unit)` — matches "pressing B on a unit displays their attack range." Both the A- and B-handlers end with `sub_08022990(x, y, mode)` (mode 0 from A, mode 1 from B) and the same SFX `sub_0803B4DC(0x69)` — matches "starts their moveUnit sprite" in both playtested descriptions, with `sub_08022990`'s third argument the plausible move/attack discriminator. | `src/decomp/c_0802E2D0.c`, `c_0802E4B4.c` | Medium-high | Proposed |
| Function | `sub_08022990` | `StartUnitSelectCursorFx` (candidate) | Called identically at the tail of both `sub_0802E4B4` (mode 0) and `sub_0802E2D0` (mode 1) right after each shows movement range or attack range. Matches "starts their moveUnit sprite" from both user descriptions. Body not read yet — only an argument-width callee note is on file (first two args word-wide, third `u16`). | `c_0802E4B4.c`, `c_0802E2D0.c`; existing callee note near the `sub_0802E4B4` block in `include/unknown-functions.h` | Medium | Research |
| Function | `sub_080201E0` | `ShowUnitAttackRange` (candidate) | Called by `sub_0802E2D0` only on the `b`-capability branch, as `sub_080201E0(unit->unk02, unit->unk03, unit)`, directly before the "moveUnit sprite" call. Matches "pressing B on a unit displays their attack range." Body not read yet. | `src/decomp/c_0802E2D0.c` | Medium | Research |
| Function | `sub_0802D5E8` | `OpenDeploymentScreen` (candidate) | Called by `sub_0802E4B4` only when `sub_080242B0(x,y)` is true, and that branch returns immediately without falling into the unit/empty-tile logic — matches "A on a controlled base/port/airport opens the deployment screen." Body not read yet. | `src/decomp/c_0802E4B4.c` | Medium | Research |
| Function | `sub_080242B0` | `CanDeployAtCursor` (candidate) | Gate for `sub_0802D5E8`; returns `u8` per the existing header callee note (result narrowed `lsls #0x18` at its one call site). Plausibly an ownership + tile-type (base/port/airport) check, not yet confirmed from its own body. | `src/decomp/c_0802E4B4.c`; callee note in `include/unknown-functions.h` | Low-medium | Research |
| Function | `sub_0802D458` | `OpenMapMenu` (candidate) | Called directly by the SELECT branch of `sub_0802DCB4` and by `sub_0802E4B4`'s "empty tile" fallback — both match "hides PPSWC and opens the map menu." Body: positions the menu by cursor-to-scroll distance, starts Proc script `gUnknown_0849AAC0` via `sub_0801A104(gUnknown_0849AAC0, v, 1, 1)`, then `IncrementMapLock()`. The lock is the leading hypothesis for how PPSWC gets hidden: it likely stops `RunMapStateMachine` from re-entering case 13 until the menu closes, so the cursor/side windows would disappear as a side effect of leaving that state rather than through any explicit "hide" call — nothing found so far writes a hide flag directly. | `src/decomp/c_0802D458.c`, `c_0802DCB4.c`, `c_0802E4B4.c` | High for role; the lock -> hide mechanism is a hypothesis | Proposed |
| Function | `sub_0802E250` | `MapCursor_OnPressStart` (candidate) | START handler: SFX `sub_0803B4DC(0x76)` then `sub_0802C2B4()`. Matches "pressing Start hides PPSWC and opens the minimap." | `src/decomp/c_0802E250.c` | High | Proposed |
| Function | `sub_0802C2B4` | `OpenMinimap` (candidate) | Starts Proc script `gUnknown_0849A990` and clears `gDispIo.disp_ct.bg0_enable` (disables BG0). If BG0 is the layer the player-phase side windows/cursor render on, this is the direct "hide PPSWC, open minimap" action; the existing header comment ("install-then-disable pair") already independently supports this reading, it simply predates the minimap context. | `src/decomp/c_0802C2B4.c`, `c_0802E250.c` | Medium-high | Proposed |
| Function | `sub_0803B4DC` | Keep (`PlayMusicOrSfx`) | Already matched and named; recorded here only to correct an earlier misreading in this investigation that treated it as a window launcher — it is a pure sound/music-id call. Observed ids: `0x76` on Start-press, `0x65` opening the unit-class-info window, `0x68` the B-press "can't do that" beep, `0x69` the shared move/attack-select confirm (both A and B tails). Worth collecting a fuller id map once more call sites are read. | `src/decomp/c_0803B4DC.c` and the call sites above | High | Keep |
| Function | `sub_0803A8F0` | `ShowUnitClassInfoWindow` (candidate) | Called by `sub_0802DCB4`'s R-branch when a unit occupies the cursor cell. Positions an info window relative to the unit and scroll (`gUnknown_0849D89C->unk00/02/03`), stores the unit pointer and a slot number (`unk04`, `unk08`) for a display Proc to read, then plays SFX `0x65`. Matches "R on an occupied tile ... displays the unit class info window." The window's actual content fields (battle sprite, move, vision, fuel, weapons) are not traced to source yet. | `src/decomp/c_0803A8F0.c`, `c_0802DCB4.c` | Medium-high for role; content fields untraced | Proposed |
| Function | `sub_080470F8` | `ShowTerrainInfoWindow` (candidate) | Called by `sub_0802DCB4`'s R-branch when no unit occupies the cell, with the terrain id returned by `sub_0803EED4`. Sets a "cursor-nudge" flag from scroll proximity and clears it when the terrain id falls in a "special" range (15-16, 21-31) — consistent with disabling a down/up scroll hint on terrain types that have no next/previous page. Matches "R on an empty tile ... displays the terrain info window, which you can scroll down/back up in." CAUTION: the function's existing header comment already describes it more generically as "Enter a map," written before this call site's role was known; reconcile the two readings (shared multi-purpose Proc entry vs. terrain-info-specific) before renaming. | `src/decomp/c_080470F8.c`, `c_0802DCB4.c`, `c_0803EED4.c` | Medium; naming conflicts with existing header framing | Proposed |
| Function | `sub_0803EED4` | `GetTerrainTypeAt` (candidate) | Returns the terrain id at a map cell, with a fog/override substitution through `sub_0803DE94`/`sub_0803DF98` when applicable. Feeds directly into `sub_080470F8`'s terrain-info-window argument. | `src/decomp/c_0803EED4.c` | Medium-high | Proposed |
| Function | `sub_0802A7C4` | `RefreshMapCursorInfoPanel` (candidate) | Guarded by `gPlaySt.dispMiniPanel`; when set, starts a fade-in Proc over `sub_0802AA78` (see row below) and sets several blend registers. Called unconditionally in `sub_0802DCB4`'s fallback tail — i.e. every idle frame the cursor is free to move and no other window is open — so this is the per-tile info popup the cursor drags around the map. | `src/decomp/c_0802A7C4.c`, `c_0802DCB4.c` | High | Proposed |
| Function | `sub_0802AA78` | Keep prior finding (cursor/tile info overlay) | Drafted from `ghidra.txt` in a previous session (`work/sub_0802AA78/sub_0802AA78.c`, own draft, not yet byte-matched): draws the CO-power icon, unit HP/ammo/fuel icons, a cargo-icon pair, and a terrain/danger text box for the cell under the cursor, choosing between two screen-half layouts via `gUnknown_0849A2A6`/`gUnknown_0849A284`. This pass independently confirms `sub_0802A7C4` (row above) as its caller, gated on `gPlaySt.dispMiniPanel` — direct corroboration of the earlier reading from asm alone. | `work/sub_0802AA78/sub_0802AA78.c`; `src/decomp/c_0802A7C4.c` | Medium (own draft, not yet matched) | Research |
| Global | `gUnknown_030033E4` | `gMapCursorPos` (candidate) | `struct Unk802C57C` (a generic u16 x/u16 y pair; several unrelated globals share the type — do not rename the type from this one instance). This global is read as `gMap->rowOffset[.unk02] + .unk00`, the standard row/x map-index idiom, and is the coordinate `sub_0802DCB4` forwards to every button handler — strong evidence this specific instance is the on-screen map cursor. | `src/decomp/c_0802DCB4.c`; `struct Unk802C57C` note in `include/unknown-globals.h` | High | Proposed |
| Function/global cluster | `sub_0802776C`, `gUnknown_03003334` | Research target | Unmatched user-reported behavior: holding B "hides PPSWC and speeds up cursor movement/map scrolling (cursor hidden), and makes units transparent." `sub_0802776C(u8)` is `sub_0802DCB4`'s fallback-tail call (arg 0) and also `RunMapStateMachine`'s own special-case call (arg 3); still asm-only, but its existing header note describes it as a switch handing Proc addresses to `sub_0801F024`, more consistent with an input-mode dispatcher than literal per-frame movement math — a good next promotion target. `gUnknown_03003334` is written to `1` by the A-handler, `6` by the B-handler, and separately to `gpKeySt->held & 2` by an unrelated function (`sub_0802E278`, next door to the Start-handler family) — do not assume these are the same semantic use without auditing every writer; see open question below. | `src/decomp/c_0802DCB4.c`, `c_080345C8.c`, `c_0802E4B4.c`, `c_0802E2D0.c`, `c_0802E250.c` | Low | Research |

### Struct notes

For each type, record field offsets, size, known users, and evidence for the
proposed meaning. Mark uncertain field labels explicitly; a better type name
does not establish every field's semantics.

### Function notes

For each function, record address or stable identifier, behavior, callers,
related data/types, and evidence supporting the verb and object in the name.
Distinguish observed behavior from inferred purpose.

### Names extracted from the older `sub_080430B0` transcription

The transcription's `coID`, `coPowerState`, and `unitType` are useful parameter
names for the existing `GetCoAttackBonus` implementation. Its `combatClass`
temporary is more precisely `combatClassColumn`: `sub_080432E0` returns an
index into `Unk085D3DD0Entry.unk24[]`, not a value stored as a class on the
unit type. Its `classOff` is likewise an index/byte offset derived from
`UnitType.unitClass`; retain the established field name `unitClass` rather
than reviving the transcription's generic `class`.

`UnitTypeRecord` is a useful descriptive synonym from the old notes, but the
repository already uses `struct UnitType` consistently and this transcription
does not establish a reason to rename that type. The transcription's
`gUnknown_085D3E2C` expression also differs from the current source's explicit
`gUnknown_085D3DD0[a].power[b].unk24[...]`; do not reuse that spelling as a
table rename based on this snippet. The old `classOff` / `powerOff` / `coOff`
arithmetic describes flattened equivalent addressing (power stride 17 words,
CO stride 65 words), not independently named semantic objects.

### AW2-InfiniteCOs cross-check

The local checkout at `/home/vesly/AW2-InfiniteCOs` is a community expansion
and partial reimplementation, so use it as corroborating evidence rather than
as authority for the original ROM. Its README explicitly describes an
"Indirect Classification" fix: applying the indirect-unit flag from class
data rather than minimum range. Its `common/definitions_unit.asm` sets
`GatherUnitClass` to decimal 24 (`0x18`) and defines `ClassSoldier` through
`ClassNaval` as values 0 through 4; `common/macros_unit.asm` writes those
categories into unit records, and `units/aw2/*.asm` assigns examples such as
Infantry -> Soldier, Tank -> Vehicle, Fighter -> Plane, and Sub -> Naval.
This independently supports the existing `UnitType.unitClass` label and
reinforces keeping it conceptually distinct from the original game's helper
that derives a CO bonus-table column from `minRange`.

InfiniteCOs also gives separate names to the neighboring fields: `setUnitAIType`
at offset `0x1b` uses role-like categories (capture, direct/indirect combat,
and transport roles), while `setUnitAIUnitClass` at offset `0x1d` writes
ground/air/sea/sub target bits. That is useful evidence for investigating
`UnitType.unk1b` and `UnitType.unk1d`, but the repository's own users remain the
authority for their names: retain `aiUnitType` and `aiUnitTargetMask` as
candidates pending a complete value and consumer audit. In particular, do not
confuse the field at `0x1d` with `unitClass` at `0x18`, or with the role-like
field at `0x1b`.

The README also means that names such as "direct unit" and "indirect unit"
should be treated as attack-range classifications, not automatically as
synonyms for the AW2-InfiniteCOs `unitClass` categories. Its `setCOD2DDirectStat`
and `setCOD2DIndirectStat` macros encode different hand-curated unit lists;
they don't establish that `GetUnitCombatClassColumn` returns a stored class.

For the CO stat readers, `common/macros_co.asm` names the four per-unit values
`Attack`, `Defence`, `Movement`, and `Range`. More directly, the comment in
`datatables/hookcounitstats.asm` says its replacement table is tabulated per
unit "instead of per unit-class"; its replacement for `0x080430B0` still
calls `0x080432E0` and indexes the same eight-slot-per-CO stats table shape.
The InfiniteCOs allocation defines `coData_Length` as 260 bytes, matching
the original `0x104` stride. Together these support a CO stats/abilities table
name for `gUnknown_085D3DD0` / `struct Unk085D3DD0`, and class-oriented bonus
lookup, while leaving individual fields and slot meanings to in-tree evidence.

`setCOD2DUnitStat`,
`setCOPowerUnitStat`, and `setCOSuperUnitStat` place those four halfwords in
separate day-to-day, CO Power, and Super CO Power blocks. This makes
`unitStatBonusPointers` a plausible descriptive name for the original entry's
eight pointers, whose pointees the existing code reads at the same four
halfword offsets. Keep it at Research: InfiniteCOs stores a flat per-unit
matrix (`PHackUltraPointer`) and therefore does not establish how the original
table's eight pointer slots divide between unit-class and combat-column uses.

### Player-phase cursor notes (RunMapStateMachine case 13 / free cursor)

`RunMapStateMachine` ([src/decomp/c_080345C8.c](../src/decomp/c_080345C8.c)) is a
21-case switch on `gUnknown_030032D8`. Case 13 calls `sub_0802DC2C`, a
one-line forwarder to `sub_0802DCB4` — the free-cursor player-phase state's
per-frame handler. None of the other 20 cases have any header commentary yet;
this pass only covers case 13's chain.

`sub_0802DCB4` reads `gpKeySt->pressed` and dispatches by button. It does not
itself read the D-pad or write the cursor position
(`gUnknown_030033E4`/candidate `gMapCursorPos`) — that is delegated to the
still-asm `sub_0802776C(0)` in its fallback tail, alongside
`sub_0802A7C4()` (the per-tile info-panel refresh). The button map below was
built by cross-referencing the promoted C against user-playtested behavior;
every branch has a matching call, which is strong corroboration for the
handler identifications even where the deeper callees are still unread.

| Button | Observed gameplay | Code path | Confidence |
|---|---|---|---|
| Start | Hides PPSWC, opens the minimap | `sub_0802E250` -> SFX `0x76` + `sub_0802C2B4` (`OpenMinimap` candidate: disables BG0, starts Proc `gUnknown_0849A990`) | High |
| Select | Hides PPSWC, opens the map menu | `sub_0802D458` (`OpenMapMenu` candidate: positions + starts Proc `gUnknown_0849AAC0`, then `IncrementMapLock()`) | High |
| A, empty tile | Hides PPSWC, opens the map menu | `sub_0802E4B4` falls through to the same `sub_0802D458` | High |
| A, controlled base/port/airport | Opens the deployment screen | `sub_0802E4B4` -> `sub_080242B0(x,y)` true -> `sub_0802D5E8` (`OpenDeploymentScreen` candidate) | Medium |
| A, unit | Starts movement selection (blocked from confirming on uncontrolled units) | `sub_0802E4B4` -> `sub_080202A4` (range) + `sub_08022990(x,y,0)`; the "cannot confirm on uncontrolled units" behavior is not yet located in this function and may live downstream of the move-select state | Medium |
| B, held | Hides PPSWC, speeds up cursor/scroll (cursor hidden), makes units transparent | Not yet located — likely inside `sub_0802776C`, still asm; see research row above | Unconfirmed |
| B, unit | Displays attack range, starts the moveUnit sprite | `sub_0802E2D0` -> `sub_080201E0(unit->unk02, unit->unk03, unit)` (range) + `sub_08022990(x,y,1)` | Medium-high |
| L | Jump cursor to next unit | `sub_08025580`/`sub_080254AC` find a unit, `sub_08029088` recenters | High (established in a prior session) |
| R, empty tile | Hides PPSW, alternate cursor, terrain info window (scrollable) | `sub_0803EED4` (terrain id) -> `sub_080470F8` (`ShowTerrainInfoWindow` candidate) | Medium |
| R, occupied tile | Hides PPSW, alternate cursor, unit class info window (swaps to terrain info on R) | `sub_0803A8F0` (`ShowUnitClassInfoWindow` candidate) | Medium-high |

No function read so far writes an explicit "hide PPSWC" flag. The leading
hypothesis is that it is an emergent effect of leaving `RunMapStateMachine` case 13 —
`sub_0802D458`'s `IncrementMapLock()` is the one concrete lead — rather than a
single dedicated call; this needs confirmation from whatever reads the map
lock or `gUnknown_030032D8` to decide what gets drawn.

## Dependency and ordering notes

Record coupled changes here, such as a type rename that affects prototypes,
callers, or multiple functions. Group proposals that should be reviewed or
implemented together, and call out any shared-header effects.

Initial review thread: the highlighted bonus lookup in `src/unit.c` indexes
`unk24` once with `UnitType.unitClass` and once with
`GetUnitCombatClassColumn(unitType)`. This suggests a useful naming question
for the table and its columns, but it does not by itself establish that the
stored `unitClass` value and the helper's derived column are the same concept.
Keep that distinction explicit while investigating candidate names.

External reference checked: `C:\Users\david\OneDrive\Desktop\SRR_AW2\XenesisDocs`
(`AW2 Datasheet.txt`, `AW2 Subroutine List.txt`). These are useful corroborating
reverse-engineering notes, not primary evidence. In particular, the Subroutine
List's entry at `0x085D3DD0` conflicts with the repository's direct evidence
that this address holds the CO bonus table; do not carry that entry into a
semantic symbol name without resolving the conflict.

## Review checklist

- [ ] Check the existing symbol and its references in the repository.
- [ ] Check relevant assembly, call sites, and promoted exemplars for evidence.
- [ ] Check for naming collisions and similar symbols.
- [ ] Confirm the proposal improves clarity without overstating uncertain
      semantics.
- [ ] Agree on the proposed name and implementation batch.
- [ ] Keep any future source changes separate from this planning pass until
      explicitly requested.

## Decisions and open questions

| Date | Decision or question | Outcome |
|---|---|---|
| 2026-09-25 | Does Xenesis's `0x080432E0` note clarify `GetUnitCombatClassColumn`? | It corroborates that the helper belongs to the CO stat-loading path and describes columns 5/6 as direct/indirect. Proposed `GetCoBonusUnitClassColumn` for review; retain the observed third mapping (0 -> 7) as an unresolved/special case. |
| 2026-09-25 | Is the Xenesis label for `0x085D3DD0` suitable for renaming the struct/table? | No: it calls the address a routine checking allegiance/HQ type, while repository users treat it as a CO-indexed bonus table. Retain the conflict as a source-quality caution. |
| 2026-09-25 | Which names from the older `sub_080430B0` transcription are reusable? | Record `coId`, `coPowerState`, `unitType`, and prefer `combatClassColumn` for the helper result. Keep established `UnitType` / `unitClass` and explicit `gUnknown_085D3DD0[a].power[b].unk24[]`; the old generic struct and flattened-offset spellings add no stronger evidence. |
| 2026-09-25 | Does AW2-InfiniteCOs clarify `UnitType.unitClass` and the helper's class terminology? | Yes: its offset-`0x18` gather and explicit Soldier/Vehicle/Plane/Copter/Naval values support `unitClass`. Its README distinguishes indirect classification from minimum range, so document the helper result as a CO bonus-table column and avoid equating it with the stored unit class. |
| 2026-09-25 | Does InfiniteCOs suggest semantic names for `Unk085D3DD0Entry.unk24[]`? | It uses the same four stat terms (Attack, Defence, Movement, Range) in D2D/Power/Super per-unit data, and describes its flat matrix as an alternative to per-unit-class boosts. Candidate `unitStatBonusPointers`; keep under research because InfiniteCOs does not prove the original eight-slot index semantics. |
| 2026-09-25 | Do InfiniteCOs' unit macros clarify adjacent unit-type bytes? | They distinguish the role-like `AI Type` byte at `0x1b` from the target-category bitmask byte at `0x1d`, separate from unit class at `0x18`. Added `aiUnitType` and `aiUnitTargetMask` as candidates; both need an in-tree consumer/value audit before approval. |
| 2026-09-25 | Does InfiniteCOs identify the `0x085D3DD0` table? | Yes: it calls the corresponding table `COStatsAbilitiesDatatable`, gives each CO record a 260-byte stride (`0x104`), and labels the 0x080430B0 replacement as switching from per-unit-class to per-unit boosts. Added `gCOStatsAbilities` / `struct COStatsAbilities` candidates and kept the exact unknown. |
| 2026-09-25 | Does `sub_0802DCB4` control the map cursor? | Not directly. It is `RunMapStateMachine` case 13's per-button action dispatcher (menus, info windows, unit select/attack); the user's playtested behavior matches every branch. Actual D-pad cursor movement and scrolling are delegated to the still-asm `sub_0802776C(0)` in its fallback tail — that is the better next promotion target for cursor-movement specifically. |
| 2026-09-25 | What hides PPSWC (player-phase side windows + cursor) when a window opens? | No dedicated "hide" call found yet. Leading hypothesis: it is a side effect of leaving `RunMapStateMachine` case 13 (`sub_0802D458`'s `IncrementMapLock()` is the concrete lead), not an explicit flag write. `sub_0802C2B4`'s BG0 disable on the Start/minimap path is a second, narrower lead. Needs confirmation from whatever reads the map lock or `gUnknown_030032D8` to gate rendering. |
| 2026-09-25 | Does `gUnknown_030040D8->unk01 & 1` gate "units you do not control" in `sub_0802E4B4`'s A-button handler? | Plausible given the user's report that A on an uncontrolled unit behaves differently, but not confirmed from the field's own evidence trail — the branch it gates (fall through to `OpenMapMenu`) doesn't obviously match "starts movement selection but can't confirm placement." Keep at Research; do not adopt an `isEnemyOrDone`-style name without auditing other `unk01` bit-0 readers. |
| 2026-09-25 | Is `gUnknown_03003334` one semantic "input/select mode" variable? | Unconfirmed. It is written `1` by the A-handler (`sub_0802E4B4`), `6` by the B-handler (`sub_0802E2D0`), and separately to `gpKeySt->held & 2` by an unrelated function (`sub_0802E278`). Do not treat these as the same use without a full writer/reader audit. |
| 2026-09-26 | Were `Unk085D3DD0`, `Unk085D3DD0Entry` and `Unk084995A0` renamed? | Yes, to `CoData`, `CoModeData` and `PropertyListEntry`, the names from npiriou's PR #1. Tag renames only; both builds reproduce the ROM. The rows above still use the old tags. `COStatsAbilities` remains a candidate for `CoData` if the evidence here favours it. |

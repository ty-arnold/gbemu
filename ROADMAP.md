# Game Boy Emulator — Roadmap

Target for v1.0: **Tetris is playable, sound off.**
That's milestones 0–11. Audio (APU) and cycle-accurate mid-scanline PPU timing
are deliberately out of scope for v1.

## Core principle

**Never write more than one unverified subsystem at a time.**

Emulators fail in a nasty way: a wrong flag bit in one opcode surfaces 400,000
instructions later as a blank screen, with nothing pointing back at the cause.
Checkpoints with external verification are the only practical defense.

Corollary: **use the test ROMs from day one.** Blargg's `cpu_instrs` suite is a
Game Boy program that tests the CPU against real hardware and reports pass/fail.
Don't guess whether the opcodes are right.

## Checkpoints

Each row ends in something observable. Don't start row N+1 until row N's
"done when" is true.

| #  | Milestone                           | Done when                                                        | New C skill                              |
|----|-------------------------------------|------------------------------------------------------------------|------------------------------------------|
| 0  | ✅ **Multi-file project**           | `make` builds `main.c` + `cart.c` into one binary                | `.h`/`.c` split, include guards, Makefile |
| 1  | **ROM loader**                      | Any ROM loads to a heap buffer; header prints; `free` runs clean | `malloc`/`free`, `fseek`/`ftell`, out-params |
| 2  | **Memory bus**                      | `bus_read8(0x0100)` returns the byte seen in the hex dump        | address decoding, region dispatch        |
| 3  | **CPU state**                       | `AF/BC/DE/HL` readable as pairs *and* halves; flags set/clear    | `union`, bit masks, `<<` `>>` `&` `\|`    |
| 4  | **Fetch-decode-execute + ~25 ops**  | Logs `PC`, opcode, registers each step; hand-verified vs. disassembly | big `switch`, function pointers (optional) |
| 5  | **Blargg `01-special.gb` passes**   | Serial output prints `Passed`                                    | debugging by differential testing        |
| 6  | **All 11 `cpu_instrs` tests pass**  | All print `Passed` — **the real milestone**                      | sustained systematic debugging           |
| 7  | **Timers + interrupts**             | `instr_timing.gb` passes; `HALT` works                           | cycle counting, IF/IE registers          |
| 8  | **PPU: background only**            | Nintendo logo renders; dump to a `.ppm` file first               | 2bpp tile decoding, scanline timing      |
| 9  | **Window + SDL2**                   | Live 160x144 window at ~60 fps                                   | linking a third-party library            |
| 10 | **Joypad input**                    | Tetris is playable                                               | input polling, interrupt-driven I/O      |
| 11 | **Sprites**                         | Tetris pieces and Mario render correctly                         | OAM, priority, flipping                  |
| 12 | **MBC1 / MBC3 banking**             | Pokemon Blue boots                                               | bank switching, RAM persistence          |

**Steps 4–6 hold roughly 70% of the total difficulty.** Budget accordingly.
Once `cpu_instrs` passes, the CPU is known-good and every later bug is provably
somewhere else. That is an enormous debugging advantage.

## Test ROM selection

Check byte `0x147` before trusting any ROM as a test case.

- **Tetris** — cart type `0x00` (ROM ONLY), 32 KiB, no banking. Correct first target.
- **Pokemon Blue** — cart type `0x13` (MBC3+RAM+BATTERY). Needs banking, full PPU,
  sprites, timers, interrupts. That's milestone 12, not milestone 1.

## Resources

- **Pandocs** — <https://gbdev.io/pandocs> — the reference.
- **gbops** — <https://izik1.github.io/gbops> — opcode table with exact cycles and
  flag effects. You'll live here during step 4.
- **Blargg's test ROMs** — <https://gbdev.gg8.se> — get these *before* step 4.
- **The Ultimate Game Boy Talk** (33c3, YouTube) — one hour; the PPU section saves
  about a week at step 8.

## Carried over from `romheader`

`parse_header`, the `RomHeader` struct, and the three `decode_*` tables survive
into milestone 1 essentially unchanged.

## Open design questions

- **Milestone 3:** represent register pairs as a `union`, or as two `uint8_t`s with
  shift-and-mask helpers? Trade-offs differ, and the `union` approach has a
  portability catch (byte order) worth understanding before committing.

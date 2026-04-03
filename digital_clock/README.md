# Digital Clock

A 7-segment digital clock built with SDL3.

## Preview

![Digital Clock](../imgs/digital-clock.png)

## What It Does

- Draws six 7-segment digits (HH:MM:SS)
- Reads local system time every frame
- Renders colon separators between hour, minute, and second

## Controls

- Close window: quit

## Dependencies

- GCC (or compatible C compiler)
- SDL3 development libraries

## Build

From project root:

```bash
gcc digital_clock/main.c digital_clock/clock.c -o digital_clock/digital_clock -lSDL3
```

From inside folder:

```bash
cd digital_clock
gcc main.c clock.c -o digital_clock -lSDL3
```

## Run

From project root:

```bash
./digital_clock/digital_clock
```

From inside folder:

```bash
./digital_clock
```

## Notes

- Segment sizes and screen dimensions are defined in `clock.h`.
- Rendering logic lives in `clock.c`.

# Fluid Simulation

A simple grid-based falling-fluid simulation built with SDL3.

## Preview

![Fluid Simulation](../imgs/fluid-v1.png)

## What It Does

- Uses a fixed grid of cells (`empty`, `fluid`, `wall`)
- Updates fluid cells each frame so they fall downward if space is empty
- Lets you paint the grid in real time

## Controls

- Left mouse drag: paint selected cell type
- `F`: switch brush to fluid
- `W`: switch brush to wall
- `E`: switch brush to empty (eraser)
- `C`: clear grid
- `Esc`: quit

## Dependencies

- GCC (or compatible C compiler)
- SDL3 development libraries

## Build

From project root:

```bash
gcc fluid/fluid.c -o fluid/fluid -lSDL3
```

From inside folder:

```bash
cd fluid
gcc fluid.c -o fluid -lSDL3
```

## Run

From project root:

```bash
./fluid/fluid
```

From inside folder:

```bash
./fluid
```

## Notes

- Grid and cell constants are at the top of `fluid.c`.
- This version models vertical falling only (no sideways spreading).

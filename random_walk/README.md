# Random Walk

A multi-agent random walk visualizer built with SDL3.

## Preview

![Random Walk](../imgs/random-walk-v1.png)

## What It Does

- Spawns multiple agents at the screen center
- Moves each agent in one random cardinal direction per step
- Uses HSL-based color generation so paths are easy to distinguish

## Usage

You can pass the number of agents as an argument.

```bash
./random_walk 20
```

If no argument is provided, it defaults to 5 agents.

## Dependencies

- GCC (or compatible C compiler)
- SDL3 development libraries
- Math library (`-lm`)

## Build

From project root:

```bash
gcc random_walk/random_walk.c -o random_walk/random_walk -lSDL3 -lm
```

From inside folder:

```bash
cd random_walk
gcc random_walk.c -o random_walk -lSDL3 -lm
```

## Run

From project root:

```bash
./random_walk/random_walk <num_of_agent>
```

From inside folder:

```bash
./random_walk <num_of_agent>
```

## Notes

- `SCALE` controls step length and visual density.
- Movement mode flags (`canFreelyMove`, `only4Dir`) are defined in `main`.

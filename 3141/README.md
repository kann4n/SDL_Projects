# 3141 - Pi From Collisions

This project is a small SDL simulation of a famous physics/math idea:
you can recover digits of pi by counting perfectly elastic collisions.

## The story

Imagine a frictionless 1D track with:

- a wall on the left,
- a small block near that wall,
- a large block moving left toward the small block.

Every collision is perfectly elastic (no energy loss):

- small block hits wall,
- blocks hit each other,
- repeat.

If the mass ratio is chosen carefully, the total number of collisions
matches leading digits of pi.

The classic result is:

$$
\text{collisions} = \left\lfloor \pi \cdot \sqrt{\frac{M}{m}} \right\rfloor
$$

where:

- $M$ is the big block mass,
- $m$ is the small block mass.

When $M/m = 100^{n-1}$, the collision count gives the first $n$ digits of pi
(without the decimal point).

Examples:

- $M/m = 1$ -> about `3`
- $M/m = 100$ -> about `31`
- $M/m = 10000$ -> about `314`
- $M/m = 1000000$ -> about `3141`

That is why this folder is named `3141`.

## What this program does

In `3141.c`:

- `small_box.mass = 1`
- `big_box.mass = 10000`

So defaults are set near the `314`.

The simulation advances in many micro-steps each frame (`steps = 100000`) to
reduce missed collisions and keep the numerical behavior stable.

## Build

From this folder:

```bash
gcc 3141.c -o 3141 -lSDL3
```

## Run

```bash
./3141
```

You should see two blocks, a wall, and a collision counter rendered on screen.

## Tune for more digits

To chase more digits, increase `big_box.mass` in `3141.c`.

- For around `3141`, use `big_box.mass = 1000000`.

Higher mass ratios usually require:

- more simulation micro-steps,
- more CPU time,

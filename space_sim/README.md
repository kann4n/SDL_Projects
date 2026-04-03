# Space Sim

A simple 2D solar system simulation built with SDL3.

## Preview

![Space Sim](../imgs/space-sim-v0.1.png)

Video demo: [Space Sim Demo](../vids/spacesim-aplhachanged.mp4)

The simulation uses parent-child orbital relationships:
- Sun is the root body.
- Mercury, Venus, Earth, and Mars orbit the Sun.
- Moon orbits Earth.

Positions are updated every frame using `delta_time` and trigonometry (`cosf`, `sinf`).

## Features

- Hierarchical orbits (body can orbit another body)
- Time-based motion using frame delta
- Texture-based rendering with SDL3_image
- Camera zoom with mouse wheel
- Optional orbit path rendering

## Controls

- Mouse wheel up: zoom in
- Mouse wheel down: zoom out
- Close window: exit simulation

## Requirements

- GCC (or compatible C compiler)
- SDL3 development libraries
- SDL3_image development libraries
- Math library (`-lm`)

On Debian/Ubuntu-like systems, package names are typically:
- `libsdl3-dev`
- `libsdl3-image-dev`

## Build

Run from the project root:

```bash
gcc space_sim/space_sim.c space_sim/body.c -o space_sim/space_sim -lSDL3 -lSDL3_image -lm
```

Or run from inside the folder:

```bash
cd space_sim
gcc space_sim.c body.c -o space_sim -lSDL3 -lSDL3_image -lm
```

## Run

From project root:

```bash
./space_sim/space_sim
```

From inside the folder:

```bash
./space_sim
```

## Assets

Textures are loaded from `../imgs/assets/` in the following order:
1. sun.png
2. mercury.png
3. venus.png
4. earth.png
5. moon.png
6. mars.png

Make sure you run the executable from a working directory where this relative path is valid.

## Configuration

You can adjust constants in `body.h`:
- `SCREEN_WIDTH`
- `SCREEN_HEIGHT`
- `DRAW_ORBITS` (set to `1` to draw orbits)

You can tune planet parameters in `space_sim.c` inside the `bodies` array:
- `orbit_speed`
- `orbit_radius`
- `rect` size

## Notes

- Orbit textures are created once at startup when `DRAW_ORBITS` is enabled.
- The simulation currently does not include panning; camera position is fixed with zoom support.
- Reference notes about real planetary masses are in `massOfPlanets.md`.

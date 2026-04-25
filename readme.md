# SDL Projects


A collection of interactive simulations and games built with SDL3.

## Setup

### Install SDL3

Arch Linux:

```bash
sudo pacman -S sdl3
```

Windows (MSYS2 UCRT64 shell):

```bash
pacman -S mingw-w64-ucrt-x86_64-SDL3
```

### Build and run (example)

From the project root:

```bash
gcc doom/doom.c doom/kengine.c -o game -lSDL3 -lm && ./game
```

### Screenshots

<div style="display: flex; flex-wrap: wrap; gap: 20px">

<div>
    <h3>Fluid</h3>
    <img src="imgs/fluid-v1.png" width="400"/>
</div>

<div>
    <h3>Random Walk</h3>
    <img src="imgs/random-walk-v1.png" width="400"/>
</div>

<div>
    <h3>Space Sim</h3>
    <img src="imgs/space-sim-v0.1.png" width="400"/>
</div>

<div>   
    <h3>Digital Clock</h3>
    <img src="imgs/digital-clock.png" width="400"/>
</div>

<div>
    <h3>Doom</h3>
    <img src="imgs/raycaster-game001.png" width="400"/>
</div>

<div>    
    <h3>3141</h3>
    <img src="imgs/3141.png" width="400"/><br>
    [But Why?](https://www.youtube.com/watch?v=jsYwFizhncE)
</div>

</div>

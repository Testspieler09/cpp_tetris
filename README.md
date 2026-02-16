# Tetris with SRS (Super Rotation System)

A Tetris implementation in C++17 with the [Super Rotation System](https://harddrop.com/wiki/SRS), featuring hold/swap functionality and a clean separation between game logic and rendering.

## Controls

| Key   | Action                   |
| ----- | ------------------------ |
| ←/→   | Move left/right          |
| ↓     | Soft drop (faster fall)  |
| ↑     | Hard drop (instant fall) |
| X     | Rotate clockwise         |
| Z     | Rotate counter-clockwise |
| Space | Hold/swap piece          |
| R     | Restart game             |

> [!NOTE]
>
> You can use vim keys `[h j k l]` instead of the arrow keys to move and drop the pieces

## Scoring

- Lines cleared: 40 (1 line) / 100 (2) / 300 (3) / 1200 (4) \* level
- Soft drop: +1 point per cell
- Hard drop: +2 points per cell
- Level up: Every 10 lines cleared

## Features

- [x] Super Rotation System (SRS) with proper wall kicks
- [x] Hold/Swap piece functionality (once per piece)
- [x] Ghost piece showing landing position
- [x] 7-bag randomizer (modern Tetris standard)
- [x] Next piece preview (2 pieces shown)
- [x] Scoring system with level progression
- [x] Soft drop and hard drop
- [x] Game over detection
- [ ] Puzzle mode ("wallkick introduction")

## Build Instructions

### Prerequisites

- C++17 compatible compiler (g++, clang++)
- Git (for submodules)
- CMake

### Setup

1. **Clone and initialize submodules**

If not cloned already run:

```bash
git clone --recurse-submodules https://github.com/Testspieler09/cpp_tetris.git
# or with ssh
git clone --recurse-submodules git@github.com:Testspieler09/cpp_tetris.git
```

Otherwise run:

```bash
git submodule update --init --recursive
```

2. **Configure the project (CMake)**

From the project root:

```bash
mkdir build
cd build
cmake ..
```

You only need to run this step once (unless you change CMakeLists.txt).

3. **Build the project**

```bash
cmake --build .
```

4. **Run the game**

From inside the `build` directory:

```bash
./tetris
```

Or from project root:

```bash
./build/tetris
```

5. **Clean build files**

From the project root:

```bash
rm -rf build
```

CMake keeps all build artifacts inside the `build/` directory, so deleting it fully resets the project.

## Troubleshooting

### Compilation errors about `raylib.h`

Make sure submodules are initialized:

```bash
git submodule update --init --recursive
```

Then completely reconfigure:

```bash
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```

### CMake cannot find raylib

Ensure the `external/raylib` directory exists and contains a `CMakeLists.txt` file.
If it's missing, re-run:

```bash
git submodule update --init --recursive
```

### Linker errors on macOS

Make sure you have Xcode Command Line Tools installed:

```bash
xcode-select --install
```

If issues persist, try a clean rebuild:

```bash
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```

### Need to rebuild everything?

```bash
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```

## License

See LICENSE file for details.

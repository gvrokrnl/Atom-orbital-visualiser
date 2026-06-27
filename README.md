# Atom Orbital Visualiser

A 3D atom orbital visualiser written in C using OpenGL, GLFW and GLAD.

## Screenshots

### Orbitals


## Features

* Interactive 3D visualization of atomic orbitals
* Adjustable quantum numbers (n, l, m)
* Mouse-controlled camera rotation
* Transparency adjustment
* Multiple orbital coloring modes
* Real-time rendering

## Controls

* Left Mouse Button + Drag — Rotate the camera
* R - Open the settings menu (in console)
* N, L, M - Change quantum numbers
* Alpha - Adjust orbital transparency
* Color Type - Switch between coloring modes

## Requirements

* C compiler (GCC or Clang)
* OpenGL
* GLFW 3.x
* GLAD

## Build

### Linux (Debian/Ubuntu)

Install dependencies:
```bash
sudo apt-get update
sudo apt-get install -y libglfw3-dev libgl1-mesa-dev build-essential
```

Build:
```bash
gcc main.c window.c shader.c mesh.c renderer.c orbital.c glad/src/gl.c \
-Iglad/include -o main -lglfw3 -lGL -lm
```

### Linux (Fedora/RHEL)

Install dependencies:
```bash
sudo dnf install glfw-devel mesa-libGL-devel gcc
```

Build:
```bash
gcc main.c window.c shader.c mesh.c renderer.c orbital.c glad/src/gl.c \
-Iglad/include -o main -lglfw3 -lGL -lm
```

### macOS

Install dependencies:
```bash
brew install glfw3
```

Build:
```bash
gcc main.c window.c shader.c mesh.c renderer.c orbital.c glad/src/gl.c \
-Iglad/include -o main -lglfw3 -framework OpenGL -lm
```

### Windows (MinGW)

```bash
gcc main.c window.c shader.c mesh.c renderer.c orbital.c glad/src/gl.c \
-Iglad/include -o main.exe -L. -lglfw3 -lopengl32 -lgdi32
```

### Windows (Visual Studio)

```bash
cl main.c window.c shader.c mesh.c renderer.c orbital.c glad/src/gl.c \
/I glad/include /link glfw3.lib opengl32.lib user32.lib
```

## Run

### Linux/macOS
```bash
./main
```

### Windows
```bash
main.exe
```

## License

MIT License - see [LICENSE](LICENSE) file for details

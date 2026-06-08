# Engine

This repository contains laboratory exercises and supporting code for the FER Engine course.

## Description

The project is a C++23 and CMake codebase for interactive computer graphics. It uses OpenGL together with GLFW and Assimp for rendering, window management, input, and model loading.

## Laboratory Exercises

The labs cover the graphics pipeline from basic shader rendering to more advanced topics such as model loading, scene handling, culling, texturing, curves, and shadows.

## Build

```bash
cmake -S . -B build
cmake --build build
```

Each lab is built as a separate executable under `src/LabXX`.

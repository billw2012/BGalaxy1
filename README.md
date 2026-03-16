# BGalaxy1 — GPU-Accelerated N-Body Galaxy Simulator

A real-time gravitational N-body simulator capable of modelling colliding galaxies with hundreds of thousands of particles, running entirely on the GPU via OpenCL.

## What It Does

The simulation models gravitational interaction between large numbers of bodies (stars) using the **Barnes-Hut algorithm** — an O(n log n) approximation that groups distant bodies into a single mass, making real-time simulation of galaxy-scale systems feasible. Each simulation step:

1. Builds an octree (Barnes-Hut tree) on the CPU over the current particle positions
2. Uploads the tree to the GPU
3. Runs an OpenCL kernel to compute gravitational acceleration for every particle in parallel
4. Runs a second OpenCL kernel to integrate velocity and position (Euler integration)
5. Signals the Qt GUI to render the updated positions via OpenGL

The result is a visually compelling simulation of galaxies forming, rotating, and colliding in real time.

## Technical Highlights

- **Barnes-Hut octree** — O(n log n) gravity approximation implemented in C++ with GPU-side tree traversal using a work-group shared stack
- **Dual OpenCL kernels** — gravity computation and body movement each run as separate parallel GPU dispatches; supports both `float` and `double` precision via compile-time defines
- **OpenCL on CUDA hardware** — uses the Khronos standard `cl_khr_fp64` extension, compatible with NVIDIA GPUs via the CUDA OpenCL runtime
- **Qt5 GUI with OpenGL rendering** — real-time 3D particle rendering via QGLWidget, interactive controls for adding galaxies, adjusting parameters, and controlling simulation speed
- **Simulation serialisation and playback** — records simulation frames to disk in a compressed format using BFGS-optimised Hermite spline interpolation (via dlib) for smooth scrubbing/playback
- **Multi-threaded architecture** — simulation runs on a dedicated boost::thread; the GUI remains responsive via Qt signals

## Build Stack

| Component | Version |
|-----------|---------|
| Compiler  | MSVC 2022 (v143), x64 |
| Qt        | 5.15 (MSVC 2019 x64) |
| OpenCL    | CUDA Toolkit (NVIDIA) |
| OpenCV    | 4.x (vcpkg) |
| Boost     | boost-thread, boost-random (vcpkg) |
| dlib      | vcpkg |

## Project Structure

```
BGalaxy1/
├── Galaxy1/
│   ├── Galaxy1.sln           # VS2022 solution
│   └── Galaxy1/
│       ├── GalaxySim.cpp/h   # Core simulation logic
│       ├── BHTree.cpp/h      # Barnes-Hut octree
│       ├── GalaxySimGLWidget # OpenGL rendering
│       ├── GalaxySerializer  # Record/playback with spline compression
│       └── Data/CL/
│           ├── gravity.cl    # GPU gravity kernel
│           └── move.cl       # GPU integration kernel
└── MyLibs/
    ├── Math/                 # Header-only math library (vectors, AABB, etc.)
    ├── Utils/                # WaitHandle, HRTimer
    ├── Ugly/                 # Qt utility widgets (QDebugStream, ProcessQueue)
    └── hpalib/               # High-precision arithmetic library
```

## History

Originally written circa 2012–2013 as an exploration of GPU-accelerated physics simulation. Modernised in 2026 from VS2010/Qt4/Win32 to VS2022/Qt5/x64 for portfolio revival.

# 🧩 OpenGraphics

**OpenGraphics** is a modular and extensible 3D rendering suite written in modern C++20.
It is designed to serve both as a foundation for real-time rendering applications and as a learning tool for students and enthusiasts entering the world of Computer Graphics.

---

## 🚀 About the Project

OpenGraphics is the evolution of a previous academic project, **OpenEngine**, originally developed as a Bachelor Thesis project. This new version is being actively refactored and extended to serve as an accessible and flexible framework for building and experimenting with real-time 3D rendering systems.

The project emphasizes modularity, clarity, and a smooth learning curve, inspired by the intuitive Scripting API of Unity. It aims to be a practical entry point for beginners in Computer Graphics.

---

## 🧠 Key Features

- **Modular Architecture** – Cleanly separated components and systems to ease extensibility and experimentation.
- **Modern C++ Codebase** – Developed using C++20 and CMake with an emphasis on maintainable and extendable code.
- **OpenGL 4.5 with DSA** – Utilizes OpenGL 4.5 with Direct State Access for improved code structure and easier migration of concepts to modern graphics APIs.
- **Unity-Inspired API** – The API design draws from Unity's scripting model to make development intuitive for learners.

---

## 🗃️ Project Structure

```
OpenGraphics/
├── OpenEngine/                # The render engine, as a shared library
│   ├── shaders/               # OpenEngine shader library
│   ├── src/                   # Source code folder
│   │   ├── OpenEngine/        # OpenEngine source code, divided in several systems
│   │   ├── ogpch.h            # Pre-compiled header
│   │   └── OpenEngine.h       # OpenEngine uber-include
│   ├── vendor/                # Third-party libraries (GLFW, glad, glm, Assimp, stb_image...)
│   └── CMakeLists.txt         # Engine-specific CMake configuration
├── Sandbox/                   # Dynamic application to explore recently added features
│   ├── assets/                # Assets to be used by the app
│   ├── src/                   # Source code folder
│   │   └── SandboxApp.cpp     # Example of app which uses OpenEngine
│   └── CMakeLists.txt         # Sandbox-specific CMake configuration
└── CMakeLists.txt             # OpenGraphics workspace CMake configuration
```

---

## 🛠️ Build Instructions

### Requirements
- C++20 compatible compiler
- CMake 3.28 or higher
- OpenGL 4.5

### Dependencies
The following libraries are included as Git submodules:
- [GLFW](https://github.com/glfw/glfw)
- [glm](https://github.com/g-truc/glm)
- [assimp](https://github.com/assimp/assimp)

Make sure to clone the repository with submodules:

```bash
git clone --recurse-submodules https://github.com/Frollo24/OpenGraphics.git
```

It is highly recommended, being a workspace designed for beginners in Computer Graphics, to open the folder containing the project (where the root CMakeLists.txt file is) with an IDE such as Visual Studio Community or CLion. However, if you have already used CMake before and you feel comfortable with it, you can try your luck using a series of commands similar to:

```bash
cd OpenGraphics
cmake -S . -B build
cmake --build build
```

---

## 🤝 Contributing

Contributions, ideas, and feedback are welcome. If you'd like to propose changes or improvements, feel free to open an issue or discussion. Code contributions will be evaluated to ensure they align with the project's educational goals and intended level of complexity.

---

## 📝 License

This project currently has no formal license. You are free to clone, modify, and fork it as you wish, provided proper credit is given to the author.

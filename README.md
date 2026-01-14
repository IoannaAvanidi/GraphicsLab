# OpenGL Maze & Treasure Hunt Game

A 3D interactive game developed as part of the **Computer Graphics and Interaction Systems** course at the **University of Ioannina (2024-2025)**. This project demonstrates the use of modern OpenGL for rendering, camera control, and game logic.

## 🚀 Project Overview

The application features a 3D maze environment where the player controls a character to collect treasures. It was built incrementally through three development phases:
* **Phase A:** 2D Maze generation and basic keyboard movement.
* **Phase B:** Transition to 3D rendering (cubes) and implementation of a 3D camera.
* **Phase C:** Dynamic treasure spawning, texture mapping, and advanced lighting.

## ✨ Key Features

* **3D Maze Engine:** Renders a maze from a 2D grid using blue cubes for walls.
* **Character Controller:** Move a yellow cube (Character A) using the **I, J, K, L** keys.
* **Treasure Mechanics:** A treasure (Object B) appears at random locations with a coin texture; it shrinks and vanishes upon collection.
* **Advanced Camera:** * Rotation (W/X for X-axis, Q/Z for Y-axis).
    * Panning (G/H for X-axis, T/B for Y-axis).
    * Zoom (Numerical +/-).
* **Visuals:** Implementation of the Phong lighting model and texture mapping using `STB_IMAGE`.

## 🛠️ Tech Stack

* **Language:** C++
* **Graphics API:** OpenGL 3.3+
* **Libraries:** * **GLFW:** Window & Input management.
    * **GLEW:** OpenGL Extension Wrangler.
    * **GLM:** OpenGL Mathematics (Linear Algebra).

## 🎮 Controls

| Key | Action |
| :--- | :--- |
| **I / K** | Move Up / Down |
| **J / L** | Move Left / Right |
| **W / X** | Rotate Camera (X-axis) |
| **Q / Z** | Rotate Camera (Y-axis) |
| **+ / -** | Zoom In / Out |
| **G / H** | Pan Left / Right |
| **T / B** | Pan Up / Down |
| **SPACE** | Exit Application |

## 📦 Installation & Build

1. Ensure you have the **GLFW**, **GLEW**, and **GLM** libraries installed.
2. Compile the source file:
   ```bash
   g++ Source-1C.cpp -lGLEW -lglfw -lGL -o TreasureHunt

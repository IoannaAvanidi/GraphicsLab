# OpenGL 2D Maze Game

[cite_start]A 2D graphics application developed using **OpenGL 3.3** for the "Computer Graphics and Interaction Systems" course (2024-2025) at the University of Ioannina[cite: 1171, 1173, 1175].

## 🎮 Project Overview

[cite_start]The objective of this project is to create a windowed application that renders a 2D maze and a movable character[cite: 1174, 1176]. [cite_start]The maze is represented by a 2D array of 0s and 1s, where "1" represents a wall and "0" represents a path[cite: 1186].

### Core Mechanics
* [cite_start]**Maze Design**: The maze consists of blue squares sitting on the XY plane with a Z component of 0[cite: 1186, 1191]. [cite_start]The center of the maze is at point (0,0)[cite: 1188].
* [cite_start]**Character A**: A yellow square with a side length of 0.5, initially placed at the maze entrance[cite: 1178, 1215, 1216].
* [cite_start]**Collision Logic**: Character A is restricted from passing through walls or leaving the maze boundaries[cite: 1224].

## ⌨️ Controls

* **Movement**:
    * [cite_start]`I`: Move Up [cite: 1223]
    * [cite_start]`K`: Move Down [cite: 1222]
    * [cite_start]`J`: Move Left [cite: 1221]
    * [cite_start]`L`: Move Right [cite: 1220]
* [cite_start]**Exit**: Press `Q` to terminate the application[cite: 1184].

## 🚀 Key Features

* [cite_start]**Geometric Rendering**: Walls are rendered using triangle primitives, with all vertex coordinates manually determined and stored in arrays[cite: 1187, 1189].
* [cite_start]**Black Background**: The 750x750 application window features a consistent black background[cite: 1182].
* [cite_start]**Pure OpenGL**: Developed strictly using the **GLFW**, **GLEW**, and **GLM** libraries[cite: 1231].

## 🛠️ Technical Specifications

* [cite_start]**Window Resolution**: 750 x 750 pixels[cite: 1182].
* [cite_start]**Window Title**: "Άσκηση 14 - 2024"[cite: 1183].
* [cite_start]**Coordinate Mapping**: Each grid square has a side length of 1[cite: 1188].

---
[cite_start]*This project was implemented for the Department of Computer Science & Engineering at the University of Ioannina.* [cite: 1171, 1172]

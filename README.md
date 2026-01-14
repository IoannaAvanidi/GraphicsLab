# OpenGL Assignment 1-C: Treasure Hunt

[cite_start]An interactive 3D game application developed using **OpenGL 3.3+**, extending the previous 3D maze project with game mechanics and texturing. [cite: 1184, 1185, 1186]

## 🎮 Game Mechanics

* [cite_start]**Maze & Character**: Features a 3D maze made of blue cubes (side length 1) and a yellow cube character (A) with a side length of 0.5. [cite: 1198, 1199]
* [cite_start]**Treasure Spawning**: A treasure object (B) appears periodically in random, valid positions within the maze. [cite: 1202, 1204]
* [cite_start]**Collision & Collection**: When Character A touches the treasure, the treasure shrinks to half its size before disappearing. [cite: 1209]
* [cite_start]**Treasure Design**: The treasure is a 0.8-scale cube textured with `coins.jpg`. [cite: 1206, 1207]

## 🚀 Key Features

* [cite_start]**3D Texturing**: Implementation of texture mapping on 3D models using provided image files. [cite: 1207]
* [cite_start]**Randomized Logic**: Automatic generation of treasure coordinates that do not overlap with walls or the player's current position. [cite: 1202, 1203]
* [cite_start]**Interactive Camera**: A comprehensive 3D camera system supporting rotation, zooming, and panning. [cite: 1211, 1212]
* [cite_start]**Visual Transitions**: Dynamic scaling of objects to provide visual feedback during game interactions. [cite: 1209, 1210]

## 🛠️ Technical Details

* [cite_start]**Libraries**: Strictly developed using **GLFW**, **GLEW**, and **GLM**. [cite: 1231]
* [cite_start]**Initial Setup**: A 950x950 window with the camera positioned at (0.0, 0.0, 20.0) looking towards (0, 0, 0.25). [cite: 1193, 1219]
* [cite_start]**Transformation Strategy**: The project utilizes coordinate determination and vertex buffers to manage 3D shapes. [cite: 1208]

## ⌨️ Controls

* [cite_start]**Movement**: Use `I` (Up), `K` (Down), `J` (Left), and `L` (Right). [cite: 1153, 1154, 1155, 1156, 1192]
* **Camera Rotation**:
    * [cite_start]**X-axis**: `<w>` and `<x>` [cite: 1214]
    * [cite_start]**Y-axis**: `<q>` and `<z>` [cite: 1215]
* **Camera Panning**:
    * [cite_start]**X-axis**: `<g>` and `<h>` [cite: 1217]
    * [cite_start]**Y-axis**: `<t>` and `<b>` [cite: 1218]
* [cite_start]**Zoom**: Use `<+>` and `<->` keys on the numerical keypad. [cite: 1216]
* [cite_start]**Exit**: Press `SPACE` to close the application. [cite: 1196]

---
[cite_start]*This project was implemented for the Department of Computer Science & Engineering at the University of Ioannina, December 2024.* [cite: 1181, 1182, 1183, 1229]

# FEngine

## In a word

**FEngine** is a 3D engine originally made by 2 students full of curiosity and time.

It is **heavily** recommended to take a look at the [Official Notion](https://personnalflo.notion.site/FEngine-60c2663a4c6d421a9e158c33331ef499). As it is our note-taking and it summary a lot of the work that has been made.

It initially was planned to remain the graphic engine it was aimed to be but after realising how powerfull a tool like this is, we decided to explore more about the physic and math that hides behind our 3 dimensional world.

## Requirements

### Conda
The only dependcy of the project is conda. If your system has conda available, please refer to the following section.

The executable can then be compiled and executed using:

```bash
cd <path/of/the/project>
chmod +x run.sh setup.sh
./setup.sh
./run.sh
```

These scripts will create a "fengine" conda environment based on the environment.yml file present at the root of the project.

### Dependencies
FEngine has currently only be tested under linux-based OS but we aim to expand its reach to Windows as well.
The libs dependecies of the project:
* GLEW
* GLFW
* OpenGL
* GLM
* Assimp

The executable can be compiled and executed using:

```bash
cd <path/of/the/project>
chmod +x run.sh
./run.sh
```

## Usage
* **WASD** To move around
* **HJKL** To look around (based on vim-like behavior)

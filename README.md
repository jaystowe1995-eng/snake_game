# 🐍 Snake Game (OpenGL + GLUT)

A two-stage Snake game developed in **C++ using OpenGL and GLUT**.  
The player controls a snake that grows by eating food while avoiding obstacles and bombs.

The game becomes more challenging as the player progresses through stages.

---
## Gameplay Demo

### Stage 1 – Collect Food
- The player controls the snake using the **arrow keys**.
- The goal is to **eat 5 food items**.
- Each food increases the snake length and score.
- If the snake **hits the outer wall**, the player loses.
- After collecting 5 foods, the player advances to **Stage 2**.

![Stage 1 Demo](/gifs/stage1.gif)

---

### Stage 2 – Bomb Challenge
- The snake continues to eat food.
- The player must **eat 5 additional food items** to win the game.
- Bomb obstacles appear on the map.

![Stage 2 Demo](/gifs/stage2.gif)

---
### ⚠ Dangers

### Collision with Wall
If the snake hits the outer wall, the game ends.

![Wall Collision](/gifs/wall.gif)

---

### Bomb Explosion
If the snake hits a bomb, it explodes and the game ends.

![Bomb Collision](/gifs/bomb.gif)

---
#  Features

- Two gameplay stages
- Snake grows when eating food
- Bomb obstacles in Stage 2
- Adjustable difficulty
- Heads-Up Display (HUD)
- Pause and restart options
---


# Controls

| Key | Action |
|----|------|
| ⬆ ⬇ ⬅ ➡ | Move snake |
| `P` | Pause / Resume |
| `R` | Restart game |
| `M` | Change difficulty |
| `H` | Show help |
| `Q` or `Esc` | Quit game |
| `Enter` | Continue / Start stage |

---

🛠 Technologies Used
- C++
- OpenGL
- GLUT

Future Improvements

Possible future improvements:
- Bomb explosion animation
- Increasing snake speed over time
- Additional stages and obstacles
- Background music
- Power-up items
- Sound effects (eat and bomb)

---

## 📖 User Manual

### 🛠 Prerequisites
Before building the project, ensure you have a C++ compiler and the necessary OpenGL/GLUT libraries installed:

* **macOS**: Install Xcode Command Line Tools. GLUT is included by default.
* **Linux (Ubuntu/Debian)**: Run `sudo apt-get install build-essential freeglut3-dev`.
* **Windows**: It is recommended to use **`MSYS2`** or **`MinGW`** to install the `freeglut` package.

---

### How to Build
Open your terminal or command prompt in the project directory and run the following command:

**On macOS**:
```bash
g++ main.cpp -framework OpenGL -framework GLUT -o snake_game
```

**On Linux**:
```bash
g++ main.cpp -lGL -lGLU -lglut -o snake_game
```

### How to Run

Once the compilation is successful, execute the generated file:

**On macOS and Linux**:
```bash
./snake_game
```

**On Windows**:
```bash
snake_game.exe
```

How to Play

Start: Press Enter on the Menu Screen to enter the Stage 1 Title Screen. Press Enter again to start moving.

Navigation: Use your Arrow Keys to change the direction of the snake.

Objective:

Stage 1: Eat 5 food items while avoiding the outer walls.

Stage 2: Eat 5 more food items while avoiding the bombs and walls.

Difficulty: Press M during the game or on the menu to cycle through difficulty levels. Lower numbers make the snake move faster.

Pause: If you need a break, press P to toggle the pause state.

Restart: If you lose or want to start over, press R to return to the main menu.
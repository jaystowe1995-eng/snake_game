# 🐍 Snake Game (OpenGL + GLUT)

A two-stage Snake game developed in **C++ using OpenGL and GLUT**.  
The player controls a snake that grows by eating food while avoiding obstacles and bombs.

The game becomes more challenging as the player progresses through stages.

---
## 🎥 Gameplay Demo

### Stage 1 – Collect Food
The snake grows each time it eats food. The goal is to collect **5 food items** to advance to Stage 2.

![Stage 1 Demo](/gifs/stage1.gif)

---

### Stage 2 – Bomb Challenge
Bombs appear in the environment. The snake must continue collecting food while avoiding bombs.

![Stage 2 Demo](/gifs/stage2.gif)

---

### Collision with Wall
If the snake hits the outer wall, the game ends.

![Wall Collision](/gifs/wall.gif)

---

### Bomb Explosion
If the snake hits a bomb, it explodes and the game ends.

![Bomb Collision](/gifs/bomb.gif)

---
# 🎮 Features

- Two gameplay stages
- Snake grows when eating food
- Bomb obstacles in Stage 2
- Adjustable difficulty
- Heads-Up Display (HUD)
- Pause and restart options 
---

# 🧩 Game Stages

## Stage 1 — Food Challenge
- The player controls the snake using the **arrow keys**.
- The goal is to **eat 5 food items**.
- Each food increases the snake length and score.
- If the snake **hits the outer wall**, the player loses.
- After collecting 5 foods, the player advances to **Stage 2**.

---

## Stage 2 — Bomb Challenge
- The snake continues to grow by eating food.
- The player must **eat 5 additional food items** to win the game.
- Bomb obstacles appear on the map.

### ⚠ Dangers

**Bombs**
- If the snake touches a bomb, it explodes and the game ends.

**Walls**
- Hitting the outer wall also ends the game.

---

# 🎯 Controls

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
- macOS afplay (for sound effects)

🚀 Future Improvements

Possible future improvements:
- Bomb explosion animation
- Increasing snake speed over time
- Additional stages and obstacles
- Background music
- Power-up items
- Sound effects (eat and bomb)



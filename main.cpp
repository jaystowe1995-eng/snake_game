#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cmath>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

/**************************************************************************
 * Colors
 ***************************************************************************/
#define mapBgR 0.25f
#define mapBgG 0.90f
#define mapBgB 0.25f

#define snakeR 0.05f
#define snakeG 0.05f
#define snakeB 0.05f

#define wallR 0.15f
#define wallG 0.15f
#define wallB 0.15f

#define foodR 1.00f
#define foodG 0.15f
#define foodB 0.15f

#define hudR 0.00f
#define hudG 0.00f
#define hudB 0.00f

/**************************************************************************
 * Config
 ***************************************************************************/
int map_size = 24;
int maxDifficulty = 7;
int Difficulty = 6;
int gameSpeed = 200;

const int STAGE1_TARGET_SCORE = 5;
const int STAGE2_TARGET_SCORE = 10;

/**************************************************************************
 * Directions
 ***************************************************************************/
#define UP     1
#define DOWN   2
#define LEFT   3
#define RIGHT  4

/**************************************************************************
 * Screens / states
 ***************************************************************************/
enum ScreenState
{
    MENU_SCREEN,
    STAGE1_TITLE_SCREEN,
    PLAYING_STAGE1,
    STAGE2_TITLE_SCREEN,
    PLAYING_STAGE2,
    WIN_SCREEN,
    GAME_OVER_SCREEN
};

ScreenState screenState = MENU_SCREEN;

/**************************************************************************
 * Game state
 ***************************************************************************/
deque<pair<int,int>> snake_body;
int food_pos[2];
vector<pair<int,int>> bombs;

int direction = RIGHT;
int score = 0;
int paused = 1;
int foodAvailable = 0;

/**************************************************************************
 * Utility
 ***************************************************************************/
void updateGameSpeed()
{
    gameSpeed = Difficulty * 40;
}

void drawText(float x, float y, const string& text, void* font = GLUT_BITMAP_HELVETICA_18)
{
    glRasterPos2f(x, y);
    for(char c : text)
        glutBitmapCharacter(font, c);
}

float getBitmapTextWidth(const string& text, void* font)
{
    int pixelWidth = 0;
    for(unsigned char c : text)
        pixelWidth += glutBitmapWidth(font, c);

    float windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
    if(windowWidth <= 0.0f)
        return 0.0f;

    return (pixelWidth / windowWidth) * map_size;
}

void drawCenteredText(float y, const string& text, void* font = GLUT_BITMAP_HELVETICA_18)
{
    float textWidth = getBitmapTextWidth(text, font);
    float x = (map_size - textWidth) / 2.0f;
    drawText(x, y, text, font);
}

void drawSquare(float x1, float y1, float x2, float y2)
{
    glBegin(GL_POLYGON);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    glBegin(GL_POLYGON);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

void drawCircle(float cx, float cy, float r, int segments = 30)
{
    glBegin(GL_POLYGON);
    for(int i = 0; i < segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = r * cos(theta);
        float y = r * sin(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

void showFinalScore()
{
    cout << "***********************\n";
    cout << "Your Final Score is: " << score << "\n";
}

void guide()
{
    printf("\n*********************************************************");
    printf("\nArrow Keys = Move snake");
    printf("\nP = Pause / Resume");
    printf("\nR = Restart");
    printf("\nM = Change difficulty");
    printf("\nH = Help");
    printf("\nQ or Esc = Quit");
    printf("\nEnter = Continue from menu / title screens");
    printf("\n");
    printf("\nStage 1: Eat 5 food and grow. Avoid outer walls.");
    printf("\nStage 2: Eat 5 more food to win.");
    printf("\nAvoid bombs and outer walls in Stage 2.");
    printf("\n*********************************************************\n");
}

/**************************************************************************
 * Collision helpers
 ***************************************************************************/
bool isOuterWall(int x, int y)
{
    return (x <= 0 || x >= map_size - 1 || y <= 0 || y >= map_size - 1);
}

bool isOnSnake(int x, int y)
{
    for(auto part : snake_body)
    {
        if(part.first == x && part.second == y)
            return true;
    }
    return false;
}

bool isBomb(int x, int y)
{
    for(auto b : bombs)
    {
        if(b.first == x && b.second == y)
            return true;
    }
    return false;
}

bool isBlockedCell(int x, int y, bool includeBombs = true)
{
    if(isOuterWall(x, y)) return true;
    if(isOnSnake(x, y)) return true;
    if(includeBombs && isBomb(x, y)) return true;
    return false;
}

/**************************************************************************
 * Drawing game objects
 ***************************************************************************/
void drawBodyPart(int x, int y)
{
    glColor3f(snakeR, snakeG, snakeB);
    glBegin(GL_POLYGON);
        glVertex2f(x + 0.1f, y + 0.1f);
        glVertex2f(x + 0.9f, y + 0.1f);
        glVertex2f(x + 0.9f, y + 0.9f);
        glVertex2f(x + 0.1f, y + 0.9f);
    glEnd();
}

void drawSnake()
{
    if(snake_body.empty()) return;

    int hx = snake_body[0].first;
    int hy = snake_body[0].second;

    glColor3f(snakeR, snakeG, snakeB);
    glBegin(GL_POLYGON);
        glVertex2f(hx + 0.1f, hy + 0.1f);
        glVertex2f(hx + 0.9f, hy + 0.1f);
        glVertex2f(hx + 0.9f, hy + 0.9f);
        glVertex2f(hx + 0.1f, hy + 0.9f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    drawSquare(hx + 0.62f, hy + 0.62f, hx + 0.76f, hy + 0.76f);
    drawSquare(hx + 0.62f, hy + 0.26f, hx + 0.76f, hy + 0.40f);

    glColor3f(0.0f, 0.0f, 0.0f);
    drawSquare(hx + 0.67f, hy + 0.67f, hx + 0.72f, hy + 0.72f);
    drawSquare(hx + 0.67f, hy + 0.31f, hx + 0.72f, hy + 0.36f);

    for(size_t i = 1; i < snake_body.size(); i++)
        drawBodyPart(snake_body[i].first, snake_body[i].second);
}

void drawFoodShape(int x, int y)
{
    glColor3f(foodR, foodG, foodB);
    drawCircle(x + 0.42f, y + 0.48f, 0.23f, 24);
    drawCircle(x + 0.60f, y + 0.48f, 0.23f, 24);
    drawTriangle(x + 0.32f, y + 0.42f, x + 0.70f, y + 0.42f, x + 0.51f, y + 0.12f);

    glColor3f(0.35f, 0.20f, 0.05f);
    drawSquare(x + 0.49f, y + 0.70f, x + 0.54f, y + 0.92f);

    glColor3f(0.10f, 0.60f, 0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(x + 0.54f, y + 0.82f);
        glVertex2f(x + 0.76f, y + 0.90f);
        glVertex2f(x + 0.62f, y + 0.70f);
    glEnd();
}

void drawFood()
{
    if(foodAvailable)
        drawFoodShape(food_pos[0], food_pos[1]);
}

void drawBrick(int x, int y)
{
    glColor3f(wallR, wallG, wallB);
    drawSquare(x + 0.05f, y + 0.05f, x + 0.95f, y + 0.95f);

    glColor3f(0.28f, 0.28f, 0.28f);
    glBegin(GL_LINES);
        glVertex2f(x + 0.10f, y + 0.50f);
        glVertex2f(x + 0.90f, y + 0.50f);
        glVertex2f(x + 0.45f, y + 0.08f);
        glVertex2f(x + 0.45f, y + 0.50f);
        glVertex2f(x + 0.65f, y + 0.50f);
        glVertex2f(x + 0.65f, y + 0.92f);
    glEnd();
}

void drawOuterWalls()
{
    for(int i = 0; i < map_size; i++)
    {
        drawBrick(i, 0);
        drawBrick(i, map_size - 1);
        drawBrick(0, i);
        drawBrick(map_size - 1, i);
    }
}

void drawBomb(int x, int y)
{
    glColor3f(0.05f, 0.05f, 0.05f);
    drawCircle(x + 0.5f, y + 0.45f, 0.25f, 30);

    glColor3f(0.35f, 0.35f, 0.35f);
    drawCircle(x + 0.40f, y + 0.55f, 0.06f, 20);

    glColor3f(0.25f, 0.15f, 0.05f);
    drawSquare(x + 0.45f, y + 0.65f, x + 0.55f, y + 0.80f);

    glColor3f(0.30f, 0.20f, 0.05f);
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
        glVertex2f(x + 0.50f, y + 0.80f);
        glVertex2f(x + 0.60f, y + 0.92f);
        glVertex2f(x + 0.70f, y + 1.00f);
    glEnd();

    glColor3f(1.0f, 0.9f, 0.0f);
    drawCircle(x + 0.70f, y + 1.02f, 0.04f, 12);

    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x + 0.70f, y + 1.10f);
        glVertex2f(x + 0.66f, y + 1.02f);
        glVertex2f(x + 0.74f, y + 1.02f);

        glVertex2f(x + 0.80f, y + 1.03f);
        glVertex2f(x + 0.72f, y + 1.00f);
        glVertex2f(x + 0.72f, y + 1.06f);

        glVertex2f(x + 0.60f, y + 1.03f);
        glVertex2f(x + 0.68f, y + 1.06f);
        glVertex2f(x + 0.68f, y + 1.00f);
    glEnd();
}

void drawBombs()
{
    for(auto b : bombs)
        drawBomb(b.first, b.second);
}

void drawHUD(const string& stageLabel)
{
    glColor3f(hudR, hudG, hudB);

    string scoreText = "Score: " + to_string(score);
    string stageText = "Stage: " + stageLabel;
    string diffText  = "Difficulty: " + to_string((maxDifficulty + 1) - Difficulty);
    string pauseText = paused ? "Paused" : "Running";
    float hudY = map_size - 1.4f;

    drawText(4.0f,  hudY, scoreText, GLUT_BITMAP_HELVETICA_12);
    drawText(8.5f,  hudY, stageText, GLUT_BITMAP_HELVETICA_12);
    drawText(13.0f, hudY, diffText, GLUT_BITMAP_HELVETICA_12);
    drawText(20.0f, hudY, pauseText, GLUT_BITMAP_HELVETICA_12);
}

/**************************************************************************
 * Stage setup
 ***************************************************************************/
void placeFood()
{
    int fx, fy;
    do
    {
        fx = rand() % (map_size - 2) + 1;
        fy = rand() % (map_size - 2) + 1;
    }
    while(isBlockedCell(fx, fy));

    food_pos[0] = fx;
    food_pos[1] = fy;
    foodAvailable = 1;
}

void placeBombsStage2()
{
    bombs.clear();

    vector<pair<int,int>> fixedBombs = {
        {6, 5},
        {10, 8},
        {15, 6},
        {18, 14},
        {8, 17}
    };

    for(auto b : fixedBombs)
    {
        if(!isBlockedCell(b.first, b.second, false))
            bombs.push_back(b);
    }
}

void setupStage1()
{
    snake_body.clear();
    bombs.clear();

    direction = RIGHT;
    foodAvailable = 0;

    snake_body.push_back({4, map_size / 2});
    snake_body.push_back({3, map_size / 2});
    snake_body.push_back({2, map_size / 2});

    placeFood();
}

void setupStage2()
{
    snake_body.clear();

    direction = RIGHT;
    foodAvailable = 0;

    snake_body.push_back({4, map_size / 2});
    snake_body.push_back({3, map_size / 2});
    snake_body.push_back({2, map_size / 2});

    placeBombsStage2();
    placeFood();
}

/**************************************************************************
 * Screens
 ***************************************************************************/
void drawMenuScreen()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    const float centerY = map_size / 2.0f;

    drawCenteredText(centerY + 2.4f, "SNAKE GAME", GLUT_BITMAP_TIMES_ROMAN_24);
    drawCenteredText(centerY + 1.0f, "Two Main Stages", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 0.4f, "Press ENTER to continue", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 1.8f, "Stage 1: Food Challenge", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 3.2f, "Stage 2: Bomb Challenge", GLUT_BITMAP_HELVETICA_18);
}

void drawStage1TitleScreen()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    const float centerY = map_size / 2.0f;

    drawCenteredText(centerY + 4.2f, "SNAKE GAME", GLUT_BITMAP_TIMES_ROMAN_24);
    drawCenteredText(centerY + 2.8f, "STAGE 1", GLUT_BITMAP_TIMES_ROMAN_24);

    drawCenteredText(centerY + 1.2f, "Instructions:", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 0.2f, "Use Arrow Keys to move the snake", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 1.6f, "Eat 5 food items to clear Stage 1", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 3.0f, "Do not hit the outer walls", GLUT_BITMAP_HELVETICA_18);

    drawCenteredText(centerY - 4.6f, "Press ENTER to start", GLUT_BITMAP_HELVETICA_18);
}

void drawStage2TitleScreen()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    const float centerY = map_size / 2.0f;

    drawCenteredText(centerY + 4.2f, "STAGE 2", GLUT_BITMAP_TIMES_ROMAN_24);
    drawCenteredText(centerY + 2.8f, "BOMB CHALLENGE", GLUT_BITMAP_TIMES_ROMAN_24);

    drawCenteredText(centerY + 1.2f, "Instructions:", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 0.2f, "Use Arrow Keys to move the snake", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 1.6f, "Eat 5 more food items to win", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 3.0f, "Avoid the bombs", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 4.4f, "Avoid the outer walls", GLUT_BITMAP_HELVETICA_18);

    drawCenteredText(centerY - 5.8f, "Press ENTER to start", GLUT_BITMAP_HELVETICA_18);
}

void drawWinScreen()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    const float centerY = map_size / 2.0f;

    drawCenteredText(centerY + 2.1f, "YOU WIN!", GLUT_BITMAP_TIMES_ROMAN_24);
    drawCenteredText(centerY + 0.7f, "You completed both stages", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 0.7f, "Final Score: " + to_string(score), GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 2.5f, "Press R to Play Again", GLUT_BITMAP_HELVETICA_18);
}

void drawGameOverScreen()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    const float centerY = map_size / 2.0f;

    drawCenteredText(centerY + 2.1f, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
    drawCenteredText(centerY + 0.7f, "Final Score: " + to_string(score), GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 0.7f, "Press R to Restart", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(centerY - 2.1f, "Press Q or Esc to Quit", GLUT_BITMAP_HELVETICA_18);
}

/**************************************************************************
 * Gameplay logic
 ***************************************************************************/
void restartToMenu()
{
    score = 0;
    paused = 1;
    direction = RIGHT;
    foodAvailable = 0;
    snake_body.clear();
    bombs.clear();
    screenState = MENU_SCREEN;
}

void gameOver(const string& msg)
{
    cout << msg << "\n";
    showFinalScore();
    paused = 1;
    screenState = GAME_OVER_SCREEN;
    glutPostRedisplay();
}

void startStage1()
{
    paused = 1;
    score = 0;
    setupStage1();
    screenState = STAGE1_TITLE_SCREEN;
    glutPostRedisplay();
}

void startStage2()
{
    paused = 1;
    setupStage2();
    screenState = STAGE2_TITLE_SCREEN;
    glutPostRedisplay();
}

void moveSnake(int newDirection)
{
    if(screenState != PLAYING_STAGE1 && screenState != PLAYING_STAGE2) return;
    if(paused) return;

    direction = newDirection;

    int delX = 0, delY = 0;
    switch(direction)
    {
        case UP:    delY = 1; break;
        case DOWN:  delY = -1; break;
        case LEFT:  delX = -1; break;
        case RIGHT: delX = 1; break;
    }

    int newX = snake_body[0].first + delX;
    int newY = snake_body[0].second + delY;

    if(isOuterWall(newX, newY))
    {
        gameOver("You hit the wall.");
        return;
    }

    bool grow = (newX == food_pos[0] && newY == food_pos[1]);

    for(size_t i = 0; i < snake_body.size(); i++)
    {
        if(!grow && i == snake_body.size() - 1) continue;

        if(snake_body[i].first == newX && snake_body[i].second == newY)
        {
            gameOver("You hit yourself.");
            return;
        }
    }

    if(screenState == PLAYING_STAGE2 && isBomb(newX, newY))
    {
        gameOver("Boom! You hit a bomb.");
        return;
    }

    if(grow)
    {
        score++;
        foodAvailable = 0;
    }

    snake_body.push_front({newX, newY});

    if(!grow)
        snake_body.pop_back();

    if(!foodAvailable)
        placeFood();

    if(screenState == PLAYING_STAGE1 && score >= STAGE1_TARGET_SCORE)
    {
        startStage2();
        return;
    }

    if(screenState == PLAYING_STAGE2 && score >= STAGE2_TARGET_SCORE)
    {
        paused = 1;
        screenState = WIN_SCREEN;
        glutPostRedisplay();
        return;
    }

    glutPostRedisplay();
}

void TimerFunc(int)
{
    if(!paused && (screenState == PLAYING_STAGE1 || screenState == PLAYING_STAGE2))
        moveSnake(direction);

    glutTimerFunc(gameSpeed, TimerFunc, 0);
}

/**************************************************************************
 * Display
 ***************************************************************************/
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, map_size, 0, map_size);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(screenState == MENU_SCREEN)
    {
        drawMenuScreen();
    }
    else if(screenState == STAGE1_TITLE_SCREEN)
    {
        drawStage1TitleScreen();
    }
    else if(screenState == PLAYING_STAGE1)
    {
        drawOuterWalls();
        drawFood();
        drawSnake();
        drawHUD("1");
    }
    else if(screenState == STAGE2_TITLE_SCREEN)
    {
        drawStage2TitleScreen();
    }
    else if(screenState == PLAYING_STAGE2)
    {
        drawOuterWalls();
        drawBombs();
        drawFood();
        drawSnake();
        drawHUD("2");
    }
    else if(screenState == WIN_SCREEN)
    {
        drawWinScreen();
    }
    else if(screenState == GAME_OVER_SCREEN)
    {
        drawGameOverScreen();
    }

    glutSwapBuffers();
}

void reshape(GLsizei, GLsizei)
{
    glutReshapeWindow(map_size * 28, map_size * 28);
}

/**************************************************************************
 * Init
 ***************************************************************************/
void initializeGame()
{
    assert(map_size >= 15);
    assert(map_size <= 50);
    assert(maxDifficulty >= 5);

    srand((unsigned)time(0));
    glClearColor(mapBgR, mapBgG, mapBgB, 1.0f);

    updateGameSpeed();
    restartToMenu();
    guide();
}

/**************************************************************************
 * Input
 ***************************************************************************/
void keyboard(unsigned char key, int, int)
{
    switch(key)
    {
        case 13:
        {
            if(screenState == MENU_SCREEN)
            {
                startStage1();
            }
            else if(screenState == STAGE1_TITLE_SCREEN)
            {
                paused = 0;
                screenState = PLAYING_STAGE1;
            }
            else if(screenState == STAGE2_TITLE_SCREEN)
            {
                paused = 0;
                screenState = PLAYING_STAGE2;
            }
            break;
        }

        case 'p':
        case 'P':
        {
            if(screenState == PLAYING_STAGE1 || screenState == PLAYING_STAGE2)
                paused = !paused;
            break;
        }

        case 'r':
        case 'R':
        {
            initializeGame();
            break;
        }

        case 'm':
        case 'M':
        {
            Difficulty = (Difficulty - 1) % (maxDifficulty + 1);
            if(Difficulty <= 0)
                Difficulty = maxDifficulty;
            updateGameSpeed();
            cout << "Difficulty changed to " << (maxDifficulty + 1 - Difficulty) << "\n";
            break;
        }

        case 'h':
        case 'H':
        {
            guide();
            break;
        }

        case '+':
        case '=':
        {
            if(map_size < 50)
                map_size++;
            initializeGame();
            break;
        }

        case '-':
        case '_':
        {
            if(map_size > 15)
                map_size--;
            initializeGame();
            break;
        }

        case 'q':
        case 'Q':
        case 27:
        {
            showFinalScore();
            exit(0);
            break;
        }
    }

    glutPostRedisplay();
}

void specialKeys(int key, int, int)
{
    if(paused) return;
    if(screenState != PLAYING_STAGE1 && screenState != PLAYING_STAGE2) return;

    switch(key)
    {
        case GLUT_KEY_UP:
            if(direction == LEFT || direction == RIGHT)
                moveSnake(UP);
            break;

        case GLUT_KEY_DOWN:
            if(direction == LEFT || direction == RIGHT)
                moveSnake(DOWN);
            break;

        case GLUT_KEY_LEFT:
            if(direction == UP || direction == DOWN)
                moveSnake(LEFT);
            break;

        case GLUT_KEY_RIGHT:
            if(direction == UP || direction == DOWN)
                moveSnake(RIGHT);
            break;
    }
}

/**************************************************************************
 * Main
 ***************************************************************************/
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(map_size * 28, map_size * 28);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Snake Game - Two Stages");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);

    updateGameSpeed();
    glutTimerFunc(gameSpeed, TimerFunc, 0);

    initializeGame();
    glutMainLoop();
    return 0;
}
//  g++ main.cpp -framework OpenGL -framework GLUT -o game
//  ./game

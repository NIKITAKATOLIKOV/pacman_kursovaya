#include <SFML/Graphics.hpp>
#include <windows.h>
#include <string>
#include <vector>
#define DISTANCE 21.5f                       // расстояние до следующей координаты массива map
#define STEP 0.25f                           // шаг, с которым перемещаются пакман и приведения

sf::Font font;                               // шрифт
sf::Text level_text("", font, 30), score_text("", font, 30), lifes_text("", font, 30), fps_text("", font, 20), Bstart("START", font, 70), Bcontrol("CONTROL", font, 70), Bexit("EXIT", font, 70);
sf::Texture TPacman_0, TPacman_1, TPacman_2, TPacman_3, TDeathP_0, TDeathP_1, TDeathP_2, TDeathP_3, TDeathP_4, TDeathP_5, TDeathP_6, TDeathP_7, TDeathP_8,
        TGhostN_0, TGhostN_1, TGhostS_0, TGhostS_1, TGhostE,
        TFruit_0, TFruit_1, TFruit_2, TFruit_3, TFruit_4, TFruit_5, TFruit_6,
        TMap, TPoint, TControl, TEscape, TSpace, TMenu,
        TEnergy_0, TEnergy_1, TEnergy_2, TEnergy_3, TEnergy_4;
sf::Sprite SMap, SPoint, SFruit, SPacman, SMenu, SMenuP;

unsigned int score;                          // количество очков
unsigned int lifes_vertex;                   // последнее значение score, за которое добавляли жизнь
unsigned int lenght;                         // длина приложения
unsigned int width;                          // ширина приложения
short fps_value;                             // количество кадров в секунду
short energy_count;                          // количество энергетиков на карте
short time_passed;                           // значение времени в секундах для анимации
short GRedX, GRedY;                          // координата красного приведения (необходимо для логики синего приведения)
signed char lifes;                           // кол-во жизней
signed char level;                           // текущий уровень
signed char lifes_add;                       // счетчик для добавления жизней
signed char number_fruit;                    // номер текущего фрукта
signed char killed_count;                    // кол-во съеденных пакманом подряд приведений
signed char second_pause;                    // кол-во секунд, при которых игра находится остановленной
signed char second_fruit;                    // кол-во секунд, при которых фрукт виден, может быть взят
signed char second_scared;                   // кол-во секунд, при которых приведения могут быть съедены
signed char second_deathP;                   // кол-во секунд, при которых происходит анимация смерти пакмана
bool isPause;                                // пауза игры
bool isEscape;                               // выход через меню
bool gameOver;                               // переменная, перезапускающая игру
bool deathPacman;                            // переменная отвечающая за анимацию смерти пакмана

// направление движения
enum Diration { Stop = 0, Right, Left, Up, Down };
// особое имя приведения
enum Ghost_type { Blue = 0, Red, Orange, Pink };
// состояние приведения
enum Ghost_state { Normal = 0, Scared, Eaten };
Diration Plast_dir;                          // последнее направление пакмана
short map[29][28] =                          // карта
        {
                0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
                0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0,
                0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0,
                0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0,
                0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
                0, 2, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0,
                0, 2, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0,
                0, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 0,
                0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 1, 1, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
                0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0,
                0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0,
                0, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 0,
                0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0,
                0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0,
                0, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 0,
                0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,
                0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,
                0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0
        };
short energy_def[29][28] =                   // расположение энергетиков по умолчанию
        {
                0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
                0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
                0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0,
                0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
                0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
                0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0,
                0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0,
                0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
                0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
                0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
                0, 2, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 2, 0,
                0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0,
                0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0,
                0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0,
                0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
        };
short energy[29][28];                        // текущее расположение энергетиков(точек)
std::vector<std::vector<bool>> dec_arr(29);  // список раскрашенных клетот карты (для алгоритма Дейкстры)
std::vector<std::vector<short>> dis_arr(29); // список расстояний каждой клетки карты (для алгоритма Дейкстры)

inline void Setup(bool isNewGame, bool isBegin = true);

class Pacman                                 // пакман (класс)
{
protected:
    float mapX, mapY;                        // координата в пикселях в приложении
    float speed;                             // скорость
    float distance;                          // оставшееся расстояние до следующей координаты массива map
    short side;                              // длина текстуры
    short x, y;                              // координата в массиве map
    Diration dir;                            // текущее направление
    Diration next_dir;                       // следующее напраление
public:
    Pacman(int x = 0, int y = 0)
    {// конструктор по умолчанию. Если ввести x и y в этот метод, то пакман появится там, где мы захотим, а так по умолчанию, он появится там же.
        this->x = x;
        this->y = y;
        mapX = x * DISTANCE + 62.5f;
        mapY = y * DISTANCE + 64.f;
        distance = DISTANCE;
        speed = 2.f;
        side = 30;
        dir = next_dir = Stop;
    }
    void draw(sf::RenderWindow& win)
    {// рисовка пакмана - анимация смерти, движения, поворота
        if (deathPacman && isPause)
        {// анимация смерти
            if (time_passed >= 0 && time_passed <= 200 && second_deathP == 0) SPacman.setTexture(TDeathP_0);
            else if (time_passed >= 200 && time_passed <= 400 && second_deathP == 0) SPacman.setTexture(TDeathP_1);
            else if (time_passed >= 400 && time_passed <= 600 && second_deathP == 0) SPacman.setTexture(TDeathP_2);
            else if (time_passed >= 600 && time_passed <= 800 && second_deathP == 0) SPacman.setTexture(TDeathP_3);
            else if (time_passed >= 800 && time_passed <= 1000 && second_deathP == 1) SPacman.setTexture(TDeathP_4);
            else if (time_passed >= 0 && time_passed <= 200 && second_deathP == 1) SPacman.setTexture(TDeathP_5);
            else if (time_passed >= 200 && time_passed <= 400 && second_deathP == 1) SPacman.setTexture(TDeathP_6);
            else if (time_passed >= 400 && time_passed <= 600 && second_deathP == 1) SPacman.setTexture(TDeathP_7);
            else if (time_passed >= 600 && second_deathP == 1) --lifes, Setup(false);
            if (lifes == 0) deathPacman = false, gameOver = true;
        }// анимация движения
        else if ((distance >= 18.8125f && distance <= 21.5f) || (distance >= 0.f && distance <= 2.6875f))
            SPacman.setTexture(TPacman_0);
        else if ((distance >= 16.125f && distance <= 18.8125f) || (distance >= 2.6875f && distance <= 5.375f))
            SPacman.setTexture(TPacman_1);
        else if ((distance >= 13.4375 && distance <= 16.125f) || (distance >= 5.375f && distance <= 8.0625f))
            SPacman.setTexture(TPacman_2);
        else if ((distance >= 10.75f && distance <= 13.4375) || (distance >= 8.0625f && distance <= 10.75f))
            SPacman.setTexture(TPacman_3);
        SPacman.setPosition(mapX, mapY);
        SPacman.setOrigin(side / 2.f, side / 2.f);
        switch (dir)
        {// поворот картинки
            case Right: SPacman.setTextureRect(sf::IntRect(0, 0, side, side)), SPacman.setRotation(0.f); break;
            case Left: SPacman.setTextureRect(sf::IntRect(side, 0, -1 * side, side)), SPacman.setRotation(0.f); break;
            case Down: SPacman.getTextureRect() == sf::IntRect(0, 0, side, side) ? SPacman.setRotation(90.f) : SPacman.setRotation(-90.f); break;
            case Up: SPacman.getTextureRect() == sf::IntRect(0, 0, side, side) ? SPacman.setRotation(-90.f) : SPacman.setRotation(90.f); break;
            case Stop:
                break;
        }
        win.draw(SPacman);
    }
    void move()
    {// логика пакмана
        short i = 0, Nspeed = short(speed / STEP);
        if (energy[y][x] > 0) // съедение энергетиков
        {
            energy[y][x] == 1 ? score += 10 : (score += 50, second_scared = 9, second_scared -= (level + 1) / 7 > 2 ? 6 : (level + 1) / 7 * 2); energy[y][x] = 0; --energy_count; if (energy_count == 180 || energy_count == 60) second_fruit += 10; if (energy_count == 0) gameOver = true;
        }
        if (std::abs(mapX - SFruit.getPosition().x) < 15.f && mapY == SFruit.getPosition().y && second_fruit != char(0))
        {// съедение фрукта
            switch (number_fruit)
            {
                case char(0) : score += 100; break;
                case char(1) : score += 200; break;
                case char(2) : score += 500; break;
                case char(3) : score += 700; break;
                case char(4) : score += 1000; break;
                case char(5) : score += 2000; break;
                case char(6) : score += 5000; break;
            }
            second_fruit = char(0);
        }// добавление жизни при определенном кол-ве очков
        if (score - lifes_vertex >= unsigned(lifes_add - 3) * 5000 + 10000) lifes_vertex += (lifes_add - 3) * 5000 + 10000, ++lifes, ++lifes_add;
        if (dir != Stop) Plast_dir = dir;
        switch (next_dir)
        {// изменение направления движения, если это возможно
            case Right:
                if (dir != next_dir && map[y][x + 1] == 2)
                {
                    if (dir == Left) { x == 0 ? x = 27 : x == 27 ? x = 0 : --x; distance = DISTANCE - distance; dir = next_dir; }
                    else if (distance == DISTANCE) dir = next_dir;
                }
                break;
            case Left:
                if (dir != next_dir && map[y][x - 1] == 2)
                {
                    if (dir == Right) { x == 0 ? x = 27 : x == 27 ? x = 0 : ++x; distance = DISTANCE - distance; dir = next_dir; }
                    else if (distance == DISTANCE) dir = next_dir;
                }
                break;
            case Down:
                if (dir != next_dir && map[y + 1][x] == 2)
                {
                    if (dir == Up) { --y; distance = DISTANCE - distance; dir = next_dir; }
                    else if (distance == DISTANCE) dir = next_dir;
                }
                break;
            case Up:
                if (dir != next_dir && map[y - 1][x] == 2)
                {
                    if (dir == Down) { ++y; distance = DISTANCE - distance; dir = next_dir; }
                    else if (distance == DISTANCE) dir = next_dir;
                }
                break;
            case Stop:
                break;
        }
        switch (dir)
        {// движение по карте при определнном напрвлении
            case Right:
                if (x == 27)
                {
                    if (distance < DISTANCE / 2) mapX = 62.5f + -1 * distance;
                    for (; i < Nspeed; ++i) distance != 0 ? (mapX += STEP, distance -= STEP) : (distance = DISTANCE, x = 0, i = Nspeed);
                }
                else if (map[y][x + 1] == 2)
                    for (; i < Nspeed; ++i) distance != 0 ? (mapX += STEP, distance -= STEP) : (distance = DISTANCE, ++x, i = Nspeed);
                else
                    dir = Stop;
                break;
            case Left:
                if (x == 0)
                {
                    if (distance < DISTANCE / 2) mapX = lenght - 62.5f + distance;
                    for (; i < Nspeed; ++i) distance != 0 ? (mapX -= STEP, distance -= STEP) : (distance = DISTANCE, x = 27, i = Nspeed);
                }
                else if (map[y][x - 1] == 2)
                    for (; i < Nspeed; ++i) distance != 0 ? (mapX -= STEP, distance -= STEP) : (distance = DISTANCE, --x, i = Nspeed);
                else
                    dir = Stop;
                break;
            case Down:
                if (map[y + 1][x] == 2)
                    for (; i < Nspeed; ++i) distance != 0 ? (mapY += STEP, distance -= STEP) : (distance = DISTANCE, ++y, i = Nspeed);
                else
                    dir = Stop;
                break;
            case Up:
                if (map[y - 1][x] == 2)
                    for (; i < Nspeed; ++i) distance != 0 ? (mapY -= STEP, distance -= STEP) : (distance = DISTANCE, --y, i = Nspeed);
                else
                    dir = Stop;
                break;
        }
    }
    // перезадание направления
    void setDiration(Diration next_dir) { this->next_dir = next_dir; }
    // перезадание дистанции
    void setDistance(float distance) { this->distance = distance; }
    // получение текущего напраления
    Diration getDiration() const { return dir; }
    // получение расстояния до следующей координаты массива map
    float getDistance() const { return distance; }
    // получение координаты абциссы приведения
    short getX() const { return x; }
    // получение координаты ординаты приведения
    short getY() const { return y; }
    // получение кол-во пикселей абсциссы приведения
    float getMapX() const { return mapX; }
    // получение кол-во пикселей ординаты приведения
    float getMapY() const { return mapY; }

};

Pacman pacman;                               // ПАКМАН (объект)

class Place                                  // класс поля (для алгоритма Дейкстры)
{
public: short x, y, dis;
    Diration dir;
    Place(short x, short y, Diration dir, short dis) { this->x = x; this->y = y; this->dis = dis; this->dir = dir; }
};
inline std::vector<Diration> Find_Path(short wX, short wY, short fwX, short fwY, Diration dir_now)
{// алгоритм Дейкстры (нахождение кратчайшего пути)
    short i, x_left = fwX, y_left = fwY, Pdis, dis_left;
    std::vector<std::vector<bool>> dec_arrV;
    std::vector<std::vector<short>> dis_arrV;
    std::vector<Diration> dir_vec;
    std::vector<Place> arr; arr.push_back(Place(wX, wY, dir_now, 0));
    dec_arrV = dec_arr; dis_arrV = dis_arr; dis_arrV[wY][wX] = 0;
    for (i = 0; i < short(arr.size()); ++i)
    {
        Place value = arr[i];
        if (value.dir != Left && value.x <= 27)
            if (value.x == 27)
            {
                if (!dec_arrV[value.y][0])
                {
                    arr.push_back(Place(0, value.y, Right, value.dis + 1));
                    if (dis_arrV[value.y][0] > dis_arrV[value.y][27] + 1)
                        dis_arrV[value.y][0] = dis_arrV[value.y][27] + 1;
                }
            }
            else if (!dec_arrV[value.y][value.x + 1])
            {
                arr.push_back(Place(value.x + 1, value.y, Right, value.dis + 1));
                if (dis_arrV[value.y][value.x + 1] > dis_arrV[value.y][value.x] + 1)
                    dis_arrV[value.y][value.x + 1] = dis_arrV[value.y][value.x] + 1;
            }
        if (value.dir != Right && value.x >= 0)
            if (value.x == 0)
            {
                if (!dec_arrV[value.y][27])
                {
                    arr.push_back(Place(27, value.y, Left, value.dis + 1));
                    if (dis_arrV[value.y][27] > dis_arrV[value.y][0] + 1)
                        dis_arrV[value.y][27] = dis_arrV[value.y][0] + 1;
                }
            }
            else if (!dec_arrV[value.y][value.x - 1])
            {
                arr.push_back(Place(value.x - 1, value.y, Left, value.dis + 1));
                if (dis_arrV[value.y][value.x - 1] > dis_arrV[value.y][value.x] + 1)
                    dis_arrV[value.y][value.x - 1] = dis_arrV[value.y][value.x] + 1;
            }
        if (value.dir != Up && value.y < 28)
            if (!dec_arrV[value.y + 1][value.x])
            {
                arr.push_back(Place(value.x, value.y + 1, Down, value.dis + 1));
                if (dis_arrV[value.y + 1][value.x] > dis_arrV[value.y][value.x] + 1)
                    dis_arrV[value.y + 1][value.x] = dis_arrV[value.y][value.x] + 1;
            }
        if (value.dir != Down && value.y > 0)
            if (!dec_arrV[value.y - 1][value.x])
            {
                arr.push_back(Place(value.x, value.y - 1, Up, value.dis + 1));
                if (dis_arrV[value.y - 1][value.x] > dis_arrV[value.y][value.x] + 1)
                    dis_arrV[value.y - 1][value.x] = dis_arrV[value.y][value.x] + 1;
            }
        dec_arrV[value.y][value.x] = true;
    }
    Pdis = dis_left = dis_arrV[y_left][x_left];
    for (i = 0; i < Pdis; ++i)
    {
        if (x_left + 1 < 28 && dis_left == dis_arrV[y_left][x_left + 1] + 1)
            dir_vec.push_back(Left), --dis_left, ++x_left;
        else if (x_left == 27 && dis_left == dis_arrV[y_left][0] + 1)
            dir_vec.push_back(Left), --dis_left, x_left = 0;
        else if (x_left - 1 >= 0 && dis_left == dis_arrV[y_left][x_left - 1] + 1)
            dir_vec.push_back(Right), --dis_left, --x_left;
        else if (x_left == 0 && dis_left == dis_arrV[y_left][27] + 1)
            dir_vec.push_back(Right), --dis_left, x_left = 27;
        else if (y_left + 1 < 29 && dis_left == dis_arrV[y_left + 1][x_left] + 1)
            dir_vec.push_back(Up), --dis_left, ++y_left;
        else if (y_left - 1 >= 0 && dis_left == dis_arrV[y_left - 1][x_left] + 1)
            dir_vec.push_back(Down), --dis_left, --y_left;
    }
    return dir_vec;
}

class Ghost                                  // приведение
{
protected:
    bool revived;                            // перерожденный призрак не может быть испуган повторно от одного и того же энергетика
    bool wasScare, wasEaten;                 // отвечает за единственное задание вектора движения
    float mapX, mapY;                        // координата в пикселях в приложении
    float speed;                             // скорость
    float distance;                          // расстояние до следующей координаты массива map
    short side;                              // длина текстуры
    short Px, Py, x, y;                      // координата в массиве map
    Ghost_type name;                         // особое имя приведения
    Ghost_state state;                       // состояние приведения
    std::vector<Diration> dir_vec;           // вектор движения
public:
    Ghost(Ghost_type name, int x = 0, int y = 0)
    {// конструктор
        this->name = name;
        this->x = x;
        this->y = y;
        Px = pacman.getX();
        Py = pacman.getY();
        mapX = x * DISTANCE + 62.5f;
        mapY = y * DISTANCE + 64.f;
        distance = DISTANCE;
        wasEaten = wasScare = revived = false;
        state = Normal;
        speed = 1.25f;
        side = 30;
    }
    void draw(sf::RenderWindow& win)
    {// рисовка приведения
        short randAngle = rand() % 4;
        sf::Sprite sprite;
        sprite.setPosition(mapX, mapY);
        sprite.setOrigin(side / 2.f, side / 2.f);
        if (state != Eaten && second_scared == char(0) && wasScare) // изменение состояния на нормальное при истечении срока действия энергетика
            distance == DISTANCE ? dir_vec = Find_Path(x, y, Px, Py, dir_vec.size() > 0 ? dir_vec.back() : Stop), wasScare = false : NULL, killed_count = char(0), revived = false, state = Normal;
        if (state != Eaten && !revived && second_scared != 0 && (!wasScare || dir_vec.size() == 0)) // изменение состояния на испуганное во время действия энергетика
            distance == DISTANCE ? (dir_vec = Find_Path(x, y, randAngle % 2 == 0 ? 1 : 26, randAngle > 1 ? 28 : 0, Stop), wasScare = true) : NULL, state = Scared;
        switch (state)
        {// анимация движения
            case Normal:
                time_passed < 250 || (time_passed > 500 && time_passed < 750) ? sprite.setTexture(TGhostN_0) : sprite.setTexture(TGhostN_1);
                break;
            case Scared:
                time_passed <= 500 && second_scared < 3 ? sprite.setTexture(TGhostN_0) : time_passed > 500 && second_scared < 3 ? sprite.setTexture(TGhostS_1) : time_passed < 250 || (time_passed > 500 && time_passed < 750) ? sprite.setTexture(TGhostS_0) : sprite.setTexture(TGhostS_1);
                break;
            case Eaten:
                sprite.setTexture(TGhostE);
                break;
        }
        switch (name)
        {// характерный имени цвет
            case Blue:
                if (state != Scared) sprite.setColor(sf::Color(128, 255, 255));
                break;
            case Red:
                if (state != Scared) sprite.setColor(sf::Color::Red);
                break;
            case Orange:
                if (state != Scared) sprite.setColor(sf::Color(255, 128, 0));
                break;
            case Pink:
                if (state != Scared) sprite.setColor(sf::Color(255, 128, 255));
                break;
        }
        win.draw(sprite);
    }
    void move()
    {// логика приведения
        short Nspeed = short(speed / STEP), i = 0, j = 0;
        Diration dir_now = dir_vec.size() > 0 ? dir_vec.back() : Stop;
        if (dir_vec.size() > 0)
            switch (dir_now)
            {// движение по карте
                case Right:
                    if (x == 27)
                    {
                        if (distance < DISTANCE / 2) mapX = 62.5f + -1 * distance;
                        for (; j < Nspeed; ++j) distance != 0 ? (mapX += STEP, distance -= STEP) : (dir_vec.pop_back(), distance = DISTANCE, x = 0, j = Nspeed);
                    }
                    else
                        for (; j < Nspeed; ++j) distance != 0 ? (mapX += STEP, distance -= STEP) : (dir_vec.pop_back(), distance = DISTANCE, ++x, j = Nspeed);
                    break;
                case Left:
                    if (x == 0)
                    {
                        if (distance < DISTANCE / 2) mapX = lenght - 62.5f + distance;
                        for (; j < Nspeed; ++j) distance != 0 ? (mapX -= STEP, distance -= STEP) : (dir_vec.pop_back(), distance = DISTANCE, x = 27, j = Nspeed);
                    }
                    else
                        for (; j < Nspeed; ++j) distance != 0 ? (mapX -= STEP, distance -= STEP) : (dir_vec.pop_back(), distance = DISTANCE, --x, j = Nspeed);
                    break;
                case Down:
                    for (; j < Nspeed; ++j) distance != 0 ? (mapY += STEP, distance -= STEP) : (dir_vec.pop_back(), distance = DISTANCE, ++y, j = Nspeed);
                    break;
                case Up:
                    for (; j < Nspeed; ++j) distance != 0 ? (mapY -= STEP, distance -= STEP) : (dir_vec.pop_back(), distance = DISTANCE, --y, j = Nspeed);
                    break;
            }// изменение состояния на нормальное при достижении точки возрождения
        if ((x == 14 || x == 13) && y == 12 && distance == DISTANCE && state == Eaten)
            state = Normal, revived = true, wasScare = false, Px = Py = -1;
        if (revived && (second_scared == char(0) || second_scared == char(9 - ((level + 1) / 7 > 2 ? 6 : (level + 1) / 7 * 2))))
            revived = false;
        if (name == Red && x != GRedX && y != GRedY) GRedX = x, GRedY = y;
        if (distance == DISTANCE && state == Normal)
        {// характерный имени логика при нормальном состоянии
            short randAngle = rand() % 4;
            if (pacman.getX() != Px || pacman.getY() != Py)
            {
                short fwX = Px = pacman.getX(), fwY = Py = pacman.getY();
                switch (name)
                {
                    case Blue: // строит вектор до первой возможной дороги от координат Red до Pacman и дальше
                        if (2 * Px - GRedX >= 0 && 2 * Px - GRedX < 28 && 2 * Py - GRedY >= 0 && 2 * Py - GRedY < 29 && map[2 * Py - GRedY][2 * Px - GRedX] != 0)
                            fwX = 2 * Px - GRedX, fwY = 2 * Py - GRedY; break;
                    case Red: // всегда идет на Pacman
                        break;
                    case Orange: // идет на Pacman, когда не находится в круге радиусом 5 единиц, потом идет в один из углов
                        if (abs(x - Px) <= 5 && abs(y - Py) <= 5) fwX = randAngle % 2 == 0 ? 1 : 26, fwY = randAngle > 1 ? 28 : 0; break;
                    case Pink: // идет через 4 единицы от Pacman
                        switch (Plast_dir)
                        {
                            case Right: if (fwX <= 23 && map[fwY][fwX + 4] != 0) fwX += 4; break;
                            case Left: if (fwX >= 4 && map[fwY][fwX - 4] != 0) fwX -= 4; break;
                            case Down: if (fwY <= 24 && map[fwY + 4][fwX] != 0) fwY += 4; break;
                            case Up: if (fwY >= 4 && map[fwY - 4][fwX] != 0) fwY -= 4; break;
                        } break;
                }
                dir_vec = Find_Path(x, y, fwX, fwY, dir_now);
            }
            else if (name == Orange)
                abs(x - Px) <= 5 && abs(y - Py) <= 5 ? dir_vec = Find_Path(x, y, randAngle % 2 == 0 ? 1 : 26, randAngle > 1 ? 28 : 0, dir_now) : dir_vec = Find_Path(x, y, Px, Py, dir_now);
        }
        if (abs(pacman.getMapX() - mapX) <= 15 && abs(pacman.getMapY() - mapY) <= 15 && state != Eaten)
        {// соприкасание с пакманом
            if (state == Normal) gameOver = deathPacman = true;
            else state = Eaten, wasEaten = true, score += 200 << (killed_count + 1 < char(5) ? killed_count++ : killed_count = char(0));
        }// нахождение пути до точки возрождения
        if (state == Eaten && wasEaten && distance == DISTANCE) dir_vec = Find_Path(x, y, 14, 13, dir_now), wasEaten = false;
    }
    // задание состояния приведения
    void setState(Ghost_state state) { this->state = state; }
    // задание скорости приведения
    void setSpeed(float speed) { this->speed = speed; }
    // задание пути приведения
    void setDir_Vec(std::vector<Diration> dir_vec) { this->dir_vec = dir_vec; }
    // получение координаты абциссы приведения
    short getX() { return x; }
    // получение координаты ординаты приведения
    short getY() { return y; }
    // получение скорости приведения
    float getSpeed() { return speed; }
};

std::vector<Ghost> ghosts;                   // ПРИВЕДЕНИЯ (объекты)

inline void Disposable()
{// изначальное заполнение объектов (одноразовая)
    font.loadFromFile("..\\data\\Times_New_Roman.ttf");
    level_text.setPosition(300.f, 390.f), score_text.setPosition(80.f, 715.f), lifes_text.setPosition(430.f, 715.f); fps_text.setPosition(350.f, 0.f);
    Bstart.setPosition(240.f, 300.f), Bcontrol.setPosition(185.f, 400.f), Bexit.setPosition(260.f, 500.f), SMenu.setPosition(52.5f, 20.f), SMenuP.setPosition(-30.f, -30.f);
    sf::Image Ipacman_0, Ipacman_1, Ipacman_2, Ipacman_3,
            IghostN_0, IghostN_1, IghostS_0, IghostS_1, IghostE,
            Ifruit_0, Ifruit_1, Ifruit_2, Ifruit_3, Ifruit_4, Ifruit_5, Ifruit_6,
            Ienergy_0, Ienergy_1, Ienergy_2, Ienergy_3, Ienergy_4,
            IdeathP_0, IdeathP_1, IdeathP_2, IdeathP_3, IdeathP_4, IdeathP_5, IdeathP_6, IdeathP_7, IdeathP_8,
            Imap, Ipoint, Icontrol, Iescape, Ispace, Imenu;
            Ipacman_0.loadFromFile("..\\data\\images\\Pacman0.png"), Ipacman_1.loadFromFile("..\\data\\images\\Pacman1.png"),
            Ipacman_2.loadFromFile("..\\data\\images\\Pacman2.png"), Ipacman_3.loadFromFile("..\\data\\images\\Pacman3.png"),
            IghostN_0.loadFromFile("..\\data\\images\\GhostN0.png"), IghostN_1.loadFromFile("..\\data\\images\\GhostN1.png"),
            IghostS_0.loadFromFile("..\\data\\images\\GhostS0.png"), IghostS_1.loadFromFile("..\\data\\images\\GhostS1.png"),
            IghostE.loadFromFile("..\\data\\images\\GhostE.png"), Ifruit_0.loadFromFile("..\\data\\images\\Fruit0.png"),
            Ifruit_1.loadFromFile("..\\data\\images\\Fruit1.png"), Ifruit_2.loadFromFile("..\\data\\images\\Fruit2.png"),
            Ifruit_3.loadFromFile("..\\data\\images\\Fruit3.png"), Ifruit_4.loadFromFile("..\\data\\images\\Fruit4.png"),
            Ifruit_5.loadFromFile("..\\data\\images\\Fruit5.png"), Ifruit_6.loadFromFile("..\\data\\images\\Fruit6.png"),
            Ienergy_0.loadFromFile("..\\data\\images\\Energy0.png"), Ienergy_1.loadFromFile("..\\data\\images\\Energy1.png"),
            Ienergy_2.loadFromFile("..\\data\\images\\Energy2.png"), Ienergy_3.loadFromFile("..\\data\\images\\Energy3.png"), Ienergy_4.loadFromFile("..\\data\\images\\Energy4.png"),
            IdeathP_0.loadFromFile("..\\data\\images\\Death0.png"), IdeathP_1.loadFromFile("..\\data\\images\\Death1.png"),
            IdeathP_2.loadFromFile("..\\data\\images\\Death2.png"), IdeathP_3.loadFromFile("..\\data\\images\\Death3.png"),
            IdeathP_4.loadFromFile("..\\data\\images\\Death4.png"), IdeathP_5.loadFromFile("..\\data\\images\\Death5.png"),
            IdeathP_6.loadFromFile("..\\data\\images\\Death6.png"), IdeathP_7.loadFromFile("..\\data\\images\\Death7.png"), IdeathP_8.loadFromFile("..\\data\\images\\Death8.png"),
            Imap.loadFromFile("..\\data\\images\\Map.png"), Ipoint.loadFromFile("..\\data\\images\\Point.png"),
            Imenu.loadFromFile("..\\data\\images\\Menu.png"), Icontrol.loadFromFile("..\\data\\images\\Control.png"),
            Iescape.loadFromFile("..\\data\\images\\Escape.png"), Ispace.loadFromFile("..\\data\\images\\Space.png"),
            Ipacman_0.createMaskFromColor(sf::Color::Black), Ipacman_1.createMaskFromColor(sf::Color::Black),
            Ipacman_2.createMaskFromColor(sf::Color::Black), Ipacman_3.createMaskFromColor(sf::Color::Black),
            IghostN_0.createMaskFromColor(sf::Color::Black), IghostN_1.createMaskFromColor(sf::Color::Black),
            IghostS_0.createMaskFromColor(sf::Color::Black), IghostS_1.createMaskFromColor(sf::Color::Black),
            IghostE.createMaskFromColor(sf::Color::Black), Ifruit_0.createMaskFromColor(sf::Color::Black),
            Ifruit_1.createMaskFromColor(sf::Color::Black), Ifruit_2.createMaskFromColor(sf::Color::Black),
            Ifruit_3.createMaskFromColor(sf::Color::Black), Ifruit_4.createMaskFromColor(sf::Color::Black),
            Ifruit_5.createMaskFromColor(sf::Color::Black), Ifruit_6.createMaskFromColor(sf::Color::Black),
            Ienergy_0.createMaskFromColor(sf::Color::Black), Ienergy_1.createMaskFromColor(sf::Color::Black),
            Ienergy_2.createMaskFromColor(sf::Color::Black), Ienergy_3.createMaskFromColor(sf::Color::Black),
            Ienergy_4.createMaskFromColor(sf::Color::Black), Ipoint.createMaskFromColor(sf::Color::Black),
            IdeathP_0.createMaskFromColor(sf::Color::Black), IdeathP_1.createMaskFromColor(sf::Color::Black),
            IdeathP_2.createMaskFromColor(sf::Color::Black), IdeathP_3.createMaskFromColor(sf::Color::Black),
            IdeathP_4.createMaskFromColor(sf::Color::Black), IdeathP_5.createMaskFromColor(sf::Color::Black),
            IdeathP_6.createMaskFromColor(sf::Color::Black), IdeathP_7.createMaskFromColor(sf::Color::Black), IdeathP_8.createMaskFromColor(sf::Color::Black),
            Icontrol.createMaskFromColor(sf::Color::Yellow), Iescape.createMaskFromColor(sf::Color::Yellow), Imenu.createMaskFromColor(sf::Color::Black);        TPacman_0.loadFromImage(Ipacman_0), TPacman_1.loadFromImage(Ipacman_1),
            TPacman_2.loadFromImage(Ipacman_2), TPacman_3.loadFromImage(Ipacman_3),
            TGhostN_0.loadFromImage(IghostN_0), TGhostN_1.loadFromImage(IghostN_1),
            TGhostS_0.loadFromImage(IghostS_0), TGhostS_1.loadFromImage(IghostS_1),
            TGhostE.loadFromImage(IghostE), TFruit_0.loadFromImage(Ifruit_0),
            TFruit_1.loadFromImage(Ifruit_1), TFruit_2.loadFromImage(Ifruit_2),
            TFruit_3.loadFromImage(Ifruit_3), TFruit_4.loadFromImage(Ifruit_4),
            TFruit_5.loadFromImage(Ifruit_5), TFruit_6.loadFromImage(Ifruit_6),
            TEnergy_0.loadFromImage(Ienergy_0), TEnergy_1.loadFromImage(Ienergy_1),
            TEnergy_2.loadFromImage(Ienergy_2), TEnergy_3.loadFromImage(Ienergy_3), TEnergy_4.loadFromImage(Ienergy_4),
            TDeathP_0.loadFromImage(IdeathP_0), TDeathP_1.loadFromImage(IdeathP_1),
            TDeathP_2.loadFromImage(IdeathP_2), TDeathP_3.loadFromImage(IdeathP_3),
            TDeathP_4.loadFromImage(IdeathP_4), TDeathP_5.loadFromImage(IdeathP_5),
            TDeathP_6.loadFromImage(IdeathP_6), TDeathP_7.loadFromImage(IdeathP_7), TDeathP_8.loadFromImage(IdeathP_8),
            TControl.loadFromImage(Icontrol), TEscape.loadFromImage(Iescape),
            TSpace.loadFromImage(Ispace), TMap.loadFromImage(Imap),
            TPoint.loadFromImage(Ipoint), TMenu.loadFromImage(Imenu);
    TPacman_0.setSmooth(true), TPacman_1.setSmooth(true),
            TPacman_2.setSmooth(true), TPacman_3.setSmooth(true),
            TGhostN_0.setSmooth(true), TGhostN_1.setSmooth(true),
            TGhostS_0.setSmooth(true), TGhostN_1.setSmooth(true),
            TGhostE.setSmooth(true), TFruit_0.setSmooth(true),
            TFruit_1.setSmooth(true), TFruit_2.setSmooth(true),
            TFruit_3.setSmooth(true), TFruit_4.setSmooth(true),
            TFruit_5.setSmooth(true), TFruit_6.setSmooth(true),
            TEnergy_0.setSmooth(true), TEnergy_1.setSmooth(true),
            TEnergy_2.setSmooth(true), TEnergy_3.setSmooth(true),
            TEnergy_4.setSmooth(true), TPoint.setSmooth(true);
    TDeathP_0.setSmooth(true), TDeathP_1.setSmooth(true),
            TDeathP_2.setSmooth(true), TDeathP_3.setSmooth(true),
            TDeathP_4.setSmooth(true), TDeathP_5.setSmooth(true),
            TDeathP_6.setSmooth(true), TDeathP_7.setSmooth(true), TDeathP_8.setSmooth(true),
            TControl.setSmooth(true), TEscape.setSmooth(true),
            TSpace.setSmooth(true), TMenu.setSmooth(true),
            SMap.setTexture(TMap), SPoint.setTexture(TPoint), SMenu.setTexture(TMenu), SMenuP.setTexture(TPacman_0);
    SFruit.setOrigin(15, 15), SPoint.setOrigin(15, 15);
    isPause = isEscape = false, level = char(-1), second_fruit = second_scared = killed_count = char(0), lifes_add = lifes = char(3), score = lifes_vertex = 0, fps_value = 60, time_passed = 0;
    for (short i = 0; i < 29; ++i)
        for (short j = 0; j < 28; ++j)
            map[i][j] > 0 ? (dec_arr[i].push_back(false), dis_arr[i].push_back(100)) : (dec_arr[i].push_back(true), dis_arr[i].push_back(-1));
}
inline void Setup(bool isNewGame, bool isBegin)
{// новая игра
    short i = 0, j = 0;
    time_passed = killed_count = second_scared = second_deathP = second_fruit = char(0), second_pause = isBegin ? char(2) : char(0), isPause = deathPacman = gameOver = false, GRedX = 12, GRedY = 10, number_fruit = char((level + 2) > 21 ? rand() % 7 : short(level + 1) % 7);
    pacman = Pacman(14, 22); SPacman.setTextureRect(sf::IntRect(0, 0, 30, 30)), SPacman.setRotation(0.f);
    ghosts.clear();
    ghosts.push_back(Ghost(Red, 12, 10)); ghosts.push_back(Ghost(Blue, 15, 10));
    ghosts.push_back(Ghost(Orange, 15, 13)); ghosts.push_back(Ghost(Pink, 12, 13));
    for (; i < 4; ++i) ghosts[i].setDir_Vec(Find_Path(ghosts[i].getX(), ghosts[i].getY(), pacman.getX(), pacman.getY(), Stop));
    if (isNewGame)
    {
        energy_count = 244, ++level;
        for (i = 0; i < 4; ++i) ghosts[i].setSpeed((level + 1) % 7 == 0 ? (level + 1) / 7 > 3 ? 2.25f : ghosts[i].getSpeed() + 0.25f : ghosts[i].getSpeed());
        for (i = 0; i < 29; ++i) for (short j = 0; j < 28; ++j) energy[i][j] = energy_def[i][j];
        switch (number_fruit)
        {
            case char(0) : SFruit.setTexture(TFruit_0); break;
            case char(1) : SFruit.setTexture(TFruit_1); break;
            case char(2) : SFruit.setTexture(TFruit_2); break;
            case char(3) : SFruit.setTexture(TFruit_3); break;
            case char(4) : SFruit.setTexture(TFruit_4); break;
            case char(5) : SFruit.setTexture(TFruit_5); break;
            case char(6) : SFruit.setTexture(TFruit_6); break;
        }
    }
}
inline void Draw(sf::RenderWindow& win)
{// рисовка
    short i = 0;
    sf::Sprite Senergy, Spacman(TPacman_0); Spacman.setPosition(390, 720);
    if (time_passed <= 100 || time_passed >= 900) Senergy.setTexture(TEnergy_0);
    else if ((time_passed >= 100 && time_passed <= 200) || (time_passed >= 800 && time_passed <= 900)) Senergy.setTexture(TEnergy_1);
    else if ((time_passed >= 200 && time_passed <= 300) || (time_passed >= 700 && time_passed <= 800)) Senergy.setTexture(TEnergy_2);
    else if ((time_passed >= 300 && time_passed <= 400) || (time_passed >= 600 && time_passed <= 700)) Senergy.setTexture(TEnergy_3);
    else if ((time_passed >= 400 && time_passed <= 500) || (time_passed >= 500 && time_passed <= 600)) Senergy.setTexture(TEnergy_4);
    SFruit.setPosition(585, 735), Senergy.setOrigin(15, 15);
    score_text.setString("Score " + std::to_string(score)),
            lifes_text.setString("x " + std::to_string(lifes)),
            fps_text.setString("FPS " + std::to_string(fps_value));
    level_text.setString("LEVEL " + std::to_string(level));
    win.draw(SMap), second_pause != 4 ? win.draw(level_text) : void(), win.draw(score_text), win.draw(Spacman), win.draw(lifes_text), win.draw(fps_text), win.draw(SFruit);
    if (second_fruit != char(0)) { SFruit.setPosition(352.25f, 408.f); win.draw(SFruit); }
    for (; i < 812; ++i)
    {
        if (energy[i / 28][i % 28] > 0)
        {
            SPoint.setPosition((i % 28) * DISTANCE + 62.5f, (i / 28) * DISTANCE + 64.f),
                    Senergy.setPosition((i % 28) * DISTANCE + 62.5f, (i / 28) * DISTANCE + 64.f);
            energy[i / 28][i % 28] == 1 ? win.draw(SPoint) : win.draw(Senergy);
        }
    }
    pacman.draw(win);
    for (i = 0; i < 4; ++i) ghosts[i].draw(win);
}
inline void Control(sf::RenderWindow& win)
{
    sf::Sprite SControl(TControl), SEscape(TEscape), SSpace(TSpace);
    sf::Text Bback("BACK", font, 70), control("CONTROL", font, 30), escape("OPEN THE MENU", font, 30), space("PAUSE", font, 30);
    Bback.setPosition(250.f, 600.f), control.setPosition(280, 100), escape.setPosition(230, 300), space.setPosition(300, 450), SControl.setPosition(180, 150), SEscape.setPosition(320, 350), SSpace.setPosition(200, 500);
    while (win.isOpen())
    {
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) { SMenuP.setPosition(-30.f, -30.f); return; }
                if (event.key.code == sf::Keyboard::Enter)
                {
                    if (Bback.getFillColor() == sf::Color::Yellow) { SMenuP.setPosition(-30.f, -30.f); return; }
                }
            }
            if (sf::IntRect(250, 600, 200, 70).contains(sf::Mouse::getPosition(win))) Bback.getFillColor() != sf::Color::Yellow ? SMenuP.setPosition(210.f, 630.f) : void(), Bback.setFillColor(sf::Color::Yellow);
            else SMenuP.setPosition(-30.f, -30.f), Bback.setFillColor(sf::Color::White);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (Bback.getFillColor() == sf::Color::Yellow) { SMenuP.setPosition(-30.f, -30.f); return; }
            }
        }
        win.clear();
        win.draw(SMenuP);
        win.draw(control);
        win.draw(SControl);
        win.draw(escape);
        win.draw(SEscape);
        win.draw(space);
        win.draw(SSpace);
        win.draw(Bback);
        win.display();
    }
}
inline void SemiMenu(sf::RenderWindow& win)
{
    sf::Text Bcontinue("CONTINUE", font, 70), Bmenu("MENU", font, 70);
    Bcontinue.setPosition(170.f, 300.f), Bmenu.setPosition(250.f, 500.f);
    while (win.isOpen())
    {
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) { SMenuP.setPosition(-30.f, -30.f); return; }
                if (event.key.code == sf::Keyboard::Enter)
                {
                    if (Bcontinue.getFillColor() == sf::Color::Yellow) { SMenuP.setPosition(-30.f, -30.f); return; }
                    if (Bcontrol.getFillColor() == sf::Color::Yellow)  SMenuP.setPosition(-30.f, -30.f), Control(win);
                    if (Bmenu.getFillColor() == sf::Color::Yellow) { isEscape = true;  SMenuP.setPosition(-30.f, -30.f); return; }
                }
            }
            if (sf::IntRect(170, 300, 355, 70).contains(sf::Mouse::getPosition(win))) Bcontinue.getFillColor() != sf::Color::Yellow ? SMenuP.setPosition(130.f, 330.f) : void(), Bcontinue.setFillColor(sf::Color::Yellow), Bcontrol.setFillColor(sf::Color::White), Bmenu.setFillColor(sf::Color::White);
            else if (sf::IntRect(185, 400, 330, 70).contains(sf::Mouse::getPosition(win))) Bcontrol.getFillColor() != sf::Color::Yellow ? SMenuP.setPosition(145.f, 430.f) : void(), Bcontinue.setFillColor(sf::Color::White), Bcontrol.setFillColor(sf::Color::Yellow), Bmenu.setFillColor(sf::Color::White);
            else if (sf::IntRect(250, 500, 160, 70).contains(sf::Mouse::getPosition(win))) Bmenu.getFillColor() != sf::Color::Yellow ? SMenuP.setPosition(210.f, 530.f) : void(), Bcontinue.setFillColor(sf::Color::White), Bcontrol.setFillColor(sf::Color::White), Bmenu.setFillColor(sf::Color::Yellow);
            else SMenuP.setPosition(-30.f, -30.f), Bcontinue.setFillColor(sf::Color::White), Bcontrol.setFillColor(sf::Color::White), Bmenu.setFillColor(sf::Color::White);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (Bcontinue.getFillColor() == sf::Color::Yellow) { SMenuP.setPosition(-30.f, -30.f); return; }
                if (Bcontrol.getFillColor() == sf::Color::Yellow)  SMenuP.setPosition(-30.f, -30.f), Control(win);
                if (Bmenu.getFillColor() == sf::Color::Yellow) { isEscape = true;  SMenuP.setPosition(-30.f, -30.f); return; }
            }
        }
        win.clear();
        win.draw(SMenu);
        win.draw(SMenuP);
        win.draw(Bcontinue);
        win.draw(Bcontrol);
        win.draw(Bmenu);
        win.display();
    }
}
inline void Play(sf::RenderWindow& win)
{// игровой процесс
    sf::Clock clock; Setup(true, false);
    while (!isEscape && win.isOpen())
    {
        if (gameOver && deathPacman) isPause = true, time_passed = 0, gameOver = false;
        else if (gameOver && energy_count == 0) Setup(true);
        else if (gameOver) break;
        clock.restart();
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) pacman.setDiration(Up);
                if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) pacman.setDiration(Down);
                if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) pacman.setDiration(Left);
                if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) pacman.setDiration(Right);
                if (event.key.code == sf::Keyboard::Escape && second_pause == 4 && !deathPacman) SemiMenu(win);
                if (event.key.code == sf::Keyboard::Space && second_pause == 4 && !deathPacman) isPause = !isPause;
            }
        }
        win.clear();
        if (!isPause && second_pause == 4) { pacman.move(); for (short i = 0; i < 4; ++i) ghosts[i].move(); }
        Draw(win);
        if (!isEscape) win.display();
        fps_value = short(1 / (clock.getElapsedTime().asSeconds()));
        time_passed > 1000 ? (time_passed = 0, (second_pause == 4 ? 0 : ++second_pause), (deathPacman ? ++second_deathP : 0), (second_scared == 0 ? 0 : --second_scared), second_fruit == 0 ? 0 : --second_fruit) : time_passed += clock.getElapsedTime().asMilliseconds();
    }
    score = 0, level = char(-1), second_pause = char(0), lifes = lifes_add = char(3);
}
inline void Menu()
{
    sf::Image icon; icon.loadFromFile("..\\data\\images\\Pacman0.png"); icon.createMaskFromColor(sf::Color::Black);
    sf::RenderWindow window(sf::VideoMode(705, 760), "Pacman", sf::Style::None);
    window.setIcon(30, 30, icon.getPixelsPtr());
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    Disposable();
    lenght = window.getSize().x, width = window.getSize().y;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::Enter)
                {
                    if (Bstart.getFillColor() == sf::Color::Yellow) isEscape = false, Bstart.setFillColor(sf::Color::Yellow), Play(window);
                    if (Bcontrol.getFillColor() == sf::Color::Yellow) Bcontrol.setFillColor(sf::Color::Yellow), Control(window);
                    if (Bexit.getFillColor() == sf::Color::Yellow) window.close();
                }
            }
            if (sf::IntRect(250, 300, 200, 70).contains(sf::Mouse::getPosition(window))) Bstart.getFillColor() != sf::Color::Yellow ? SMenuP.setPosition(200.f, 330.f) : void(), Bstart.setFillColor(sf::Color::Yellow), Bcontrol.setFillColor(sf::Color::White), Bexit.setFillColor(sf::Color::White);
            else if (sf::IntRect(185, 400, 330, 70).contains(sf::Mouse::getPosition(window))) Bcontrol.getFillColor() != sf::Color::Yellow ? SMenuP.setPosition(145.f, 430.f) : void(), Bstart.setFillColor(sf::Color::White), Bcontrol.setFillColor(sf::Color::Yellow), Bexit.setFillColor(sf::Color::White);
            else if (sf::IntRect(260, 500, 160, 70).contains(sf::Mouse::getPosition(window))) Bexit.getFillColor() != sf::Color::Yellow ? SMenuP.setPosition(220.f, 530.f) : void(), Bstart.setFillColor(sf::Color::White), Bcontrol.setFillColor(sf::Color::White), Bexit.setFillColor(sf::Color::Yellow);
            else SMenuP.setPosition(-30.f, -30.f), Bstart.setFillColor(sf::Color::White), Bcontrol.setFillColor(sf::Color::White), Bexit.setFillColor(sf::Color::White);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (Bstart.getFillColor() == sf::Color::Yellow) isEscape = false, Bstart.setFillColor(sf::Color::Yellow), Play(window);
                if (Bcontrol.getFillColor() == sf::Color::Yellow)  SMenuP.setPosition(-30.f, -30.f), Bcontrol.setFillColor(sf::Color::Yellow), Control(window);
                if (Bexit.getFillColor() == sf::Color::Yellow) window.close();
            }
        }
        window.clear();
        window.draw(SMenu);
        window.draw(SMenuP);
        window.draw(Bstart);
        window.draw(Bcontrol);
        window.draw(Bexit);
        window.display();
    }
}

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    Menu();
    return 0; sizeof(Place), sizeof(Pacman), sizeof(Ghost);
}

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
using namespace std;
using namespace sf;

// 난수를 메르헨 트위스터 방식으로 생성
mt19937 engine((unsigned int)time(NULL));

class Bullet
{
public:
    Sprite shape;

    Bullet(Texture *texture, Vector2f pos)
    {
        this->shape.setTexture(*texture); // 새우깡 텍스처 설정

        this->shape.setScale(0.3f, 0.3f); // 새우깡 크기 설정

        this->shape.setPosition(pos); // 새우깡 위치 설정
    }
    ~Bullet() {}
};

class User
{
public:
    Sprite shape;
    Texture *texture;

    int HP;
    int HPMax;

    vector<Bullet> bullets; // 스택기반으로 배열만들기

    User(Texture *texture)
    {
        this->HPMax = 10;
        this->HP = this->HPMax;

        this->texture = texture;
        this->shape.setTexture(*texture);

        this->shape.setScale(0.1f, 0.1f);
    }
    ~User() {}
};

class Enemy
{
public:
    Sprite shape;
    int HP;
    int HPMax;

    Enemy(Texture *texture, Vector2u windowSize)
    {
        this->HPMax = engine() % 3 + 1;
        this->HP = this->HPMax;

        this->shape.setTexture(*texture);

        this->shape.setScale(0.3f, 0.3f);

        this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width, engine() % (int)(windowSize.y - this->shape.getGlobalBounds().height)); // 짤린 부분 없애기 위해 int형 선언
    }
    ~Enemy() {}
};

class Fever
{
public:
    Sprite shape;

    Fever(Texture *texture, Vector2u windowSize)
    {
        this->shape.setTexture(*texture);

        this->shape.setScale(0.3f, 0.3f);

        this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width, engine() % (int)(windowSize.y - this->shape.getGlobalBounds().height)); // 짤린 부분 없애기 위해 int형 선언
    }
    ~Fever() {}
};

class Heal
{
public:
    Sprite shape;

    Heal(Texture *texture, Vector2u windowSize)
    {
        this->shape.setTexture(*texture);

        this->shape.setScale(0.15f, 0.15f);

        this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width, engine() % (int)(windowSize.y - this->shape.getGlobalBounds().height)); // 짤린 부분 없애기 위해 int형 선언
    }
    ~Heal() {}
};

int main()
{

    // 메인메뉴

    RenderWindow window(VideoMode(800, 600), "GGANG");
    window.setFramerateLimit(60);

    // 배경음악
    Music music;
    if (!music.openFromFile("things/GGANG.ogg"))
    {
        return -1;
    }
    music.setVolume(3.f);
    music.play();

    // 상대 피격 소음
    SoundBuffer buffer1;
    if (!buffer1.loadFromFile("things/BAT.ogg"))
    {
        return -1;
    }
    Sound enemySound;
    enemySound.setVolume(5.f);
    enemySound.setBuffer(buffer1);

    // 유저 피격 소음
    SoundBuffer buffer2;
    if (!buffer2.loadFromFile("things/ping_warning.ogg"))
    {
        return -1;
    }
    Sound userSound;
    userSound.setVolume(5.f);
    userSound.setBuffer(buffer2);

    // 게임오버 사운드
    SoundBuffer buffer3;
    if (!buffer3.loadFromFile("things/GAMEOVER.ogg"))
    {
        return -1;
    }
    Sound gameoverSound;
    gameoverSound.setBuffer(buffer3);

    // 피버 사운드
    SoundBuffer buffer4;
    if (!buffer4.loadFromFile("things/sonega.ogg"))
    {
        return -1;
    }
    Sound feverSound;
    feverSound.setBuffer(buffer4);

    // 감자깡(힐 사운드)
    SoundBuffer buffer5;
    if (!buffer5.loadFromFile("things/summoner_heal.ogg"))
    {
        return -1;
    }
    Sound recoverSound;
    recoverSound.setVolume(5.f);
    recoverSound.setBuffer(buffer5);

    // 폰트 설정
    Font font;
    font.loadFromFile("things/BMDOHYEON_ttf.ttf");

    //텍스쳐 설정
    Texture userTex;
    userTex.loadFromFile("images/bee.PNG");

    Texture enemyTex;
    enemyTex.loadFromFile("images/rain.PNG");

    Texture bulletTex;
    bulletTex.loadFromFile("images/seawoo.png");

    Texture feverTex;
    feverTex.loadFromFile("images/chip.PNG");

    Texture healTex;
    healTex.loadFromFile("images/potato.PNG");

    // 점수
    Text scoreTex;
    scoreTex.setFont(font);
    scoreTex.setCharacterSize(20);
    scoreTex.setFillColor(Color::White);
    scoreTex.setPosition(10.f, 10.f);
    int score = 0;

    // 플레이어 설정
    User user(&userTex);
    int shootTimer = 20;
    Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(12);
    hpText.setFillColor(Color::White);

    // 적(비) 설정
    vector<Enemy> enemies;
    int enemyspawnTimer = 0;
    enemies.push_back(Enemy(&enemyTex, window.getSize()));
    Text ehpText;
    ehpText.setFont(font);
    ehpText.setCharacterSize(12);
    ehpText.setFillColor(Color::White);

    // 새우깡 피버 설정
    vector<Fever> fevers;
    int feverspawnTimer = 0;
    fevers.push_back(Fever(&feverTex, window.getSize()));

    // 감자깡 힐 설정
    vector<Heal> heals;
    int healspawnTimer = 0;
    heals.push_back(Heal(&healTex, window.getSize()));

    // 총알 빈번도 초기화
    int bullet_frequency = 40;

    // 게임 오버
    Text gameoverText;
    gameoverText.setFont(font);
    gameoverText.setCharacterSize(20);
    gameoverText.setFillColor(Color::Red);
    gameoverText.setPosition(100.f, window.getSize().y / 2);
    gameoverText.setString("GAME OVER!!");

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        ///////////////////////////////////////////////////////////////////////////// 피버모드일 때  //////////////////////////////////////////////
        if (user.HP > 0 && bullet_frequency == 30)
        {

            // User 움직이기
            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                user.shape.move(0.f, -5.f);
            }
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                user.shape.move(-5.f, 0.f);
            }
            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                user.shape.move(0.f, 5.f);
            }
            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                user.shape.move(5.f, 0.f);
            }
            // user HP, HPMax
            hpText.setPosition(user.shape.getPosition().x, user.shape.getPosition().y - hpText.getGlobalBounds().height);
            hpText.setString(to_string(user.HP) + " / " + to_string(user.HPMax));

            //  윈도우에 부딪히면
            if (user.shape.getPosition().x <= 0) // 왼쪽을 넘어설 경우
            {
                user.shape.setPosition(0.f, user.shape.getPosition().y);
            }
            if (user.shape.getPosition().x >= window.getSize().x - user.shape.getGlobalBounds().width) // 오른쪽을 넘어설 경우
            {
                user.shape.setPosition(window.getSize().x - user.shape.getGlobalBounds().width, user.shape.getPosition().y); // GlobalBounds는 bounding box를 리턴합니다.
            }                                                                                                                // 벽에 부딪히면 바운딩하는 것을 빼줌으로써 바운딩하지 않게 했습니다.
            if (user.shape.getPosition().y <= 0)                                                                             // 아래쪽을 넘어설 경우
            {
                user.shape.setPosition(user.shape.getPosition().x, 0.f);
            }
            if (user.shape.getPosition().y >= window.getSize().y - user.shape.getGlobalBounds().height) // 위쪽을 넘어설 경우
            {
                user.shape.setPosition(user.shape.getPosition().x, window.getSize().y - user.shape.getGlobalBounds().height);
            }

            if (shootTimer < bullet_frequency)
            {
                shootTimer++;
            }

            // 업데이트 부분
            if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= bullet_frequency) // <- 이부분도 빈도 수정 해줘야함
            {
                user.bullets.push_back(Bullet(&bulletTex, user.shape.getPosition()));
                shootTimer = 0;
            }

            // 총알 윈도우 벗어나면 없어짐
            for (size_t i = 0; i < user.bullets.size(); i++)
            {
                // 총알 나가는 속도---------ㄱ
                user.bullets[i].shape.move(10.f, 0.f);

                if (user.bullets[i].shape.getPosition().x > window.getSize().x)
                {
                    user.bullets.erase(user.bullets.begin() + i);
                    break;
                }

                // 새우깡이랑 적이랑 부딛히면
                for (size_t k = 0; k < enemies.size(); k++)
                {
                    if (user.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
                    {

                        if (enemies[k].HP <= 1)
                        {
                            enemySound.play();
                            score += enemies[k].HPMax; // 적의 체력만큼 점수 획득
                            enemies.erase(enemies.begin() + k);
                        }
                        else
                        {
                            enemies[k].HP--;
                        }
                        user.bullets.erase(user.bullets.begin() + i);
                        break;
                    }
                }
            }

            // 적 왼쪽으로 이동 및 적과 부딪히면
            if (enemyspawnTimer < 10)
            {
                enemyspawnTimer++;
            }
            // 적 스폰
            if (enemyspawnTimer >= 10)
            {
                enemies.push_back(Enemy(&enemyTex, window.getSize()));
                enemyspawnTimer = 0;
            }

            // 유저 적에게 피격했을 경우와 적 속도 지정
            for (size_t i = 0; i < enemies.size(); i++)
            { // 적 속도-------------ㄱ
                enemies[i].shape.move(-8.f, 0.f);

                if (enemies[i].shape.getPosition().x <= 0 - enemies[i].shape.getGlobalBounds().width)
                {
                    enemies.erase(enemies.begin() + i);
                    break;
                }

                if (enemies[i].shape.getGlobalBounds().intersects(user.shape.getGlobalBounds()))
                {
                    bullet_frequency = 40;
                    feverSound.stop();
                    userSound.play();
                    enemies.erase(enemies.begin() + i);

                    user.HP--; // 체력 깎임

                    if (user.HP == 0) // 체력 0 되면 비가 일침합니다.
                    {
                        gameoverSound.play();
                    }
                    break;
                }
            }

            // 새우깡피버 왼쪽으로 이동
            if (feverspawnTimer < 800)
            {
                feverspawnTimer++;
            }
            // 새우깡 피버 스폰
            if (feverspawnTimer >= 800)
            {
                fevers.push_back(Fever(&feverTex, window.getSize()));
                feverspawnTimer = 0;
            }

            for (size_t i = 0; i < fevers.size(); i++)
            {
                fevers[i].shape.move(-3.f, 0.f);

                if (fevers[i].shape.getPosition().x <= 0 - fevers[i].shape.getGlobalBounds().width)
                {
                    fevers.erase(fevers.begin() + i);
                    break;
                }

                if (fevers[i].shape.getGlobalBounds().intersects(user.shape.getGlobalBounds()))
                {

                    feverSound.play();
                    fevers.erase(fevers.begin() + i);
                    bullet_frequency = 30;
                    break;
                }
            }

            // 감자깡 (힐) 스폰
            if (healspawnTimer < 700)
            {
                healspawnTimer++;
            }
            if (healspawnTimer >= 700)
            {
                heals.push_back(Heal(&healTex, window.getSize()));
                healspawnTimer = 0;
            }
            for (size_t i = 0; i < heals.size(); i++)
            {
                heals[i].shape.move(-4.f, 0.f);

                if (heals[i].shape.getPosition().x <= 0 - heals[i].shape.getGlobalBounds().width)
                {
                    heals.erase(heals.begin() + i);
                    break;
                }

                if (heals[i].shape.getGlobalBounds().intersects(user.shape.getGlobalBounds()))
                {

                    recoverSound.play();
                    heals.erase(heals.begin() + i);
                    if (user.HP == user.HPMax)
                    {
                        break;
                    }
                    if (user.HP < user.HPMax)
                    {
                        user.HP++;
                    }
                    break;
                }
            }
            // 점수표 생성 업데이트
            scoreTex.setString("Total Score : " + to_string(score));
        } // 체력이 1 이하로 떨어졌을 때 이 구문을 빠져나감..

        ///////////////////////////////////////////////////////////////////////////// 피버모드 아닐 때 //////////////////////////////////////////////

        if (user.HP > 0 && bullet_frequency == 40)
        {
            // User 움직이기
            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                user.shape.move(0.f, -5.f);
            }
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                user.shape.move(-5.f, 0.f);
            }
            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                user.shape.move(0.f, 5.f);
            }
            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                user.shape.move(5.f, 0.f);
            }
            // user HP, HPMax
            hpText.setPosition(user.shape.getPosition().x, user.shape.getPosition().y - hpText.getGlobalBounds().height);
            hpText.setString(to_string(user.HP) + " / " + to_string(user.HPMax));

            //  윈도우에 부딪히면
            if (user.shape.getPosition().x <= 0) // 왼쪽을 넘어설 경우
            {
                user.shape.setPosition(0.f, user.shape.getPosition().y);
            }
            if (user.shape.getPosition().x >= window.getSize().x - user.shape.getGlobalBounds().width) // 오른쪽을 넘어설 경우
            {
                user.shape.setPosition(window.getSize().x - user.shape.getGlobalBounds().width, user.shape.getPosition().y); // GlobalBounds는 bounding box를 리턴합니다.
            }                                                                                                                // 벽에 부딪히면 바운딩하는 것을 빼줌으로써 바운딩하지 않게 했습니다.
            if (user.shape.getPosition().y <= 0)                                                                             // 아래쪽을 넘어설 경우
            {
                user.shape.setPosition(user.shape.getPosition().x, 0.f);
            }
            if (user.shape.getPosition().y >= window.getSize().y - user.shape.getGlobalBounds().height) // 위쪽을 넘어설 경우
            {
                user.shape.setPosition(user.shape.getPosition().x, window.getSize().y - user.shape.getGlobalBounds().height);
            }

            if (shootTimer < bullet_frequency)
            {
                shootTimer++;
            }

            // 업데이트 부분
            if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= bullet_frequency) // <- 이부분도 빈도 수정 해줘야함
            {
                user.bullets.push_back(Bullet(&bulletTex, user.shape.getPosition()));
                shootTimer = 0;
            }

            // 총알 윈도우 벗어나면 없어짐
            for (size_t i = 0; i < user.bullets.size(); i++)
            {
                // 총알 나가는 속도---------ㄱ
                user.bullets[i].shape.move(10.f, 0.f);

                if (user.bullets[i].shape.getPosition().x > window.getSize().x)
                {
                    user.bullets.erase(user.bullets.begin() + i);
                    break;
                }

                // 새우깡이랑 적이랑 부딛히면
                for (size_t k = 0; k < enemies.size(); k++)
                {
                    if (user.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
                    {

                        if (enemies[k].HP <= 1)
                        {
                            enemySound.play();
                            score += enemies[k].HPMax; // 적의 체력만큼 점수 획득
                            enemies.erase(enemies.begin() + k);
                        }
                        else
                        {
                            enemies[k].HP--;
                        }
                        user.bullets.erase(user.bullets.begin() + i);
                        break;
                    }
                }
            }

            // 적 왼쪽으로 이동 및 적과 부딪히면
            if (enemyspawnTimer < 10)
            {
                enemyspawnTimer++;
            }
            // 적 스폰
            if (enemyspawnTimer >= 10)
            {
                enemies.push_back(Enemy(&enemyTex, window.getSize()));
                enemyspawnTimer = 0;
            }

            // 유저 적에게 피격했을 경우와 적 속도 지정
            for (size_t i = 0; i < enemies.size(); i++)
            { // 적 속도-------------ㄱ
                enemies[i].shape.move(-8.f, 0.f);

                if (enemies[i].shape.getPosition().x <= 0 - enemies[i].shape.getGlobalBounds().width)
                {
                    enemies.erase(enemies.begin() + i);
                    break;
                }

                if (enemies[i].shape.getGlobalBounds().intersects(user.shape.getGlobalBounds()))
                {
                    bullet_frequency = 40;
                    feverSound.stop();
                    userSound.play();
                    enemies.erase(enemies.begin() + i);

                    user.HP--; // 체력 깎임

                    if (user.HP == 0) // 체력 0 되면 비가 일침합니다.
                    {
                        gameoverSound.play();
                    }
                    break;
                }
            }

            // 새우깡피버 왼쪽으로 이동
            if (feverspawnTimer < 800)
            {
                feverspawnTimer++;
            }
            // 새우깡 피버 스폰
            if (feverspawnTimer >= 800)
            {
                fevers.push_back(Fever(&feverTex, window.getSize()));
                feverspawnTimer = 0;
            }

            for (size_t i = 0; i < fevers.size(); i++)
            {
                fevers[i].shape.move(-3.f, 0.f);

                if (fevers[i].shape.getPosition().x <= 0 - fevers[i].shape.getGlobalBounds().width)
                {
                    fevers.erase(fevers.begin() + i);
                    break;
                }

                if (fevers[i].shape.getGlobalBounds().intersects(user.shape.getGlobalBounds()))
                {

                    feverSound.play();
                    fevers.erase(fevers.begin() + i);
                    bullet_frequency = 30;
                    break;
                }
            }

            // 감자깡 (힐) 스폰
            if (healspawnTimer < 700)
            {
                healspawnTimer++;
            }
            if (healspawnTimer >= 700)
            {
                heals.push_back(Heal(&healTex, window.getSize()));
                healspawnTimer = 0;
            }
            for (size_t i = 0; i < heals.size(); i++)
            {
                heals[i].shape.move(-4.f, 0.f);

                if (heals[i].shape.getPosition().x <= 0 - heals[i].shape.getGlobalBounds().width)
                {
                    heals.erase(heals.begin() + i);
                    break;
                }
                if (heals[i].shape.getGlobalBounds().intersects(user.shape.getGlobalBounds()))
                {

                    recoverSound.play();
                    heals.erase(heals.begin() + i);
                    if (user.HP == user.HPMax)
                    {
                        break;
                    }
                    if (user.HP < user.HPMax)
                    {
                        user.HP++;
                    }
                    break;
                }
                //점수표 생성 업데이트
                scoreTex.setString("Total Score : " + to_string(score));
            }

        } // 체력이 1 이하로 떨어졌을 때 이 구문을 빠져나감..

        /////////////////////////////////////////////////////////// 그리기 부분 ///////////////////////////////////////////////////////////////////////////////
        window.clear();
        // 그리기 - user
        window.draw(user.shape);
        // 그리기 - 총알(새우깡)
        for (size_t i = 0; i < user.bullets.size(); i++)
        {
            window.draw(user.bullets[i].shape);
        }
        // 그리기 - 적(비)
        for (size_t i = 0; i < enemies.size(); i++)
        {
            ehpText.setString(to_string(enemies[i].HP) + " / " + to_string(enemies[i].HPMax));
            ehpText.setPosition(enemies[i].shape.getPosition().x + 20.0f, enemies[i].shape.getPosition().y - ehpText.getGlobalBounds().height - 4.0f);
            window.draw(ehpText);
            window.draw(enemies[i].shape);
        }
        // 그리기 - 새우깡 피버
        for (size_t i = 0; i < fevers.size(); i++)
        {
            window.draw(fevers[i].shape);
        }
        // 그리기 - 감자깡 힐
        for (size_t i = 0; i < heals.size(); i++)
        {
            window.draw(heals[i].shape);
        }
        // HP바 생성, 스코어 생성
        window.draw(scoreTex);
        window.draw(hpText);

        // 게임오버
        if (user.HP <= 0)
        {
            music.stop();
            window.draw(gameoverText);
        }

        window.display();
    }

    return 0;
}

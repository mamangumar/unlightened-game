#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "candle.h"

using namespace sf;
using namespace std;

int main()
{
  RenderWindow window(VideoMode(1920, 1080), "PointNClick", Style::Default);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);
  window.setTitle("unlighten'd");
  window.setFramerateLimit(60);

  auto wCenter = window.getView().getCenter();
  int gameCondition = 3, // mengatur layar dan mode yang ditampilkan (main menu, gameplay, gameover)
      audioStay = 1,     // menjaga agar audio tidak terjebak dalam for loop
      countdown;         // menjadi hitung mundur sebelum game over
  enum
  {
    START = 1,
    GAMEOVER,
    IDLE
  };

  // membuat clock dan time untuk mengatur waktu jalannya gameplay dan pencegahan penekanan tombol ketika baru pindah mode nantinya
  Clock gameTime, immunityFrame;
  Time elapsedInGame, ImmunityTime;

  // memasukkan beragam file audio yang diperlukan
  Music gamePlayMusic, gameoverMusic, mainmenuMusic;
  SoundBuffer extinguishedSound;
  gamePlayMusic.openFromFile("buat audio game nnti.wav");
  gameoverMusic.openFromFile("buat audio game nnti_gameover.wav");
  mainmenuMusic.openFromFile("buat audio game nnti juga.wav");
  extinguishedSound.loadFromFile("inibuatsuaralilinnya.wav");
  Sound candleOff(extinguishedSound);
  candleOff.setVolume(45);

  // memasukkan dan menyetting background dari gameplay
  Texture background;
  background.loadFromFile("background.png", IntRect(0, 0, 1920, 1080));
  Sprite wallpaper(background);
  wallpaper.setScale(1.15, 1.15);

  // memasukkan dan menyetting background dan semua tombol pada main menu
  Texture mainmenu, startButton, exitButton;
  mainmenu.loadFromFile("main menu polos.png", IntRect(0, 0, 1920, 1080));
  Sprite MainMenu(mainmenu);
  startButton.loadFromFile("START.png");
  Sprite start(startButton);
  exitButton.loadFromFile("exit.png");
  Sprite Exit(exitButton);
  Exit.setScale(0.35, 0.35);
  start.setPosition(wCenter.x - start.getGlobalBounds().width / 2, 700);
  Exit.setPosition(wCenter.x - Exit.getGlobalBounds().width / 2, 900);

  // memasukkan dan mengatur background dan semua tombol pada gameover
  Texture gameOver, goToMenu, Retry;
  gameOver.loadFromFile("GO plos.png");
  Sprite GameOver(gameOver);
  goToMenu.loadFromFile("menu (1).png");
  Sprite menu(goToMenu);
  Retry.loadFromFile("retry.png");
  Sprite retry(Retry);
  menu.setPosition(wCenter.x - start.getGlobalBounds().width, 700);
  retry.setPosition(wCenter.x, 700);

  // pendeklarasian texture lilin mati dan nyala dan mengatur vektor dari class Candle
  Texture lilinNyala{}, lilinMati{};
  lilinNyala.loadFromFile("lilin trans.png");
  lilinMati.loadFromFile("lilin mati.png");

  vector<Lilin> vectorLilin{};

  int totalLilinMati = 0; // membuat nilai awal jumlah lilin mati
  int level = 1;          // mendeklarasikan level dengan nilai 1
  int score = 0;          // mendeklarasikan score dengan nilai 0

  // memasukkan font
  Font font{};
  font.loadFromFile("assets/fonts/Poppins-Regular.ttf");

  // menambahkan custom cursor
  Texture cursor;
  cursor.loadFromFile("cursor.png");
  Sprite mouseCursor(cursor);
  mouseCursor.setScale(48.0f / 360.0f, 48.0f / 360.0f);
  window.setMouseCursorVisible(false);

  // menambahkan sprite hantu dalam gameplay
  Texture hantu;
  hantu.loadFromFile("timer (icon hantu biar lucu).png");
  Sprite ghost(hantu);
  ghost.setPosition({70, 500});
  ghost.setScale(0.8, 0.8);

  // menambahkan sprite papan skor pada gameplay
  Texture scoreBoard;
  scoreBoard.loadFromFile("papan.png");
  Sprite papanskor(scoreBoard);

  // pengaturan teks skor dan timer
  Text textScore, timer;
  textScore.setFont(font);
  textScore.setCharacterSize(56);
  textScore.setFillColor(Color::White);
  textScore.setStyle(Text::Bold);
  textScore.rotate(345.f);

  timer.setFont(font);
  timer.setCharacterSize(72);
  timer.setFillColor(Color::Black);
  timer.setStyle(Text::Bold);

  while (window.isOpen())
  {
    // harus di dalam while karena posisi mouse berpindah pindah selama render
    auto mousePos = Mouse::getPosition(window);

    Event event{};

    while (window.pollEvent(event))
    {
      if (event.type == Event::Closed)
        window.close();
      for (int a = 0; a < vectorLilin.size(); a++)
      {
        Lilin *lilinPtr = &vectorLilin.at(a);

        // cek posisi mouse
        if (event.type == Event::MouseButtonPressed)
        {
          if (event.mouseButton.button == Mouse::Left)
          {
            if (lilinPtr->getSprite()->getGlobalBounds().contains(mousePos.x,
                                                                  mousePos.y)) // konfirmasi posisi lilin dan kursor
            {
              candleOff.play();
              lilinPtr->setIsOn(false);
              score++;
            }
          }
        }
      }
      if (Event::MouseMoved)
      {
        mouseCursor.setPosition(Mouse::getPosition(window).x,
                                Mouse::getPosition(window).y);
      }
    }
    ImmunityTime = immunityFrame.getElapsedTime();

    window.clear(Color::White);

    // pemilihan mode
    switch (gameCondition)
    {
    case START:           // memilih mode gameplay
      if (audioStay == 1) // pencegahan for loop audio
      {
        gamePlayMusic.play();
        gamePlayMusic.setLoop(true);
        audioStay = 0;
      }
      elapsedInGame = gameTime.getElapsedTime();
      countdown = 7 - (int)elapsedInGame.asSeconds();

      // render text
      textScore.setString("score - " + to_string(score));
      timer.setString(to_string(countdown));

      textScore.setPosition(100, 170);
      timer.setPosition(250, 700);

      window.draw(wallpaper);
      window.draw(ghost);
      window.draw(papanskor);
      window.draw(textScore);
      window.draw(timer);

      // generate lilin sesuai dengan level
      if (vectorLilin.size() == 0)
      {
        for (int a = 0; a < level; a++)
        {
          Lilin newLilin{};
          newLilin.setPosition(
              Vector2f(570 + (rand() % 1070), 135 + (rand() % 720)));
          newLilin.setIsOn(true);
          vectorLilin.push_back(newLilin);
        }
      }

      totalLilinMati = 0;

      //  merender semua lilin dalam vector
      for (int a = 0; a < vectorLilin.size(); a++)
      {
        Lilin *lilinPtr = &vectorLilin.at(a);
        bool keadaanLilin = lilinPtr->getIsOn();

        // menyetel texture sesuai dengan keadaan lilin
        if (keadaanLilin)
        {
          // kondisi nyala
          lilinPtr->setTexture(lilinNyala);
        }
        else
        {
          // kondisi mati
          lilinPtr->setTexture(lilinMati);
          totalLilinMati++;
        }

        lilinPtr->draw(window);

        if (totalLilinMati == vectorLilin.size()) // naik level apabila lilin mati semua
        {
          vectorLilin.clear();
          level++;
          gameTime.restart();
        }
      }
      // pemindahan ke gameover apabila waktu habis
      if (elapsedInGame.asSeconds() > 7)
      {
        gamePlayMusic.stop();
        vectorLilin.clear();
        level = 1;
        audioStay = 1;
        score = 0;
        immunityFrame.restart();
        gameCondition = GAMEOVER;
      }
      break;

    case GAMEOVER:        // memilih mode gameover
      if (audioStay == 1) // pencegahan for loop audio
      {
        gameoverMusic.play();
        audioStay = 0;
      }

      if (menu.getGlobalBounds().contains(mousePos.x, mousePos.y) && ImmunityTime.asSeconds() > 1) // konfirmasi posisi tombol dan kursor
      {
        if (Mouse::isButtonPressed(Mouse::Left)) // konfirmasi klik kiri ditekan
        {
          gameoverMusic.stop();
          audioStay = 1;
          immunityFrame.restart();
          gameCondition = IDLE; // pindah ke main menu
        }
      }
      if (retry.getGlobalBounds().contains(mousePos.x, mousePos.y) && ImmunityTime.asSeconds() > 1) // konfirmasi posisi tombol dan kursor
      {
        if (Mouse::isButtonPressed(Mouse::Left)) // konfirmasi klik kiri ditekan
        {
          gameoverMusic.stop();
          gameTime.restart();
          immunityFrame.restart();
          audioStay = 1;
          gameCondition = START; // kembali ke gameplay
        }
      }
      window.draw(GameOver);
      window.draw(menu);
      window.draw(retry);
      break;

    case IDLE:            // memilih mode gameover
      if (audioStay == 1) // pencegahan for loop audio
      {
        mainmenuMusic.play();
        mainmenuMusic.setLoop(true);
        audioStay = 0;
      }
      if (start.getGlobalBounds().contains(mousePos.x, mousePos.y) && ImmunityTime.asSeconds() > 1) // konfirmasi seperti yang tombol pada game over
      {
        if (Mouse::isButtonPressed(Mouse::Left))
        {
          gameTime.restart();
          immunityFrame.restart();
          mainmenuMusic.stop();
          audioStay = 1;
          gameCondition = START;
        }
      }
      if (Exit.getGlobalBounds().contains(mousePos.x, mousePos.y) && ImmunityTime.asSeconds() > 1)
      {
        if (Mouse::isButtonPressed(Mouse::Left))
        {
          exit(0);
        }
      }
      window.draw(MainMenu);
      window.draw(start);
      window.draw(Exit);
      break;
    }
    window.draw(mouseCursor);
    window.display();
  }

  return 0;
}
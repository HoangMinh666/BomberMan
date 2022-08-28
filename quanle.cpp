//UI
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Until.h"
#include "Map2D.h"
#include "Wall.h"
#include "Portal.h"
#include "Brick.h"
#include "BomberMan.h"
#include "Bomb.h"
#include <conio.h>
#include <thread>
#include <mutex>
#include <ctime>
using namespace std;

#pragma comment (lib, "sfml-graphics-d.lib")
#pragma comment (lib, "sfml-audio-d.lib")
#pragma comment (lib, "sfml-network-d.lib")
#pragma comment (lib, "sfml-window-d.lib")
#pragma comment (lib, "sfml-system-d.lib")
char ch;
using namespace std;
using namespace sf;
int main() {
	//create Object
	
	Map2D map;
	Wall wall(map);  
	Brick brick(map);
	BomberMan player;
	Bomb bomb;
	Portal portal(brick);

	//create UI
	sf::RenderWindow window(sf::VideoMode(MAX_HEIGHT*sizeCell , MAX_WIDTH*sizeCell), "SFML example.");
	
	Texture t1, t2, t3, t4, t5, t6;
	t1.loadFromFile("data/back.png");
	t2.loadFromFile("data/pink.png");
	t3.loadFromFile("data/wall.jpg");
	t4.loadFromFile("data/brick.png");
	t5.loadFromFile("data/white.png");
	t6.loadFromFile("data/bomb.png");
//	t7.loadFromFile("data/player.png");

	Sprite spriteBack(t1);
	Sprite spritePink(t2);
	Sprite spriteWall(t3);
	Sprite spriteBrick(t4);
	Sprite spritePlayer(t5);
	Sprite spriteBomb(t6);

	while (window.isOpen()) {
		Event e; 
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) ch = 75;
		if (Keyboard::isKeyPressed(Keyboard::Right))  ch = 77;
		if (Keyboard::isKeyPressed(Keyboard::Up)) ch = 72;
		if (Keyboard::isKeyPressed(Keyboard::Down)) ch = 80;
		if (Keyboard::isKeyPressed(Keyboard::BackSpace)) ch = 32;

		//Process
		if (ch == 72 || ch == 80 || ch == 75 || ch == 77) player.Move(map, ch, bomb);
		else if (ch == 32) player.SetBomb(bomb, map); 

		player.GetPortal(map, bomb, portal);
		if (!bomb._isExplosion)
			if (bomb.BombExplosion(map, portal, brick, player._bomberMan))
			{
				player.ResetBomberMan(bomb);
				GoToXY(17, MAX_HEIGHT); cout << player._liveLeft;
			}
		//else if (ch == 'p') _getch();
		//player.Move(map, ch, bomb);
		//snake.Catch(&worm);
		//if (snake.CheckIsDead()) {
		//	break;
		//}

		////update UI
		//window.clear();
//		wall.DisplayWallUI(spriteBlue, window);
		
		map.DisplayMapUI(spriteBack, spriteWall,spriteBrick, window);
		//worm.DisplayWormUI(spritePink, window);
		player.DisplayUI(spritePlayer, window);
		bomb.DisplayBombUI(spriteBomb, window);
		window.display();
	}

	return 0;
}

//////////////////////////////////////////////////////////////
//Map2D
void Map2D::DisplayMapUI(sf::Sprite &spBack, sf::Sprite &spWall, sf::Sprite &spBrick, sf::RenderWindow &window) {
	for (int y = 0; y < MAX_HEIGHT; y++)
		for (int x = 0; x < MAX_WIDTH; x++)
		{
			if (_map[y][x].GetC() == '#' || _map[y][x].GetC() == 'W') {
				spWall.setPosition(x*sizeCell , y*sizeCell);
				window.draw(spWall);
			}
			
			else if (_map[y][x].GetC() == 'B')
			{
				spBrick.setPosition(x*sizeCell, y*sizeCell);
				window.draw(spBrick);
			}
			else
			{
				spBack.setPosition(x*sizeCell, y*sizeCell);
				window.draw(spBack);
			}
		}
}

/////////////////////////////////////////////////////////
//Bomberman
void BomberMan::DisplayUI(sf::Sprite &spPlayer, sf::RenderWindow &window)
{
		if(_bomberMan.GetC() == 'X')
		{
			spPlayer.setPosition(_bomberMan.GetX()*sizeCell, _bomberMan.GetY()*sizeCell);
			window.draw(spPlayer);
		
		}

}

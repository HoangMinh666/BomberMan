#include "Game.h"
#include "UI.h"

char ch;

GameBomberMan::GameBomberMan()
{
	map = new Map2D;
	wall = new Wall(map);
	brick = new Brick(map);
	portal = new Portal(brick);
	monster = new Monster(map);
	bomb1 = new Bomb; bomb2 = new Bomb;
	player1 = new BomberMan(1, 1, 'X');
	player2 = new BomberMan(15, 15, 'Y');
}

GameBomberMan::~GameBomberMan()
{
	delete map;
	delete wall;
	delete brick;
	delete portal;
	delete monster;
}

void GameBomberMan::OnePlayerMode(UI *ui)
{
	system("CLS");
	bool finish = false;

	while (ui->window->isOpen()) 
	{
		Event e;

		while (ui->window->pollEvent(e))
		{
			if (e.type == Event::Closed)
				ui->window->close();

			ch = 0;
			if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Left) ch=75;
			else if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Right) ch=77;
			else if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Up) ch=72;
			else if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Down) ch=80;
			else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space) ch = 57;

			//if (Keyboard::isKeyPressed(Keyboard::Left)) ch = 75;
			//if (Keyboard::isKeyPressed(Keyboard::Right)) ch = 77;
			//if (Keyboard::isKeyPressed(Keyboard::Up)) ch = 72;
			//if (Keyboard::isKeyPressed(Keyboard::Down)) ch = 80;

			if (ch == 72 || ch == 80 || ch == 75 || ch == 77) player1->Move(map, ch, bomb1);
			else if (ch == 57 ) player1->SetBomb(bomb1, map);
			//else if (ch == 'p') _getch();


		}
		monster->Move(map);
		player1->GetPortal(map, bomb1, portal);
		if (!bomb1->_isExplosion)
		{
			if (bomb1->BombExplosion(map, portal, brick, &player1->_bomberMan, monster))
				player1->ResetBomberMan(bomb1);
		}
		if (player1->CollideMonster(monster))
		{
			player1->ResetBomberMan(bomb1); GoToXY(17, MAX_HEIGHT);
			cout << player1->_liveLeft << endl;

		}


		if (brick->IsDestroyed() || !player1->_liveLeft || monster->IsDeadAll())
		{
			finish = true;
			break;
		}
		//update UI
		ui->window->clear();
		ui->DisplayMap(map); 
		monster->DisplayMonsterUI(*ui->spriteMonster, *ui->window);	
		player1->DisplayBomberManUI(*ui->spriteBomberMan, *ui->window);
		ui->window->display();

		GoToXY(0, MAX_HEIGHT);  cout << "Remaining lives: " << player1->_liveLeft;

	}
	system("CLS");
	if (brick->IsDestroyed() || monster->IsDeadAll()) cout << "CHUC MUNG BAN DA CHIEN THANG";
	else if (player1->_liveLeft == 0) cout << "GAME OVER";
}

void GameBomberMan::TwoPlayersMode(UI *ui)
{
	system("CLS");

	bool finish = false;
	int scorePlayer1 = 0, scorePlayer2 = 0;

	map->DisplayMap(); player1->Display(); player2->Display();
	GoToXY(0, MAX_HEIGHT);  cout << "Remaining lives of player 1: " << player1->_liveLeft;
	GoToXY(0, MAX_HEIGHT + 1);  cout << "Score of player 1: " << scorePlayer1;
	GoToXY(0, MAX_HEIGHT + 2);  cout << "Remaining lives of player 2: " << player2->_liveLeft;
	GoToXY(0, MAX_HEIGHT + 3);  cout << "Score of player 2: " << scorePlayer2;

	while (ui->window->isOpen()) 
	{
		Event e1;

		while(ui->window->pollEvent(e1))
		{
			if (e1.type == Event::Closed)
				ui->window->close();

			ch = 0;
			if(e1.type == sf::Event::KeyPressed && e1.key.code == sf::Keyboard::A) ch='a';
			else if(e1.type == sf::Event::KeyPressed && e1.key.code == sf::Keyboard::D) ch='d';
			else if(e1.type == sf::Event::KeyPressed && e1.key.code == sf::Keyboard::W) ch='w';
			else if(e1.type == sf::Event::KeyPressed && e1.key.code == sf::Keyboard::S) ch='s';
			else if (e1.type == sf::Event::KeyPressed && e1.key.code == sf::Keyboard::Space) ch = ' ';
			if (ch == 'a' || ch == 'd' || ch == 'w' || ch == 's') player1->Move(map, ch, bomb1);
			else if (ch == ' ' ) player1->SetBomb(bomb1,map);

			if(e1.type == sf::Event::KeyPressed && e1.key.code == sf::Keyboard::Left) ch=75;
			else if(e1.type == sf::Event::KeyPressed && e1.key.code == sf::Keyboard::Right) ch=77;
			else if(e1.type == sf::Event::KeyPressed && e1.key.code == sf::Keyboard::Up) ch=72;
			else if(e1.type == sf::Event::KeyPressed && e1.key.code == sf::Keyboard::Down) ch=80;
			else if (e1.type == sf::Event::KeyPressed && e1.key.code == sf::Keyboard::Return) ch = 58;
			if (ch == 72 || ch == 80 || ch == 75 || ch == 77) player2->Move(map, ch, bomb2);
			else if (ch == 58 ) player2->SetBomb(bomb2, map);

		
		}
		monster->Move(map);
		if (!player1->CheckIsDead())
		{
			player1->Display();
			player1->GetPortal(map, bomb1, portal); GoToXY(19, MAX_HEIGHT + 1);  cout << scorePlayer1;
			if (!bomb1->_isExplosion)
			{
				bool isCompetitorDead = player2->CheckIsDead();
				if (bomb1->BombExplosionMode2(map, portal, brick, &player1->_bomberMan, &player2->_bomberMan, monster, scorePlayer1, isCompetitorDead))
				{
					player1->ResetBomberManMode2(bomb1, 1, 1, 'X');
					GoToXY(29, MAX_HEIGHT);	cout << player1->_liveLeft;
				}
				if (isCompetitorDead)
				{
					player2->ResetBomberManMode2(bomb2, 15, 15, 'Y');
					GoToXY(29, MAX_HEIGHT + 2);	cout << player2->_liveLeft;
				}
				GoToXY(19, MAX_HEIGHT + 1);  cout << scorePlayer1;
			}
			if (player1->CollideMonster(monster))
			{
				player1->ResetBomberManMode2(bomb1, 1, 1, 'X');
				GoToXY(29, MAX_HEIGHT);	cout << player1->_liveLeft;
			}
		}

		if (!player2->CheckIsDead())
		{
			player2->Display();
			player2->GetPortal(map, bomb2, portal); GoToXY(19, MAX_HEIGHT + 3);  cout << scorePlayer2;
			if (!bomb2->_isExplosion && !player2->CheckIsDead())
			{
				bool isCompetitorDead = player1->CheckIsDead();
				if (bomb2->BombExplosionMode2(map, portal, brick, &player2->_bomberMan, &player1->_bomberMan, monster, scorePlayer2, isCompetitorDead))
				{
					player2->ResetBomberManMode2(bomb2, 15, 15, 'Y');
					GoToXY(29, MAX_HEIGHT + 2);	cout << player2->_liveLeft;
				}
				if (isCompetitorDead)
				{
					player1->ResetBomberManMode2(bomb1, 1, 1, 'X');
					GoToXY(29, MAX_HEIGHT);	cout << player1->_liveLeft;
				}
				GoToXY(19, MAX_HEIGHT + 3);  cout << scorePlayer2;
			}

			if (player2->CollideMonster(monster))
			{
				player2->ResetBomberManMode2(bomb2, 15, 15, 'Y');
				GoToXY(29, MAX_HEIGHT + 2);	cout << player2->_liveLeft;
			}
		}

		//update UI
		ui->window->clear();
		ui->DisplayMap(map); 
		monster->DisplayMonsterUI(*ui->spriteMonster, *ui->window);	
		player1->DisplayBomberManUI(*ui->spriteBomberMan, *ui->window);
		player2->DisplayBomberManUI(*ui->spriteBomberMan2, *ui->window);
		ui->window->display();

		if (brick->IsDestroyed() || monster->IsDeadAll() || (player2->CheckIsDead() && player1->CheckIsDead()))
		{
			finish = true;
			break;
		}
	}
	system("CLS");
	cout << scorePlayer1 << " " << scorePlayer2 << endl;

	if (scorePlayer1 > scorePlayer2) cout << "PLAYER 1 WIN!";
	else if (scorePlayer1 < scorePlayer2) cout << "PLAYER 2 WIN!";
	else cout << "DRAW!";
}

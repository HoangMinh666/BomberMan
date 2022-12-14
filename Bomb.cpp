#include "Bomb.h"
#include <iostream>
using namespace std;

const int px[4] = { 0, 0, -1, 1 };
const int py[4] = { -1, 1, 0, 0 };

Bomb::Bomb()
{
	_power = 1; _isExplosion = true; _start = 0;
}

Bomb::~Bomb() {}

bool Bomb::BombExplosion(Map2D* map, Portal* portal, Brick* brick, Point2D* player, Monster* monster)
{
	if (_start > 0 && ((clock() - _start) / (double)CLOCKS_PER_SEC) < 2)
		return false;
	_start = 0;
	int x = _bomb.GetX(), y = _bomb.GetY();
	bool isBomberManExplosion = false;

	//Hien thi power cua bom
	GoToXY(x, y); cout << '=';
	bool direct[4] = { true, true, true, true };
	for (int i = 1; i <= _power; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (!direct[j]) continue;
			else
			{
				int x1 = x + i * px[j], y1 = y + i * py[j];
				if (map->_map[y1][x1].GetC() == 'W' || map->_map[y1][x1].GetC() == '#') { direct[j] = false; continue; }
				else
				{
					GoToXY(x1, y1), cout << '=';
					if (map->_map[y1][x1].GetC() == 'B') direct[j] = false;
				}
			}
		}
	}
	Sleep(90);

	//Hien thi portal
	direct[0] = direct[1] = direct[2] = direct[3] = true;
	for (int i = 1; i <= _power; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (!direct[j]) continue;
			else
			{
				int x1 = x + i * px[j], y1 = y + i * py[j];
				int idPortal = portal->GetIdOfPortal(x1, y1);
				int idMonster = monster->GetIdOfMonster(x1, y1);

				if (idMonster < MAX_MONSTER) monster->_checkDead[idMonster] = true;
				if (x1 == player->GetX() && y1 == player->GetY()) isBomberManExplosion = true;

				if (map->_map[y1][x1].GetC() == 'W' || map->_map[y1][x1].GetC() == '#')
				{
					direct[j] = false;
					continue;
				}
				if (map->_map[y1][x1].GetC() == 'B' && !brick->_isDestroyed[y1][x1])
				{
					map->_map[y1][x1] = portal->_portal[idPortal];
					direct[j] = false;
					brick->_isDestroyed[y1][x1] = true;
				}
				map->_map[y1][x1].Display();
			}
		}
	}
	map->_map[y][x] = Point2D(x, y, ' '); map->_map[y][x].Display();
	_isExplosion = true;
	if (x == player->GetX() && y == player->GetY()) isBomberManExplosion = true;

	if (isBomberManExplosion) return true;
	else return false;
}

bool Bomb::BombExplosionMode2(Map2D* map, Portal* portal, Brick* brick, Point2D* player, Point2D* competitor, Monster* monster, int& score, bool& checkCompetitor)
{
	if (_start > 0 && ((clock() - _start) / (double)CLOCKS_PER_SEC) < 2)
		return false;
	_start = 0;
	int x = _bomb.GetX(), y = _bomb.GetY();
	bool isBomberManExplosion = false;

	//Hien thi power cua bom
	GoToXY(x, y); cout << '=';
	bool direct[4] = { true, true, true, true };
	for (int i = 1; i <= _power; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (!direct[j]) continue;
			else
			{
				int x1 = x + i * px[j], y1 = y + i * py[j];
				if (map->_map[y1][x1].GetC() == 'W' || map->_map[y1][x1].GetC() == '#') { direct[j] = false; continue; }
				else
				{
					GoToXY(x1, y1), cout << '=';
					if (map->_map[y1][x1].GetC() == 'B') direct[j] = false;
				}
			}
		}
	}
	Sleep(90);

	//Hien thi portal
	direct[0] = direct[1] = direct[2] = direct[3] = true;
	for (int i = 1; i <= _power; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (!direct[j]) continue;
			else
			{
				int x1 = x + i * px[j], y1 = y + i * py[j];
				int idPortal = portal->GetIdOfPortal(x1, y1);
				int idMonster = monster->GetIdOfMonster(x1, y1);

				if (idMonster < MAX_MONSTER)
				{
					monster->_checkDead[idMonster] = true;
					score += 100;
				}
				if (x1 == player->GetX() && y1 == player->GetY()) isBomberManExplosion = true;
				if (x1 == competitor->GetX() && y1 == competitor->GetY() && !checkCompetitor)
				{
					map->_map[y1][x1] = Point2D(x1, y1, ' ');
					checkCompetitor = true;
					score += 100;
				}

				if (map->_map[y1][x1].GetC() == 'W' || map->_map[y1][x1].GetC() == '#')
				{
					direct[j] = false;
					continue;
				}

				if (map->_map[y1][x1].GetC() == 'B' && !brick->_isDestroyed[y1][x1])
				{
					map->_map[y1][x1] = portal->_portal[idPortal];
					score += 50;
					direct[j] = false;
					brick->_isDestroyed[y1][x1] = true;
				}
				map->_map[y1][x1].Display();
			}
		}
	}
	map->_map[y][x] = Point2D(x, y, ' '); map->_map[y][x].Display();
	_isExplosion = true;
	if (x == player->GetX() && y == player->GetY()) isBomberManExplosion = true;

	if (isBomberManExplosion) return true;
	else return false;
}

void Bomb::SetBomb(int x, int y, Map2D* map)
{
	if (map->_map[y][x].GetC() == ' ')
	{
		_bomb = Point2D(x, y, 'O');
		map->_map[y][x] = _bomb;
		_bomb.Display();
	}
	_isExplosion = false;
	_start = clock();
}

void Bomb::PowerUp()
{
	if (_power < 5) ++_power;
}

void Bomb::ResetBomb()
{
	_power = 1;
}
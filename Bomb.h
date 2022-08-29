#pragma once
#include "Point2D.h"
#include "Wall.h"
#include "Brick.h"
#include "Portal.h"
#include "Monster.h"
#include "UI.h"
#include <ctime>

class Bomb
{
public:
	Bomb();
	~Bomb();

	Point2D _bomb;
	Point2D _portalBehind[4][5];
	int		_power;
	bool	_isExplosion;
	bool	_isReset;
	int		_start;
	int		_starExplosion;

	bool	BombExplosion(Map2D* map, Portal* portal, Brick* brick, Point2D* player, Monster* monster);
	bool	BombExplosionMode2(Map2D* map, Portal* portal, Brick* brick, Point2D* player, Point2D* competitor, Monster* monster, int& score, bool& checkCompetitor);
	void	SetBomb(int x, int y, Map2D* map);
	void	PowerUp();
	void	ResetBomb();
	void	ResetDisplayBomb(Map2D *map);

private:

};

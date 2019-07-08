#pragma once
#include "ScreenController.h"
#include "GameEntity.h"
#include <cstdlib>
#include <ctime>
#include <deque>
#include "PlayerEntity.h"


enum TileType { GRASS, WATER, ROAD, RAIL, TREE };

typedef struct Tile {
	TileType type;
	GE_Rect rect;
	GE_Texture* texture;
};


class GameMapBuilder {
protected:
	int columns;
	int rows;
	Tile** map;
	const int TILE_LENGTH = 100;
	GE_Rect realPlayerClip = { 30,0,40,100 };
	Tile grassTile, treeTile, waterTile, roadTile, railTile;
	int maxTreesInARow = 2;
	GameEntity Car, Stick, Train, Lamp, Eagle, Coin;
	deque < GameEntity>objects;
	GE_Texture* car2Texture, * car3Texture;
	int maxSticksInARow = 3;  int minSticksInARow = 1;
	int FPS = 60;
public:
	GameMapBuilder() {
		columns = ceil(GE::WINDOW_WIDTH / TILE_LENGTH);
		rows = ceil(GE::WINDOW_HEIGHT / TILE_LENGTH) + 1;
		map = new Tile * [columns];
		for (int i = 0; i < columns; i++)
			map[i] = new Tile[rows];
		load();
		for (int y = rows - 1; y >= 0; y--)
		{
			if (y >= rows - 2) {
				for (int x = 0; x < columns; x++) {
					map[x][y] = (x == 0 || x == columns - 1) ? treeTile : grassTile;
					map[x][y].rect.x = x * 100;
					map[x][y].rect.y = (y - 1) * 100;
				}
			}
			else
			{
				generateTiles(y);
				addObjects(y);
			}
		}
	}

	void drawTiles()
	{
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				GE::GE_RenderCopy(map[i][j].texture, &map[i][j].rect);
			}
		}
	}

	void drawObjects()
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i].direction == LEFT)
				GE::GE_RenderCopy(objects[i].texture, &objects[i].rect);
			else
				GE::GE_RenderCopyEx(objects[i].texture, &objects[i].rect, SDL_FLIP_HORIZONTAL);
		}
	}


	void generateTiles(int row)
	{
		TileType rowType = (TileType)(rand() % 4);
		int rowChance = rand() % 100 + 1;
		int Trees = 0;
		for (int x = 0; x < columns; x++)
		{
			if (rowChance <= 50 /*rowType == GRASS*/) {
				int chance = rand() % 100 + 1;
				if (x == 0 || x == columns - 1)
					map[x][row] = treeTile;
				else {
					if (chance >= 80 && Trees < maxTreesInARow) {
						map[x][row] = treeTile;
						Trees++;
					}
					else {
						map[x][row] = grassTile;
					}
				}
			}
			else if (rowChance <= 70 /*rowType == ROAD*/) {
				map[x][row] = roadTile;
			}
			else if (rowChance <= 75 /*rowType == RAIL*/) {
				map[x][row] = railTile;
			}
			else if (rowChance <= 100 /*rowType == WATER*/) {
				map[x][row] = waterTile;
			}

			map[x][row].rect.x = x * 100;
			map[x][row].rect.y = map[x][row + 1].rect.y - TILE_LENGTH;
		}
	}

	void addObjects(int row)
	{
		if (map[0][row].type != WATER) addCoins(row);
		switch (map[0][row].type)
		{
		case ROAD:
			addCar(row);
			break;
		case WATER:
			addStick(row);
			break;
		case RAIL:
			addTrain(row);
			break;
		}
	}

	void addCar(int row) {
		Direction dir = (Direction)((rand() % 2) + 2);
		Car.direction = dir;
		Car.rect.y = map[0][row].rect.y;
		Car.point.y = row;
		Car.moveSpeed = (rand() % 5 + 2) * (dir == RIGHT ? 1 : -1);
		int num = (rand() % 3) + 1;
		int space = (GE::WINDOW_WIDTH - ((num - 1) * Car.rect.w)) / num;
		int randPos = rand() % GE::WINDOW_WIDTH;
		for (int i = 0; i < num; i++)
		{
			objects.push_back(Car);
			if (dir == LEFT)
				objects.back().rect.x = GE::WINDOW_WIDTH + (i * (space + Car.rect.w)) - randPos;
			else
				objects.back().rect.x = -Car.rect.w - (i * (space + Car.rect.w)) + randPos;
			short int tex = rand() % 3;
			switch (tex)
			{
			case 1:
				objects.back().texture = car2Texture;
				break;
			case 2:
				objects.back().texture = car3Texture;
				break;
			}
		}
	}
	void addStick(int row)
	{
		Stick.rect.y = map[0][row].rect.y;
		Stick.point.y = row;
		if (map[0][row + 1].type == WATER) { // Direction of two rivers should be diffrent.
			for (int i = objects.size() - 1; i >= 0; i--) {
				if (objects[i].point.y == row + 1 && objects[i].type == STICK) {
					Stick.direction = (objects[i].direction == LEFT ? RIGHT : LEFT);
					Stick.isMoving = (objects[i].isMoving ? rand() % 2 : true); //We Cant Have Two Stopped Rivers
					break;
				}
			}
		}
		else {
			Stick.isMoving = rand() % 2;
			Stick.direction = (Direction)((rand() % 2) + 2);
		}

		if (Stick.isMoving) {
			Stick.moveSpeed = (rand() % 3 + 1) * (Stick.direction == RIGHT ? 1 : -1);
			int num = (rand() % 4) + 3;
			int space = (GE::WINDOW_WIDTH - ((num - 1) * Stick.rect.w)) / num;
			int randPos = rand() % 200;
			for (int i = 0; i < num; i++)
			{
				objects.push_back(Stick);
				if (Stick.direction == LEFT)
					objects.back().rect.x = randPos + (i * (space + Stick.rect.w));
				else
					objects.back().rect.x = GE::WINDOW_WIDTH - randPos - (i * (space + Stick.rect.w));
			}
		}
		else
		{
			int num = 0;
			do {
				for (int i = 0; i < columns; i++)
				{
					if (map[i][row + 1].type != TREE) {
						int chance = rand() % 100 + 1;
						if (chance >= 50 && num < maxSticksInARow) {
							num++;
							Stick.rect.x = map[i][row].rect.x;
							objects.push_back(Stick);
						}
					}
				}
			} while (num < minSticksInARow);
		}
	}

	void addTrain(int row) {
		Direction dir = (Direction)((rand() % 2) + 2);
		Train.direction = dir;
		Train.rect.y = map[0][row].rect.y;
		Train.point.y = row;
		Train.timer = ((rand() % 5) + 2) * FPS;

		objects.push_back(Train);
		if (dir == LEFT)
			objects.back().rect.x = GE::WINDOW_WIDTH + Train.rect.w;
		else
			objects.back().rect.x = -Train.rect.w;

		Lamp.point.y = row;
		Lamp.rect.y = map[0][row].rect.y + TILE_LENGTH - Lamp.rect.h;
		Lamp.rect.x = map[0][row].rect.x + 20;
		objects.push_back(Lamp);
	}

	void addCoins(int row) {
		int maxCoinsInARow = 3;
		int num = 0;
		Coin.rect.y = map[0][row].rect.y;
		Coin.rect.w = Coin.rect.h = 100;
		Coin.point.y = row;
		for (int i = 0; i < columns; i++)
		{
			if (map[i][row].type != TREE) {
				int chance = rand() % 101;
				if (chance >= 90 && num < maxCoinsInARow) {
					num++;
					Coin.rect.x = map[i][row].rect.x;
					objects.push_back(Coin);
				}
			}
		}
	}

	void deleteObjects()
	{
		while (!objects.empty() && objects.front().point.y == rows - 1) {
			objects.pop_front();
		}
	}

	void draw()
	{
		drawTiles();
		drawObjects();
	}


	void load() {
		grassTile.texture = GE::GE_LoadImage("assets/image/grass.png");
		grassTile.rect = { 0,0,100,100 };
		grassTile.type = GRASS;

		treeTile.texture = GE::GE_LoadImage("assets/image/tree.png");
		treeTile.rect = { 0,0,100,100 };
		treeTile.type = TREE;

		waterTile.texture = GE::GE_LoadImage("assets/image/sea.png");
		waterTile.rect = { 0,0,100,100 };
		waterTile.type = WATER;

		roadTile.texture = GE::GE_LoadImage("assets/image/street.png");
		roadTile.rect = { 0,0,100,100 };
		roadTile.type = ROAD;

		railTile.texture = GE::GE_LoadImage("assets/image/rail.png");
		railTile.rect = { 0,0,100,100 };
		railTile.type = RAIL;


		Car.texture = GE::GE_LoadImage("assets/image/car.png");
		Car.rect = { 0,0,169,100 };
		Car.isMoving = true;
		Car.type = CAR;

		car2Texture = GE::GE_LoadImage("assets/image/car2.png");
		car3Texture = GE::GE_LoadImage("assets/image/car3.png");

		Stick.texture = GE::GE_LoadImage("assets/image/stick1.png");
		Stick.rect = { 0,0,100,100 };
		Stick.type = STICK;

		Train.texture = GE::GE_LoadImage("assets/image/train.png");
		Train.rect = { 0,0,544,100 };
		Train.type = TRAIN;

		Lamp.texture = GE::GE_LoadImage("assets/image/green-lamp.png");
		Lamp.rect = { 0,0,39,93 };
		Lamp.type = LAMP;

		Eagle.texture = GE::GE_LoadImage("assets/image/eagle.png");
		Eagle.rect = { 0,0,426,184 };
		Eagle.type = EAGLE;
		Eagle.moveSpeed = 20;

		Coin.texture = GE::GE_LoadImage("assets/image/coin.png");
		Coin.rect = { GE::WINDOW_WIDTH - 100,20,100,100 };
		Coin.direction = LEFT;
		Coin.type = COIN;


	}

};
class PlayGameScreen : public ScreenController {
public:
	GameMapBuilder mapbuilder;
	PlayGameScreen() {
		
	}
	void Render() {
		mapbuilder.draw();
	}
private:

};
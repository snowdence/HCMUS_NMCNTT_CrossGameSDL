#pragma once
#include "ScreenController.h"
#include "GameEntity.h"
#include <cstdlib>
#include <ctime>
#include <deque>
enum TileType { GRASS, WATER, ROAD, RAIL, TREE };

struct Tile {
	TileType type;
	GE_Rect position;
	GE_Texture* texture;
};
class PlayGameScreen : public ScreenController
{
private:
public:
	GE_Texture* redLampTexture;

	PlayGameScreen() {
		this->load();
		
	}
	GameEntity Stick;
	GE_Texture* car2Texture;
	GE_Texture* car3Texture;
	deque<GameEntity> objects;
	GameEntity Car;
	Tile** map = NULL;
	Tile grassTile, treeTile, waterTile, roadTile, railTile;
	int columns = 0; 
	int rows = 0;
	const int TILE_LENGTH = 100;
	const int playerMoveSpeed = 10;
	const int maxTreesInARow = 2; //Choose it carefully! if it be great enough , Player stuck in trees!!!!
	const int maxSticksInARow = 5;
	const int minSticksInARow = 2;
	const int maxCoinsInARow = 2;

	void load() {
		Car.textures = GE::GE_LoadImage("assets/image/car.png");
		Car.rect = { 0,0,169,100 };
		Car.isMoving = true;
		Car.type = CAR;

		car2Texture = GE::GE_LoadImage("assets/image/car2.png");
		car3Texture = GE::GE_LoadImage("assets/image/car3.png");



		waterTile.texture = GE::GE_LoadImage("assets/image/sea.png");
		waterTile.position = { 0,0,100,100 };
		waterTile.type = WATER;

		roadTile.texture =  GE::GE_LoadImage("assets/image/street.png");
		roadTile.position = { 0,0,100,100 };
		roadTile.type = ROAD;

		railTile.texture = GE::GE_LoadImage("assets/image/rail.png");
		railTile.position = { 0,0,100,100 };
		railTile.type = RAIL;

		Stick.textures = GE::GE_LoadImage("assets/image/stick1.png");
		Stick.rect = { 0,0,100,100 };
		Stick.type = STICK;

		grassTile.texture = GE::GE_LoadImage("assets/image/grass.png");
		grassTile.position = { 0,0,100,100 };
		grassTile.type = GRASS;

		treeTile.texture = GE::GE_LoadImage("assets/image/tree.png");
		treeTile.position = { 0,0,100,100 };
		treeTile.type = TREE;

		waterTile.texture = GE::GE_LoadImage("assets/image/sea.png");
		waterTile.position = { 0,0,100,100 };
		waterTile.type = WATER;


		Lamp.textures = GE::GE_LoadImage("assets/image/green-lamp.png");
		Lamp.rect = { 0,0,39,93 };
		Lamp.type = LAMP;
		redLampTexture = GE::GE_LoadImage("assets/image/red-lamp.png");

		initTiles();

	}

	

	void Render() {
		this->draw();
	}
	void initTiles()
	{
		columns = ceil(GE::WINDOW_WIDTH / TILE_LENGTH);
		rows = ceil(GE::WINDOW_HEIGHT / TILE_LENGTH) + 1;
		map = new Tile * [columns];
		for (int i = 0; i < columns; i++)
			map[i] = new Tile[rows];

		for (int y = rows - 1; y >= 0; y--)
		{
			if (y >= rows - 2) {
				for (int x = 0; x < columns; x++) {
					map[x][y] = (x == 0 || x == columns - 1) ? treeTile : grassTile;
					map[x][y].position.x = x * 100;
					map[x][y].position.y = (y - 1) * 100;
				}
			}
			else
			{
				generateTiles(y);
				addObjects(y);
			}
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

			map[x][row].position.x = x * 100;
			map[x][row].position.y = map[x][row + 1].position.y - TILE_LENGTH;
		}
	}
	void addObjects(int row)
	{
		//if (map[0][row].type != WATER) addCoins(row);
		switch (map[0][row].type)
		{
		case ROAD:
			addCar(row);
			break;
		case WATER:
			addStick(row);
			break;
		case RAIL:
			addCar(row);
			break;
		}
	}
	GameEntity Train;
	GameEntity Lamp;
	void addTrain(int row) {
		Direction dir = (Direction)((rand() % 2) + 2);
		Train.direction = dir;
		Train.rect.y = map[0][row].position.y;
		Train.point.y = row;
		Train.timer = ((rand() % 5) + 2) * 60;

		objects.push_back(Train);
		if (dir == LEFT)
			objects.back().rect.x = GE::WINDOW_WIDTH + Train.rect.w;
		else
			objects.back().rect.x = -Train.rect.w;

		Lamp.point.y = row;
		Lamp.rect.y = map[0][row].position.y + TILE_LENGTH - Lamp.rect.h;
		Lamp.rect.x = map[0][row].position.x + 20;
		objects.push_back(Lamp);
	}
	void addCar(int row) {
		Direction dir = (Direction)((rand() % 2) + 2);
		Car.direction = dir;
		Car.rect.y = map[0][row].position.y;
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
				objects.back().textures = car2Texture;
				break;
			case 2:
				objects.back().textures = car3Texture;
				break;
			}
		}
	}
	void addStick(int row)
	{
		Stick.rect.y = map[0][row].position.y;
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
							Stick.rect.x = map[i][row].position.x;
							objects.push_back(Stick);
						}
					}
				}
			} while (num < minSticksInARow);
		}
	}
	
	void drawTiles()
	{
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				GE::GE_RenderCopy(map[i][j].texture, &map[i][j].position, 0);
			}
		}
	}
	void drawObjects()
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i].direction == LEFT)
				GE::GE_RenderCopy(objects[i].textures, &(objects[i].rect), false);
			else
				GE::GE_RenderCopy(objects[i].textures, &objects[i].rect, SDL_FLIP_HORIZONTAL, false);
		}
	}

	void draw() {
		drawTiles();
		drawObjects();

	}
	
};


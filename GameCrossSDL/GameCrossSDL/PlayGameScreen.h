#pragma once
#include "ScreenController.h"
#include "GELibrary.h"
#include <cstdlib>
#include <iostream>
#include "PlayerEntity.h"
#include <deque>
#include <ctime>
#include "CarGameEntity.h"
using namespace std;

enum TileType { GRASS, WATER, ROAD, RAIL, TREE };

struct Tile {
	TileType type;
	GE_Rect rect;
	GE_Texture* texture;
};


class MapBuilder {
protected:
#pragma region DEFINE Variable

	//Constants
	const int TILE_LENGTH = 100;
	const int playerMoveSpeed = 10;
	const int maxTreesInARow = 2; 
	const int maxSticksInARow = 3;
	const int minSticksInARow = 2;
	const int maxCoinsInARow = 2;
	const int FPS = 60;
	const int delay = 1000 / FPS;
	const int cameraBaseSpeed = 1;
	int trainMoveSpeed = 18;

	//Variables
	int startLoop, endLoop;
	int cameraSpeed = cameraBaseSpeed;
	int gameEvent;
	int rows, columns;
	int maxScore = 0, score = 0, topScore = 0, coins = 0;
	char chars[10] = ""; //Used in updateScore();
	bool eagleIntersection = false;

	GE_Rect realPlayerClip = { 20,0,40,100 };
	Tile grassTile, treeTile, waterTile, roadTile, railTile;
	deque<GameEntity> objects;

	Tile** map = NULL;
	GameEntity _Car, Stick, Train, Lamp, Eagle, Coin;
	GE_Texture* car2Texture;
	GE_Texture* car3Texture;
	GE_Texture* redLampTexture;
	PlayerEntity* Player;
	CarGameEntity Car;
#pragma endregion

public:
	EGameController* state;
	
	GE_Font* font;
	MapBuilder(PlayerEntity* player) {
		this->Player = player;
		this->load();
	
	}


	void update()
	{
		adjustCameraSpeed();

		//update all
		for (int y = 0; y < rows; y++)
			for (int x = 0; x < columns; x++)
				map[x][y].rect.y += cameraSpeed;

		for (int i = 0; i < objects.size(); i++) {
			objects[i].rect.y += cameraSpeed;
			if (objects[i].type == CAR){
				objects[i].HandleOutBorder();
			}
			/*if (objects[i].type == CAR) {
				if (objects[i].direction == LEFT && objects[i].rect.x < -Car.rect.w) {
					objects[i].rect.x = GE::WINDOW_WIDTH;
				}
				else if (objects[i].direction == RIGHT && objects[i].rect.x > GE::WINDOW_WIDTH) {
					objects[i].rect.x = -Car.rect.w;
				}
			}*/

			if (objects[i].type == STICK) {
				if (objects[i].direction == LEFT && objects[i].rect.x < -Stick.rect.w)
					objects[i].rect.x = GE::WINDOW_WIDTH;
				else if (objects[i].direction == RIGHT && objects[i].rect.x > GE::WINDOW_WIDTH)
					objects[i].rect.x = -Stick.rect.w;
			}

			if (objects[i].type == TRAIN) {
				if (objects[i].isMoving) {
					if (objects[i].direction == LEFT && objects[i].rect.x < -Train.rect.w) {
						objects[i].rect.x = GE::WINDOW_WIDTH;
						objects[i].isMoving = false;
						objects[i + 1].texture = Lamp.texture; // We Are Sure there is a lamp just after a train!!
						objects[i].timer = ((rand() % 5) + 2) * FPS;
					}
					else if (objects[i].direction == RIGHT && objects[i].rect.x > GE::WINDOW_WIDTH) {
						objects[i].rect.x = -Train.rect.w;
						objects[i].isMoving = false;
						objects[i + 1].texture = Lamp.texture;
						objects[i].timer = ((rand() % 5) + 2) * FPS;
					}
				}
				else {
					if (objects[i].timer == FPS) {  //1 Second Before it move we should change the lamp!!
						objects[i + 1].texture = redLampTexture;
					}
					if (objects[i].timer == 0) {
						objects[i].isMoving = true;
					}
					else
						objects[i].timer--;
				}
			}
			if (objects[i].isMoving) {
				objects[i].HandleMove();
			}
/*
			if (objects[i].isMoving) {
				if (objects[i].type == TRAIN)
					objects[i].rect.x += (objects[i].direction == LEFT ? -trainMoveSpeed : trainMoveSpeed);
				else
					objects[i].rect.x += objects[i].moveSpeed;
			}*/
		}

		if (map[0][rows - 1].rect.y > GE::WINDOW_HEIGHT) {
			for (int y = rows - 1; y > 0; y--)
				for (int x = 0; x < columns; x++)
					map[x][y] = map[x][y - 1];

			generateTiles(0);
			deleteObjects();

			for (int i = 0; i < objects.size(); i++)
				objects[i].point.y += 1;
			addObjects(0);
			Player->point.y += 1;
		}
		Player->rect.y += cameraSpeed;

		checkPlayerStatus();
	}
	//enum TileType { GRASS, WATER, ROAD, RAIL, TREE };

	void checkPlayerStatus() {
		if (*state != EGameController::PLAY) return;

		if (Player->rect.y + 5 >= GE::WINDOW_HEIGHT) {
			*state = EGameController::OUT;
			Eagle.rect.x = Player->rect.x - (Eagle.rect.w / 2);
			Eagle.rect.y = -Eagle.rect.h;
			eagleIntersection = false;
			return;
		}

		if (Player->rect.x <= -TILE_LENGTH || Player->rect.x >= GE::WINDOW_WIDTH){
#ifdef GE_DEBUG
			cout << "Out of border window" << endl;
#endif
			* state = EGameController::GAME_OVER;
		}

		bool onStick = false;
		GE_Rect temp = { Player->rect.x + realPlayerClip.x + 5 , Player->rect.y + realPlayerClip.y + 5 , realPlayerClip.w - 5,realPlayerClip.h - 5 };
		for (int i = 0; i < objects.size(); i++) {
			if (SDL_HasIntersection(&temp, &objects[i].rect) == SDL_TRUE) {
				if (objects[i].type == CAR || objects[i].type == TRAIN) {
					*state = EGameController::GAME_OVER;
			
#ifdef GE_DEBUG
					cout << "Train or Car collision" << endl;
#endif // GE_DEBUG

				}
				else if (objects[i].type == STICK) {
					onStick = true;
					if (!Player->isMoving) {
						if (objects[i].isMoving) Player->rect.x += objects[i].moveSpeed;
						if (Player->rect.x >= map[Player->point.x][0].rect.x + TILE_LENGTH) Player->point.x++;
						else if (Player->rect.x <= map[Player->point.x][0].rect.x - TILE_LENGTH) Player->point.x--;
						if (Player->point.x >= columns || Player->point.x < 0) { 
#ifdef GE_DEBUG
							cout << "Stick drop out" << endl;
#endif // GE_DEBUG

							*state = EGameController::GAME_OVER;
						}
					}
				}
				else if (objects[i].type == COIN) {
					coins++;
					//G_PlaySound(Coin.sound, 0);
					//updateScore();
					Player->setCoin(1); 
					cout << "Coin : " << Player->getCoin() << endl;
					objects.erase(objects.begin() + i);
					i--;
					continue;
				}
			}
		}
		if (!Player->isMoving && !onStick && map[Player->point.x][Player->point.y].type == WATER)
		{
#ifdef GE_DEBUG
			cout << "Water drop out " << endl;
#endif // DEBUG

			* state = EGameController::GAME_OVER;
		}
	}

	void adjustCameraSpeed()
	{
		if (*state == EGameController::OUT) {
			cameraSpeed = -2;
			return;
		}
		if (*state != EGameController::PLAY) {
			cameraSpeed = 0;
			return;
		}
		cameraSpeed = cameraBaseSpeed;
		switch (Player->point.y) {
		case 4:
			cameraSpeed += 1;
			break;
		case 3:
			cameraSpeed += 3;
			break;
		case 2:
			cameraSpeed += 6;
			break;
		case 1:
			cameraSpeed = playerMoveSpeed;
			break;
		}
	}
	void initTiles() {

		srand(time(NULL));
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
			Car.RandomSkin();
			objects.push_back(Car);
			if (dir == LEFT)
				objects.back().rect.x = GE::WINDOW_WIDTH + (i * (space + Car.rect.w)) - randPos;
			else
				objects.back().rect.x = -Car.rect.w - (i * (space + Car.rect.w)) + randPos;
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


		/*Car.texture = GE::GE_LoadImage("assets/image/car.png");
		Car.rect = { 0,0,169,100 };
		Car.isMoving = true;
		Car.type = CAR;
		car2Texture = GE::GE_LoadImage("assets/image/car2.png");
		car3Texture = GE::GE_LoadImage("assets/image/car3.png");*/

		Stick.texture = GE::GE_LoadImage("assets/image/stick1.png");
		Stick.rect = { 0,0,100,100 };
		Stick.type = STICK;

		Train.texture = GE::GE_LoadImage("assets/image/train.png");
		Train.rect = { 0,0,544,100 };
		Train.type = TRAIN;

		Lamp.texture = GE::GE_LoadImage("assets/image/green-lamp.png");
		Lamp.rect = { 0,0,39,93 };
		Lamp.type = LAMP;
		redLampTexture = GE::GE_LoadImage("assets/image/red-lamp.png");

		Eagle.texture = GE::GE_LoadImage("assets/image/eagle.png");
		Eagle.rect = { 0,0,426,184 };
		Eagle.type = EAGLE;
		Eagle.moveSpeed = 20;

		Coin.texture = GE::GE_LoadImage("assets/image/coin.png");
		Coin.rect = { GE::WINDOW_WIDTH - 100,20,100,100 };
		Coin.direction = LEFT;
		Coin.type = COIN;

		initTiles();



		Player->point = { columns / 2 , rows - 2 };
		Player->rect = map[Player->point.x][Player->point.y].rect;
		Player->type = PLAYER;


	}

	void HandleMove() {
		if (Player->isMoving) {
			switch (Player->direction)
			{
			case UP:
				Player->rect.y -= playerMoveSpeed;
				if (Player->rect.y <= map[Player->point.x][Player->point.y - 1].rect.y) {
					Player->isMoving = false;
					score++;
					Player->point.y--;
					Player->rect.y = map[Player->point.x][Player->point.y].rect.y;
					//Player->rect.x = map[Player->point.x][Player->point.y].rect.x;
				}
				break;
			case RIGHT:
				Player->rect.x += playerMoveSpeed;
				if (Player->rect.x >= map[Player->point.x + 1][Player->point.y].rect.x) {
					Player->isMoving = false;
					Player->point.x++;
					Player->rect.x = map[Player->point.x][Player->point.y].rect.x;
				}
				break;
			case LEFT:
				Player->rect.x -= playerMoveSpeed;
				if (Player->rect.x <= map[Player->point.x - 1][Player->point.y].rect.x) {
					Player->isMoving = false;
					Player->point.x--;
					Player->rect.x = map[Player->point.x][Player->point.y].rect.x;
				}
				break;
			case DOWN:
				Player->rect.y += playerMoveSpeed;
				if (Player->rect.y >= map[Player->point.x][Player->point.y + 1].rect.y) {
					Player->isMoving = false;
					score--;
					Player->point.y++;
					Player->rect.y = map[Player->point.x][Player->point.y].rect.y;
				}
				break;
			}
		}

		if (GE::event.type == GE_KEYDOWN && !Player->isMoving) {
			switch (GE_Keyboard) {
			case GEK_UP:
				if (map[Player->point.x][Player->point.y - 1].type != TREE) {
					Player->isMoving = true;
					Player->direction = UP;
					//Player->rect.y -= playerMoveSpeed;
				}
				break;
			case GEK_RIGHT:
				if (Player->point.x + 1 < columns && map[Player->point.x + 1][Player->point.y].type != TREE) {
					Player->isMoving = true;
					Player->direction = RIGHT;
					//Player->rect.x += playerMoveSpeed;
				}
				break;
			case GEK_LEFT:
				if (Player->point.x - 1 >= 0 && map[Player->point.x - 1][Player->point.y].type != TREE) {
					Player->isMoving = true;
					Player->direction = LEFT;
					//Player->rect.x -= playerMoveSpeed;
				}
				break;
			case GEK_DOWN:
				if (Player->point.y + 1 < rows && map[Player->point.x][Player->point.y + 1].type != TREE) {
					Player->isMoving = true;
					Player->direction = DOWN;
					//Player->rect.y += playerMoveSpeed;
				}
				break;
			}
		}

	}
};
class PlayGameScreen : public ScreenController {
public:
	PlayerEntity* player;
	EGameController* state;
	MapBuilder* map;

	PlayGameScreen(PlayerEntity* _player, EGameController* _state) {
		this->player = _player;
		this->state = _state;

		map = new MapBuilder(_player);
		map->state = _state;
	}
	void handleMove() {
		map->HandleMove();
	}
	void Update() {
		map->update();
	}
	void Render() {
		map->draw();
		this->player->Render();
	}
protected:
};
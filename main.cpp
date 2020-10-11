#include "crispyOctoSporkEngine.h"
#include <vector>
using namespace CrispyOctoSpork;

enum class GameState
{
	MENU,
	PLAYING,
	LOSE,
	PAUSE
};

/// <summary>
/// Struct for the apple that the snake wants.
/// </summary>
struct Apple
{
	float x = 0;
	float y = 0;
	Texture* texture = NULL;
};

/// <summary>
/// Struct for a peice of the snakes tail.
/// </summary>
struct Tail
{
	float x = 0;
	float y = 0;
};

/// <summary>
/// Struct for the snakey boi itself.
/// </summary>
struct Snake
{
	float xVelocity = 0;
	float yVelocity = 0;
	std::vector<Tail> tail;
	int tailLength = 0;
	Texture* texture = NULL;
};

/// <summary>
/// Struct for handling score.
/// </summary>
struct Score
{
	Texture* texture = NULL;
	int score = 0;
};

/// <summary>
/// A basic snake game written with SDL2 and the CrispyOctoSporkEngine.
/// </summary>
class SnakeGame : public Engine
{
public:
	const int WIDTH = 640;
	const int HEIGHT = 480;
private:
	const float GRID_SIZE = 32.0;
	const int GRID_WIDTH = WIDTH / GRID_SIZE;
	const int GRID_HEIGHT = HEIGHT / GRID_SIZE;
	GameState state = GameState::MENU;

	float movesPerSecond = 10.0;
	int movesPerformedThisSecond = 0;
	float secondAccumulator = 0;

	Snake* snake = NULL;
	Apple* apple = NULL;
	Score* score = NULL;
	Texture* menu = NULL;
	Texture* lose = NULL;

	SoundEffect* nice = NULL;

	bool OnCreate() override
	{
		snake = new Snake();
		apple = new Apple();
		score = new Score();

		snake->texture = new Texture(renderer);
		apple->texture = new Texture(renderer);
		score->texture = new Texture(renderer, "assets/coder-crux.ttf", 28);
		menu = new Texture(renderer);
		lose = new Texture(renderer);

		snake->texture->LoadTextureFromFile("assets/snake.png");
		apple->texture->LoadTextureFromFile("assets/apple.png");
		menu->LoadTextureFromFile("assets/menu.png");
		lose->LoadTextureFromFile("assets/lose.png");

		nice = new SoundEffect();

		nice->LoadSoundFromFile("assets/nice.wav");

		return true;
	}

	bool InitPlayingState()
	{
		float startingX = 8.0;
		float startingY = 8.0;
		snake->xVelocity = 1.0;
		snake->yVelocity = 0;
		snake->tailLength = 6;
		snake->tail.clear();

		for (int i = 0; i < snake->tailLength; i++)
		{
			snake->tail.push_back({ startingX + (-snake->xVelocity * i), startingY });
		}

		apple->x = rand() % GRID_WIDTH;
		apple->y = rand() % GRID_HEIGHT;

		score->score = 0;
		score->texture->LoadFromRenderedText("Score: " + std::to_string(score->score), COLOR_WHITE);

		return true;
	}

	bool OnEvent(SDL_Event event) override
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		switch (state)
		{
		case GameState::MENU:
			OnEventMenu(currentKeyStates);
			break;
		case GameState::PLAYING:
			OnEventPlaying(currentKeyStates);
			break;
		case GameState::LOSE:
			OnEventMenu(currentKeyStates);
			break;
		}

		return true;
	}

	bool OnEventMenu(const Uint8* currentKeyStates)
	{
		if (currentKeyStates[SDL_SCANCODE_SPACE])
		{
			state = GameState::PLAYING;
			InitPlayingState();
		}

		return true;
	}

	bool OnEventPlaying(const Uint8* currentKeyStates)
	{
		if (currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP])
		{
			if (snake->yVelocity != 1.0) 
			{
				snake->yVelocity = -1.0;
				snake->xVelocity = 0;
			}			
		}
		else if (currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN])
		{
			if (snake->yVelocity != -1.0) 
			{
				snake->yVelocity = 1.0;
				snake->xVelocity = 0;
			}			
		}
		else if (currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT])
		{
			if (snake->xVelocity != 1.0) 
			{
				snake->yVelocity = 0;
				snake->xVelocity = -1.0;
			}			
		}
		else if (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			if (snake->xVelocity != -1.0)
			{
				snake->yVelocity = 0;
				snake->xVelocity = 1.0;
			}			
		}
		else if (currentKeyStates[SDL_SCANCODE_ESCAPE])
		{
			state = GameState::MENU;
		}

		return true;
	}

	bool OnUpdate(float deltaTime) override
	{
		switch (state)
		{
		case GameState::MENU:
			OnUpdateMenu(deltaTime);
			break;
		case GameState::PLAYING:
			OnUpdatePlaying(deltaTime);
			break;
		case GameState::LOSE:
			OnUpdateLose(deltaTime);
			break;
		}

		return true;
	}

	bool OnUpdateMenu(float deltaTime)
	{
		menu->Render(0, 0);

		return true;
	}

	bool OnUpdatePlaying(float deltaTime) 
	{
		bool moveThisFrameUpdate = false;
		secondAccumulator += deltaTime;

		if (secondAccumulator >= 1000.0)
		{
			secondAccumulator = 0;
			movesPerformedThisSecond = 0;
		}

		if (movesPerformedThisSecond < (secondAccumulator * movesPerSecond / 1000.0f)) 
		{
			moveThisFrameUpdate = true;
			movesPerformedThisSecond++;
		}

		if (moveThisFrameUpdate)
		{
			float newHeadX = snake->tail[0].x + snake->xVelocity;
			float newHeadY = snake->tail[0].y + snake->yVelocity;

			if (newHeadX > GRID_WIDTH - 1)
			{
				newHeadX = 0;
			}

			if (newHeadX < 0)
			{
				newHeadX = GRID_WIDTH - 1;
			}

			if (newHeadY > GRID_HEIGHT - 1)
			{
				newHeadY = 0;
			}

			if (newHeadY < 0)
			{
				newHeadY = GRID_HEIGHT - 1;
			}

			for (int i = 0; i < snake->tailLength; i++)
			{
				if ((snake->tail[i].x == newHeadX) && (snake->tail[i].y == newHeadY))
				{
					state = GameState::LOSE;
				}
			}

			Tail head = { newHeadX, newHeadY};
			snake->tail.insert(snake->tail.begin(), head);

			if ((snake->tail[0].x == apple->x) && (snake->tail[0].y == apple->y))
			{
				//nice->PlaySound();
				score->score++;
				score->texture->LoadFromRenderedText("Score: " + std::to_string(score->score), COLOR_WHITE);

				bool validAppleSpawn = false;

				while (!validAppleSpawn)
				{
					apple->x = rand() % GRID_WIDTH;
					apple->y = rand() % GRID_HEIGHT;

					validAppleSpawn = true;
					for (int i = 0; i < snake->tailLength; i++)
					{
						if ((snake->tail[i].x == apple->x) && (snake->tail[i].y == apple->y))
						{
							validAppleSpawn = false;
						}
					}
				}

				snake->tailLength++;
			} 
			else
			{
				snake->tail.pop_back();
			}
		}

		apple->texture->Render(apple->x * GRID_SIZE, apple->y * GRID_SIZE);

		for (int i = 0; i < snake->tailLength; i++)
		{
			snake->texture->Render(snake->tail[i].x * GRID_SIZE, snake->tail[i].y * GRID_SIZE);
		}

		score->texture->Render(10, 10);

		return true;
	}

	bool OnUpdateLose(float deltaTime)
	{
		lose->Render(0, 0);
		score->texture->Render(WIDTH / 2 - score->texture->width / 2, HEIGHT / 2 - score->texture->height / 2);

		return true;
	}

	bool OnDestroy() override
	{
		// clean up textures
		snake->texture->Free();
		apple->texture->Free();
		score->texture->Free();
		lose->Free();

		nice->Free();

		// free the pointers
		delete snake->texture;
		delete apple->texture;
		delete score->texture;

		// kill all the objects
		delete snake;
		delete apple;
		delete menu;
		delete score;
		delete lose;
		
		delete nice;

		return true;
	}
};


/// <summary>
/// The main entry point of your game.
/// </summary>
/// <param name="argc">The number of arguments.</param>
/// <param name="argv">A char array of arguments provided.</param>
/// <returns>Returns an integer indicating exit status.</returns>
int main(int argc, char* argv[])
{
	SnakeGame game;

	// Create a new instance of your game. If successful, then start the main loop.
	if (game.Create("Snake", game.WIDTH, game.HEIGHT, false, false))
	{
		game.Start();
	}

	return 0;
}
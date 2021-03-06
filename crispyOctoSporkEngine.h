#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_TTF.h>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

namespace CrispyOctoSpork
{
	/// <summary>
	/// Base class for objects that should be updated and rendered.
	/// </summary>
	class Entity
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Entity();

		/// <summary>
		/// Constructs a new instance of <see cref="Entity"/>.
		/// </summary>
		/// <param name="x">The x location of the entity in the engine.</param>
		/// <param name="y">The y location of the entity in the engine.</param>
		Entity(float x, float y);

		/// <summary>
		/// Default deconstructor.
		/// </summary>
		~Entity();

		/// <summary>
		/// Called once a frame to be overridden by derived classes.
		/// </summary>
		/// <param name="deltaTime">The delta time since the last frame.</param>
		/// <returns>Returns a boolean.</returns>
		virtual bool OnUpdate(float deltaTime);

		/// <summary>
		/// Called once a frame to be overridden by derived classes.
		/// </summary>
		/// <param name="deltaTime">The delta time since the last frame.</param>
		/// <returns>Returns a boolean.</returns>
		virtual bool OnRender(float deltaTime);

	protected:
		float x;
		float y;
	};

	/// <summary>
	/// A few SDL_Colors to use.
	/// </summary>
	SDL_Color COLOR_RED = { 255, 0, 0, 255 }, COLOR_GREEN = { 0, 255, 0, 255 }, COLOR_BLUE = { 0, 0, 255, 255 }, COLOR_WHITE = {255, 255, 255, 255};

	/// <summary>
	/// Class for calculating and storing the current frame rate.
	/// </summary>
	class FrameRate
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		FrameRate();

		/// <summary>
		/// Gets the current frame rate.
		/// </summary>
		/// <returns>Returns an int indicating the number of frames that occurd in the last second.</returns>
		int GetCurrentFramesPerSecond();

		/// <summary>
		/// Called once per frame in order to update the frame rate.
		/// </summary>
		/// <returns>Returns 0 or the current framerate if there is an update (once per second.)</returns>
		int OnUpdate();

	private:
		Uint32 timeStampOfBeginingOfSecond;
		int currentSecondsFrameCount;
		int currentFramesPerSecond;
	};

	/// <summary>
	/// The main class that games should create an instance of.
	/// </summary>
	class Engine
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Engine();

		/// <summary>
		/// Overridable deconstructor.
		/// </summary>
		virtual ~Engine();

		/// <summary>
		/// First thing to call after creating an instance of <see cref="Engine"/>.
		/// </summary>
		/// <param name="name">The name of the game, will be displayed in the title bar.</param>
		/// <param name="width">The width of the screen in pixels.</param>
		/// <param name="height">The height of the screen in pixels.</param>
		/// <param name="vsync">Indicates if the renderer should be created with vsync on.</param>
		/// <param name="fullscreen">Indicates if the renderer should be created with fullscreen set.</param>
		/// <returns>Returns a boolean indicating if creation was successful.</returns>
		bool Create(std::string name, int width, int height, bool vsync, bool fullscreen);

		/// <summary>
		/// Called to start the game engines main game loop.
		/// </summary>
		/// <returns>Returns a boolean indicating if there was an issue.</returns>
		bool Start();

		/// <summary>
		/// Called once at engine startup.
		/// </summary>
		/// <returns>Returns a boolean.</returns>
		virtual bool OnCreate();

		/// <summary>
		/// Called once per frame after the engine is started.
		/// </summary>
		/// <param name="deltaTime">The time delta from the previous frame.</param>
		/// <returns>Returns a boolean indicating if the engine should continue running.</returns>
		virtual bool OnUpdate(float deltaTime);

		/// <summary>
		/// Called once per frame after update to render stuff
		/// </summary>
		/// <param name="deltaTime">The time delta from the previous frame.</param>
		/// <returns>Returns a boolean indicating if the engine should continue running.</returns>
		virtual bool OnRender(float deltaTime);

		virtual bool OnEvent(SDL_Event event);

		/// <summary>
		/// Called before the engine terminates.
		/// </summary>
		/// <returns>Returns a boolean.</returns>
		virtual bool OnDestroy();

		/// <summary>
		/// Adds an <see cref="Entity"/> to the engines vector of entites. Will be cleaned up by the engine.
		/// </summary>
		/// <param name="entity">The entity pointer to add to the vector.</param>
		void AddEntity(Entity* entity);

		/// <summary>
		/// Draws a qaud with an optional rotation.
		/// Realized this is kinda useless right now since SDL doesn't have a way
		/// to actually fill in the geometry with a color.
		/// </summary>
		/// <param name="points">The array of points to draw.</param>
		/// <param name="color">The color to make the quad</param>
		/// <param name="rotation">The angle to rotate the quad in radians.</param>
		void DrawQuad(SDL_FPoint* points, SDL_Color color, float rotation = 0.0);

	protected:
		SDL_Window* window;
		SDL_Renderer* renderer;
		int screenWidth;
		int screenHeight;
		bool isVsyncEnabled;
		bool isFullscreenEnabled;
		std::string name;
		bool isEngineRunning;
		std::vector <Entity*> entities;
		float lastFrameTime;
		FrameRate frameRate;

		/// <summary>
		/// The main loop. To support emscripten the current <see cref="Engine"/> instance is passed in.
		/// </summary>
		/// <param name="arg">A pointer to the current <see cref="Engine"/> instance.</param>
		static void Update(void* arg);
	};

	class SoundEffect
	{
	public:
		SoundEffect();
		~SoundEffect();
		bool LoadSoundFromFile(const char* filepath);
		bool PlaySound();
		void Free();
	private:
		Mix_Chunk* mixChunk;
	};

	/// <summary>
	/// An object for holding texture information and to perform basic renders of it.
	/// </summary>
	class Texture
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Texture();

		/// <summary>
		/// Creates a new instance of <see cref="Texture"/>.
		/// </summary>
		Texture(SDL_Renderer* renderer, const char* fontFilePath = NULL, int fontSize = 0);

		/// <summary>
		/// Default deconstructor.
		/// </summary>
		~Texture();

		/// <summary>
		/// Populates the <see cref="SDL_Texture"/> from a file.
		/// </summary>
		/// <param name="filepath">The file path to load an image from for the texture.</param>
		/// <returns>Returns a boolean indicating success.</returns>
		bool LoadTextureFromFile(const char* filepath);

		/// <summary>
		/// Populates the <see cref="SDL_Texture"/> from a rendered text string.
		/// </summary>
		/// <param name="text">The text to display.</param>
		/// <param name="textColor">The color of the text.</param>
		/// <returns>Returns a boolean indicating success.</returns>
		bool LoadFromRenderedText(std::string text, SDL_Color textColor);

		/// <summary>
		/// Called to cleanup and free the texture.
		/// </summary>
		void Free();

		/// <summary>
		/// Renders the texture to the current <see cref="SDL_Renderer"/>.
		/// </summary>
		/// <param name="x">The x location to draw to.</param>
		/// <param name="y">The y location to draw to.</param>
		/// <param name="clip">The rectangle to pull texture information from. Useful for sprite sheets.</param>
		/// <param name="angle">The angle at which to rotate the texture.</param>
		/// <param name="center">The center to rotate about.</param>
		/// <param name="flip">Determines if the texture will be flipped.</param>
		void Render(float x, float y, SDL_Rect* clip = NULL, float angle = 0.0, SDL_FPoint* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, int alpha = 255);

		/// <summary>
		/// Gets the current renderer associated with this texture.
		/// </summary>
		/// <returns>Returns a pointer to a <see cref="SDL_Renderer"/>.</returns>
		SDL_Renderer* GetRenderer();

		int width;
		int height;

	private:
		SDL_Texture* texture;
		SDL_Renderer* renderer;
		TTF_Font* font;
		int fontSize;
	};

	/// <summary>
	/// Sprite class to hold an entity and a texture.
	/// </summary>
	class Sprite : public Entity
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Sprite();

		/// <summary>
		/// Creates a new instance of <see cref="Sprite"/>.
		/// </summary>
		/// <param name="x">The x location of the sprite in the engine.</param>
		/// <param name="y">The y location of the sprite in the engine.</param>
		/// <param name="width">The width of the sprite.</param>
		/// <param name="height">The height of the sprite.</param>
		/// <param name="texture">A pointer to the texture to use for the sprite.</param>
		/// <param name="renderer">A pointer to the renderer to render the sprite to.</param>
		Sprite(float x, float y, float width, float height, Texture* texture, SDL_Renderer* renderer);

		/// <summary>
		/// Called once per frame. Renders the sprite to its current location.
		/// </summary>
		/// <param name="deltaTime">The delta time since the last frame.</param>
		/// <returns>Returns a boolean indicating success.</returns>
		virtual bool OnRender(float deltaTime);

	protected:
		float width;
		float height;
		Texture* texture;
		SDL_Renderer* renderer;

	};

	/// <summary>
	/// Class to hold an entity represented by a rectangle.
	/// </summary>
	class Rectangle : public Entity
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Rectangle();

		/// <summary>
		/// Creates a new instance of <see cref="Rectangle"/>.
		/// </summary>
		/// <param name="x">The x location of the rectangle in the engine.</param>
		/// <param name="y">The y location of the rectangle in the engine.</param>
		/// <param name="width">The width of the rectangle.</param>
		/// <param name="height">The height of the rectangle.</param>
		/// <param name="color">The color to draw the rectangle with.</param>
		/// <param name="renderer">A pointer to the renderer to render the rectangle to.</param>
		Rectangle(float x, float y, float width, float height, SDL_Color color, SDL_Renderer* renderer);

		/// <summary>
		/// Called once per frame. Renders the rectangle to its current location.
		/// </summary>
		/// <param name="deltaTime">The delta time since the last frame.</param>
		/// <returns>Returns a boolean indicating success.</returns>
		virtual bool OnRender(float deltaTime);

	protected:
		float width;
		float height;
		SDL_Color color;
		SDL_Renderer* renderer;
	};

	/// <summary>
	/// Class to hold an entity represented by a circle.
	/// </summary>
	class Circle : public Entity
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Circle();

		/// <summary>
		/// Creates a new instance of <see cref="Circle"/>.
		/// </summary>
		/// <param name="x">The x location of the center of the circle.</param>
		/// <param name="y">The y location of the center of the circle.</param>
		/// <param name="radius">The radius of the circle</param>
		/// <param name="color">The color to draw the circle with.</param>
		/// <param name="renderer">A pointer to the renderer to render the circle to.</param>
		Circle(float x, float y, float radius, SDL_Color color, SDL_Renderer* renderer);

		/// <summary>
		/// Called once per frame. Renders the circle to its current location.
		/// </summary>
		/// <param name="deltaTime">The delta time since the last frame.</param>
		/// <returns>Returns a boolean indicating success.</returns>
		virtual bool OnRender(float deltaTime);

	protected:
		float radius;
		SDL_Color color;
		SDL_Renderer* renderer;
	};

	/// <summary>
	/// Struct used to contain a single particle of a <see cref="ParticleEmitter"/>.
	/// </summary>
	struct Particle
	{
		float x = 0;
		float y = 0;
		float xVelocity = 0;
		float yVelocity = 0;
		float totalLifeTime = 0;
		float lifeTimeRemaining = 0;
		bool active = false;
	};

	class ParticleEmitter
	{
	public:
		ParticleEmitter();
		~ParticleEmitter();
		ParticleEmitter(float x, float y, float lifeInSeconds, Texture* texture, float speed = 0.5, int newParticlesPerSecond = 5, float startSizeMultiplier = 1.0, float endSizeMultiplier = 0.0, int maxParticles = 20000);
		void OnUpdate(float deltaTime);
		void OnRender();
		bool active;
		int newParticlesPerSecond;
		float startOfSecond;
		float particlesCreatedThisSecond;
	private:
		float x;
		float y;
		float lifeInMiliseconds;
		Texture* texture;
		float startSizeMultiplier = 1.0;
		float endSizeMultiplier = 1.0;
		float speed;
		Particle* particlePool;
		int currentParticlePoolIndex = 0;
		int maxParticles;
	};

	Engine::Engine()
	{
		SDL_Init(SDL_INIT_VIDEO);

		window = NULL;
		renderer = NULL;
		screenWidth = 0;
		screenHeight = 0;
		isVsyncEnabled = true;
		isFullscreenEnabled = false;
		name = "";
		isEngineRunning = false;
	}

	Engine::~Engine()
	{}

	bool Engine::Create(std::string name, int width, int height, bool vsync, bool fullscreen)
	{
		this->name = name;
		this->screenWidth = width;
		this->screenHeight = height;
		this->isVsyncEnabled = vsync;
		this->isFullscreenEnabled = fullscreen;

		Uint32 windowFlags = SDL_WINDOW_SHOWN;

		if (this->isFullscreenEnabled)
		{
			windowFlags |= SDL_WINDOW_FULLSCREEN;
		}

		this->window = SDL_CreateWindow(this->name.c_str(), 
										SDL_WINDOWPOS_UNDEFINED, 
										SDL_WINDOWPOS_UNDEFINED, 
										this->screenWidth, 
										this->screenHeight, 
										windowFlags);

		if (this->window == NULL) 
		{
			std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
			return false;
		}

		Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;

		if (this->isVsyncEnabled)
		{
			rendererFlags |= SDL_RENDERER_PRESENTVSYNC;
		}

		this->renderer = SDL_CreateRenderer(window, -1, rendererFlags);

		if (renderer == NULL)
		{
			std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
			return false;
		}

		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			std::cout << "Failed to create renderer: " << IMG_GetError() << std::endl;
			return false;
		}

		if (TTF_Init() == -1)
		{
			std::cout << "SDL_ttf could not initialize!: " << TTF_GetError() << std::endl;
			return false;
		}

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cout << "Failed to start the mixer: " << Mix_GetError() << std::endl;
			return false;
		}

		return true;
	}

	bool Engine::Start()
	{
		isEngineRunning = true;

		this->OnCreate();

		#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(Engine::Update, this, -1, 1);
		#else
		while (isEngineRunning)
		{
			Update(this);
		}
		#endif

		OnDestroy();
		SDL_Quit();
		IMG_Quit();
		return true;
	}


	void Engine::Update(void* arg)
	{
		Engine* engine = (Engine*)arg;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				engine->isEngineRunning = false;
				break;
			}

			engine->OnEvent(event);
		}

		Uint32 currentFrameTime = SDL_GetTicks();
		float deltaTime = currentFrameTime - engine->lastFrameTime;
		engine->lastFrameTime = currentFrameTime;

		SDL_SetRenderDrawColor(engine->renderer, 135, 206, 235, 255);
		SDL_RenderClear(engine->renderer);

		engine->OnUpdate(deltaTime);
		SDL_RenderPresent(engine->renderer);

		if (engine->frameRate.OnUpdate() != 0.0)
		{
			std::string newWindowTitle = engine->name + " - " + std::to_string(engine->frameRate.GetCurrentFramesPerSecond()) + " FPS - " + std::to_string(deltaTime);
			SDL_SetWindowTitle(engine->window, newWindowTitle.c_str());
		}
	}

	bool Engine::OnCreate()
	{
		lastFrameTime = SDL_GetTicks();
		return true;
	}

	bool Engine::OnUpdate(float deltaTime)
	{
		return true;
	}

	bool Engine::OnRender(float deltaTime)
	{
		for (auto& entity : entities)
		{
			entity->OnRender(deltaTime);
		}

		return true;
	}

	bool Engine::OnEvent(SDL_Event event)
	{
		return true;
	}

	bool Engine::OnDestroy()
	{
		for (auto p : entities)
		{
			delete p;
		}

		entities.clear();
		return true;
	}

	void Engine::AddEntity(Entity* entity)
	{
		entities.push_back(entity);
	}

	void Engine::DrawQuad(SDL_FPoint* points, SDL_Color color, float rotation)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

		// if we have a rotation then let's apply the rotation matrix.
		if (rotation == 0.0)
		{
			SDL_RenderDrawLinesF(renderer, points, 4);
			return;
		}

		SDL_FPoint rotatedPoints[4];

		for (int i = 0; i < 3; i++)
		{
			rotatedPoints[i].x = (points[i].x * cosf(rotation)) - (points[i].y * sinf(rotation));
			rotatedPoints[i].y = (points[i].x * sinf(rotation)) + (points[i].y * cosf(rotation));
		}

		SDL_RenderDrawLinesF(renderer, rotatedPoints, 4);
	}

	FrameRate::FrameRate()
	{
		timeStampOfBeginingOfSecond = SDL_GetTicks();
		currentFramesPerSecond = 0.0;
		currentSecondsFrameCount = 0;
	}

	int FrameRate::GetCurrentFramesPerSecond()
	{
		return currentFramesPerSecond;
	}

	int FrameRate::OnUpdate() 
	{
		Uint32 currentFrameTime = SDL_GetTicks();

		if (currentFrameTime - timeStampOfBeginingOfSecond >= 1000.0)
		{
			currentSecondsFrameCount++;
			currentFramesPerSecond = currentSecondsFrameCount;
			currentSecondsFrameCount = 0;
			timeStampOfBeginingOfSecond = currentFrameTime;
			return currentFramesPerSecond;
		}

		currentSecondsFrameCount++;
		return 0;
	}

	Entity::Entity()
	{
		x = 0;
		y = 0;
	}

	Entity::Entity(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Sprite::Sprite()
	{
		this->width = 0;
		this->height = 0;
		this->texture = NULL;
		this->renderer = NULL;
	}

	Sprite::Sprite(float x, float y, float width, float height, Texture* texture, SDL_Renderer* renderer) : Entity(x, y)
	{
		this->width = width;
		this->height = height;
		this->texture = texture;
		this->renderer = renderer;
	}

	bool Sprite::OnRender(float deltaTime)
	{
		texture->Render(x, y);

		return true;
	}

	Rectangle::Rectangle()
	{
		this->width = 0;
		this->height = 0;
		this->color = SDL_Color{255, 255, 255, 255};
		this->renderer = NULL;
	}

	Rectangle::Rectangle(float x, float y, float width, float height, SDL_Color color, SDL_Renderer* renderer) : Entity(x, y)
	{
		this->width = width;
		this->height = height;
		this->color = color;
		this->renderer = renderer;
	}

	bool Rectangle::OnRender(float deltaTime)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.b, color.g, color.a);
		SDL_FRect rect = { x, y, width, height };
		SDL_RenderFillRectF(renderer, &rect);
		return false;
	}

	Circle::Circle()
	{
		this->radius = 0;
		this->color = SDL_Color{ 255, 255, 255, 255 };
		this->renderer = NULL;
	}

	Circle::Circle(float x, float y, float radius, SDL_Color color, SDL_Renderer* renderer) : Entity(x, y)
	{
		this->radius = radius;
		this->color = color;
		this->renderer = renderer;
	}

	bool Circle::OnRender(float deltaTime)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.b, color.g, color.a);
		for (int w = 0; w < radius * 2; w++)
		{
			for (int h = 0; h < radius * 2; h++)
			{
				int dx = radius - w; // horizontal offset
				int dy = radius - h; // vertical offset
				if ((dx * dx + dy * dy) <= (radius * radius))
				{
					SDL_RenderDrawPoint(renderer, x + dx, y + dy);
				}
			}
		}
		return false;
	}

	Entity::~Entity()
	{
	}

	bool Entity::OnUpdate(float deltaTime)
	{
		return true;
	}

	bool Entity::OnRender(float deltaTime)
	{
		return true;
	}

	SoundEffect::SoundEffect()
	{
		mixChunk = NULL;
	}

	SoundEffect::~SoundEffect()
	{
		Free();
	}

	bool SoundEffect::LoadSoundFromFile(const char* filepath)
	{
		mixChunk = Mix_LoadWAV(filepath);
		if (mixChunk == NULL)
		{
			printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			return false;
		}

		return true;
	}

	void SoundEffect::Free()
	{
		if (mixChunk != NULL) 
		{
			Mix_FreeChunk(mixChunk);
		}

		mixChunk = NULL;		
	}

	bool SoundEffect::PlaySound()
	{
		if (mixChunk == NULL) 
		{
			return false;
		}

		Mix_PlayChannel(-1, mixChunk, 0);
	}

	Texture::Texture()
	{
		this->texture = NULL;
		this->renderer = NULL;
		this->width = 0;
		this->height = 0;
		this->font = NULL;
	}

	Texture::Texture(SDL_Renderer* renderer, const char* fontFilePath, int fontSize)
	{
		this->texture = NULL;
		this->width = 0;
		this->height = 0;
		this->renderer = renderer;
		this->fontSize = fontSize;
		this->font = NULL;

		if (fontFilePath != NULL) 
		{
			font = TTF_OpenFont(fontFilePath, fontSize);
			if (font == NULL)
			{
				std::cout << "Could not load the font" << TTF_GetError() << std::endl;
			}
		}
	}

	Texture::~Texture()
	{
		Free();

		if (font != NULL) 
		{
			TTF_CloseFont(font);
		}
	}

	bool Texture::LoadTextureFromFile(const char* filepath)
	{
		Free();

		texture = IMG_LoadTexture(renderer, filepath);

		if (texture == NULL)
		{
			std::cout << "Could not load the texture from: " << filepath << " Error:" << SDL_GetError() << std::endl;
			return false;
		}

		SDL_QueryTexture(texture, NULL, NULL, &width, &height);

		return true;
	}

	inline bool Texture::LoadFromRenderedText(std::string text, SDL_Color textColor)
	{
		Free();

		if (font == NULL)
		{
			std::cout << "No font associated with this texture! Make sure you construct it with one." << std::endl;
			return false;
		}

		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);

		if (textSurface == NULL)
		{
			std::cout << "There was an error rendering the text surface." << TTF_GetError() << std::endl;
			return false;
		}

		texture = SDL_CreateTextureFromSurface(renderer, textSurface);

		if (texture == NULL)
		{
			std::cout << "There was an error creating the texture" << SDL_GetError() << std::endl;
			return false;
		}

		width = textSurface->w;
		height = textSurface->h;

		SDL_FreeSurface(textSurface);

		return true;
	}

	void Texture::Free()
	{
		if (texture != NULL)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
			width = 0;
			height = 0;
		}
	}

	void Texture::Render(float x, float y, SDL_Rect* clip, float angle, SDL_FPoint* center, SDL_RendererFlip flip, int alpha)
	{
		SDL_FRect renderQuad = { x, y, (float) width, (float) height };

		if (clip != NULL)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		SDL_SetTextureAlphaMod(texture, alpha);
		SDL_RenderCopyExF(renderer, texture, clip, &renderQuad, angle, center, flip);
	}
	SDL_Renderer* Texture::GetRenderer()
	{
		return renderer;
	}

	ParticleEmitter::ParticleEmitter()
	{
		this->x = 0;
		this->y = 0;
		this->lifeInMiliseconds = 0;
		this->speed = 0;
		this->texture = NULL;
		this->startSizeMultiplier = 0;
		this->endSizeMultiplier = 0;
		this->maxParticles = 0;
		this->currentParticlePoolIndex = 0;
		this->newParticlesPerSecond = 0;
		this->startOfSecond = 0;
		this->particlesCreatedThisSecond = 0;
		this->active = false;
		this->maxParticles = 0;
		this->particlePool = NULL;
	}

	ParticleEmitter::~ParticleEmitter()
	{
		delete[] this->particlePool;
	}

	ParticleEmitter::ParticleEmitter(float x, float y, float lifeInMiliseconds, Texture* texture, float speed, int newParticlesPerSecond, float startSizeMultiplier, float endSizeMultiplier, int maxParticles)
	{
		this->x = x;
		this->y = y;
		this->lifeInMiliseconds = lifeInMiliseconds;
		this->speed = speed;
		this->texture = texture;
		this->startSizeMultiplier = startSizeMultiplier;
		this->endSizeMultiplier = endSizeMultiplier;
		this->maxParticles = maxParticles;
		this->currentParticlePoolIndex = 0;
		this->newParticlesPerSecond = newParticlesPerSecond;
		this->startOfSecond = SDL_GetTicks();
		this->particlesCreatedThisSecond = 0;
		this->active = true;
		this->particlePool = new Particle[maxParticles];
	}

	void ParticleEmitter::OnUpdate(float deltaTime)
	{
		if (!active)
		{
			return;
		}

		// first get the amount of time since the begining of the second
		// and determine if we've created enough particles at this point yet.
		float currentTime = SDL_GetTicks();

		float elapsedTimeInSecond = currentTime - startOfSecond;
		int particlesThatShouldHaveBeenCreatedThisSecond = (elapsedTimeInSecond * newParticlesPerSecond) / 1000;

		if (particlesThatShouldHaveBeenCreatedThisSecond > particlesCreatedThisSecond)
		{
			int particlesToCreate = particlesThatShouldHaveBeenCreatedThisSecond - particlesCreatedThisSecond;
			for (int i = 0; i < particlesToCreate; i++)
			{
				Particle* particle = &particlePool[currentParticlePoolIndex];
				particle->active = true;
				particle->lifeTimeRemaining = lifeInMiliseconds;
				particle->totalLifeTime = lifeInMiliseconds;
				particle->x = x;
				particle->y = y;
				particle->xVelocity = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0 - -1.0)));
				particle->yVelocity = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0 - -1.0)));//-0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (-1.0 - -0.1)));

				currentParticlePoolIndex++;
				if (currentParticlePoolIndex > maxParticles - 2)
				{
					currentParticlePoolIndex = 0;
				}

				particlesCreatedThisSecond++;
			}
		}		

		// next let's update the particles
		for (int i = 0; i < maxParticles; i++)
		{
			Particle* particle = &particlePool[i];

			if (particle->active) 
			{
				particle->lifeTimeRemaining -= deltaTime;

				if (particle->lifeTimeRemaining <= 0.0)
				{
					particle->active = false;
					continue;
				}

				particle->x += particle->xVelocity * speed * deltaTime;
				particle->y += particle->yVelocity * speed * deltaTime;
			}
		}

		OnRender();
	}
	void ParticleEmitter::OnRender()
	{
		for (int i = 0; i < maxParticles; i++)
		{
			Particle* particle = &particlePool[i];

			if (particle->active) 
			{
				int cross = particle->lifeTimeRemaining * 255;
				int alpha = cross / particle->totalLifeTime;

				texture->Render(particle->x, particle->y, NULL, 0.0, NULL, SDL_FLIP_NONE, alpha);
			}			
		}
	}
}
#include <algorithm>
#include <sstream>
#include <iostream>

#include <irrklang/irrklang.h>

using namespace irrklang;

#include "game.h"
#include "resource_manager.h"

#include "sprite_renderer.h"
#include "game_object.h"
#include "ball_object.h"
#include "particle_generator.h"

#if 0
#include "post_processor.h"
#include "text_renderer.h"
#endif // 0


// Game-related state data

SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;
ParticleGenerator* Particles;

#if 0
PostProcessor* Effects;
ISoundEngine* SoundEngine = createIrrKlangDevice();
TextRenderer* Text;
#endif // 0

float ShakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
	:State(GAME_MENU), Keys(), KeysProcessed(), Width(width), Height(height), Level(0), Lives(3)
{

}

Game::~Game()
{

	delete Renderer;
	delete Player;
	delete Ball;
#if 0
	delete Particles;
	delete Effects;
	delete Text;
	SoundEngine->drop();
#endif // 0
}

void Game::Init()
{
	// load shaders
	ResourceManager::LoadShader("src/game/shaders/sprite.vert", "src/game/shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("src/game/shaders/particle.vert", "src/game/shaders/particle.frag", nullptr, "particle");
	ResourceManager::LoadShader("src/game/shaders/post_processing.vert", "src/game/shaders/post_processing.frag", nullptr, "postprocessing");

	//configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").Use().SetMatrix4("projection", projection);
	// load textures

	ResourceManager::LoadTexture("src/game/resources/textures/background.jpg", false, "background");
	ResourceManager::LoadTexture("src/game/resources/textures/awesomeface.png", true, "face");
	ResourceManager::LoadTexture("src/game/resources/textures/block.png", false, "block");
	ResourceManager::LoadTexture("src/game/resources/textures/block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("src/game/resources/textures/paddle.png", true, "paddle");
	ResourceManager::LoadTexture("src/game/resources/textures/particle.png", true, "particle");
	ResourceManager::LoadTexture("src/game/resources/textures/powerup_speed.png", true, "powerup_speed");
	ResourceManager::LoadTexture("src/game/resources/textures/powerup_sticky.png", true, "powerup_sticky");
	ResourceManager::LoadTexture("src/game/resources/textures/powerup_increase.png", true, "powerup_increase");
	ResourceManager::LoadTexture("src/game/resources/textures/powerup_confuse.png", true, "powerup_confuse");
	ResourceManager::LoadTexture("src/game/resources/textures/powerup_chaos.png", true, "powerup_chaos");
	ResourceManager::LoadTexture("src/game/resources/textures/powerup_passthrough.png", true, "powerup_passthrough");

	// set render-specific controls

	Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
#if 0
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("src/game/resources/fonts/OCRAEXT.TTF", 24);
#endif
	// load levels
#if 0
	GemeLevel one; one.Load("src/game/resources/levels/one.lvl", this->Width, this->Height / 2);
	GameLevel two; two.Load("src/game/resources/levels/two.lvl", this->Width, this->Height / 2);
	GameLevel three; three.Load("src/game/resources/levels/three.lvl", this->Width, this->Height / 2);
	GameLevel four; four.Load("src/game/resources/levels/four.lvl", this->Width, this->Height / 2);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;
#endif
	// configure game objects

	glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));

	// audio
#if 0
	SoundEngine->play2D("src/game/resources/audio/breakout.mp3", true);
#endif


}

void Game::Update(float dt)
{

}
void Game::ProcessInput(float dt)
{

}

void Game::Render()
{

}

void Game::ResetLevel()
{

}

void Game::ResetPlayer()
{
}

// powerups
// bool IsOtherPowerUpActive(std::vector<PowerUp>& powerup, std::string type);

void Game::UpdatePowerUps(float dt)
{

}
#if 0
void Game::SpawnPowerUps(GameObject& block)
{

}
#endif


void Game::DoCollisions()
{
}


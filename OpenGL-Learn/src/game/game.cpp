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
#include "post_processor.h"
#include "text_renderer.h"


// Game-related state data
SpriteRenderer*		Renderer;
GameObject*			Player;
BallObject*			Ball;
ParticleGenerator*	Particles;
PostProcessor*		Effects;
ISoundEngine*		SoundEngine = createIrrKlangDevice();
TextRenderer*		Text;

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
	delete Particles;
	delete Effects;
	delete Text;
	SoundEngine->drop();
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
	ResourceManager::GetShader("sprite").Use().SetMatrix("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").Use().SetMatrix("projection", projection);
	// load textures




}



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

SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;
ParticleGenerator* Particles;
PostProcessor* Effects;


ISoundEngine* SoundEngine = createIrrKlangDevice();
TextRenderer* Text;


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
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);

	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("src/game/resources/fonts/OCRAEXT.TTF", 24);

	// load levels
	GameLevel one; one.Load("src/game/resources/levels/one.lvl", this->Width, this->Height / 2);
	GameLevel two; two.Load("src/game/resources/levels/two.lvl", this->Width, this->Height / 2);
	GameLevel three; three.Load("src/game/resources/levels/three.lvl", this->Width, this->Height / 2);
	GameLevel four; four.Load("src/game/resources/levels/four.lvl", this->Width, this->Height / 2);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;


	// configure game objects
	glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));


	SoundEngine->play2D("src/game/resources/audio/breakout.mp3", true);


}

void Game::Update(float dt)
{
	// update objects
		Ball->Move(dt, this->Width);
	// check for collisions
	this->DoCollisions();
	// update particles
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));
	// update PowerUps
	this->UpdatePowerUps(dt);
	// reduce shake time
	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
			Effects->Shake = false;
	}
	// check loss condition
	if (Ball->Position.y >= this->Height) // did ball reach bottom edge?
	{
		--this->Lives;
		// did the player lose all his lives? : game over
		if (this->Lives == 0)
		{
			this->ResetLevel();
			this->State = GAME_MENU;
		}
		this->ResetPlayer();
	}
	// check win condition
	if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
	{
		this->ResetLevel();
		this->ResetPlayer();
		Effects->Chaos = true;
		this->State = GAME_WIN;
	}

}
void Game::ProcessInput(float dt)
{
	if (this->State == GAME_MENU)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = true;
		}
		if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
		{
			this->Level = (this->Level + 1) % 4;
			this->KeysProcessed[GLFW_KEY_W] = true;
		}
		if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
		{
			if (this->Level > 0)
				--this->Level;
			else
				this->Level = 3;

			this->KeysProcessed[GLFW_KEY_S] = true;
		}
	}
	if (this->State == GAME_WIN)
	{
		if (this->Keys[GLFW_KEY_ENTER]) {
			this->KeysProcessed[GLFW_KEY_ENTER] = true;
			Effects->Chaos = false;
			this->State = GAME_MENU;
		}
	}
	if (this->State == GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		// move playerborard
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0.0f)
			{
				Player->Position.x -= velocity;
				if (Ball->Stuck)
					Ball->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;
				if (Ball->Stuck)
					Ball->Position.x += velocity;
			}
		}

		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = false;
	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN)
	{
	
		//begin rendering to postprocessing framebuffer
		Effects->BeginRender();

		// draw background
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
		
		// draw level
		this->Levels[this->Level].Draw(*Renderer);

		// draw player
		Player->Draw(*Renderer);
		// draw PowerUps
		for (PowerUp& powerUp : this->PowerUps)
			if (!powerUp.Destroyed)
				powerUp.Draw(*Renderer);
		// draw particles	
		Particles->Draw();
		// draw ball
		Ball->Draw(*Renderer);

		// end rendering to postprocessing framebuffer
		Effects->EndRender();
		Effects->Render(glfwGetTime());

		// render text (don't include in postprocessing)
		std::stringstream ss; ss << this->Lives;
		Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}

	if (this->State == GAME_MENU)
	{
		Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f, 1.0f);
		Text->RenderText("Press W or S to select level", 245.0f, this->Height / 2.0f + 20.0f, 0.75f);
	}
	if (this->State == GAME_WIN)
	{
		Text->RenderText("You WON!!!", 320.0f, this->Height / 2.0f - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
	}
}

void Game::ResetLevel()
{

}

void Game::ResetPlayer()
{
}

// powerups
bool IsOtherPowerUpActive(std::vector<PowerUp>& powerup, std::string type);

void Game::UpdatePowerUps(float dt)
{

}

void Game::SpawnPowerUps(GameObject& block)
{

}

void ActivatePowerUp(PowerUp& powerUp)
{
	if (powerUp.Type == "speed")
	{
		Ball->Velocity *= 1.2;
	}
	else if (powerUp.Type == "sticky")
	{
		Ball->Sticky = true;
		Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == "pass-through")
	{
		Ball->PassThrough = true;
		Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == "pad-size-increase")
	{
		Player->Size.x += 50;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effects->Chaos)
			Effects->Confuse = true; // only activate if chaos wasn't already active
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effects->Confuse)
			Effects->Chaos = true;
	}
}


// collision detection
bool CheckCollision(GameObject&, GameObject&);
Collision CheckCollision(BallObject&, GameObject&);
Direction VectorDirection(glm::vec2 closest);


void Game::DoCollisions()
{
	for (GameObject& box : this->Levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			if (std::get<0>(collision)) // if collision is true
			{
				// destroy block if not solid
				if (!box.IsSolid)
				{
					box.Destroyed = true;
					this->SpawnPowerUps(box);
					SoundEngine->play2D("src/game/resources/audio/bleep.mp3", false);
				}
				else
				{
					// if block is solid, enable shake effect
					ShakeTime = 0.05f;
					Effects->Shake = true;
					SoundEngine->play2D("src/game/resources/audio/bleep.map3", false);
				}
				// collision reslution
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (!(Ball->PassThrough && !box.IsSolid)) // don't do collision resoution on non-solid bricks if pass-through is activated
				{
					if (dir == LEFT || dir == RIGHT) // horizontal collision
					{
						Ball->Velocity.x = -Ball->Velocity.x;  // reverse hrizontal velocity
						// relocate
						float penetration = Ball->Radius - std::abs(diff_vector.x);
						if (dir == LEFT)
							Ball->Position.x += penetration;
						else
							Ball->Position.x -= penetration;
					}
					else // vectical collision
					{
						Ball->Velocity.y = -Ball->Velocity.y; // reverse vertical velocity
						// relocae
						float penetration = Ball->Radius - std::abs(diff_vector.y);
						if (dir == UP)
							Ball->Position.y -= penetration;  // move ball back up
						else
							Ball->Position.y += penetration;  // move ball back down
					}
				}
			}
		}
	}

	// also check collisions on PowerUps an if so, activate them
	for (PowerUp& powerUp : this->PowerUps)
	{
		if (!powerUp.Destroyed)
		{
			// first check if powerup passed bottom edge, if so: keep as inactive and destroy
			if (powerUp.Position.y >= this->Height)
				powerUp.Destroyed = true;

			if (CheckCollision(*Player, powerUp))
			{
				// collided with player, now activate powerup
				ActivatePowerUp(powerUp);
				powerUp.Destroyed = true;
				powerUp.Activated = true;
				SoundEngine->play2D("src/game/resources/audio/powerup.wav", false);
			}
		}
	}

	// and finally check collisions for player pad (unless stuck)
	Collision result = CheckCollision(*Ball, *Player);
	if (!Ball->Stuck && std::get<0>(result))
	{
		// check where it hit the board, and change velocity based on where it hit the board
		float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
		float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
		float percentage = distance / (Player->Size.x / 2.0f);
		// then move accordingly
		float strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		//Ball->Velocity.y = -Ball->Velocity.y;
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
		// fix sticky paddle
		Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);

		// if Sticky powerup is activated, also stick ball to paddle once new velocity vectors were calculated
		Ball->Stuck = Ball->Sticky;

		SoundEngine->play2D("src/game/resources/audio/bleep.wav", false);
	}
}


bool CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
	// collision x-axis?
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// collision y-axis?
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// collision only if on both axes
	return collisionX && collisionY;
}

Collision CheckCollision(BallObject& one, GameObject& two) // AABB - Circle collision
{
	// get center point circle first 
	glm::vec2 center(one.Position + one.Radius);
	// calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
	glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
	// get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// now that we know the clamped values, add this to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// now retrieve vector between center circle and closest point AABB and check if length < radius
	difference = closest - center;

	if (glm::length(difference) < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
		return std::make_tuple(true, VectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

// calculates which direction a vector is facing (N,E,S or W)
Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

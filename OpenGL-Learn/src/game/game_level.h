#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

/// GameLevel holds all Tiles as part of a Breakout leval and
/// hosts functionality to Load/render levels from the harddisk
class GameLevel
{
public:
	// level state
	std::vector<GameObject> Bricks;
	// constructor
	GameLevel() { }
	// ���ļ��м��عؿ�
	// loads level from file
	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	// ��Ⱦ�ؿ�
	// render level
	void Draw(SpriteRenderer& renderer);
	
	// ���һ���ؿ��Ƿ������(���зǼ�Ӳ�Ĵ�ש�����ݻ�)
	// check if the level is completed (all non-solid tiles are destroyed)
	bool IsCompleted();
private:
	// ��ש�����ݳ�ʼ���ؿ�
	// initialize level from tile data
	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

/**
* ����0����ש�飬��ʾ�ؿ��пյ�����
* ����1��һ����Ӳ��ש�飬���ɱ��ݻ�
* ����1�����֣�һ���ɱ��ݻٵ�ש�飬��ͬ����������ש�����ɫ
*/

#endif // !GAMELEVEL_H

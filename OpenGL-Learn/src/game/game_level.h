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
	// 从文件中加载关卡
	// loads level from file
	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	// 渲染关卡
	// render level
	void Draw(SpriteRenderer& renderer);
	
	// 检查一个关卡是否已完成(所有非坚硬的瓷砖均被摧毁)
	// check if the level is completed (all non-solid tiles are destroyed)
	bool IsCompleted();
private:
	// 由砖块数据初始化关卡
	// initialize level from tile data
	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

/**
* 数字0：无砖块，表示关卡中空的区域
* 数字1：一个坚硬的砖块，不可被摧毁
* 大于1的数字：一个可被摧毁的砖块，不同的数字区分砖块的颜色
*/

#endif // !GAMELEVEL_H

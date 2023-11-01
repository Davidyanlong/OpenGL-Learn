#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include<glad/glad.h>
#include<glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "shader.h"

// PostProcessor hosts all PostProcessing effects for the Breakout
// Game. It renders the game on a textued quad after which one can 
// enable specific effects by enabling either the Confuse, Chaos or
// Shake boolean
// It is required to all BeginRender() before rendering the game
// and EndRender() after rendering the game for the class to work.

class PostProcessor
{
public:
	// state
	Shader PostProcessingShader;
	Texture2D Texture;
	unsigned int Width, Height;
	// options
	bool Confuse, Chaos, Shake;
	// constructor
	PostProcessor(const Shader shader, unsigned int width, unsigned int height);
	// prepares the postprocesssor's framebuffer oprations before rendering the game
	void BeginRender();
	// should be called after rendering the game, so it stores all the rendered data into a textue object
	void EndRender();
	// renders the PostProcessor texture quad (as a screen-encompassing large sprite)
	void Render(float time);
private:
	// render state
	unsigned int MSFBO, FBO; // MSFBO = Multisameped FBO, FBO is regular, used for blitting MS color-buffer to texture
	unsigned int RBO; // RBO is used for multisampled color buffer
	unsigned int VAO;
	// initialize uad for rendering postprocessing texture
	void initRenderData();
};

#endif // !POST_PROCESSOR_H


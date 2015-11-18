/*

Name: Malcolm Mallory
ID: N15265745
Class: CS 3113


*/
#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
//Textures and Loading Images
GLuint LoadTexture(const char *image_path){

	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// IF the below line generates an error, re make the image using photoshop

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Do not use both Clamp and Repeat at once!

	/*
	//For repeating textures that exceed 0-1
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Clamping: Extends the last pixel of the texture onward
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	*/


	SDL_FreeSurface(surface);

	return textureID;
}

void DrawSprite(GLint texture, float x, float y, float rotation, float scale = 1.0) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	glScalef(scale, 1.0, 1.0);

	GLfloat tex[] = { -0.2f, 0.2f, -0.2f, -0.2f, 0.2f, -0.2f, 0.2f, 0.2f };
	GLfloat texUVs[] = { 0.0, 0.0, 0.0, 0.8, 0.8, 0.8, 0.8, 0.0 };
	glVertexPointer(2, GL_FLOAT, 0, tex);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

//HW2
float ballStart(){
	int remainder = rand() % 2 + 1;
	if (remainder == 1) return 1.0f;
	return -1.0f;
}

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	glViewport(0, 0, 640, 360);


	//For No Textures used
	//ShaderProgram program = ShaderProgram("vertex.glsl", "fragment.glsl");

	//For Textures used
	ShaderProgram program = ShaderProgram("vertex_textured.glsl", "fragment_textured.glsl");

	//Sets clear color of screen. (0.0f, 0.0f, 0.0f, 1.0f) is black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Black Color

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	//HW2
	Matrix modelMatrixleftpaddle;
	Matrix modelMatrixrightpaddle;
	Matrix modelMatrixBall;
	Matrix modelMatrixBorder;

	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

	//Declare variables outisde of event loop
	float lastFrameTicks = 0.0f;

	float angle = 0.0f;

	GLuint redTextureID = LoadTexture(RESOURCE_FOLDER"red.png");
	GLuint blueTextureID = LoadTexture(RESOURCE_FOLDER"blue.png");
	GLuint yellowTextureID = LoadTexture(RESOURCE_FOLDER"yellow.png");
	GLuint purpleTextureID = LoadTexture(RESOURCE_FOLDER"purple.png");
	GLuint greenTextureID = LoadTexture(RESOURCE_FOLDER"green.png");
	GLuint orangeTextureID = LoadTexture(RESOURCE_FOLDER"orange.png");

	//HW2
	float paddleLength = 0.4f;
	float paddleWidth = 0.1f;
	float leftpaddleY = 0.0f;
	float leftpaddleX = 0.0f;
	float rightpaddleY = 0.0f;
	float rightpaddlex = 0.0f;

	float ballPosiX = 0.0f;
	float ballPosiY = 0.0f;
	float ballDirectX = 0.0f;
	float ballDirectY = 0.0f;
	float ballSpeed = 1.0f;
	float ballSize = 0.1f;


	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}

			else if (event.type == SDL_KEYDOWN){
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					//make character jump
					//character should be affected by gravity
				}

			}

			else if (event.type == SDL_MOUSEMOTION){
				//event.motion.x is our new x
				//float unitX = (((float)event.motion.x / 640.0f) * 3.554f) - 1.777f;

				//event.motion.y is our new y
				//float unitY = (((float)(360-event.motion.y) / 360.0f) * 2.0f) - 1.0f;
			}

			else if (event.type == SDL_MOUSEBUTTONDOWN){
				//event.button.x = click x position
				//event.button.y = click y position
				//event.button.button is the mouse button clicked (1,2,3,etc.)
			}
		}

		const Uint8 *keys = SDL_GetKeyboardState(NULL); //returns an array of keyboard keys, 1 if key is pressed
		
		//entities[0].acceleration_x = 0.0f //sets player motion to 0 on x axis


		//Polling Input
		if (keys[SDL_SCANCODE_LEFT]) {
			//MOVE CHARACTER LEFT/ROTATE LEFT
		}
		else if (keys[SDL_SCANCODE_RIGHT]) {
			//MOVE CHARACTER RIGHT/ROTATE RIGHT
		}

		else if (keys[SDL_SCANCODE_UP]){
			//MOVE CHARACTER UP
		}

		else if (keys[SDL_SCANCODE_DOWN]){
			//MOVE CHARACTER DOWN
		}

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		program.setModelMatrix(modelMatrix);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);


		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program.programID);



		//HW2 Pong

		//Left Paddle
		program.setModelMatrix(modelMatrixleftpaddle);

		if (keys[SDL_SCANCODE_W] && leftpaddleY < 0.8f) { leftpaddleY += elapsed; } // Up
		else if (keys[SDL_SCANCODE_S] && leftpaddleY > -0.8f){ leftpaddleY -= elapsed; } // Down

		float leftvertices[] = { 
		    -paddleWidth / 2, -paddleLength / 2,
			-paddleWidth / 2, paddleLength / 2,
			paddleWidth / 2, paddleLength / 2,
			-paddleWidth / 2, -paddleLength / 2,
			paddleWidth / 2, -paddleLength / 2,
			paddleWidth / 2, paddleLength / 2 
			};

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, leftvertices);
		glEnableVertexAttribArray(program.positionAttribute);


		modelMatrixleftpaddle.identity();
		modelMatrixleftpaddle.Translate(-1.652f, leftpaddleY, 0.0f);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);

		//Right Paddle
		program.setModelMatrix(modelMatrixrightpaddle);

		if (keys[SDL_SCANCODE_UP] && rightpaddleY < 0.8f) { rightpaddleY += elapsed; } // Up
		else if (keys[SDL_SCANCODE_DOWN] && rightpaddleY > -0.8f){ rightpaddleY -= elapsed; } // Down

		float rightvertices[] = { 
		    -paddleWidth / 2, -paddleLength / 2, 
			-paddleWidth / 2, paddleLength / 2,
			paddleWidth / 2, paddleLength / 2,
			-paddleWidth / 2, -paddleLength / 2,
			paddleWidth / 2, -paddleLength / 2, 
			paddleWidth / 2, paddleLength / 2 
			};

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, rightvertices);
		glEnableVertexAttribArray(program.positionAttribute);

		modelMatrixrightpaddle.identity();
		modelMatrixrightpaddle.Translate(1.652f, rightpaddleY, 0.0f);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);

		//Ball
		program.setModelMatrix(modelMatrixBall);

		if (keys[SDL_SCANCODE_SPACE] && ballDirectX == 0 && ballDirectY == 0)
		{
			ballDirectX = ballStart();
			ballDirectY = ballStart();
		}

		ballPosiX += elapsed * ballSpeed * ballDirectX;
		ballPosiY += elapsed * ballSpeed * ballDirectY;
		
		if (ballPosiY <= -0.975f) { ballDirectY = 1.0f; } // Ball impacting bottom
		else if (ballPosiY >= 0.975f) { ballDirectY = -1.0f; } // Ball impacting top

		// Ball impacting left paddle
		if (-1.652f + paddleWidth / 2 >= ballPosiX - ballSize / 2 &&
			leftpaddleY - paddleLength / 2 <= ballPosiY - ballSize / 2 &&
			leftpaddleY + paddleLength / 2 >= ballPosiY + ballSize / 2) {
			ballDirectX = 1.0f;
		} 

		// Ball impact right paddle
		if (1.652f - paddleWidth / 2 <= ballPosiX + ballSize / 2 &&
			rightpaddleY - paddleLength / 2 <= ballPosiY - ballSize / 2 &&
			rightpaddleY + paddleLength / 2 >= ballPosiY + ballSize / 2) {
			ballDirectX = -1.0f;
		}

		// Ball impacting goal
		if (ballPosiX >= 1.752f || ballPosiX <= -1.752) {
			ballPosiX = 0.0f; ballPosiY = 0.0f; ballDirectX = 0.0f; ballDirectY = 0.0f;
			leftpaddleY = 0.0f;  rightpaddleY = 0.0f; //Resets the paddles
		} 

		float ballvertices[] = { -ballSize / 2, -ballSize / 2, ballSize / 2, ballSize / 2, -ballSize / 2, ballSize / 2,
			-ballSize / 2, -ballSize / 2, ballSize / 2, -ballSize / 2, ballSize / 2, ballSize / 2 };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ballvertices);
		glEnableVertexAttribArray(program.positionAttribute);

		modelMatrixBall.identity();
		modelMatrixBall.Translate(ballPosiX, ballPosiY, 0.0f);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);

		//Border?
		program.setModelMatrix(modelMatrixBorder);
		float bottomvertices[] = {
			1.77f, -1.0f, //top right 
			-1.77f, -1.0f, //top left
			-1.77f, -1.0f, //bottom left
			1.77f, -1.0f }; //bottom right
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, bottomvertices);
		glEnableVertexAttribArray(program.positionAttribute);

		float bottomtexcoords[] = { 1.5f, 1.5f, -1.5f, 1.5f, -1.5f, -1.5f, 1.5f, -1.5f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, bottomtexcoords);
		glEnableVertexAttribArray(program.texCoordAttribute);




		glBindTexture(GL_TEXTURE_2D, blueTextureID);
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
		

		//HW1 Textures
		/*
		//Blue Box
		modelMatrix.identity();


		program.setModelMatrix(modelMatrix);

		float iconvertices2[] = {
			3.56f, 2.0f, //top right 
			-3.56f, 2.0f, //top left
			-3.56f, -2.0f, //bottom left
			3.56f, -2.0f }; //bottom right
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, iconvertices2);
		glEnableVertexAttribArray(program.positionAttribute);

		float iconTexCoords2[] = { 1.5f, 1.5f, -1.5f, 1.5f, -1.5f, -1.5f, 1.5f, -1.5f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, iconTexCoords2);
		glEnableVertexAttribArray(program.texCoordAttribute);




		glBindTexture(GL_TEXTURE_2D, blueTextureID);
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//yellow Box
		modelMatrix.identity();
		modelMatrix.Rotate(-ticks * ticks);

		program.setModelMatrix(modelMatrix);

		float iconvertices1[] = {
			0.0f, 0.0f, //top right 
			-1.0f, 0.0f, //top left
			-1.0f, -1.0f, //bottom left
			0.0f, -1.0f }; //bottom right
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, iconvertices1);
		glEnableVertexAttribArray(program.positionAttribute);

		float iconTexCoords1[] = { 1.5f, 1.5f, -1.5f, 1.5f, -1.5f, -1.5f, 1.5f, -1.5f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, iconTexCoords1);
		glEnableVertexAttribArray(program.texCoordAttribute);




		glBindTexture(GL_TEXTURE_2D, yellowTextureID);
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);



		//Red Box
		modelMatrix.identity();
		modelMatrix.Rotate(ticks);

		program.setModelMatrix(modelMatrix);

		float iconvertices3[] = { 
			0.5f, 0.5f, //top right 
			-0.5f, 0.5f, //top left
			-0.5f, -0.5f, //bottom left
			0.5f, -0.5f }; //bottom right
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, iconvertices3);
		glEnableVertexAttribArray(program.positionAttribute);

		float iconTexCoords3[] = { 1.5f, 1.5f, -1.5f, 1.5f, -1.5f, -1.5f, 1.5f, -1.5f};
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, iconTexCoords3);
		glEnableVertexAttribArray(program.texCoordAttribute);




		glBindTexture(GL_TEXTURE_2D, redTextureID);
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
		*/
		
		//HW1 No Textures
		/* //HW1 no textures
		modelMatrix.identity();
		modelMatrix.Rotate(ticks);
		program.setModelMatrix(modelMatrix);

		//Triangle Declaration
		float vertices[] = {
			-0.5f, -0.5f, //Bottom Left
			0.5f, -0.5f, // bottom right
			0.5f, 0.5f, //Top right
			-0.5f, 0.5f //top left
		};
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		//Triangle Drawing
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);

		modelMatrix.identity();
		modelMatrix.Rotate(-ticks);
		program.setModelMatrix(modelMatrix);

		//Triangle Declaration
		float vertices2[] = {
			-0.5f, -0.5f, //Bottom Left
			-1.5f, -1.5f, // bottom right
			-1.5f, 0.5f, //Top right
			-0.5f, 0.5f //top left
		};
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
		glEnableVertexAttribArray(program.positionAttribute);

		//Triangle Drawing
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);

		modelMatrix.identity();
		modelMatrix.Rotate((ticks * ticks));
		program.setModelMatrix(modelMatrix);

		//Triangle Declaration
		float vertices3[] = {
			0.5f, 0.0f, //Bottom Left
			1.5f, 0.0f, // bottom right
			1.5f, 1.5f, //Top right
			1.0f, 1.5f //top left
		};
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
		glEnableVertexAttribArray(program.positionAttribute);

		//Triangle Drawing
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);

		*/

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}

//Vertex Shader

//Vertex Shader is where the model, view, and projection matrices are applied to the vertex
//Runs numerous times a second, can pass vertex color to shaders
//Animation is from the vertex shader; entities are skeletal meshes that are then animated

//Fragment Shader

//Fragment Shader is where the returns the color of each pixel when the geometry is rasterized on the GPU
//Fairly complicated, running for every single pixel, really fast. Has to run on GPU
//Rasterizing
//Responsible for determining the final pixel seen on the screen.
//Gloom and lighting is determined by Fragment Shader

//Shader Program

//Shader Program is a combination of a vertex and a fragment shader.
//Written in GLSL; openGL Shading Language

//Circle-Circle

/*
//If the distance between two circles is less or equal to the sum of their
//radii, they are colliding.
*/

//Circle-Point
/*
//If the distance between a circle and a point is less than the circle's radius
//they are colliding.
*/

//Box-Box
/*
//if any of the following are true, not intersecting

//R1's Bottom is higher than R2's Top
//R1's Top is lower than R2's Bottom
//R1's left is larger than R2's Right
//R1's right is smaller than R2's left
*/

//Box-Point
/*
// If point x is larger than box left and smaller than box right
// If point y is larger than box bottom and smaller than box top
*/

//Paddle
//y_position += elapsed * distance_to_travel_in_one_second

//Ball
//position.y += sin(angle) * elapsed * units_a_second
//position.x += cos(angle) * elapsed * units a second

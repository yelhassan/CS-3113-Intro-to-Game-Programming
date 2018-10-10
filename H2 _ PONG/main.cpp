#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define STB_IMAGE_IMPLEMENTATION //required for stb image library
#include "stb_image.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#include <unistd.h>

SDL_Window* displayWindow;

GLuint LoadTexture(const char *filePath);

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif

    //SETUP
    glViewport(0, 0, 640, 360);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    ShaderProgram program;
    ShaderProgram texturedProgram;
    program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    texturedProgram.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    
    projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    //background color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    //paddle variables
    float paddleHeight = 0.6;
    float paddleWidth = 0.1;
    
    float leftPaddleX = -1.65 + (paddleWidth/2);
    float leftPaddleY = 0.0f;
    
    float rightPaddleX = 1.6 + (paddleWidth/2);
    float rightPaddleY = 1.0f;
    
    float speed = 1.5f;
    
    //ball variables
    float ballX = 0.0f;
    float ballY = 0.0f;
    float ballHeight = 0.1f;
    float ballWidth = 0.1f;
    
    float dirX = 1.0f;
    float dirY = 0.0f;
    float ballSpeed = 2.0f;
    
    // score keeping
    int scorePlayer1 = 0;
    int scorePlayer2 = 0;
    
    bool winPlayer1 = false;
    bool winPlayer2 = false;
    
    int round = 1;

    //time keeping
    float ticks;
    float timeElapsed;
    float lastFrameTicks = 0;
    
    //GAME LOOP
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        
        // KEEP TIME -- ANIMATE & MOVE
        ticks = (float)SDL_GetTicks()/1000.0f;
        timeElapsed = (ticks - lastFrameTicks) * 1.5;
        lastFrameTicks = ticks;
        
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program.programID);
        
        //GET KEYBOARD STATE
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        
//LEFT PADDLE
        program.SetColor(0.2f, 0.8f, 0.4f, 1.0f);
        
        float leftPaddlePosY = leftPaddleY + (paddleHeight/2);
        float leftPaddleNegY = leftPaddleY - (paddleHeight/2);
        
        modelMatrix = glm::mat4(1.0f);
        
        // move the paddle with WASD keys within walls
        if(keys[SDL_SCANCODE_W]) {
            if (leftPaddleY + (paddleHeight/2) < 1.0f) { leftPaddleY += timeElapsed * speed; }
        }
        else if(keys[SDL_SCANCODE_S]) {
            if (leftPaddleY - (paddleHeight/2) > -1.0f ){ leftPaddleY -= timeElapsed * speed;}
        }
        
        program.SetModelMatrix(modelMatrix);
        program.SetProjectionMatrix(projectionMatrix);
        program.SetViewMatrix(viewMatrix);
        
        float vertices[] = {-1.7, leftPaddleNegY, -1.6, leftPaddleNegY, -1.6, leftPaddlePosY, -1.7, leftPaddleNegY, -1.6, leftPaddlePosY, -1.7, leftPaddlePosY};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program.positionAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        
//RIGHT PADDLE
        program.SetColor(0.2f, 0.8f, 0.4f, 1.0f);
        
        float rightPaddlePosY = rightPaddleY + (paddleHeight/2);
        float rightPaddleNegY = rightPaddleY - (paddleHeight/2);
        
        modelMatrix = glm::mat4(1.0f);
        
        program.SetModelMatrix(modelMatrix);
        program.SetProjectionMatrix(projectionMatrix);
        program.SetViewMatrix(viewMatrix);
        
        float vertices2[] = { 1.7,rightPaddleNegY , 1.6, rightPaddleNegY, 1.6, rightPaddlePosY, 1.7, rightPaddleNegY,1.6, rightPaddlePosY, 1.7, rightPaddlePosY};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
        glEnableVertexAttribArray(program.positionAttribute);
        
        // move the paddle with arrow keys within screen
        if(keys[SDL_SCANCODE_UP]) {
            if (rightPaddleY + (paddleHeight/2) < 1.0f ) { rightPaddleY += timeElapsed * speed; }
        }
        else if(keys[SDL_SCANCODE_DOWN]) {
            if (rightPaddleY - (paddleHeight/2) > -1.0f){ rightPaddleY -= timeElapsed * speed;}
        }
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
 //BALL
        program.SetColor(15.0f, 15.0f, 15.0f, 1.0f);
        
        modelMatrix = glm::mat4(1.0f);
        
        program.SetModelMatrix(modelMatrix);
        program.SetProjectionMatrix(projectionMatrix);
        program.SetViewMatrix(viewMatrix);
        
        float ballPosX = ballX + (ballWidth/2);
        float ballNegX = ballX - (ballWidth/2);
        
        float ballPosY = ballY + (ballHeight/2);
        float ballNegY = ballY - (ballHeight/2);
        
        float vertices3[] = {ballNegX, ballNegY,ballPosX, ballNegY, ballPosX, ballPosY, ballNegX, ballNegY, ballPosX, ballPosY, ballNegX, ballPosY};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
        glEnableVertexAttribArray(program.positionAttribute);
        
        //Keep score & bring the ball back if it leaves the screen
        // right wall, left side score
        if ( ballX + ballWidth > 2.0 ) {
            ballX = 0.0f;
            ballY = 0.0f;
            scorePlayer1++;
            std::cout << "PLAYER 1 SCORED!" << "\n" <<  "________________" << "\n" << "  SCOREBOARD" <<  "\n" <<"\n" << "PLAYER 1: " << scorePlayer1 <<  "\n" << "PLAYER 2: " << scorePlayer2 << "\n" << "\n" ;
        }
        
        //left wall, leftside score
        if ( ballX - ballWidth < -2.0 ) {
            ballX = 0.0f;
            ballY = 0.0f;
            scorePlayer2++;
            
            std::cout << "PLAYER 2 SCORED!" << "\n" <<  "________________" << "\n" << "  SCOREBOARD" <<  "\n" << "\n" << "PLAYER 1: " << scorePlayer1 <<  "\n" << "PLAYER 2: " << scorePlayer2 << "\n" << "\n" ;
            
            //hold ball at that position for a few screens to indicate begining of new round
            //sleep(2);
        }
        
        
        //check rounds & track wins
        if (scorePlayer1 == 7 ){
            winPlayer1 = true;
            
            //reset scores -- new round
            scorePlayer1 = 0;
            scorePlayer2 = 0;
            
            round++;
            
            std::cout << "PLAYER 1 WINS!" << "\n";
            std::cout << "ROUND " << round << "\n" << "\n";
        }
        
        if (scorePlayer2 == 7 ){
            winPlayer2 = true;
    
            //reset scores -- new round
            scorePlayer1 = 0;
            scorePlayer2 = 0;
            
            std::cout << "PLAYER 2 WINS!" << "\n";
            std::cout << "ROUND " << round << "\n" << "\n";
        }
        

        float  rightDistanceX = abs(rightPaddleX - ballX) - ((ballWidth + paddleWidth)/2);
        float  rightDistanceY = abs(rightPaddleY - ballY) - ((ballHeight + paddleHeight)/2);
        
        if(rightDistanceX < 0 && rightDistanceY < 0){
            
            ballX = rightPaddleX - paddleWidth - 0.1;
            dirX *= -1.0f;
            
            //if it hits the top of the paddle, hit it back upwards;
            if (ballY > rightPaddleY ) {dirY = 1.3;}
            //if it hits the bottom of the paddle, hit it back downwards
            if (ballY < rightPaddleY ) {dirY = -1.3;}
            //if it hits the center, hit it back with no y.change
            if (ballY == rightPaddleY) {dirY = 0;}
        }
        
        float  leftDistanceX = abs(leftPaddleX - ballX) - ((ballWidth + paddleWidth)/2);
        float  leftDistanceY = abs(leftPaddleY - ballY) - ((ballHeight + paddleHeight)/2);
        
        if(leftDistanceX < 0 && leftDistanceY < 0){
            ballX = leftPaddleX + paddleWidth + 0.1;
            dirX *= -1.0f;
            
            //if it hits the top of the paddle, hit it back upwards;
            if (ballY > leftPaddleY ) {dirY = 1.3;}
            //if it hits the bottom of the paddle, hit it back downwards
            if (ballY < leftPaddleY ) {dirY = -1.3;}
            //if it hits the center, hit it back with no y.change
            if (ballY == leftPaddleY) {dirY = 0;}
        }
        
        //bounce off top & bottom walls
        if ( ballY + ballHeight > 1.0 || ballY - ballHeight < -1.0 ) {
            dirY *= -1;
        }
        
        //launch the ball
        ballX += dirX * timeElapsed;
        ballY += dirY * timeElapsed;

        glDrawArrays(GL_TRIANGLES, 0, 6);

        /////////////////////////////////
        glDisableVertexAttribArray(program.positionAttribute);
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}


// texture function, returns textureID
GLuint LoadTexture(const char *filePath) {
    
    int w,h,comp;
    
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
    
    if(image == NULL) { //check if loaded
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false); // triggers an exception; no point in continuing program
    }
    
    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(image);
    return retTexture;
}

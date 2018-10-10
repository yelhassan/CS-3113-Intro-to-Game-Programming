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
    
    
  
    
 

    
    float paddleHeight = 0.6;
    float paddleWidth = 0.1;

    float leftPaddleX = -1.65 + (paddleWidth/2);
    float leftPaddleY = 0.0f;
    
    float rightPaddleX = 1.6 + (paddleWidth/2);
    float rightPaddleY = 1.0f;
    float rightPaddleWidth = 0.0f;
    float rightPaddlehHeight = 0.0f;
    
    
    int screenWidth = 640;
    int screenHeight = 360;
    
    glViewport(0, 0, screenWidth, screenHeight);
    
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
    //program.SetColor(0.2f, 0.8f, 0.4f, 1.0f);
    
//    float posY = 0;
//    float posY1 = 0;
    float speed = 1.5f;
    
    float dirX = 1.0f;
    float dirY = 0.0f;
    float ballSpeed = 2.0f;
    
    
    float ballX = 0.0f;
    float ballY = 0.0f;
    float ballHeight = 0.1f;
    float ballWidth = 0.1f;
    
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
//        modelMatrix = glm::translate(modelMatrix, glm::vec3(leftPaddleX,0.0f,0.0f));
////        modelMatrix = glm::scale(modelMatrix, glm::vec3(PaddleWidth, PaddlehHeight, 1.0f));
//        modelMatrix = glm::translate(modelMatrix, glm::vec3(leftPaddleX,leftPaddleY,0.0f));

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
    
        
        
        
                //rightPaddleX = 0.0f;
                //rightPaddleY = 0.0f;
        
        
        
        //float rightPaddleX;
        //rightPaddleY = 0.0f;
        float rightPaddlePosY = rightPaddleY + (paddleHeight/2);
        float rightPaddleNegY = rightPaddleY - (paddleHeight/2);

//
modelMatrix = glm::mat4(1.0f);
//    modelMatrix = glm::translate(modelMatrix, glm::vec3(rightPaddleX,0.0f,0.0f));
//    modelMatrix = glm::scale(modelMatrix, glm::vec3(rightPaddleWidth, rightPaddlehHeight, 1.0f));
//    modelMatrix = glm::translate(modelMatrix, glm::vec3(rightPaddleX,rightPaddleY,0.0f));
    
   
    program.SetModelMatrix(modelMatrix);
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
       // float vertices[] = {-1.7, -0.3, -1.6, -0.3, -1.6, 0.3, -1.7, -0.3, -1.6, 0.3, -1.7, 0.3};

    float vertices2[] = { 1.7,rightPaddleNegY , 1.6, rightPaddleNegY, 1.6, rightPaddlePosY, 1.7, rightPaddleNegY,1.6, rightPaddlePosY, 1.7, rightPaddlePosY};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
    glEnableVertexAttribArray(program.positionAttribute);
    
        
        //float bottomPaddle = rightPaddleY + PaddleHeight/2;
        
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
        

        //launch the ball
        //ballY += dirY * ballSpeed * timeElapsed;
        
        //bounce off
        //if ((ballX + ((ballWidth * 0.5) /2)) > (rightPaddleX - 1.37) - (rightPaddleWidth / 0.15)) { dirX = -1.0f;}
        
        
//        if ((ballX + ballWidth/2) > 14.5f) { dirX = -1.0f;}
//        if (ballX + ballWidth/2 < -14.5f) { dirX = 1.0f;}
        
        
        
        
        //        //if { dirX = 1;}
        //        if (ballX > 5.0f) { dirX = -1.0;}
        //        if (ballX < - 1.0f) { dirX = 1.0;}
        
        
       //// //right paddle hit
//        if(ballX + (ballWidth/2) >= rightPaddleX - (PaddleWidth/2))
//        {dirX = -1.0f;}
//        else if( ballX - (ballWidth/2) <= leftPaddleX + (PaddleWidth/2) )
//                {dirX = 1.0f;}
        
    
       // bounce off top & bottom wall
//        if (ballX - (ballWidth/2) <= -1.777 || ballX + (ballWidth/2) >= 1.777)
//        { dirX *= -1;}
//        if (ballY + (ballHeight/2) >= 1.0 || ballY - (ballHeight/2) <= -1.0)
//        { dirY *= -1;}
//
//        //right paddle hit
//            // distance on X & Y from right paddle
//        float  distanceX = abs(rightPaddleX - ballX) - ((ballWidth/2 + PaddleWidth/2)/2);
//        float  distanceY = abs(rightPaddleY - ballY) - ((ballHeight/2 + PaddleHeight/2)/2);
//        if (distanceX < 0 || distanceY < 0) {
//            hitRight = true;
//            dirX = -1.0f;
//        }
        
//        if (ballX + (ballWidth/2) >= 1.777)
//        { dirX *= -1;}
        
//        //right paddle hit
                     //distance on X & Y from right paddle
//        float  distanceX = abs(rightPaddleX - ballX) - ((ballWidth + PaddleWidth)/2);
//        float  distanceY = abs(rightPaddleY - ballY) - ((ballHeight + PaddleHeight)/2);
//
//
//        if (distanceX <= 0 && distanceY <= 0) {
//            dirX *= -1.0f; }
//
//
//        if ( ballX + (ballWidth/2) >= rightPaddleX - (PaddleWidth/2) && ballY + (ballHeight/2) <= rightPaddleY + (PaddleHeight/2) && ballY - (ballHeight/2) >= rightPaddleY - (PaddleHeight/2) ) {
//            //dirX *= -1.0f;
//
//            std::cout << "hit \n";
//        }
        
//
//        std::cout << "X:";
//        std::cout << ballX;
//        std::cout << "\n";
//        std::cout << " Y:";
//        std::cout << ballY;

//
//        if (ballX + (ballWidth/2) >= rightPaddleX - (paddleWidth/2) && (ballY + (ballHeight/2) >= rightPaddleY - (paddleHeight/2) || ballY - (ballHeight/2) <= rightPaddleY - (paddleWidth/2)))
//        {
//            ballX = rightPaddleX - paddleWidth - 0.1;
//            dirX *= -1.0f;
//
//        }
//
//        if (ballX - (ballWidth/2) <= leftPaddleX + (paddleWidth/2)) {
//            ballX = leftPaddleX + paddleWidth + 0.1;
//            dirX *= -1.0f;
//
//        }
        
        
        //bring the ball back if it leaves the screen
        if ( ballX + (ballWidth/2) >= 1.777 || ballX - (ballWidth/1) <= -1.777 ) {
            ballX = 0.0f;
            ballY = 0.0f;
            
            //hold ball at that position for a few screens to indicate begining of new round
            //sleep(2);
        }
        
        float  rightDistanceX = abs(rightPaddleX - ballX) - ((ballWidth + paddleWidth)/2);
        float  rightDistanceY = abs(rightPaddleY - ballY) - ((ballHeight + paddleHeight)/2);
       
        if(rightDistanceX < 0 && rightDistanceY < 0){
            
            ballX = rightPaddleX - paddleWidth - 0.1;
            dirX *= -1.0f;
            
        }
        
        float  leftDistanceX = abs(leftPaddleX - ballX) - ((ballWidth + paddleWidth)/2);
        float  leftDistanceY = abs(leftPaddleY - ballY) - ((ballHeight + paddleHeight)/2);
        
        if(leftDistanceX < 0 && leftDistanceY < 0){
            ballX = leftPaddleX + paddleWidth + 0.1;
            dirX *= -1.0f;
        }
        
       
        
        ballX += dirX * timeElapsed;

        

        
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

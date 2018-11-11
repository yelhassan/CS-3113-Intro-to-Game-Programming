//tilesheet

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

#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <ctime>


SDL_Window* displayWindow;

// linear interpolation (curve fitting); value changes smoothly
float lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}

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

class SheetSprite {
public:
    SheetSprite();
    SheetSprite(unsigned int ID, float U, float V, float Width, float Height, float Size);
    
    void Draw(ShaderProgram &program);
    
    float size;
    unsigned int textureID;
    float u;
    float v;
    float width;
    float height;
};


SheetSprite::SheetSprite(){
    
}

SheetSprite::SheetSprite(unsigned int ID, float U, float V, float Width, float Height, float Size) {
    
    textureID = ID;
    u = U;
    v = V;
    width = Width;
    height = Height;
    size = Size;
}

void SheetSprite::Draw(ShaderProgram &program) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLfloat texCoords[] = {
        u, v+height,
        u+width, v,
        u, v,
        u+width, v,
        u, v+height,
        u+width, v+height
    };
    float aspect = width / height;
    float vertices[] = {
        -0.5f * size * aspect, -0.5f * size,
        0.5f * size * aspect, 0.5f * size,
        -0.5f * size * aspect, 0.5f * size,
        0.5f * size * aspect, 0.5f * size,
        -0.5f * size * aspect, -0.5f * size ,
        0.5f * size * aspect, -0.5f * size};
    
    glUseProgram(program.programID);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawText(ShaderProgram &program, int fontTexture, std::string text, float size, float spacing) {
    float character_size = 1.0/16.0f;
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    for(int i=0; i < text.size(); i++) {
        int spriteIndex = (int)text[i];
        float texture_x = (float)(spriteIndex % 16) / 16.0f;
        float texture_y = (float)(spriteIndex / 16) / 16.0f;
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
        });
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + character_size,
            texture_x + character_size, texture_y, //
            texture_x + character_size, texture_y + character_size,
            texture_x + character_size, texture_y,
            texture_x, texture_y + character_size,
        }); }
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glUseProgram(program.programID);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program.texCoordAttribute);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, (int) text.size()*6);
}


enum EntityType {ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_COIN};

class Entity{
public:
    
    Entity();
    void DrawSprite(ShaderProgram &program, int index, int spriteCountX,int spriteCountY);
    
    //    glm::vec3 position;
    //    glm::vec3 velocity;
    //    glm::vec3 size;
    
    float xPos;
    float yPos;
    float Vx;
    float Vy;
    float Ax;
    float Ay;
    
    float width;
    float height;
    
    float health;
    float rotation;
    
    bool collision = false;
    bool isStatic;
    
    EntityType entityType;
};


Entity::Entity () {
    
}

void Entity::DrawSprite(ShaderProgram &program, int index, int spriteCountX,int spriteCountY) {
    
    float u = (float)(((int)index) % spriteCountX) / (float) spriteCountX;
    float v = (float)(((int)index) / spriteCountY) / (float) spriteCountY;
    float spriteWidth = 1.0/(float)spriteCountX;
    float spriteHeight = 1.0/(float)spriteCountY;
    
    float texCoords[] = {
        u, v+spriteHeight,
        u+spriteWidth, v,
        u, v,
        u+spriteWidth, v,
        u, v+spriteHeight,
        u+spriteWidth, v+spriteHeight};
    
    float vertices[] = {-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,  -0.5f,-0.5f, 0.5f, -0.5f};
    
    glUseProgram(program.programID);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
}

//gamestates
class MainMenu {
    
    //text
public:
    Entity welcome;
};

/******************************************************************************************/

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
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    //background color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    //time keeping
    float ticks;
    float elapsedTime;
    float lastFrameTicks = 0;
    
    // 60 FPS (1.0f/60.0f) (update sixty times a second)
//    #define FIXED_TIMESTEP 0.0166666f
//    #define MAX_TIMESTEPS 6
    
    //float accumulator = 0.0f;
    
    GLuint LoadTexture(const char *filePath);
    // prep screens
    MainMenu mainMenu;
    
    enum GameMode { STATE_MAIN_MENU, STATE_GAME_LEVEL};
    GameMode mode;
    mode = STATE_MAIN_MENU;
    
    //    SheetSprite mySprite;
    //    int spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"tilesheet.png");
    //    mySprite = SheetSprite(spriteSheetTexture, 425.0f/1024.0f, 468.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f, 0.2f)
    
    int EntitySheetTexture = LoadTexture(RESOURCE_FOLDER"spritesheet.png");
    
    //initialize player at center
    Entity player;
    Entity platform;
    
    //create an array of enemies
    std::vector<Entity> enemies;
    for(int i = 0; i < 10; i++){
        Entity enemy;
        enemies.push_back(enemy);
    }
    
    //prep bullets/flowers
    #define MAX_BULLETS 10
    int bulletIndex = 0;
    Entity bullets[MAX_BULLETS];
    for(int i=0; i < MAX_BULLETS; i++) {
        bullets[i].xPos = player.xPos;
    }
    
    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float accelerationX = 2.0f;
    float accelerationY = 2.0f;
    float frictionX = 0.7f;
    float frictionY = 0.7f;
    float gravityX = 0.0;
    float gravityY = - 1.0;
    
    bool hasCollided = false;
    
    float penetration = 0 ;

    float playerWidth;
    float playerHeight;
    float platformWidth;
    float platformHeight;
    
    #define LEVEL_HEIGHT 2
    #define LEVEL_WIDTH 22
    #define SPRITE_COUNT_X 16
    #define SPRITE_COUNT_Y 8
    
    float TILE_SIZE = 0.1;
    float numberOfBlocks = 0;

    unsigned int levelData[LEVEL_HEIGHT][LEVEL_WIDTH] = {
        {0,20,4,4,4,4,4,4,0,0,0,0,0,0,4,4,4,4,4,4,20,0},
        {0,20,4,4,4,4,4,4,0,0,0,0,0,0,4,4,4,4,4,4,20,0}
    };
    
    for(int y=0; y < LEVEL_HEIGHT; y++) {
        for(int x=0; x < LEVEL_WIDTH; x++) {
            if(levelData[y][x] != 0) {
                numberOfBlocks++;
            }
        }
    }
    

    //////////////
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            } else if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    velocityY += 1.5f;
                } }
        }
        
        ticks = (float)SDL_GetTicks()/1000.0f;
        elapsedTime = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        //        elapsedTime += accumulator;
        //        if(elapsedTime < FIXED_TIMESTEP) {
        //            accumulator = elapsedTime;
        //            continue;
        //        }
        //        while(elapsedTime >= FIXED_TIMESTEP) {
        //            //Update(FIXED_TIMESTEP);
        //            elapsedTime -= FIXED_TIMESTEP;
        //        }
        //        accumulator = elapsedTime;
        
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program.programID);
        glUseProgram(texturedProgram.programID);
     
        std::vector<float> vertexData;
        std::vector<float> texCoordData;
        for(int y=0; y < LEVEL_HEIGHT; y++) {
            for(int x=0; x < LEVEL_WIDTH; x++) {
                
                float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
                float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
                float spriteWidth = 1.0f/(float)SPRITE_COUNT_X;
                float spriteHeight = 1.0f/(float)SPRITE_COUNT_Y;
                
                if(levelData[y][x] != 0) {
                vertexData.insert(vertexData.end(),{
                    TILE_SIZE * x, -TILE_SIZE * y,
                    TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                    TILE_SIZE * x, -TILE_SIZE * y,
                    (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y });
                
                texCoordData.insert(texCoordData.end(), {
                    u, v,
                    u, v+(spriteHeight),
                    u+spriteWidth, v+(spriteHeight),
                    u, v,
                    u+spriteWidth, v+(spriteHeight),
                    u+spriteWidth, v});
                
                    modelMatrix = glm::mat4(1.0f);
                    texturedProgram.SetModelMatrix(modelMatrix);
                
                    glBindTexture(GL_TEXTURE_2D, EntitySheetTexture);
                    glUseProgram(texturedProgram.programID);
                glVertexAttribPointer(texturedProgram.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
                glEnableVertexAttribArray(texturedProgram.texCoordAttribute);
                glVertexAttribPointer(texturedProgram.positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
                glEnableVertexAttribArray(texturedProgram.positionAttribute);
                
                    glDrawArrays(GL_TRIANGLES, 0, numberOfBlocks * 6 );
                }
            }
        }
       
       
        //glDrawArrays(GL_TRIANGLES, 0, (int) text.size()*6);
        
        program.SetColor( 1.0f, 0.0f, 0.0f, 1.0f);
        
        platform.xPos = -0.3f;
        platform.yPos = -1.0f;
        platformWidth = 3.5f;
        platformHeight = 0.15;
        
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(platform.xPos, platform.yPos, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(platformWidth, platformHeight, 1.0f));
        program.SetModelMatrix(modelMatrix);
        platform.DrawSprite(program, 0, 1, 2);
        
        //apply gravity -- constant acceleration
//        velocityY += gravityY * elapsedTime;
//        player.yPos += velocityY * elapsedTime;

        playerWidth = 0.2;
        playerHeight = 0.2;
        
        //detect collision with platform
        float  distanceX = abs(player.xPos - platform.xPos) - ((playerWidth + platformWidth)/2);
        float  distanceY = abs(player.yPos - platform.yPos) - ((playerHeight + platformHeight)/2);
        
        if(distanceX < 0 & distanceY < 0) {
            hasCollided = true;
            //calculate penetration
            penetration = fabs((player.yPos - platform.yPos) - playerHeight/2 - platformHeight/2);
            //raise up by penetration
            player.yPos += penetration;
            
            velocityY = 0;
        }
        
        program.SetColor( 1.0f, 1.0f, 1.0f, 1.0f);
        
        player.yPos = -0.5f;
        
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(player.xPos, player.yPos, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 1.0f));
        program.SetModelMatrix(modelMatrix);
        
        //move player
        if(keys[SDL_SCANCODE_LEFT]) {
            if(player.xPos - playerWidth/2 > -1.77f) {
                velocityX = lerp(velocityX, 0.0f, elapsedTime * frictionX);
                velocityX += accelerationX * elapsedTime;
                player.xPos -= velocityX * elapsedTime * 3.0;
            }
        }
        else if(keys[SDL_SCANCODE_RIGHT]) {
            velocityX = lerp(velocityX, 0.0f, elapsedTime * frictionX);
            velocityX += accelerationX * elapsedTime;
            player.xPos += velocityX * elapsedTime * 3.0;
        }
        
        player.DrawSprite(program, 0, 1, 2);
        
        viewMatrix = glm::mat4(1.0f);
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-1 * player.xPos, 0.0f, 0.0f));
        program.SetViewMatrix(viewMatrix);
        
        /*******************************/
        texturedProgram.SetViewMatrix(viewMatrix);
        texturedProgram.SetProjectionMatrix(projectionMatrix);
        program.SetViewMatrix(viewMatrix);
        program.SetProjectionMatrix(projectionMatrix);
        
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(texturedProgram.positionAttribute);
        glDisableVertexAttribArray(texturedProgram.texCoordAttribute);
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}



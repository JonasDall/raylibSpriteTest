#include <iostream>
#include <string>
#include <vector>

#include "raylib.h"

#define CYAN CLITERAL(Color){ 100, 255, 224, 255 }
#define ANIM_FRAMERATE 12

class OutputTexture
{
private:
    RenderTexture2D m_texture;
    Rectangle m_source;
    Rectangle m_destination;
    Vector2 m_origin;
    int m_textureWidth;
    int m_textureHeight;

public:
    OutputTexture(int texture_width, int texture_height) : m_textureWidth{texture_width}, m_textureHeight{texture_height}
    {
        unsigned int width = GetScreenWidth();
        unsigned int height = GetScreenHeight();
        m_texture = LoadRenderTexture(m_textureWidth, m_textureHeight);
        float ratio = m_textureWidth / width;

        m_source ={0.0f, 0.0f, (float)m_textureWidth, -(float)m_textureHeight};
        m_destination ={-ratio, -ratio, width + (ratio*2), height + (ratio*2) };
        m_origin ={0.0f, 0.0f};
        
        // char *log;
        // TraceLog(LOG_ERROR, log);
        // m_source = {0.0f, 0.0f, (float)m_texture.texture.width, -(float)m_texture.texture.height};
        // m_destination ={-0.0, -0.0, (float)width, (float)height};
    }

    RenderTexture2D getTexture()
    {
        return m_texture;
    }

    void draw()
    {
        DrawTexturePro(m_texture.texture, m_source, m_destination, m_origin, 0.0f, WHITE);
    }

    void updateSize()
    {
        // int width = GetMonitorWidth(GetCurrentMonitor());
        // int height = GetMonitorHeight(GetCurrentMonitor());
        // Rectangle destRect  ={-ratio, -ratio, screenWidth + (ratio*2), screenHeight + (ratio*2) };
        // Vector2 origin      ={0.0f, 0.0f};

    }
};

class Sprite
{
private:
    Texture2D m_texture;
    Rectangle m_frame;
    int m_currentFrame = {0};
    int m_frames;
    Vector2 m_position = {0.0f, 0.0f};
    double m_currentFrameTime = {0.0f};
    double m_frameTime;

public:
    Sprite(std::string location)
    {
        m_texture = LoadTexture(location.c_str());
        m_frame = {0.0f, 0.0f, (float)m_texture.height, (float)m_texture.height};
        m_frames = (m_texture.width / m_texture.height) -1;
        std::cout << m_frames << '\n';
        m_frameTime = 1.0 / (double)ANIM_FRAMERATE;
    }

    void draw()
    {
        DrawTextureRec(m_texture, m_frame, m_position, WHITE);
    }

    void update()
    {
        m_currentFrameTime += GetFrameTime();

        if (m_currentFrameTime > m_frameTime)
        {
            m_currentFrameTime -= m_frameTime;

            if (++m_currentFrame > m_frames) m_currentFrame = 0;

            m_frame = {(float)m_texture.height*m_currentFrame, 0.0f, (float)m_texture.height, (float)m_texture.height};
        }

        if (m_currentFrame > m_frames) m_currentFrame = 0;

        return;
    }

    void move()
    {
        m_currentFrame++;
        if (m_currentFrame > m_frames) m_currentFrame = 0;
        m_frame = {(float)m_texture.height*m_currentFrame, 0.0f, (float)m_texture.height, (float)m_texture.height};

        return;
    }
};

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1000, 500, "template");
    SetTargetFPS(300);

    OutputTexture mainTexture(320, 180);

    // Rectangle sourceRect={0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height};
    // Rectangle destRect  ={-ratio, -ratio, screenWidth + (ratio*2), screenHeight + (ratio*2) };
    // Vector2 origin      ={0.0f, 0.0f};

    std::vector<Sprite> sprites;
    sprites.push_back(Sprite("resources/skellyAttack.png"));

    while (!WindowShouldClose())
    {
        // INPUT CHECK
        if (IsWindowResized())
        {
            SetWindowSize(GetScreenWidth(), GetScreenHeight());
        }

        // UPDATE
        for (unsigned int i{0}; i < sprites.size(); i++)
        {
            sprites[i].update();
        }

        // DRAW
        BeginTextureMode(mainTexture.getTexture());
            ClearBackground(GREEN);
            for (unsigned int i{0}; i < sprites.size(); i++)
            {
                sprites[i].draw();
            }

        EndTextureMode();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            // DrawTexturePro(target.texture, sourceRect, destRect, origin, 0.0f, WHITE);
            mainTexture.draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
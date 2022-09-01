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
    int m_ratio;

public:
    OutputTexture(int ratio)
    {
        m_ratio = ratio;
        unsigned int width = GetMonitorWidth(GetCurrentMonitor());
        unsigned int height = GetMonitorHeight(GetCurrentMonitor());
        m_texture = LoadRenderTexture(width / ratio, height / ratio);
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
        UnloadRenderTexture(m_texture);
        m_texture = LoadRenderTexture(5, 5);
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
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    const float ratio = 8;

    const int virtualWidth = screenWidth / ratio;
    const int virtualHeight = screenHeight / ratio;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "template");
    SetTargetFPS(300);

    ToggleFullscreen();

    RenderTexture2D target = LoadRenderTexture(virtualWidth, virtualHeight);

    OutputTexture mainTexture(8);

    Rectangle sourceRect={0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height};
    Rectangle destRect  ={-ratio, -ratio, screenWidth + (ratio*2), screenHeight + (ratio*2) };
    Vector2 origin      ={0.0f, 0.0f};

    std::vector<Sprite> sprites;
    sprites.push_back(Sprite("resources/skellyAttack.png"));

    while (!WindowShouldClose())
    {
        // INPUT CHECK
        if (IsKeyPressed(KEY_SPACE))
        {
            sprites[0].move();
        }

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
        BeginTextureMode(target);
            ClearBackground((Color){100, 255, 224, 255});
            for (unsigned int i{0}; i < sprites.size(); i++)
            {
                sprites[i].draw();
            }

        EndTextureMode();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexturePro(target.texture, sourceRect, destRect, origin, 0.0f, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
#include <platform/platform.hpp>
#include <string>
#include <glad/glad.h>

#include <iomanip>
#include <iostream>

Platform::~Platform()
{
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Platform::Initialize(const int &width, const int &height, const char *title)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Set OpenGL attributes before creating the window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    window = SDL_CreateWindow(
        title,
        width, height,
        flags);
    if (!window)
    {
        SDL_Log("Failed to Create SDL_Window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context)
    {
        SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_Log("Failed to initialize GLAD");
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // LOG OPENGL VERSION, VENDOR (IMPLEMENTATION), RENDERER, GLSL, ETC.:
    std::cout << std::setw(34) << std::left << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
    std::cout << std::setw(34) << std::left << "OpenGL Shading Language Version: " << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << std::setw(34) << std::left << "OpenGL Vendor:" << (char *)glGetString(GL_VENDOR) << std::endl;
    std::cout << std::setw(34) << std::left << "OpenGL Renderer:" << (char *)glGetString(GL_RENDERER) << std::endl;

    int maj;
    int min;

    glGetIntegerv(GL_MAJOR_VERSION, &maj);
    glGetIntegerv(GL_MINOR_VERSION, &min);
    SDL_Log("Context  : %d.%d", maj, min);

    running = true;
    lastTime = SDL_GetPerformanceCounter();
    return running;
}

bool Platform::ShouldClose() const
{
    return !running;
}

void Platform::PollEvents()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            running = false;
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            if (callback)
                callback(event.window.data1, event.window.data2); // width, height
            break;
        }
    }

    Uint64 currentTime = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();
    deltaTime = static_cast<float>(currentTime - lastTime) / static_cast<float>(freq);
    lastTime = currentTime;
}

void Platform::SwapBuffers()
{
    SDL_GL_SwapWindow(window);
}

void Platform::ListenEvent(const std::function<void(int, int)> &func)
{
    callback = func;
}

float Platform::GetDeltaTime() const
{
    return deltaTime;
}
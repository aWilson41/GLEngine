#include "App.h"

int main(int argc, char* argv[])
{
    // Start SDL2
    SDL_Init(SDL_INIT_EVERYTHING);

    // Initialize the engine and user code from app
    App engine;
    engine.EngineInitialize();
    engine.EngineLoad();

    // Main game loop
    SDL_Event e;
    while (engine.IsRunning())
    {
	    engine.Tick();
	    SDL_SetWindowTitle(engine.GetSDLWindow(), std::to_string(engine.GetFrameTime()).c_str());

	    // Clear Input
	    // previousKeyState = currentKeyState
	    // currentKeyState = Nothing
	    engine.PushKeyState();

	    // Fill currentKeyState
	    while (SDL_PollEvent(&e) != 0)
	    {
		    // Call input to notify
		    engine.Input(e);
		    if (e.type == SDL_QUIT)
		        engine.Quit();
	    }

	    // Return control to programmer for implementation
	    engine.EngineUpdate();     

	    // Draw a pretty picture
	    engine.EngineDraw();
    }

    engine.EngineUnLoad();

    SDL_Quit();

    return 0;
}


Engine::Engine() : 
    mRunning(true), fTime(0.0f), prevTime(0), currTime(0)
{
    UINT width = 1600;
    UINT height = 900;

    mWindow = SDL_CreateWindow("Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

    if (mWindow == NULL)
	    throw "Could not create SDL Window";

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    // Turn on double buffering with 24bit zbuffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    glContext = SDL_GL_CreateContext(mWindow);

    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

Engine::~Engine()
{
   
}


void Engine::EngineInitialize()
{
	Initialize();
}

// Virtual Method Implemented in App
void Engine::Initialize()
{

}


void Engine::EngineLoad()
{
    shaderProgram = glCreateProgram();
	Load();
}

// Virtual Method Implemented in App
void Engine::Load()
{

}


void Engine::Tick()
{
    prevTime = currTime;
    currTime = SDL_GetTicks();
    fTime = (float)(currTime - prevTime) * 0.001f;
}


void Engine::EngineUpdate()
{
    Update();
}

void Engine::Update()
{

}


void Engine::Input(SDL_Event e)
{
    switch (e.type)
    {
    case SDL_KEYDOWN:
	    KeyIsDown(e.key.keysym.sym);
	    break;

    case SDL_KEYUP:
	    KeyIsUp(e.key.keysym.sym);
	    break;  

    default:
	    break;
    }
}

inline void Engine::PushKeyState()
{
    // Put previousKeyState in currentKeyState
    memcpy(&mPreviousKeyState, mCurrentKeyState, INPUT_SIZE);
}


void Engine::EngineUnLoad()
{
    // Cleanup and Quit
    UnLoad();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(mWindow);
    glUseProgram(0);
    for (UINT i = 0; i < shaders.size(); i++)
    {
        glDetachShader(shaderProgram, shaders[i]);
    }

    glDeleteProgram(shaderProgram);

    for (UINT i = 0; i < shaders.size(); i++)
    {
        glDeleteShader(shaders[i]);
    }
    SDL_Quit();
}

// Virtual Method Implemented in App
void Engine::UnLoad()
{
	
}


void Engine::EngineDraw()
{
    // Display texture
    //glClearColor(0.5f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Prepare contents on screen buffer
    Draw();

    // Swap the buffer
    SDL_GL_SwapWindow(mWindow);
}

// Virtual Method Implemented in App
void Engine::Draw()
{

}
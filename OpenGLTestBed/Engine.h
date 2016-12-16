// Engine is an abstract class whose functionality lies mainly in App.
// App is the clean template and interface for the programmer while this
// Does "behind the scenes" work
#pragma once
#include <GL\glew.h>
#include <SDL.h>
#include "Scene.h"

const UINT INPUT_SIZE = 122;

class Engine
{
private:
   // The index of each element in the array represents the ascii value of the key pressed
   // This way I can garuntee input is always ordered and it doesn't require a search   
   bool mCurrentKeyState[INPUT_SIZE];
   bool mPreviousKeyState[INPUT_SIZE];
   SDL_Window* mWindow;
   bool mRunning;

private:
   // Posts the key
   void KeyIsDown(char key) { mCurrentKeyState[key] = true; }
   void KeyIsUp(char key) { mCurrentKeyState[key] = false; }


protected:
   SDL_GLContext glContext;
   std::vector<GLuint> shaders;
   float fTime;
   UINT prevTime;
   UINT currTime;

protected:
   // Gets the key
   bool IsCurrentKeyDown(char key) { return mCurrentKeyState[key]; }
   bool IsPreviousKeyDown(char key) { return mPreviousKeyState[key]; }


public:
   bool IsRunning() const { return mRunning; }
   void Quit() { mRunning = false; }
   float GetFrameTime() { return fTime; }
   SDL_Window* GetSDLWindow() { return mWindow; }

public:
   Engine();
   ~Engine();

   void Tick();

   void EngineInitialize();
   virtual void Initialize();

   void EngineLoad();
   virtual void Load();

   void EngineUpdate();
   virtual void Update();

   void Input(SDL_Event e);
   inline void PushKeyState();

   void EngineUnLoad();
   virtual void UnLoad();

   void EngineDraw();
   virtual void Draw(); 
};
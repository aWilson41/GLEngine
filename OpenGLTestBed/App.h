#pragma once
#include "Engine.h"

class App : public Engine
{
private:
   Scene scene;

protected:
   void Initialize() override;
   void UnLoad() override;
   void Load() override;
   void Update() override;
   void Draw() override;
};
#include "fman_window_manager.h"

// Main code
int main(int, char**)
{
    fman::WindowManager* wm = new fman::WindowManager();

    wm->setup_sdl("Dear ImGui SDL3+SDL_Renderer example", 1280, 720);
    wm->setup_imgui(NO_RESIZE | NO_MOVE | NO_COLLAPSE | NO_BRING_TO_FRONT);

    // Our state

    wm->setup_client();

    // Main loop
    while (!wm->isDone()){

        if(wm->handleEvents())
            continue;

        wm->update();
        wm->render();
    }

    // Cleanup
    wm->cleanup();

    return 0;
}

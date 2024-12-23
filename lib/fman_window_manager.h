#ifndef FMAN_WINDOW_MANAGER_H
#define FMAN_WINDOW_MANAGER_H

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h>
#include "fman_http.h"
#include "fman_paths.h"

#include <iostream>
#include <cstdint>

#define NO_TITEL_BAR (1 << 0)
#define NO_SCROLLBAR (1 << 1)
#define NO_MENU (1 << 2)
#define NO_MOVE (1 << 3)
#define NO_RESIZE (1 << 4) 
#define NO_COLLAPSE (1 << 5)
#define NO_CLOSE (1 << 6)
#define NO_NAV (1 << 7)
#define NO_BACKGROUND (1 << 8)
#define NO_BRING_TO_FRONT (1 << 9)
#define UNSAVED_DOCUMENT (1 << 10)

namespace fman {

    class WindowManager {
    public:
        SDL_Window* window;
        SDL_Renderer* renderer;

        Uint32 window_flags = 0;

        ImGuiWindowFlags imgui_flags = 0;
        ImVec4 clear_color;

        bool done = false;
        SDL_Event event;

        WindowManager();
        ~WindowManager();

        bool isDone();
        int handleEvents();

        int setup_sdl(const char* title, int width, int height);
        void setup_imgui(uint16_t imgui_flags = 0, ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f));
        void setup_client();
        
        
        void update();
        void render();


        int cleanup();

    private:
        fman::HttpRequest request;
        fman::HttpResponse response;
        fman::CurlClient* curl_client;

        ImFont* font;

        std::string no_res = "No response";

        uint16_t flags = 0;

        void flagUpdateImgui_();

    };
}



#endif
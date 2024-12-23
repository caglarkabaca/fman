#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <stdio.h>
#include <SDL3/SDL.h>
#include "fman_http.h"
#include "fman_paths.h"
// Main code
int main(int, char**)
{
    // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return -1;
    }

    // Create window with SDL_Renderer graphics context
    Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL3+SDL_Renderer example", 1280, 720, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (renderer == nullptr)
    {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFont* roboto_light = io.Fonts->AddFontFromFileTTF(ROBOTO_LIGHT_PATH, 16);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    std::string get_url = "\0";
    char responseBuffer[4096] = {0};
    char inputBuffer[128] = {0};
    fman::CurlClient *curl_client = fman::CurlClient::GetInstance();

    // Main loop
    bool done = false;
    while (!done){
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();


        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::PushFont(roboto_light);
            ImGui::Begin("API Client");

            // Ana pencereyi üç bölüme ayır
            const float tabWidth = 150.0f;
            const float contentPadding = 10.0f;
            float availWidth = ImGui::GetContentRegionAvail().x;
            float contentWidth = (availWidth - tabWidth - contentPadding) / 2;

            // Sol taraftaki tablar için
            ImGui::BeginChild("Tabs", ImVec2(tabWidth, 0), true);
            static int selected = 0;
            if (ImGui::Selectable("GET", selected == 0)) selected = 0;
            if (ImGui::Selectable("POST", selected == 1)) selected = 1;
            if (ImGui::Selectable("DELETE", selected == 2)) selected = 2;
            if (ImGui::Selectable("UPDATE", selected == 3)) selected = 3;
            ImGui::EndChild();

            ImGui::SameLine();

            // Orta kısım - Request alanı
            ImGui::BeginChild("Content", ImVec2(contentWidth, 0), true);

            // Her tab için ayrı input ve buton
            switch (selected) {
                case 0: // GET
                    ImGui::Text("GET Request");
                    ImGui::InputText("URL##get", &get_url);
                    if (ImGui::Button("Fetch##get")) {
                        curl_client->Get(get_url);
                    }
                    break;

                case 1: // POST
                    ImGui::Text("POST Request");
                    ImGui::InputTextMultiline("Data##post", &get_url,  ImVec2(-1, ImGui::GetWindowHeight() * 0.4f));
                    if (ImGui::Button("Fetch##post")) {
                        
                    }
                    break;

                case 2: // DELETE
                    ImGui::Text("DELETE Request");
                    ImGui::InputText("ID##delete", &get_url);
                    if (ImGui::Button("Fetch##delete")) {
                        
                    }
                    break;

                case 3: // UPDATE
                    ImGui::Text("UPDATE Request");
                    ImGui::InputTextMultiline("Data##update", &get_url, ImVec2(-1, ImGui::GetWindowHeight() * 0.4f));
                    if (ImGui::Button("Fetch##update")) {
                        
                    }
                    break;
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // Sağ kısım - Response alanı
            ImGui::BeginChild("Response", ImVec2(0, 0), true);
            ImGui::Text("Response");

            // Response başlık bilgileri
            static int statusCode = 200;
            static float responseTime = 0.0f;
            ImGui::Text("Status: %d", statusCode);
            ImGui::SameLine();
            ImGui::Text("Time: %.2f ms", responseTime);

            // Response içeriği
            ImGui::Separator();
            ImGui::BeginChild("ResponseContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
            ImGui::TextWrapped("%s", responseBuffer);
            ImGui::EndChild();

            // Clear button
            if (ImGui::Button("Clear Response", ImVec2(-1, 0))) {
                memset(responseBuffer, 0, sizeof(responseBuffer));
            }

            ImGui::EndChild();
            ImGui::End();
            ImGui::PopFont();
        }

        // Rendering
        ImGui::Render();
        SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#include "fman_window_manager.h"

fman::WindowManager::WindowManager() {
    
}

fman::WindowManager::~WindowManager() {
    // TODO: Implement destructor
}

bool fman::WindowManager::isDone()
{
    return done;
}

int fman::WindowManager::handleEvents()
{
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
        return 1;
    }
    return 0;
}

int fman::WindowManager::setup_sdl(const char* title, int width, int height) 
{   
    // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return -1;
    }

    // Create window with SDL_Renderer graphics contextconst char* title, int width, int height
    this->window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    window = SDL_CreateWindow(title, width, height, this->window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (renderer == nullptr)
    {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);
    return 0;
}

void fman::WindowManager::setup_imgui(uint16_t imgui_flags, ImVec4 clear_color) 
{

    this->flags = imgui_flags;
    flagUpdateImgui_();
    this->flags = 0;
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    font = io.Fonts->AddFontFromFileTTF(ROBOTO_REGULAR_PATH, 16);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
    
    this->clear_color = clear_color;

}

void fman::WindowManager::setup_client()
{
    curl_client = fman::CurlClient::GetInstance();
    request.url = "https://httpbin.org/anything";
    
}

void fman::WindowManager::update()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();


    ImVec2 viewport = ImVec2(io.DisplaySize.x, io.DisplaySize.y);
    ImVec2 pos = ImGui::GetMainViewport()->Pos;
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        ImGui::PushFont(font);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(viewport);

        ImGui::Begin("API Client", nullptr, imgui_flags);

        // Ana pencereyi üç bölüme ayır
        const float tabWidth = 100.0f;
        const float contentPadding = 10.0f;
        float availWidth = ImGui::GetContentRegionAvail().x;
        float contentWidth = (availWidth - tabWidth - contentPadding) / 2;
        static int selected = 0;
        {   // Sol taraftaki tablar için
            ImGui::BeginChild("Tabs", ImVec2(tabWidth, 0), true);
            
            if (ImGui::Selectable("GET", selected == 0)) selected = 0;
            if (ImGui::Selectable("POST", selected == 1)) selected = 1;
            if (ImGui::Selectable("DELETE", selected == 2)) selected = 2;
            if (ImGui::Selectable("PUT", selected == 3)) selected = 3;
            if (ImGui::Selectable("PATCH", selected == 4)) selected = 4;
            ImGui::EndChild();
        }
        ImGui::SameLine();
        {   // Orta kısım - Request alanı
            ImGui::BeginChild("Content", ImVec2(contentWidth, 0), true);
            // Her tab için ayrı input ve buton
            switch (selected) {
                case 0: // GET
                    ImGui::Text("GET Request");
                    ImGui::InputText("URL##get", &request.url);
                    ImGui::InputTextMultiline("Data##get", &request.body,  ImVec2(ImGui::GetContentRegionAvail().x * 0.8f, ImGui::GetWindowHeight() * 0.4f));
                    if (ImGui::Button("Fetch##get")) {
                        request.method = fman::HttpMethod::GET;
                        curl_client->Request(request, response);
                    }
                    break;
                case 1: // POST
                    ImGui::Text("POST Request");
                    ImGui::InputText("URL##post", &request.url);
                    ImGui::InputTextMultiline("Data##post", &request.body,  ImVec2(ImGui::GetContentRegionAvail().x * 0.8f, ImGui::GetWindowHeight() * 0.4f));
                    if (ImGui::Button("Fetch##post")) {
                        request.method = fman::HttpMethod::POST;
                        curl_client->Request(request, response);
                    }
                    break;
                case 2: // DELETE
                    ImGui::Text("DELETE Request");
                    ImGui::InputText("ID##delete", &request.url);
                    if (ImGui::Button("Fetch##delete")) {
                        // curl
                    }
                    break;
                case 3: // PUT
                    ImGui::Text("PUT Request");
                    ImGui::InputTextMultiline("Data##put", &request.url, ImVec2(ImGui::GetContentRegionAvail().x * 0.8f, ImGui::GetWindowHeight() * 0.4f));
                    if (ImGui::Button("Fetch##put")) {
                        // curl
                    }
                    break;
                case 4: // PATCH
                    ImGui::Text("PATCH Request");
                ImGui::InputTextMultiline("Data##patch", &request.url, ImVec2(ImGui::GetContentRegionAvail().x * 0.8f, ImGui::GetWindowHeight() * 0.4f));
                if (ImGui::Button("Fetch##patch")) {
                    // curl
                }
                break;
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();
        {
            // Sağ kısım - Response alanı
            ImGui::BeginChild("Response", ImVec2(0, 0), true);
            ImGui::Text("Response");
            // Response başlık bilgileri
            ImGui::Text("Status: %d", response.code.value_or(0));
            ImGui::SameLine();
            ImGui::Text("Time: %.2f ms", response.time_took);
            // Response içeriği
            ImGui::Separator();
            ImGui::BeginChild("ResponseContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
            ImGui::InputTextMultiline("##", (response.data.has_value()) ? &response.data.value() : &no_res,
                                      ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_ReadOnly);
            ImGui::EndChild();
        }
        // Clear button
        if (ImGui::Button("Clear Response", ImVec2(-1, 0))) {
            response = fman::HttpResponse();
        }
        ImGui::EndChild();
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopFont();
    }
}

void fman::WindowManager::render()
{
    ImGui::Render();
    SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
}

int fman::WindowManager::cleanup() 
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void fman::WindowManager::flagUpdateImgui_() 
{
    if (flags & NO_TITEL_BAR)       imgui_flags |= ImGuiWindowFlags_NoTitleBar;
    if (flags & NO_SCROLLBAR)       imgui_flags |= ImGuiWindowFlags_NoScrollbar;
    if (flags & NO_MENU)            imgui_flags |= ImGuiWindowFlags_MenuBar;
    if (flags & NO_MOVE)            imgui_flags |= ImGuiWindowFlags_NoMove;
    if (flags & NO_RESIZE)          imgui_flags |= ImGuiWindowFlags_NoResize;
    if (flags & NO_COLLAPSE)        imgui_flags |= ImGuiWindowFlags_NoCollapse;
    if (flags & NO_CLOSE)           imgui_flags |= ImGuiWindowFlags_NoMove;
    if (flags & NO_NAV)             imgui_flags |= ImGuiWindowFlags_NoNav;
    if (flags & NO_BACKGROUND)      imgui_flags |= ImGuiWindowFlags_NoBackground;
    if (flags & NO_BRING_TO_FRONT)  imgui_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    //if (flags & NO_DOCKING)       imgui_flags |= ImGuiWindowFlags_NoDocking;
    if (flags & UNSAVED_DOCUMENT)   imgui_flags |= ImGuiWindowFlags_UnsavedDocument;
    
}

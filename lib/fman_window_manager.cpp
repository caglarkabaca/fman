#include "fman_window_manager.h"

fman::WindowManager::WindowManager(uint16_t flags): flags(flags) {
    flagUpdate_();
}

fman::WindowManager::~WindowManager() {
    // TODO: Implement destructor
}

void fman::WindowManager::update() {
    //
}

void fman::WindowManager::flagUpdate_() {
    if (flags & NO_TITEL_BAR)       window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (flags & NO_SCROLLBAR)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (flags & NO_MENU)            window_flags |= ImGuiWindowFlags_MenuBar;
    if (flags & NO_MOVE)            window_flags |= ImGuiWindowFlags_NoMove;
    if (flags & NO_RESIZE)          window_flags |= ImGuiWindowFlags_NoResize;
    if (flags & NO_COLLAPSE)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (flags & NO_NAV)             window_flags |= ImGuiWindowFlags_NoNav;
    if (flags & NO_BACKGROUND)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (flags & NO_BRING_TO_FRONT)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (flags & UNSAVED_DOCUMENT)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
    
}
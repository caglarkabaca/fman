#ifndef FMAN_WINDOW_MANAGER_H
#define FMAN_WINDOW_MANAGER_H

#include "imgui.h"

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
        ImGuiWindowFlags window_flags = 0;

        WindowManager(uint16_t flags) : flags(flags) {
            flagUpdate_();
        }
        ~WindowManager();

        void update() {

        }

    private:
        uint16_t flags = 0;

        void flagUpdate_() {
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
    };

}



#endif
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

        WindowManager(uint16_t flags);
        ~WindowManager();

        void update();

    private:
        uint16_t flags = 0;

        void flagUpdate_();

    };
}



#endif
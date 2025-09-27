#pragma once

#include "menu/menuItem.h"
#include "menu/menuUtils.h"
#include <vector.h>

struct Menu {
    MenuPage* page; // Pointer to the currently rendering page
    bool active;    // Flag to indicate if the menu is active

    Menu() : page(NULL), active(false) {};
    void update(); // Function to update the menu (e.g., handle input, navigate items, etc.)
    void render(); // Function to render the menu on the screen
    void enable() { active = true; };
    void disable() { active = false; };
    void toggle() { active = !active; };
    bool isActive() { return active; };
};

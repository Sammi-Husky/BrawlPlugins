#pragma once

#include "menu/menuItem.h"
#include "menu/menuUtils.h"
#include <vector.h>

struct Menu {
    MenuPage* page; // Pointer to the currently rendering page

    Menu() : page(NULL) // Constructor to initialize the menu
    {
    }
    void update(); // Function to update the menu (e.g., handle input, navigate items, etc.)
    void render(); // Function to render the menu on the screen
};

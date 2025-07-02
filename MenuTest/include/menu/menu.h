#pragma once

#include "menu/menuItem.h"
#include <vector.h>

struct Menu {
    const char* title;           // Title of the menu
    const char* description;     // Description of the menu
    Vector<MenuItemBase*> items; // Array of menu items
    int selectedItem;            // Index of the currently selected item
    Menu* parentMenu;            // Pointer to the parent menu (if any)

    Menu() : title(NULL), description(NULL), selectedItem(0), parentMenu(NULL) // Constructor to initialize the menu
    {
    }
    void update();                                         // Function to update the menu (e.g., handle input, navigate items, etc.)
    void render();                                         // Function to render the menu on the screen
    void addItem(MenuItemBase* item) { items.push(item); } // Function to add a menu item
    int getNumItems() { return items.size(); }
};

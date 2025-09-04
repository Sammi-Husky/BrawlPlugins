#pragma once

#include "menu/menuItem.h"

class Menu;
class BoolMenuItem : public MenuItem<bool> {
public:
    BoolMenuItem(const char* name, bool* value, const char* description) : MenuItem<bool>(value)
    {
        this->name = name;
        this->description = description;
    }
    BoolMenuItem(const char* name, bool* value) : MenuItem<bool>(value) // Constructor to initialize the menu item
    {
        BoolMenuItem(name, value);
    }

    virtual void render(nw4r::ut::TextWriterBase<char>& writer); // Render function for the boolean menu item
    virtual void handleInput(gfPadStatus& status);               // Handle input for the boolean menu item
};
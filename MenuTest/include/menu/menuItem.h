#pragma once

#include <gf/gf_pad_status.h>
#include <nw4r/ut/TextWriterBase.h>
#include <stddef.h>
#include <types.h>

struct MenuItemBase {
    const char* name;        // Name of the menu item
    const char* description; // Description of the menu item
    bool isSelected;         // Flag to indicate if the item is currently selected

    MenuItemBase()
    {
        name = NULL;        // Initialize name to NULL
        description = NULL; // Initialize description to NULL
        isSelected = false; // Initialize isSelected to false
    }

    virtual void render(nw4r::ut::TextWriterBase<char>& writer) = 0;
    virtual void handleInput(gfPadStatus& status) = 0; // Handle input for the menu item
};

template <typename T>
struct MenuItem : public MenuItemBase {
    T* value; // Pointer to the value associated with the menu item

    MenuItem(T* value) : MenuItemBase()
    {
        this->value = value;
    }
    virtual void render(nw4r::ut::TextWriterBase<char>& writer) = 0; // Render function for the menu item
    virtual void handleInput(gfPadStatus& status) = 0;               // Handle input for the menu item
    virtual void set(T* value) { this->value = value; };             // Set function to assign the value pointer
    virtual void set(T value) { *this->value = value; };             // Set function to assign the value pointer
};

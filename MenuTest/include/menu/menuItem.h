#pragma once

#include <gf/gf_pad_status.h>
#include <nw4r/ut/TextWriterBase.h>
#include <stddef.h>
#include <types.h>
#include <vector.h>

template <typename T>
class Vector;

struct Menu;

struct MenuItemBase {
    Menu* menu;
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
    virtual void setDescription(const char* description) { this->description = description; };
    virtual void setName(const char* name) { this->name = name; };
    virtual void setMenu(Menu* menu) { this->menu = menu; };
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

struct MenuPage : public MenuItemBase {
    MenuPage* parent;
    Vector<MenuItemBase*> items;
    const char* title;
    const char* footer;
    int selectedItem;

    MenuPage(Menu* menu);
    /**
     * @brief Renders the menu page.
     *
     * @param writer The text writer to use for rendering
     */
    virtual void render(nw4r::ut::TextWriterBase<char>& writer);
    /**
     * @brief Handles input for the menu page.
     *
     * @param status The current pad status to process inputs from
     */
    virtual void handleInput(gfPadStatus& status);
    /**
     * @brief Sets the title of the menu page. This title will be
     * Rendered at the top of the page.
     *
     * @param title The title to set for the page
     */
    virtual void setTitle(const char* title);
    /**
     * Sets the footer text to be rendered at the bottom of the page
     *
     * @param footer The text to set the footer to
     */
    virtual void setFooter(const char* footer);
    /**
     * Adds a new item to the page
     *
     * @param item The item to add
     */
    virtual void addItem(MenuItemBase* item);
    /**
     * Creates a new MenuPage with this one set as the parent
     */
    virtual MenuPage* createSubMenu();
};

struct MenuItemSubMenu : public MenuItemBase {
    MenuPage* subMenu;
    MenuItemSubMenu(const char* name, MenuPage* subMenu, const char* description);
    MenuItemSubMenu(const char* name, MenuPage* subMenu);

    virtual void render(nw4r::ut::TextWriterBase<char>& writer);
    virtual void handleInput(gfPadStatus& status);
};
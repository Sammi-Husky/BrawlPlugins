#include "menu/menuItem.h"
#include "menu/menu.h"

#include <snd/snd_system.h>

void MenuItemBase::render(nw4r::ut::TextWriterBase<char>& writer)
{
    // Render the menu item using the provided writer
    if (isSelected)
    {
        writer.SetTextColor(nw4r::ut::Color::YELLOW);
    }
    else
    {
        writer.SetTextColor(nw4r::ut::Color::WHITE);
    }
}

MenuItemSubMenu::MenuItemSubMenu(const char* name, MenuPage* subMenu, const char* description) : MenuItemBase(), subMenu(subMenu)
{
    this->name = name;
    this->description = description;
}
MenuItemSubMenu::MenuItemSubMenu(const char* name, MenuPage* subMenu) : MenuItemBase(), subMenu(subMenu)
{
    MenuItemSubMenu::MenuItemSubMenu(name, subMenu, NULL);
}
void MenuItemSubMenu::render(nw4r::ut::TextWriterBase<char>& writer)
{
    // Handles base functionality like selection color
    MenuItemBase::render(writer);

    Printf(writer, "%s >\n", name);
}

void MenuItemSubMenu::handleInput(gfPadStatus& status)
{
    // If the item is selected and the A button is pressed, navigate to the submenu
    if (isSelected && (status.m_buttonsPressedThisFrame.bits & GF_PAD_BUTTON_BIT_A))
    {
        g_sndSystem->playSE(snd_se_system_Page_Change, -1, 0, 0, -1);
        this->menu->page = subMenu; // Set the current page to the submenu
    }
}

MenuPage::MenuPage(Menu* menu) : MenuItemBase(), parent(NULL), title(NULL), footer(NULL), selectedItem(0)
{
    setMenu(menu);
}

void MenuPage::render(nw4r::ut::TextWriterBase<char>& writer)
{
    // Reset the cursor
    writer.SetCursor(3, 3);

    // Render the page title
    if (title != NULL)
    {
        writer.SetTextColor(nw4r::ut::Color::GREEN);
        Printf(writer, "%s\n", title);
    }

    if (description != NULL)
    {
        writer.SetTextColor(nw4r::ut::Color::GREEN);
        Printf(writer, "%s\n", description);
    }

    if (description != NULL || title != NULL)
    {
        writer.Print("\n");
    }

    // Render each menu item
    int rowStart = writer.GetCursorY();
    int itemCount = items.size();
    int col = 0;
    for (int i = 0; i < itemCount; i++)
    {
        if (i % 10 == 0 && i > 0)
        {
            col++;
            writer.SetCursor(3 + (col * 200), rowStart);
        }

        if (items[i] == NULL)
            continue;

        items[i]->render(writer); // Render each menu item
    }

    // Render the footer if it exists
    if (footer != NULL)
    {
        writer.SetTextColor(nw4r::ut::Color::GRAY);
        Printf(writer, "\n%s\n", footer);
    }

    if (items[selectedItem] != NULL && items[selectedItem]->description != NULL)
    {
        writer.SetTextColor(nw4r::ut::Color::MAGENTA);
        Printf(writer, "\n%s\n", items[selectedItem]->description);
    }
}

void MenuPage::handleInput(gfPadStatus& status)
{
    int itemCount = items.size();
    if (status.m_buttonsPressedThisFrame.m_dpadUp)
    {
        g_sndSystem->playSE(snd_se_system_cursor, -1, 0, 0, -1);
        selectedItem = (selectedItem - 1 + itemCount) % itemCount;
    }
    else if (status.m_buttonsPressedThisFrame.m_dpadDown)
    {
        g_sndSystem->playSE(snd_se_system_cursor, -1, 0, 0, -1);
        selectedItem = (selectedItem + 1) % itemCount;
    }
    else if (status.m_buttonsPressedThisFrame.m_b)
    {
        if (this->parent != NULL)
        {
            g_sndSystem->playSE(SND_SE_SYSTEM_CURSOR_L, -1, 0, 0, -1);
            this->menu->page = this->parent; // Go back to parent if it exists
            return;
        }
    }

    // Update the selection state of each item
    for (int i = 0; i < itemCount; i++)
    {
        if (items[i] != NULL)
        {
            items[i]->isSelected = (i == selectedItem); // Update selection state
        }
    }

    items[selectedItem]->handleInput(status);
}

void MenuPage::addItem(MenuItemBase* item)
{
    item->setMenu(this->menu);
    items.push(item);
}

void MenuPage::setTitle(const char* title)
{
    this->title = title;
}

void MenuPage::setFooter(const char* footer)
{
    this->footer = footer;
}
MenuPage* MenuPage::createSubMenu()
{
    MenuPage* subMenu = new MenuPage(menu);
    subMenu->parent = this;
    return subMenu;
}
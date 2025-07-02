#include "menu/menuItem.h"

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
#include "menu/options/boolOption.h"
#include "menu/menuUtils.h"
#include <gf/gf_pad_status.h>

void BoolMenuItem::render(nw4r::ut::TextWriterBase<char>& writer)
{
    MenuItemBase::render(writer); // Call the base class render function

    // Print the name and value of the boolean menu item
    Printf(writer, "%s: %s\n", name, *value ? "true" : "false");
}

void BoolMenuItem::handleInput(gfPadStatus& status)
{
    // Handle input for the boolean menu item
    if (status.m_buttonsPressedThisFrame.m_dpadLeft)
    {
        set(false);
    }
    else if (status.m_buttonsPressedThisFrame.m_dpadRight)
    {
        set(true); // Set value to true if right button is pressed
    }
}
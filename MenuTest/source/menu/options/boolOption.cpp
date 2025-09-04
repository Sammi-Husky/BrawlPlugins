#include "menu/options/boolOption.h"
#include "menu/menuUtils.h"
#include <gf/gf_pad_status.h>
#include <snd/snd_system.h>

void BoolMenuItem::render(nw4r::ut::TextWriterBase<char>& writer)
{
    MenuItemBase::render(writer); // Call the base class render function

    // Print the name and value of the boolean menu item
    Printf(writer, "%s: %s\n", name, *value ? "true" : "false");
}

void BoolMenuItem::handleInput(gfPadStatus& status)
{
    // Handle input for the boolean menu item
    if (status.m_buttonsPressedThisFrame.m_dpadLeft || status.m_buttonsPressedThisFrame.m_dpadRight)
    {
        g_sndSystem->playSE(snd_se_system_Counter, -1, 0, 0, -1);
        set(!*value);
    }
}
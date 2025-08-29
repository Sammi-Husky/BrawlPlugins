#include "sandbox.h"
#include "menu/menu.h"
#include "menu/options/boolOption.h"

#include <OS/OSError.h>
#include <gf/gf_heap_manager.h>
#include <gf/gf_pad_status.h>
#include <gf/gf_pad_system.h>
#include <hook.hpp>
#include <plugin.hpp>
#include <sy_core.hpp>

using namespace SyringeCore;
namespace Sandbox {
    static Menu* mainMenu = NULL; // Pointer to the main menu

    static bool menu_enable = false;
    void updateMenuEnabled()
    {
        static gfPadStatus status;
        g_gfPadSystem->getSysPadStatus(0, &status);

        int mask = GF_PAD_BUTTON_BIT_L | GF_PAD_BUTTON_BIT_R | GF_PAD_BUTTON_BIT_Y | GF_PAD_BUTTON_BIT_DPAD_UP; // Mask for A and X buttons

        // Don't want to toggle if all are in the hold state
        // This is to prevent the menu from being toggled when the buttons are held down
        if ((status.m_buttonsHeld.bits & mask) == mask)
            return;

        // Check if the L, R, Y, and DPad Up buttons are pressed and the last button in the combo was pressed this frame
        u32 bits = status.m_buttonsHeld.bits | status.m_buttonsPressedThisFrame.bits;
        if ((bits & mask) == mask)
        {
            menu_enable = !menu_enable; // Toggle the menu on/off
        }
    }

    class gfTaskScheduler;
    typedef void (*renderPreFN)(gfTaskScheduler* scheduler);
    static renderPreFN _renderPre;
    void renderPre(gfTaskScheduler* scheduler)
    {
        _renderPre(scheduler);

        // Enables the menu if the button combination is pressed
        updateMenuEnabled();

        // Pause the game logic if the menu is enabled
        *(((char*)scheduler) + 0xb) = menu_enable;

        if (menu_enable)
        {
            mainMenu->update(); // Update the main menu (input handling, etc.)
            mainMenu->render(); // Render the main menu if enabled
        }
    }

    void testAction()
    {
        // Action to be performed when the action item is selected
        // For example, you can print a message or perform some action here
        OSReport("Test Action Triggered!\n");
    }

    static bool testBool = false;
    static bool testBool2 = false;
    void Init(Plugin* plg)
    {
        Hook* hook = plg->addHookEx(0x8002e79c, renderPre, OPT_DIRECT);
        hook->getTrampoline(reinterpret_cast<void**>(_renderPre));

        mainMenu = new (Heaps::Syringe) Menu;
        mainMenu->title = "Main Menu";
        mainMenu->description = "This is the main menu";

        mainMenu->addItem(new BoolMenuItem("Test Option 1", &testBool));
        mainMenu->addItem(new BoolMenuItem("Test Option 2", &testBool2));
    }
}

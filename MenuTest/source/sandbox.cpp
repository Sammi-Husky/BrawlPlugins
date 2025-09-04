#include "sandbox.h"
#include "menu/menu.h"
#include "menu/options/boolOption.h"

#include <OS/OSError.h>
#include <VI/vi.h>
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
    /**
     * @brief
     * Updates the menu enabled state based on the current button status.
     *
     * @returns whether this function processed the current inputs
     */
    bool updateMenuEnabled()
    {
        static gfPadStatus status;
        g_gfPadSystem->getSysPadStatus(0, &status);

        int mask = GF_PAD_BUTTON_BIT_L | GF_PAD_BUTTON_BIT_R | GF_PAD_BUTTON_BIT_Y | GF_PAD_BUTTON_BIT_DPAD_UP; // Mask for A and X buttons

        // Don't want to toggle if all are in the hold state
        // This is to prevent the menu from being toggled when the buttons are held down
        if ((status.m_buttonsHeld.bits & mask) == mask)
            return 0;

        // Check if the L, R, Y, and DPad Up buttons are pressed and the last button in the combo was pressed this frame
        u32 bits = status.m_buttonsHeld.bits | status.m_buttonsPressedThisFrame.bits;
        if ((bits & mask) == mask)
        {
            menu_enable = !menu_enable; // Toggle the menu on/off
            return 1;
        }

        return 0;
    }

    class gfTaskScheduler;
    typedef void (*renderPreFN)(gfTaskScheduler* scheduler);
    static renderPreFN _renderPre;
    void renderPre(gfTaskScheduler* scheduler)
    {
        _renderPre(scheduler);

        // Enables the menu if the button combination is pressed
        // If the menu was enabled this frame, the input has been handled
        // and we should return to prevent inputs from being processed
        // further down the chain
        if (updateMenuEnabled())
            return;

        // Pause the game logic if the menu is enabled
        *(((char*)scheduler) + 0xb) = menu_enable;

        if (menu_enable)
        {
            mainMenu->update(); // Update the main menu (input handling, etc.)
            mainMenu->render(); // Render the main menu if enabled
        }
    }

    static bool testBool = false;
    static bool testBool2 = false;
    void Init(Plugin* plg)
    {
        Hook* hook = plg->addHookEx(0x8002e79c, renderPre, OPT_DIRECT);
        hook->getTrampoline(reinterpret_cast<void**>(&_renderPre));

        mainMenu = new (Heaps::Syringe) Menu;

        MenuPage* homePage = new (Heaps::Syringe) MenuPage(mainMenu);
        MenuPage* subMenu = homePage->createSubMenu();

        // Home page
        {
            homePage->setTitle("Main Menu");
            homePage->setDescription("This is the main menu");
            homePage->setFooter("Use D-Pad Up/Down to navigate, Left/Right to change options");
            homePage->addItem(new BoolMenuItem("Test Option 1", &testBool, "This is a test boolean option 1"));
            homePage->addItem(new BoolMenuItem("Test Option 2", &testBool2, "This is a test boolean option 2"));
            homePage->addItem(new MenuItemSubMenu("Test Sub Menu", subMenu, "Press A to enter the submenu"));
            homePage->addItem(new BoolMenuItem("Test Option 3", &testBool2, "This is a test boolean option 3"));
            homePage->addItem(new BoolMenuItem("Test Option 4", &testBool2, "This is a test boolean option 4"));
        }

        // Sub Menu
        {
            subMenu->setTitle("Sub Menu");
            subMenu->setDescription("This is the sub menu");
            subMenu->setFooter("Use D-Pad Up/Down to navigate, Left/Right to change options\nPress B to return to the previous menu");
            subMenu->addItem(new BoolMenuItem("Sub Option 1", &testBool, "This is a test sub option 1"));
            subMenu->addItem(new BoolMenuItem("Sub Option 2", &testBool2, "This is a test sub option 2"));
            subMenu->addItem(new BoolMenuItem("Sub Option 3", &testBool2, "This is a test sub option 3"));
            subMenu->addItem(new BoolMenuItem("Sub Option 4", &testBool2, "This is a test sub option 4"));
            subMenu->addItem(new BoolMenuItem("Sub Option 5", &testBool2, "This is a test sub option 5"));
        }

        mainMenu->page = homePage;
    }
}

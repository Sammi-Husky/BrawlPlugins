#include "menu/menu.h"

#include <gf/gf_heap_manager.h>
#include <gf/gf_pad_status.h>
#include <gf/gf_pad_system.h>
#include <ms/ms_resfont.h>
#include <nw4r/ut/RomFont.h>
#include <nw4r/ut/TextWriterBase.h>

extern void* gfFramebuff;
void* getNextRenderableXFB(void* gfFramebuffer);
void setGx2d()
{
    // Set up GX for 2D rendering
    Mtx44 projection;
    Mtx mv;
    GXSetScissor(0, 0, 640, 480);
    GXSetViewport(0, 0, 640, 480, 0, 1);
    GXSetScissorBoxOffset(0, 0);
    C_MTXOrtho(projection, 0, 480, 0, 640, 0, 1);
    GXSetProjection(projection, GX_ORTHOGRAPHIC);
    PSMTXIdentity(mv);
    GXLoadPosMtxImm(mv, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    void* framebuffer = getNextRenderableXFB(gfFramebuff);
    GXColor black = { 0, 0, 0, 0 };
    GXSetCopyClear(black, -1);
    GXCopyDisp(framebuffer, true);
}

void Menu::update()
{
    // Handle input and update menu state
    gfPadStatus status;
    g_gfPadSystem->getSysPadStatus(0, &status);

    int itemCount = getNumItems();
    if (status.m_buttonsPressedThisFrame.m_dpadUp)
    {
        selectedItem = (selectedItem - 1 + itemCount) % itemCount;
    }
    else if (status.m_buttonsPressedThisFrame.m_dpadDown)
    {
        selectedItem = (selectedItem + 1) % itemCount;
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

void Menu::render()
{
    nw4r::ut::TextWriterBase<char> writer;

    // setup gx for our menu
    setGx2d();

    nw4r::ut::Font& font = *FontData::getFontResource(FontData::FONT_MELEE);

    // init writer
    writer.SetFont(font);
    writer.SetupGX();
    writer.SetCursor(3, 3);
    writer.SetScale(.7f);
    writer.SetTextColor(nw4r::ut::Color::WHITE);

    int itemCount = getNumItems();
    for (int i = 0; i < itemCount; i++)
    {
        if (items[i] == NULL)
            continue;

        items[i]->render(writer); // Render each menu item
    }
}
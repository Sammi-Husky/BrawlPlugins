#include "sel_char_load_thread.h"
#include <OS/OSCache.h>
#include <VI/vi.h>
#include <gf/gf_heap_manager.h>
#include <memory.h>
#include <mu/menu.h>
#include <cstdio>

selCharLoadThread::selCharLoadThread(muSelCharPlayerArea* area)
{
    m_toLoad = -1;
    m_playerArea = area;
    m_dataReady = false;
    m_isRunning = false;
    m_shouldExit = false;

    m_buffer = gfHeapManager::alloc(Heaps::MenuResource, 0x40000);

    OSCreateThread(&m_thread, selCharLoadThread::main, this, m_stack + sizeof(m_stack), sizeof(m_stack), 31, 0);
}

void* selCharLoadThread::main(void* arg)
{
    selCharLoadThread* thread = static_cast<selCharLoadThread*>(arg);
    muSelCharPlayerArea* area = thread->m_playerArea;
    const char* format = "/menu/common/char_bust_tex/MenSelchrFaceB%02d0.brres";
    char filepath[0x34];
    int charKind;

    while (!thread->m_shouldExit)
    {
        if (thread->m_toLoad != -1)
        {
            charKind = thread->m_toLoad;

            // If read is already in progress, cancel it and start new read request
            if (thread->m_isRunning)
            {
                thread->reset();
            }

            // Handles conversions for poketrio and special slots
            int id = muMenu::exchangeMuSelchkind2MuStockchkind(charKind);
            id = muMenu::getStockFrameID(id);

            sprintf(filepath, format, id);

            // Start the read process
            thread->m_handle.readRequest(filepath, thread->m_buffer, 0, 0);

            // Clear read request and signal that read is in progress
            thread->m_toLoad = -1;
            thread->m_isRunning = true;
            thread->m_dataReady = false;
            continue;
        }

        // Data is finished loading
        if (thread->m_isRunning && thread->m_handle.isReady())
        {
            thread->m_dataReady = true;
            thread->m_isRunning = false;

            area->setCharPic(charKind,
                             area->playerKind,
                             area->curCostume,
                             area->isTeamBattle(),
                             area->teamColor,
                             area->curTeamSet);
        }
    }

    return NULL;
}

void selCharLoadThread::start()
{
    OSResumeThread(&m_thread);
}

void selCharLoadThread::requestLoad(int charKind)
{
    m_toLoad = charKind;
    m_dataReady = false;
}

void selCharLoadThread::reset()
{
    if (m_isRunning)
    {
        m_handle.cancelRequest();
        m_isRunning = false;
    }

    m_dataReady = false;
    m_toLoad = -1;
}

selCharLoadThread::~selCharLoadThread()
{
    exit();
    free(m_buffer);
    m_handle.release();
}
#include "sel_char_load_thread.h"
#include <VI/vi.h>
#include <memory.h>
#include <mu/mu_menu.h>
#include <printf.h>

void* selCharLoadThread::main(void* arg)
{
    selCharLoadThread* thread = static_cast<selCharLoadThread*>(arg);
    muSelCharPlayerArea* area = thread->m_playerArea;
    const char* format = "/menu/common/char_bust_tex/MenSelchrFaceB%02d0.brres";
    char filepath[0x34];
    int charKind;

    while (!thread->m_shouldExit)
    {
        OSLockMutex(&thread->m_mutex);
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
            thread->m_handle.readRequest(filepath, static_cast<void*>(area->charPicData), 0, 0);

            // Clear read request and signal that read is in progress
            thread->m_toLoad = -1;
            thread->m_isRunning = true;
            thread->m_dataReady = false;
        }

        // Data is finished loading
        if (thread->m_handle.isReady() && thread->m_isRunning)
        {
            thread->m_dataReady = true;
            thread->m_isRunning = false;

            area->setCharPic(area->selectedChar,
                             area->playerKind,
                             area->curCostume,
                             area->isTeamBattle(),
                             area->teamColor,
                             area->curTeamSet);

            thread->m_handle.release();
        }
        OSUnlockMutex(&thread->m_mutex);
    }

    return NULL;
}

selCharLoadThread::selCharLoadThread(muSelCharPlayerArea* area)
{
    m_toLoad = -1;
    m_playerArea = area;
    m_dataReady = false;
    m_isRunning = false;
    m_shouldExit = false;
    // m_dataBuffer = new (Heaps::MenuResource) char[0x40000];
    OSInitMutex(&m_mutex);

    OSCreateThread(&m_thread, selCharLoadThread::main, this, m_stack + sizeof(m_stack), sizeof(m_stack), 31, 0);
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
        m_handle.release();
        m_isRunning = false;
    }

    m_dataReady = false;
    m_toLoad = -1;
}

selCharLoadThread::~selCharLoadThread()
{
    m_shouldExit = true;
    OSJoinThread(&m_thread, NULL);
    this->m_handle.release();
    // free(this->m_dataBuffer);
}
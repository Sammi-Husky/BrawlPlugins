#pragma once

#include <OS/OSMutex.h>
#include <OS/OSThread.h>
#include <gf/gf_file_io_handle.h>
#include <mu/mu_selchar_player_area.h>

class selCharLoadThread {
protected:
    OSThread m_thread;
    char m_stack[0x1000];
    gfFileIOHandle m_handle;
    muSelCharPlayerArea* m_playerArea;
    bool m_shouldExit;
    void* m_buffer;
    int m_toLoad;
    bool m_dataReady;
    bool m_isRunning;

public:
    selCharLoadThread(muSelCharPlayerArea* area);
    void requestLoad(int charKind);
    static void* main(void* arg);
    void start();
    void reset();
    void* getBuffer() { return m_buffer; }
    bool isRunning() { return m_isRunning; }
    bool isReady() { return m_dataReady; }
    void exit()
    {
        m_shouldExit = true;
        OSJoinThread(&m_thread, NULL);
    }

    ~selCharLoadThread();
};
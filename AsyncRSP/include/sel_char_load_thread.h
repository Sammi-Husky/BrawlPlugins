#pragma once

#include <OS/OSMutex.h>
#include <OS/OSThread.h>
#include <gf/gf_file_io_handle.h>
#include <mu/mu_sel_char_player_area.h>

class selCharLoadThread {
protected:
    OSThread m_thread;
    char m_stack[0x1000];
    gfFileIOHandle m_handle;
    muSelCharPlayerArea* m_playerArea;
    int m_toLoad;
    bool m_shouldExit;

public:
    OSMutex m_mutex;
    bool m_isRunning;
    bool m_dataReady;
    // void* m_dataBuffer;

    selCharLoadThread(muSelCharPlayerArea* area);
    void requestLoad(int charKind);
    static void* main(void* arg);
    void start();
    void reset();

    ~selCharLoadThread();
};
#include <CX.h>
#include <OS/OSCache.h>
#include <gf/gf_archive.h>
#include <memory.h>
#include <modules.h>
#include <mu/menu.h>
#include <mu/selchar/mu_selchar_player_area.h>
#include <nw4r/g3d/g3d_resfile.h>
#include <sy_core.h>
#include <types.h>
#include <vector.h>

#include "sel_char_load_thread.h"

using namespace nw4r::g3d;

namespace CSSHooks {

    extern gfArchive* selCharArchive;
    selCharLoadThread* threads[4];

    void createThreads(muSelCharPlayerArea* area)
    {
        selCharLoadThread* thread = new (Heaps::Thread) selCharLoadThread(area);
        threads[area->m_areaIdx] = thread;
        thread->start();
    }

    // NOTE: This hook gets triggered again by the load thread since
    // the thread calls `setCharPic` when data is finished loading
    ResFile* getCharPicTexResFile(register muSelCharPlayerArea* area, u32 charKind)
    {
        selCharLoadThread* thread = threads[area->m_areaIdx];

        // Handles conversions for poketrio and special slots
        int id = muMenu::exchangeMuSelchkind2MuStockchkind(charKind);
        id = muMenu::getStockFrameID(id);

        // check if CSP exists in archive first.
        void* data = selCharArchive->getData(Data_Type_Misc, id, 0xfffe);

        // if the CSP is not in the archive request to load the RSP instead
        if (!thread->isReady() && data == NULL)
        {
            thread->requestLoad(charKind);
            return &area->m_charPicRes;
        }

        // if the CSP data is in the archive, load the data from there
        void* buffer = thread->getBuffer();

        if (data != NULL)
            CXUncompressLZ(data, buffer);

        // copy data from temp load buffer
        memcpy(area->m_charPicData, buffer, 0x40000);

        // flush cache
        DCFlushRange(area->m_charPicData, 0x40000);

        // set ResFile to point to filedata
        area->m_charPicRes = (ResFile)area->m_charPicData;

        // init resFile and return
        ResFile::Init(&area->m_charPicRes);

        // to ensure we load more than just
        // the first hovered character
        thread->reset();

        return &area->m_charPicRes;
    }

    muSelCharPlayerArea* (*_destroyPlayerAreas)(void*, int);
    muSelCharPlayerArea* destroyPlayerAreas(muSelCharPlayerArea* object, int external)
    {
        // destroy our load thread
        delete threads[object->m_areaIdx];
        threads[object->m_areaIdx] = 0;

        return _destroyPlayerAreas(object, external);
    }

    void InstallHooks(CoreApi* api)
    {
        // hook to load portraits from RSPs
        api->syReplaceFuncRel(0x1107c,
                              reinterpret_cast<void*>(getCharPicTexResFile),
                              NULL,
                              Modules::SORA_MENU_SEL_CHAR);

        // hook to clean up our mess when unloading CSS
        api->syReplaceFuncRel(0x10EF8,
                              reinterpret_cast<void*>(destroyPlayerAreas),
                              (void**)&_destroyPlayerAreas,
                              Modules::SORA_MENU_SEL_CHAR);

        // hook to create threads when booting the CSS
        api->syInlineHookRel(0x3524, reinterpret_cast<void*>(createThreads), Modules::SORA_MENU_SEL_CHAR);
    }
} // namespace CSSHooks

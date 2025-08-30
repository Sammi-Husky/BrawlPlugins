#pragma c99 on
#include "ftp.h"
#include <memory.h>
#include <plugin.hpp>
#include <sy_core.hpp>
namespace Syringe {
    extern "C" {
    typedef void (*PFN_voidfunc)();
    __attribute__((section(".ctors"))) extern PFN_voidfunc _ctors[];
    __attribute__((section(".ctors"))) extern PFN_voidfunc _dtors[];

    const PluginMeta* _prolog();
    void _epilog();
    void _unresolved();
    void main(Plugin* plg);
    }

    const PluginMeta META = {
        "ftp",                    // name
        "Sammi Husky",            // author
        Version("0.5.0"),         // version
        Version(SYRINGE_VERSION), // core version
        &main,
        .FLAGS = {
            .timing = TIMING_BOOT,
            .loading = LOAD_PERSIST,
            .heap = Heaps::Syringe,
        }
    };

    void main(Plugin* plg)
    {
        FTP::start(plg);
    }

    const PluginMeta* _prolog()
    {
        // Run global constructors
        PFN_voidfunc* ctor;
        for (ctor = _ctors; *ctor; ctor++)
        {
            (*ctor)();
        }

        return &META;
    }

    void _epilog()
    {
        // run the global destructors
        PFN_voidfunc* dtor;
        for (dtor = _dtors; *dtor; dtor++)
        {
            (*dtor)();
        }
    }

    void _unresolved(void)
    {
    }

}
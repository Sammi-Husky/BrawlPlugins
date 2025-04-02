#include "net_log.h"
#include "sy_core.h"

namespace Syringe {

    const PluginMeta META = {
        "NetLog",                // name
        "Sammi Husky",           // author
        Version("0.0.0"),        // version
        Version(SYRINGE_VERSION) // core version
    };

    extern "C" {
    typedef void (*PFN_voidfunc)();
    __attribute__((section(".ctors"))) extern PFN_voidfunc _ctors[];
    __attribute__((section(".ctors"))) extern PFN_voidfunc _dtors[];

    const PluginMeta* _prolog(CoreApi* api);
    void _epilog();
    void _unresolved();
    }

    const PluginMeta* _prolog(CoreApi* api)
    {
        // Run global constructors
        PFN_voidfunc* ctor;
        for (ctor = _ctors; *ctor; ctor++)
        {
            (*ctor)();
        }

        NetLog::Init(api);

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
#include <gf/gf_file_io_request.h>
#include <gf/gf_memory_pool.h>
#include <sy_core.h>

#include "codemenu.h"

namespace Syringe {

    const PluginMeta META = {
        "CodeMenu",              // name
        "Project+",              // author
        Version("0.5.0"),        // version
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

        mu_CodeMenu::Initialize(api);

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

        mu_CodeMenu::Destroy();
    }

    void _unresolved(void)
    {
    }

}
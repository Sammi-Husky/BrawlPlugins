#include <gf/gf_file_io_request.h>
#include <gf/gf_memory_pool.h>
#include <plugin.hpp>
#include <sr/sr_common.h>

#include "sandbox.h"

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
        "Sandbox",                // name
        "Sammi",                  // author
        Version("1.0.0"),         // version
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
        Sandbox::Init(plg);
    }

    /**
     * @brief This function is called when the plugin is loaded. It runs the global constructors and returns the plugin meta data.
     * @note The plugin meta data contains a pointer to the plugin's "main" function, which will be automatically called during loading.
     * @return const PluginMeta* Pointer to the plugin meta data.
     */
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

    /**
     * @brief This function is called when the plugin is unloaded. It runs the global destructors.
     * @return void
     */
    void _epilog()
    {
        // run the global destructors
        PFN_voidfunc* dtor;
        for (dtor = _dtors; *dtor; dtor++)
        {
            (*dtor)();
        }
    }

    /**
     * @brief This is a placeholder function for unresolved symbols. It does nothing.
     * @note This function is not used in the current version of the plugin, but it is included for completeness.
     * @return void
     */
    void _unresolved(void)
    {
    }
}
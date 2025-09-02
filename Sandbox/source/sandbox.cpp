#include <os/OSError.h>
#include <plugin.hpp>
namespace Sandbox {
    void Init(Plugin* plg)
    {
        OSReport("Hello from plugin\n");
    }

}

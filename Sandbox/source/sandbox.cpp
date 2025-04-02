#include <os/OSError.h>
#include <sy_core.h>

namespace Sandbox {
    void Init(CoreApi* api)
    {
        OSReport("Hello from plugin\n");
    }

}

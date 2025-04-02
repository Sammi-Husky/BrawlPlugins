#pragma once

class CoreApi;
namespace NetLog {
    int Init(CoreApi* api);
    void InstallHooks();
}
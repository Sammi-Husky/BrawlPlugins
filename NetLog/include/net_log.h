#pragma once

class Plugin;
namespace NetLog {
    int Init(Plugin* plg);
    void InstallHooks();
}
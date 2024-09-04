#pragma once

<<<<<<< HEAD
#define SYRINGE_VERSION "0.5.1"
=======
#define SYRINGE_VERSION "0.5.4"
>>>>>>> acfb47bb9bd2e18642d3d1299cb15cae600883c8

namespace Syringe {
    class Version {
    public:
        Version(const char* versionStr);
        friend bool operator<(const Version& lh, const Version& rh);
        friend bool operator==(const Version& lh, const Version& rh);
        friend bool operator!=(const Version& lh, const Version& rh);

        int major, minor, revision;
    };

    void versionToString(const Version& version, char* buffer);
}
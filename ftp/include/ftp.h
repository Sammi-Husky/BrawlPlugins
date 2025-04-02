#pragma once

#define DATA_BUF_SIZE 1024
#define ARG_BUF_SIZE 4096
#define PATH_BUF_SIZE 4096

enum FTP_DATA_MODE {
    DATA_MODE_ASCII,
    DATA_MODE_IMAGE
};
enum FTP_XFER_MODE {
    XFER_MODE_PORT,
    XFER_MODE_PASV
};

struct FTPSession;
struct FTPCommand {
    char* name;
    int (*handler)(FTPSession* session, char* args);
};

class CoreApi;
namespace FTP {

    void start(CoreApi* api);

} // namespace FTP
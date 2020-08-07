#include <SocketCommunication/MultiClientTCPStreamListener.h>

int main(int argc, char const *argv[]) {
    MultiClientTCPStreamListener<5> stream;
    stream.init(2222);
    Logger logger("MAIN");
    logger.info("Press [Enter] to exit");
//    getchar();
    usleep(500000);
    logger.info("Exiting main...");
    return 0;
}


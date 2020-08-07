#include <SocketCommunication/MultiClientTCPStreamListener.h>

int main(int argc, char const *argv[]) {

    MultiClientTCPStreamListener stream({
                                                2222,
                                                5
                                        });
    stream.init();
    Logger logger("MAIN");
    logger.info("Press [Enter] to exit");
    getchar();
    usleep(50000);
    logger.info("Exiting main...");
    return 0;
}


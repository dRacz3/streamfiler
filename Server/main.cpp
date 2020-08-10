#include <SocketCommunication/MultiClientTCPStreamListener.h>

static void show_usage()
{
   std::cerr << "streamfiler [-options] portnumber " << std::endl
             << "DESCRIPTION: " << std::endl
             << "A streamfiler egy porton figyelő alkalmazás, ami a csatlakozott connection tartalmát lemezre írja. A "
                "kiírt fájl neve megegyezik az érkezési időponttal (milisecundum pontossággal). Egyszerre több "
                "connection-t tud fogadni paraméterezéstől függően. \n"
             << "OPTIONS:\n"
             << "\t-c connections\t\tA maximálisan fogadható párhuzamos connection-ök száma\n"
             << "\t-f folder\t\tA mappa, ahova az érkztetett állományokat letárolja a program. Ha nem létezik "
                "induláskor, akkor a program létrehozza azt.\n"
             << "\t-h \t\t\tShow this help message\n"
             << "\t-l limit\t\tEkkora limit adatmennyiséget enged szálanként és másodpercenként kilobyte-ban a "
                "portjára folyatni a program\n"
             << "\t-t timeout\t\tAzt az időt határozza meg másodpercben, ami után a program bontja az idle "
                "connection-öket. Timeout -1 esetén nem bontja.\n";
}

int main(int argc, char const* argv[])
{
   if (argc < 4)
   {
      show_usage();
      return 1;
   }

   std::vector<std::string> sources;
   int connections = 1;
   std::string folder = "./";
   int limit = 1024;
   double timeout = -1;

   for (int i = 1; i < argc; ++i)
   {
      if (std::string(argv[i]) == "-f")
      {
         if (i + 1 < argc)
         {
            folder = argv[++i];
            std::cout << "Streams will be saved in  folder: " << folder << std::endl;
         }
         else
         {
            std::cerr << "-f option requires one argument." << std::endl;
            return 1;
         }
      }
      if (std::string(argv[i]) == "-c")
      {
         if (i + 1 < argc)
         {
            connections = std::stoi(argv[++i]);
            std::cout << "Max connections: " << connections << std::endl;
         }
         else
         {
            std::cerr << "-c option requires one argument." << std::endl;
            return 1;
         }
      }
      if (std::string(argv[i]) == "-l")
      {
         if (i + 1 < argc)
         {
            limit = std::stoi(std::string(argv[++i]));
            std::cout << "Limit: " << limit << std::endl;
         }
         else
         {
            std::cerr << "-l option requires one argument." << std::endl;
            return 1;
         }
      }
      if (std::string(argv[i]) == "-t")
      {
         if (i + 1 < argc)
         {
            auto arg = std::string(argv[++i]);
            timeout = std::stof(arg);
            std::cout << "Timeout: " << timeout << std::endl;
         }
         else
         {
            std::cerr << "-t option requires one argument." << std::endl;
            return 1;
         }
      }
      else
      {
         sources.emplace_back(argv[i]);
      }
   }

   MultiClientTCPStreamListener stream({2222, 5});
   stream.init();
   Logger logger("MAIN");
   logger.info("Press [Enter] to exit");
   getchar();
   usleep(50000);
   logger.info("Exiting main...");
   return 0;
}

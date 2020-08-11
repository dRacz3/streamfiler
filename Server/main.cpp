#include <SocketCommunication/MultiClientTCPStreamListener.h>

static void show_usage()
{
   std::cerr
       << "streamfiler [-options] portnumber " << std::endl
       << "DESCRIPTION: " << std::endl
       << "Streamfiler is an application that is listening on a specified port. It expects a stream of data from "
          "a connected client and will writes it to the specified location. The created file's name equals to "
          "the connections setup timestamp with millisec precision. Can handle multiple connections based on "
          "parametrisation.\n"
       << "OPTIONS:\n"
       << "\t-c connections\t\tMaximum number of parralel connections.\n"
       << "\t-f folder\t\tFolder, where the content of the TCP stream is written. Will be created if does not exist.\n"
       << "\t-h \t\t\tDisplay this help message\n"
       << "\t-l limit\t\toverall bandwith limitation in kb/s\n"
       << "\t-t timeout\t\tTimeout in seconds. Idle connections after this time will be closed. If set to -1, "
          "connections will not be timed out.\n";
}

int main(int argc, char const* argv[])
{
   std::vector<std::string> sources;
   int connections = 1;
   std::string folder = "./";
   int limit = 32 * 1024;  // byte / sec
   double timeout = -1;

   int port = 2222;
   try
   {
      port = std::stoi(argv[argc - 1]);
   }
   catch (std::logic_error&)
   {
      std::cerr << " ERROR! Please provide port number." << std::endl;
      show_usage();
      exit(EXIT_FAILURE);
   }

   for (int i = 1; i < argc; ++i)
   {
      if (std::string(argv[i]) == "-h")
      {
         show_usage();
         exit(0);
      }
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
            limit = std::stoi(std::string(argv[++i])) * 1024;
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

   MultiClientTCPStreamListener stream(
       {port, static_cast<size_t>(connections), static_cast<int>(timeout * 1000), folder, limit});
   stream.init();
   Logger logger("MAIN");
   logger.info("Press [Enter] to exit");
   getchar();
   logger.info("Exiting main...");
   return 0;
}

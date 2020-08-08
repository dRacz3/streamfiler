#include "DiskWriter.h"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

DiskWriter::~DiskWriter() { close(); }

void DiskWriter::write(const std::string& content)
{
   output << content;
   output.flush();
}
void DiskWriter::close() { output.close(); }

DiskWriter::DiskWriter(const std::string& directory, const std::string& filename)
    : filename(filename), directory(directory), m_logger("DW" + filename)
{
   if (!fs::is_directory(directory) || !fs::exists(directory))
   {
      fs::create_directory(directory);
   }
   output = std::ofstream(directory + SEPARATOR + filename);
   if (!output.good() || output.fail())
   {
      m_logger.warning("Could not start writing the file : " + directory + SEPARATOR + filename);
   }
}

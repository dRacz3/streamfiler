#ifndef STREAMFILER_DISKWRITER_H
#define STREAMFILER_DISKWRITER_H

#include <Utilities/Logger.h>

#include <fstream>
#include <string>

#include "ITextOutput.h"

const char SEPARATOR =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

class DiskWriter : public ITextOutput
{
  public:
   explicit DiskWriter(const std::string& directory, const std::string& filename);
   virtual ~DiskWriter();
   void write(const std::string& content) override;
   void close() override;

  protected:
   std::string filename;
   std::string directory;

   std::ofstream output;
   Logger m_logger;
};

#endif  // STREAMFILER_DISKWRITER_H

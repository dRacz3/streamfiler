#ifndef STREAMFILER_DISKWRITER_H
#define STREAMFILER_DISKWRITER_H

#include <string>

#include "ITextOutput.h"

class DiskWriter : public ITextOutput
{
  public:
   void write(const std::string& content) override;
   void close() override;
};

#endif  // STREAMFILER_DISKWRITER_H

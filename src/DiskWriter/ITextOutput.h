#ifndef STREAMFILER_ITEXTOUTPUT_H
#define STREAMFILER_ITEXTOUTPUT_H

#include <string>

class ITextOutput
{
  public:
   virtual ~ITextOutput() {};
   //! Writes the provided content to the target.
   //! \param content
   virtual void write(const std::string& content) = 0;
   //! \brief Finalize writing.
   virtual void close() = 0;
};

#endif  // STREAMFILER_ITEXTOUTPUT_H

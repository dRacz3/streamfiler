#include "DiskWriter.h"
#include "gtest/gtest.h"

TEST(DiksWriterTest, test1)
{
   DiskWriter w("/var/tmp/", "somefile");
   w.write("Hello");
   w.close();

   std::ifstream input("/var/tmp/somefile");
   std::string line;
   getline(input, line);
   EXPECT_EQ("Hello", line );

   // TODO : cleanup file
}
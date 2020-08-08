#include "ConsoleWriter.h"

#include <iostream>
#include <utility>

ConsoleWriter::ConsoleWriter(const std::string& filename) : m_logger(filename + "_WRITER") {}

ConsoleWriter::~ConsoleWriter() { close(); }

void ConsoleWriter::close() { m_logger.info("---- closed ----"); }

void ConsoleWriter::write(const std::string& content) { m_logger.info(content); }

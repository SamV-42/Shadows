#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>

#define OUTPUT_LOG std::cout
#define ERROR_LOG std::cout


//Below is broken
/*#include <ostream>
#include <iostream>

class Logging {
public:
  static const std::ostream & output;
  static const std::ostream & error;
};


const std::ostream & Logging::output = std::cout;
const std::ostream & Logging::error = std::cout;*/

#endif

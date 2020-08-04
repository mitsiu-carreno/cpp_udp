#include "inputValidation.hpp"
#include "thread.hpp"
#include <iostream>

namespace utils{
  bool GetUserInterruption(){
    std::string input;

    while(true){
      thread::PrintSafe("Press\"q\" at anytime to quit!\n");
      std::getline(std::cin, input);
      // Check if std::cin is still on normal mode
      utils::InputValidation();
      if(input == "q"){
        return true;
      }else{
        thread::PrintSafe("Input: " + input + "\n");
      }
    }
  }
}

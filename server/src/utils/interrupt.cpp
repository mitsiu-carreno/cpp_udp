#include "inputValidation.hpp"
#include "thread.hpp"
#include <iostream>

namespace utils{
  bool GetUserInterruption(){
    std::string input;

    while(true){
      thread::PrintSafe("Press \"q\" at anytime to quit!\n");
      
      std::getline(std::cin, input);
      // Check if std::cin is still on normal mode
      // (Depreciated delays thread)
      // utils::InputValidation();
       
      // If input ends with "q", exit
      if(input[input.size()-1] == 'q'){
        return true;
      }
    }
  }
}

#include "inputValidation.hpp"
#include <iostream>

namespace utils{
  bool GetUserInterruption(){
    std::string input;

    while(true){
      std::cout << "Press \"q\" at anytime to quit: ";
      std::getline(std::cin, input);
      // Check if std::cin is still on normal mode
      utils::InputValidation();
      if(input == "q"){
        return true;
      }else{
        std::cout << "Input: " << input << "\n";
      }
    }
  }
}

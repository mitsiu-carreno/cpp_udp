#include <iostream>

namespace utils{
  bool InputValidation(){
    bool flag {false};
    if(std::cin.fail()){
      std::cin.clear();
      flag = false;
    }else{
      flag = true;
    }
    
    std::cin.ignore(32767, '\n');
    return flag;
  }
}

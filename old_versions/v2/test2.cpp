# include <iostream>
# include <memory>

int main(){
  std::unique_ptr<int> p[2] = {nullptr, nullptr};
  std::unique_ptr<int> q(new int(1));

  std::cout << (p[0] == nullptr) << "\n";
  std::cout << std::is_null_pointer<decltype( *(p[0]) )>::value << "\n";
  std::cout << std::is_null_pointer<decltype( *q )>::value << "\n";

};

#include <iostream>
int main()
{
  int a = 0;
  int num_of_elements = 0;
  int b = 0;
  int c = 0;
  int length = 0;
  const int min_length = 3;

  try {
      if (!(std::cin >> b)) {
         throw 1;
      }
      if (b == 0) {
         std::cout << num_of_elements << "\n";
         return 0;
      }
      length+=1;

      if (!(std::cin >>a)) {
         throw 1;
      }
      if (a == 0) {
         std::cout << num_of_elements << "\n";
         return 0;
      }
      length+=1;

      for(;;) {
           if (!(std::cin >> c)) {
              throw 2;
      }
      if (c == 0) {
         if (length<min_length) {
            throw 2;
         }

         break;
     }
     length+=1;

     if (a < b and a > c) {
        num_of_elements+=1;
     }

     b = a;
     a = c;

 }
 std::cout << num_of_elements << "\n";
  }
  catch (int thr) {
        if (thr == 1) {
           std::cerr << "Invalid Input" << "\n";
           return 1;
        }
        if (thr == 2) {
           std::cerr << "Type at least 3 numbers for a sequence" << "\n";
           return 2;
        }
  }

  return 0;
}

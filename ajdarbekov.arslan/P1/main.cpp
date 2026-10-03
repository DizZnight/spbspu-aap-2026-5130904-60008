#include <iostream>
#include <stdexcept>

int main(){
	int tek = 0, prosh = 0, tek_dl = 0, max_dl = 0;

	try {
		while (std::cin >> tek) {
			if (tek == 0) {
				break;
			}

			if (tek <= prosh) {
				++tek_dl;
			}
			else {
				tek_dl = 1;
			}

			if (tek_dl > max_dl) {
				max_dl = tek_dl;
			}

			prosh = tek;


		} 			
		if (!std::cin) {
			throw std::invalid_argument("Not a sequence");
		}

		std::cout << max_dl << "\n";

	}
	catch (const std::invalid_argument &ex) {
		std::cerr <<  ex.what() << "\n";
		std::exit(1);
	}
	return 0;
}

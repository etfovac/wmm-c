#include <iostream>
using namespace std;

int check_ptr(void* ptr, string info) {

    if (!ptr) {
        std::cout << "Item not find " << info << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

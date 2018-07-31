#include <iostream>

#include <BigInteger.hpp>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Wrong arguments! Usage: big-number-example operand_1 operand_2" << std::endl;
        return -1;
    }

    try {
        std::string result;
        result = (apmath::BigInteger(argv[1]) += apmath::BigInteger(argv[2])).toString();
        std::cout << result << std::endl;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    return 0;
}

#include "normal.hpp"

int main(int argc, const char *argv[])
{
    int Run = atoi(argv[1]);
    int Dim = atoi(argv[2]);
    int Pop = atoi(argv[3]);
    int Fun_num = atoi(argv[4]);
    int Ben_num = atoi(argv[5]);
    DE de;
    de.run(Run, Dim, Pop, Fun_num, Ben_num);
}
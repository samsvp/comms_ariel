#include "../include/connect.hpp"

using namespace std;

/* simple little function to write an error string and exit */
void err(const char* s) {
    perror(s);
    exit(EXIT_FAILURE);
}

#include "Dictionary.h"
#include <string>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iostream>

#define MAX_LEN 300

using namespace std;

int main(int argc, char *argv[]){
    FILE *in;
    char line[MAX_LEN];


    if (argc != 3){
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }
    in = fopen(argv[1], "r");
    if( in==NULL ){
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    ofstream out(argv[2]);
    std::string s, g;
    Dictionary A = Dictionary();

    int i=0;
    while (fgets(line, MAX_LEN, in) != NULL){
        s = line;
        s.erase(std::prev(s.end()));

        A.setValue(s, i+1);
        ++i;
    }

    out << A << "\n";
    g = A.pre_string();
    out << g;

    out.close();
    fclose(in);



    return 0;
}
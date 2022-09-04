#include "List.h"
#include "BigInteger.h"
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


    fgets(line, MAX_LEN, in);
    std::string s;
    s += line;
    s.erase(std::prev(s.end()));
    // Constants -----------------------------------------------------------------
    BigInteger three = BigInteger("3");
    BigInteger two = BigInteger("2");
    BigInteger nine = BigInteger("9");
    BigInteger sixteen = BigInteger("16");

    // A -------------------------------------------------------------------------
    BigInteger A = BigInteger(s);

    out << A << "\n\n";


    // B -------------------------------------------------------------------------
    fgets(line, MAX_LEN, in);
    fgets(line, MAX_LEN, in);
    std::string p;
    p += line;
    BigInteger B = BigInteger(p);
    out << B << "\n\n";

    // A + B ---------------------------------------------------------------------

    out << A+B << "\n\n";

    // A - B ---------------------------------------------------------------------

    out << A-B << "\n\n";

    // A - A ---------------------------------------------------------------------

    out << A-A << "\n\n";

    // 3A - 2B -------------------------------------------------------------------

    out << (A*three)-(B*two) << "\n\n";

    // A * B ---------------------------------------------------------------------

    out << A*B << "\n\n";

    // A * A ---------------------------------------------------------------------

    out << A*A << "\n\n";

    // B * B ---------------------------------------------------------------------

    out << B*B << "\n\n";

    // 9(A * A * A * A) + 16(B * B * B * B * B) ----------------------------------

    out << nine*(A*A*A*A)+sixteen*(B*B*B*B*B) << "\n";




    out.close();
    fclose(in);



    return 0;
}
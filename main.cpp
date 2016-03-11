#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

unsigned int Random(int nLow, int nHigh) {
    return (rand() % (nHigh - nLow + 1)) + nLow;
}

int getNumber() {
    cout << "Please enter a number between 1 and 9: ";
    int nineInput = 0;
    cin >> nineInput;
    return nineInput;
}

int main()
{
    int nineInput = getNumber();
    if (nineInput > 0 && nineInput < 10) {
        cout << "Good Job." << endl;
    }
    else {
        cout << "That's not a number." << endl;
        main();
    }
    return 0;
}

/*
Typically, enumerations are used to index arrays. Super neat, because that way it's easier to tell what each element is.


*/

// project identifer  = 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include <algorithm>
#include <iostream>
#include <getopt.h>
#include <iomanip>
#include "pokemon.cpp"

using namespace std;

enum class Mode
{
    kNone = 0,
    kMST,
    kFastTSP,
    kOptTSP,
    kHelp
};

// Print help for the user when requested.
void printHelp(char *argv[])
{
    cout << "Usage: " << argv[0] << " --mode {MST|FASTTSP|OPTTSP}\n";
    cout << "Modes:\n";
    cout << "  MST      Find the minimum spanning tree (MST) of the map.\n";
    cout << "  FASTTSP  Find a fast approximation of the Travelling Salesperson Problem (TSP).\n";
    cout << "  OPTTSP   Find the optimal solution to the TSP.\n";
    cout << "Options:\n";
    cout << "  -h, --help  Display this help message.\n";
}

// Process the command line; there is no return value, but the Options
void getMode(int argc, char *argv[], Mode &mode)
{
    opterr = false;
    int choice;
    int index = 0;
    option long_options[] = {
        {"mode", required_argument, nullptr, 'm'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, '\0'}};

    while ((choice = getopt_long(argc, argv, "hm:", long_options, &index)) != -1)
    {
        switch (choice)
        {
        case 'h':
            mode = Mode::kHelp;
            break;
        case 'm':
            if (string(optarg) == "MST")
                mode = Mode::kMST;
            else if (string(optarg) == "FASTTSP")
                mode = Mode::kFastTSP;
            else if (string(optarg) == "OPTTSP")
                mode = Mode::kOptTSP;
            else
            {
                cerr << "Error: Invalid mode" << optarg << "\n";
                exit(1);
            }
            break;

        default:
            cerr << "Error: Invalid command line option\n";
            exit(1);
        }
    }
    if (mode == Mode::kNone)
    {
        cerr << "Error: No mode specified\n";
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(false);
    cerr << fixed << showpoint << setprecision(2) << boolalpha;
    cout << fixed << showpoint << setprecision(2);
    Mode mode;

    getMode(argc, argv, mode);

    if (mode == Mode::kHelp)
    {
        printHelp(argv);
        return 0;
    }
    else
    {

        if (mode == Mode::kMST)
        {
            Pokemon pokemon(true);
            pokemon.MST(true);
        }
        else if (mode == Mode::kFastTSP)
        {
            Pokemon pokemon(false);
            pokemon.FASTTSP(true);
        }
        else if (mode == Mode::kOptTSP)
        {
            Pokemon pokemon(false);
            pokemon.OPTTSP();
        }
    }

    return 0;
}

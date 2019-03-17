#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "KL_Algorithm.h"
#include "DataStructure.h"
#include "Critical_Path_Stage.h"
using namespace std;
// Global Variables.
string File = "Inputs/Stages Milestone 2.txt";
string CM_File = "Outputs/Connectivity_Matrix.txt";
string KL_File = "Outputs/Partitioning_Output.txt";

int main()
{
    cout << "EDA Project Tool Jan. 2019" << endl <<  "By: Mohamed Monged, Mohamed Saad, Mohamed Helmy." << endl << endl;
    // Declaring Variable Number of each Element.
    int Lines_No = 0, I_No = 0, O_No = 0, FF_No = 0, Inv_No = 0, L_No = 0;
    // Declaring Clock.
    float Clock;
    // Getting Number of Elements.
    No_Elements(Lines_No, I_No, O_No, FF_No, Inv_No, L_No, File);
    // Initializing Data.
    InputPins IPins[I_No];
    OutputPins OPins[O_No];
    Inverters Inv[Inv_No];
    Flipflops FF[FF_No];
    LogicGates LG[L_No];
    ReadElements(IPins, OPins, Inv, FF, LG, File);
    CM(Lines_No , I_No, O_No, FF_No, Inv_No, L_No, IPins, OPins, Inv, FF, LG, CM_File);
    KL(CM_File, KL_File);
    Clock = Clock_Period(File);
    cout << "The Clock Period should not be less Than " << Clock << " ns." << endl;

    return 0;
}

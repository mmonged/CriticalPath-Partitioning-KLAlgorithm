using namespace std;
// Global Variable.
#define Max_Input_Numbers 6

// Create Class for Input Pins.
struct InputPins
{
    string Name;
    string Net;
    float Delay = 0;
	int Transistors = 0;
};

// Create Class for Output Pins.
struct OutputPins
{
    string Name;
    string Net;
    float Delay = 0;
    float Max_Delay = 0;
	int Transistors = 0;
};
// Create Class for Inverters.
struct Inverters
{
    string Name;
    string In;
    string Out;
	float Delay = 1;
	float Max_Delay = 0;
	int Level;
	int Transistors = 1;
};
// Create Class for Flip-Flops.
struct Flipflops
{
    string Name;
    string D;
    string clk;
    string Q;
	float Delay;
    float Max_Delay = 0;
	int Level;
	int Transistors = 8;
};
// Create Class for Logic Gates.
struct LogicGates
{
    string Type;
    string Name;
    string In[Max_Input_Numbers];
    string Out;
	int n;
	float Delay;
	float Max_Delay = 0;
	int Level;
	int Transistors;
};

// Delete Characters From the Line.
void CharacterRemover(string &Line)
{
    Line.erase(remove(Line.begin(), Line.end(), '('), Line.end());
    Line.erase(remove(Line.begin(), Line.end(), ')'), Line.end());
    Line.erase(remove(Line.begin(), Line.end(), ','), Line.end());
    Line.erase(remove(Line.begin(), Line.end(), ';'), Line.end());
}

// Get Words from Line;
string GetWord(string &Line)
{
    string Word = {};
    int cnt = 0;
    for(cnt = 0; cnt < Line.length(); cnt++)
    {
        if(Line[cnt] == ' ')
            break;
        else
            Word += Line[cnt];
    }
    Line.erase(0, cnt + 1);
    return Word;
}

// Returning Sizes of Elements.
void No_Elements(int &Lines_No,int &I_No,int &O_No,int &FF_No,int &Inv_No,int &L_No, string File)
{
    // Initializing Variables.
    string Line, Word;
    // Opening Stream File Net-list.
    ifstream Netlist;
    Netlist.open(File);

    while(!Netlist.eof())
    {
        getline(Netlist, Line);								                                            // Reading the new Line.
        Word = GetWord(Line);                                                                           // Reading First Word.
        // Updating Element Sizes.
        if(Word == "In")
            I_No++;
        else if(Word == "Out")
            O_No++;
        else if(Word == "Fipflop")
            FF_No++;
        else if(Word == "Inv")
            Inv_No++;
        else
            L_No++;
    }
    // Getting Total Number of Elements.
    Lines_No = I_No + O_No + FF_No + Inv_No + L_No;
    // Close File.
    Netlist.close();
}

// Reading Pin Element.
void GetIN(InputPins &I, string Line)
{
    string Word;
    I.Name = GetWord(Line);
    // Reading Twice for removing in/out words.
    Word = GetWord(Line);
    I.Net = GetWord(Line);
}

// Reading Pin Elements.
void GetOut(OutputPins &O, string Line)
{
    string Word;
    O.Name = GetWord(Line);
    // Reading Twice for removing in/out words.
    Word = GetWord(Line);
    O.Net = GetWord(Line);
}

// Reading FlipFlop Elements.
void GetFF(Flipflops &FF, string Line)
{
    string Word;
    FF.Name = GetWord(Line);
    // Reading Twice for removing in word and Getting D.
    Word = GetWord(Line);
    FF.D = GetWord(Line);
    // Reading Twice for removing clk word and Getting clk.
    Word = GetWord(Line);
    FF.clk = GetWord(Line);
    // Reading Twice for removing out word and Getting Q.
    Word = GetWord(Line);
    FF.Q = GetWord(Line);
}

// Reading Inverter Elements.
void GetInv(Inverters &Inv, string Line)
{
    string Word;
    Inv.Name = GetWord(Line);
    // Reading Twice for removing in word and Getting in.
    Word = GetWord(Line);
    Inv.In = GetWord(Line);
    // Reading Twice for removing out word and Getting out.
    Word = GetWord(Line);
    Inv.Out = GetWord(Line);
}

// Reading Gate Elements.
void GetLG(LogicGates &LG, string Line)
{
    string Word;
    LG.Name = GetWord(Line);
	for(int i = 0; i < Max_Input_Numbers; i++)
	{
		if(i < LG.n)
		{
			// Reading Twice for removing in word and Getting in.
			Word = GetWord(Line);
			LG.In[i] = GetWord(Line);
		}
		else
			LG.In[i] = "";
	}
    // Reading Twice for removing out word and Getting out.
    Word = GetWord(Line);
    LG.Out = GetWord(Line);

	if(LG.Type == "And")
	{
        LG.Delay = (LG.n / 2) + 1;
		LG.Transistors = (LG.n + 1) * (LG.n + 1);
	}
    else if(LG.Type == "Or")
	{
        LG.Delay = LG.n + 1;
		LG.Transistors = (LG.n + 1) * (LG.n + 1);
	}
    else if(LG.Type == "Xor")
	{
        LG.Delay = LG.n;
		LG.Transistors = 2 * LG.n * (LG.n + 1);
	}
    else if(LG.Type == "Nand")
	{
        LG.Delay = LG.n / 2;
		LG.Transistors = LG.n * LG.n;
	}
	else if(LG.Type == "Nor")
	{
        LG.Delay = LG.n;
		LG.Transistors = LG.n * LG.n;
	}
    else if(LG.Type == "Xnor")
	{
		LG.Delay = LG.n;
		LG.Transistors = 2 * LG.n * LG.n;
	}
}

// Split Logic Type from Logic Number of Inputs and Assigning Their Values
void Split(LogicGates &LG, string Word)
{
    string Logic_Type;
    string Input_Number;
    string Temp;

	for(int i = 0; i < Word.length(); i++)
	{
        Temp = Word[i];
        if(Word[i] > 47 && Word[i] < 58)
        {
            Input_Number.append(Temp);
        }
        else
            Logic_Type.append(Temp);
	}
    LG.Type = Logic_Type;
    LG.n = atoi(Input_Number.c_str());
}

// Reading Elements and Connections.
void ReadElements(InputPins IPins[], OutputPins OPins[], Inverters Inv[], Flipflops FF[], LogicGates LG[], string File)
{
    // Initializing Variables.
    int NIn = 0, NOut = 0, NInv = 0, NFF = 0, NLG = 0;
    string Line, Word;
    // Opening Stream File Net-list.
    ifstream Netlist;
    Netlist.open(File);

    while(!Netlist.eof())
    {
        getline(Netlist, Line);								                                            // Reading the new Line.
		CharacterRemover(Line);                                                                         // Remove Characters ')' ',' '(' ';'.
        Word = GetWord(Line);                                                                           // Reading First Word.

        if(Word == "In")
        {
            // Reading Pin Elements.
            GetIN(IPins[NIn], Line);
            NIn++;
        }
        else if(Word == "Out")
        {
            // Reading Pin Elements.
            GetOut(OPins[NOut], Line);
            NOut++;
        }
        else if(Word == "Fipflop")
        {
            // Reading FlipFlop Elements.
            GetFF(FF[NFF], Line);
            NFF++;
        }
        else if(Word == "Inv")
        {
            // Reading Inverter Elements.
            GetInv(Inv[NInv], Line);
            NInv++;
        }
        else
        {
            // Reading Type of Logic AND, OR,... and Assigning Input Numbers.
            Split(LG[NLG], Word);
            // Reading Gate Elements.
            GetLG(LG[NLG], Line);
            NLG++;
        }
    }

}

// Connectivity Matrix and Critical Path.
void CM(int Lines_No ,int I_No,int O_No,int FF_No,int Inv_No,int L_No, InputPins IPins[], OutputPins OPins[], Inverters Inv[], Flipflops FF[], LogicGates LG[], string CM_File)
{
    // Opening Output Text File.
    ofstream CMatrix;                                                                    	// Create Output Stream.
    CMatrix.open(CM_File);                                              	// Open Output File.
    CMatrix << Lines_No << endl;                                                            // Writing First Line. (Matrix Size).

    // Declaring Connectivity Matrix Data type.
    int **CM = new int *[Lines_No];
    for(int i = 0; i < Lines_No; i++)
        CM[i] = new int [Lines_No];
    // Initializing Connectivity Matrix With Zeros.
    for(int i = 0; i < Lines_No; i++)
        for(int j = 0; j < Lines_No; j++)
                CM[i][j] = 0;

    // Connecting Inputs With Flip Flops.
    for(int i = 0; i < I_No; i++)
        for (int j = 0; j < FF_No; j++)
            if(IPins[i].Net == FF[j].D)
            {
                CM[i][I_No + j]++;
                CM[I_No + j][i]++;
                // Updating Timing.
                FF[j].Max_Delay = FF[j].Delay;
                // Setting Level.
                FF[j].Level = 1;
            }

    // Connecting Flip Flops With Gates, Inverters, and Output Pins.
    for(int i = 0; i < FF_No; i++)
    {
        // Logic Gates.
        for(int j = 0; j < L_No; j++)
        {
            for(int k = 0; k < Max_Input_Numbers; k++)
                if(FF[i].Q == LG[j].In[k] || FF[i].D == LG[j].In[k])
                {
                    CM[i + I_No][j + I_No + FF_No]++;
                    CM[j + I_No + FF_No][i + I_No]++;
                }
            if(FF[i].D == LG[j].Out)
            {
                CM[i + I_No][j + I_No + FF_No]++;
                CM[j + I_No + FF_No][i + I_No]++;
            }
        }

        // Inverters.
        for(int j = 0; j < Inv_No; j++)
        {
            if(FF[i].Q == Inv[j].In || FF[i].D == Inv[j].In)
            {
                CM[i + I_No][j + I_No + FF_No + L_No]++;
                CM[j + I_No + FF_No + L_No][i + I_No]++;
            }
            if(FF[i].D == Inv[j].Out)
            {
                CM[i + I_No][j + I_No + FF_No + L_No]++;
                CM[j + I_No + FF_No + L_No][i + I_No]++;
            }
        }

        // Output Pins.
        for(int j = 0; j < O_No; j++)
        {
            if(FF[i].Q == OPins[j].Net)
            {
                CM[i + I_No][j + I_No + FF_No + L_No + Inv_No]++;
                CM[j + I_No + FF_No + L_No + Inv_No][i + I_No]++;
            }
        }
    }

    // Connecting Gates with other Gates and Inverters.
    for(int i = 0; i < L_No; i++)
	{
		// Logic Gates With Each Other.
        for(int j = i + 1; j < L_No; j++)
		{
			for(int k = 0; k < Max_Input_Numbers; k++)
			{
				for(int l = 0; l < Max_Input_Numbers; l++)
					if(LG[i].In[k] == LG[j].In[l] && LG[i].In[k] != "")
					{
                        CM[i + I_No + FF_No][j + I_No + FF_No]++;
                        CM[j + I_No + FF_No][i + I_No + FF_No]++;
					}

				if(LG[i].In[k] == LG[j].Out || LG[i].Out == LG[j].In[k])
				{
					CM[i + I_No + FF_No][j + I_No + FF_No]++;
                    CM[j + I_No + FF_No][i + I_No + FF_No]++;
				}
			}

			if(LG[i].Out == LG[j].Out)
			{
				CM[i + I_No + FF_No][j + I_No + FF_No]++;
				CM[j + I_No + FF_No][i + I_No + FF_No]++;
			}
		}

		// Logic Gates With Inverters.
		for(int j = 0; j < Inv_No; j++)
        {
            for(int k = 0; k < Max_Input_Numbers; k++)
            {
                if(LG[i].In[k] == Inv[j].Out)
                {
                    CM[i + I_No + FF_No][j + I_No + FF_No + L_No]++;
                    CM[j + I_No + FF_No + L_No][i + I_No + FF_No]++;
                }
				if(LG[i].In[k] == Inv[j].In)
                {
                    CM[i + I_No + FF_No][j + I_No + FF_No + L_No]++;
                    CM[j + I_No + FF_No + L_No][i + I_No + FF_No]++;
                }
            }
            if(LG[i].Out == Inv[j].In)
            {
                CM[i + I_No + FF_No][j + I_No + FF_No + L_No]++;
                CM[j + I_No + FF_No + L_No][i + I_No + FF_No]++;
            }
        }
	}

    // Writing Connectivity Matrix.
    for(int i = 0; i < Lines_No; i++)
    {
        for(int j = 0; j < Lines_No; j++)
            CMatrix << CM[i][j] << " ";
        CMatrix << endl;
    }

    // Close Output Stream File.
    CMatrix.close();
}

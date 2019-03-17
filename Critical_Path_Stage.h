using namespace std;
// Global Variables.
#define Max_Input_Numbers 6

// Logic Gates Structure.
struct Logic
{
    string Name;
    string Type;
    vector <string> In;
    string Out;
    int n;
    float LDelay;
    float ODelay = 0;
    int path;
};

// Flip-Flops Tracks.
struct Tracks
{
    string Name;
    string D;
    float Delay;
    int path;
};

// Split Words and Number.
void SplitNumbers(string Word, string &Type, int &n)
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
            Type.append(Temp);
	}

    n = atoi(Input_Number.c_str());
}

// Calculate Delay of a Gate.
float Delay (string Type, int n)
{
    if (Type == "And")
        return (n*1.0/2) + 1;
    else if (Type == "Or")
        return n*1.0 + 1;
    else if (Type == "Xor")
        return n*1.0;
    else if (Type == "Nand")
        return n*1.0/2;
    else if (Type == "Nor")
        return n*1.0;
    else if (Type == "Xnor")
        return n*1.0;
    else if (Type == "Inv")
        return 1.0;
}

// Read Net-list and Update Tracks and FF Numbers.
void Read_Stage(vector <string> &Elements, int &TracksNo, int &FFNo, string File_n)
{
    // Defining Variable.
    vector <string> Tracks;
    // Opening Net-list File.
    ifstream level;
    level.open(File_n);

    // Reading Net-list File.
    while(!level.eof())
    {
        string Line, Copy, Word;
        getline(level, Line);
        CharacterRemover(Line);                 // Clean Line from Characters [,)(;].
        Copy = Line;
        Word = GetWord(Copy);
        Tracks.push_back(Word);
        Elements.push_back(Line);
        if(Word == "Fipflop")
            FFNo++;
    }

    // Close Stream File.
    level.close();

    // Counting Number of Tracks.
    for(unsigned int i = Tracks.size(); i > 0; i--)
        if(Tracks[i - 1] == "Fipflop")
            TracksNo++;
        else
            break;
}

// Read Tracks.
void Tracks_Fill(vector <string> Elements, Tracks FFTracks[], int TracksNo, int FFNo, int FFFNo, int LNo)
{
    int j = 0;
    for (unsigned int i = FFFNo + LNo; i < FFNo + LNo; i++)
    {
        string Line, Word;
        Line = Elements[i];
        Word = GetWord(Line);
        Word = GetWord(Line);
        FFTracks[j].Name = Word;
        Word = GetWord(Line);
        Word = GetWord(Line);
        FFTracks[j].D = Word;
        j++;
    }
}

// Fill up Logic.
void Logic_Fill(Logic LG[], vector <string> Elements, int LNo, int FFFNo)
{
    for(unsigned int i = 0; i < LNo; i++)
    {
        // Defining Variables.
        string Word, Line, Type;
        int n;
        // Reading Line.
        Line = Elements[FFFNo + i];
        // Reading Gate Structure.
        Word = GetWord(Line);
        SplitNumbers(Word, Type, n);
        // Fill in Type, N of Inputs and Logic Internal Delay.
        LG[i].Type = Type;
        if (Type == "Inv")
            LG[i].n = 1;
        else
            LG[i].n = n;
        LG[i].LDelay = Delay(Type, n);
        // Reading Name.
        Word = GetWord(Line);
        LG[i].Name = Word;
        // Reading Inputs.
        for(unsigned int j = 0; j < LG[i].n; j++)
        {
            // Reading Input Name.
            Word = GetWord(Line);
            Word = GetWord(Line);
            LG[i].In.push_back(Word);
        }
        // Reading Output.
        Word = GetWord(Line);
        Word = GetWord(Line);
        LG[i].Out = Word;
    }
}

void Timing(vector <string> Elements, Logic LG[], Tracks FFTracks[], int FFFNo, int LNo, int TracksNo)
{
    for(unsigned int l = 0; l < LNo; l++)
    {
        int Number;
        float Max_t = 0;
        // Searching by Inputs.
        for(unsigned int i = 0; i < LG[l].n; i++)
        {
            // Variable Declaration.
            int n, tempNumber;
            float temp;
            string Link, In;
            In = LG[l].In[i];
            // Check for Flip-Flip.
            SplitNumbers(In, Link, n);
            // Update Delay.
            for(unsigned int j = 0; j < FFFNo + LNo; j++)
            {
                // Variable Declaration.
                string Line, Word;
                Line = Elements[j];
                // Obtaining Path and Calculating Delay.
                if(Line.find("out " + In) != string::npos)
                {
                    if(Link == "Q")
                    {
                        Word = GetWord(Line);
                        Word = GetWord(Line);
                        temp = 2.0;
                        tempNumber = j;
                        break;
                    }
                    else
                    {
                        temp = LG[j - FFFNo].ODelay;
                        tempNumber = j;
                        break;
                    }
                }
            }
            // Max Value.
            if (temp > Max_t)
            {
                Max_t = temp;
                Number = tempNumber;
            }
        }
        LG[l].ODelay = Max_t + LG[l].LDelay;
        LG[l].path = Number;
    }

    for(unsigned int i = 0; i < TracksNo; i++)
    {
        string Word = FFTracks[i].D;
        for(unsigned int j = FFFNo; j < FFFNo + LNo; j++)
        {
            string line = Elements[j];
            if(line.find("out " + Word) != string::npos)
            {
                FFTracks[i].Delay = LG[j - FFFNo].ODelay;
                FFTracks[i].path = j;
            }
        }
    }
}

// Calculate Stage Critical Path.
float Critical_Path_Stage(string File_n, int itr)
{
    // Defining Variables.
    vector <string> Elements;
    int TracksNo = 0, FFNo = 0, LNo = 0, FFFNo;
    // Read String Vectors.
    Read_Stage(Elements, TracksNo, FFNo, File_n);
    FFFNo = FFNo - TracksNo;
    // Update Logic Size and Define Logic Array.
    LNo = Elements.size() - FFNo;
    Logic LG[LNo];
    Logic_Fill(LG, Elements, LNo, FFFNo);
    // Declaration and Filling Tracks.
    Tracks FFTracks[TracksNo];
    Tracks_Fill(Elements, FFTracks, TracksNo, FFNo, FFFNo, LNo);
    // Calculate Delay.
    Timing(Elements, LG, FFTracks, FFFNo, LNo, TracksNo);

    // Representing Path.
    float Clock = 0;
    int Pos;
    bool run = true;
    for(unsigned int i = 0; i < TracksNo; i++)
    {
        float tempclk;
        int tempj;
        tempclk = FFTracks[i].Delay;
        tempj = FFTracks[i].path;
        if(tempclk > Clock)
        {
            Clock = tempclk;
            Pos = tempj;
        }
    }

    cout << "Critical Path of Stage no. " << itr << " has delay of: " << Clock << " ns." << endl << "The Critical Path of Stage no. " << itr << " is: ";
    while(run)
    {
        string Line = Elements[Pos];
        string Word = GetWord(Line);
        Word = GetWord(Line);
        cout << Word << " ";
        Pos = Pos - FFFNo;
        Pos = LG[Pos].path;
        Line = Elements[Pos];
        Word = GetWord(Line);
        if(Word == "Fipflop")
        {
            cout << GetWord(Line) << "." << endl;
            run = false;
        }
    }
    return Clock;
}

// Read Net-list File without Pins.
void Read_NetVector(vector <string> &LElements, string File)
{
    // Opening File.
    ifstream Netlist;
    Netlist.open(File);

    while(!Netlist.eof())
    {
        string Line, Copy;
        getline(Netlist, Line);
        Copy = Line;
        Copy = GetWord(Copy);
        if(Copy != "In" && Copy != "Out")
            LElements.push_back(Line);
    }

}

// Getting Number of Stages.
int No_Stages(vector <string> LElements)
{
    int C = 0;
    for(unsigned int i = 0; i < LElements.size() - 1; i++)
    {
        string Line1, Line2, Word1, Word2;
        Line1 = LElements[i];
        Line2 = LElements[i+1];
        Word1 = GetWord(Line1);
        Word2 = GetWord(Line2);
        if(Word1 != "Fipflop" && Word2 == "Fipflop")
            C++;
    }
    return C;
}

// Divide Stages
void Divide_Stages(vector <string> &LElements, int N)
{
    // Declaring Variables.
    int c = 1;
    int p = 0, a = 0;

    for(unsigned int i = 0; i < N; i++)
    {
        // Declaring Variables.
        int x, y;
        string Line, Word;
        // Initiating Output File Name
        string OFile = "Outputs/SubNets/SubNet";
        OFile += to_string(c);
        OFile += ".txt";
        // Open Output File.
        ofstream SubNet;
        SubNet.open(OFile);

        // Searching for the End.
        for(unsigned int j = p; j < LElements.size() - 1; j++)
        {
            string Line1, Line2, Word1, Word2;
            Line1 = LElements[j];
            Line2 = LElements[j+1];
            Word1 = GetWord(Line1);
            Word2 = GetWord(Line2);
            if(Word1 != "Fipflop" && Word2 == "Fipflop")
            {
                x = j;
                break;
            }
        }

        // Streaming Output.
        for (unsigned int j = p; j < x + 1; j++)
        {
            SubNet << LElements[j] << endl;
        }
        y = x + 1;
        Line = LElements[y];
        Word = GetWord(Line);
        while(Word == "Fipflop")
        {
            SubNet << LElements[y];
            y++;
            Line = LElements[y];
            Word = GetWord(Line);
            if(Word == "Fipflop")
                SubNet << endl;
        }
        // Close File.
        SubNet.close();
        p = x + 1;
        c++;
    }
}

int Split_NetList(string File)
{
    int N;
    vector <string> LElements;
    Read_NetVector(LElements, File);
    N = No_Stages(LElements);
    Divide_Stages(LElements, N);
    return N;
}

float Clock_Period(string File)
{
    int N;
    N = Split_NetList(File);
	// Calculate Paths.
    float Delays[N], Clock = 0, temp;
    for(int i = 0; i < N; i++)
    {
        string File_n = "Outputs/SubNets/SubNet";
        File_n += to_string(i+1);
        File_n += ".txt";
        Delays[i] = Critical_Path_Stage(File_n, i + 1);
        cout << endl;
    }

	for(int i = 0; i < N; i++)
	{
		temp = Delays[i];
		if(temp > Clock)
		{
			Clock = temp;
		}
	}
    return Clock;
}

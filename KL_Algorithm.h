using namespace std;

// Global Variables.
int IndexI, IndexII;

// Generate Cut.
void Generate_Cut(int m, int R1[], int R2[], int R3[], int R4[])
{
    for(int i = 0; i < m; i++)
    {
        if(i < m/2)
        {
            R1[i] = i;
            R3[i] = i;
        }
        else
        {
            R2[i - m/2] = i;
            R4[i - m/2] = i;
        }
    }
}

// Random Cut Generation Function.
void Generate_Random_Cut(int m, int R1[], int R2[], int R3[], int R4[])
{
    // Variables for Random Cut.
    int Rand_N;                                 // Rand_N is the output Random Number Generated.
    bool RG[m] = {false};
    // Generate Random Cut.
    for(int i = 0; i < m; i++)
    {
        Rand_N = rand() %m;
        while(RG[Rand_N] == true)
        {
            Rand_N = rand() %m;
        }
        RG[Rand_N] = true;

        if (i < m/2)
        {
            R1[i] = Rand_N;
            R3[i] = Rand_N;
        }
        else
        {
            R2[i - m/2] = Rand_N;
            R4[i - m/2] = Rand_N;
        }
    }
}

// Calculate the Cost Function.
void Calculate_Cost_Function(int m, int A[], int D[], int R1[], int R2[])
{
    int *c = new int;
    *c = 0;
    int M[m][m];
    // Converting Array to 2D Matrix.
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            M[i][j] = A[*c];
            *c = *c + 1;
        }
    }
    // Calculating The Cost Function.
    for (int i = 0; i < m; i++)
    {
        int c = 0;
        int nc = 0;
        if (i < m/2)
        {
            int v = R1[i];
            for(int j = 0; j < m/2; j++)
            {
                int p = R2[j];
                c = c + M[v][p];
            }
            for(int j = 0; j < m/2; j++)
            {
                int p = R1[j];
                nc = nc + M[v][p];
            }
            D[v] = c - nc;
        }
        else
        {
            int v = R2[i - m/2];
            for(int j = 0; j < m/2; j++)
            {
                int p = R1[j];
                c = c + M[v][p];
            }
            for(int j = 0; j < m/2; j++)
            {
                int p = R2[j];
                nc = nc + M[v][p];
            }
            D[v] = c - nc;
        }
    }
}

// Calculate the Maximum Delta g Function.
int Max_Delta_g(int m, int A[], int D[], int R1[], int R2[], bool F[])
{
    int g[m], I1[m], I2[m], r, c;
    int Max_Value = -10000000000;
    int *t = new int;
    *t = 0;
    int M[m][m];
    // Converting Array to 2D Matrix.
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            M[i][j] = A[*t];
            *t = *t + 1;
        }
    }
    delete t;
    // Calculating the Delta (g)es.
    for(int i = 0; i < m/2; i++)
    {
        g[i] = -10000000000;
        r = R1[i];
        for(int j = m/2; j < m; j++)
        {
            c = R2[j - m/2];
            if(F[i] != true && F[j] != true)
            {
                Max_Value = D[r] + D[c] - (2 * M[r][c]);
                if(Max_Value > g[i])
                {
                    g[i] = Max_Value;
                    I1[i] = r;
                    I2[i] = c;
                }
            }
        }
    }
    // Calculating the Maximum Delta g.
    Max_Value = -10000000000;
    for (int i = 0; i < m/2; i++)
    {
        if(g[i] > Max_Value)
        {
            Max_Value = g[i];
            IndexI = I1[i];
            IndexII = I2[i];
        }
    }
    return Max_Value;
}

// Swap and Fix Function.
void Swap_Fix(int m, int R1[], int R2[], bool F[])
{
    int temp, temp1, temp2;
    // Searching for the Nodes.
    for(int i = 0; i < m/2; i++)
    {
      if(R1[i] == IndexI)
        temp1 = i;
      if(R2[i] == IndexII)
        temp2 = i;
    }
    // Swapping.
    temp = R1[temp1];
    R1[temp1] = R2[temp2];
    R2[temp2] = temp;
    // Fix Nodes.
    F[IndexI] = true;
    F[IndexII] = true;
}

// KL For a one Path.
void Path(int m, int A[], int R1[], int R2[], int R3[], int R4[], int &G)
{
	// Initializing Variables.
	bool F[m] = {false};                        // Flag for Swapping.
    int D[m], g[m/2] = {0}, Stop = 0;           // Cost Function.
    G = 0;

	// Looping to Find All Delta (g)es.
    for(int i = 0; i < m/2; i++)
    {
        // Calculate Cost Function.
        Calculate_Cost_Function(m, A, D, R1, R2);
        // Calculate Maximum Delta g.
        g[i] = Max_Delta_g(m, A, D, R1, R2, F);
        // Swap the Nodes.
        Swap_Fix(m, R1, R2, F);
    }

	// Calculating the Maximum G.
    for(int i = 0; i < m/2; i++)
    {
        F[i] = false;
        F[i+4] = false;
        if(g[i] + G > G)
        {
            G = G + g[i];
            Stop = i;
        }
    }

	// Calculating the Perfect Partitioning.
    for(int i = 0; i < Stop + 1; i++)
    {
        // Calculate Cost Function.
        Calculate_Cost_Function(m, A, D, R3, R4);
        // Calculate Maximum Delta g.
        g[i] = Max_Delta_g(m, A, D, R3, R4, F);
        // Swap the Nodes.
        Swap_Fix(m, R3, R4, F);
    }
}

void KL(string CM_File, string KL_File)
{
	// Activate Paths.
    bool Paths = true;

	// To initiate Random Number.
    srand(time(NULL));

	// Reading Data from file.
    ifstream input;
    input.open(CM_File);

	// Reading number of elements.
    int m;
    input >> m;

    // Creating the Connectivity Matrix, Region arrays and Flag Boolean.
    int M[m][m];                                // Connectivity Matrix
    int A[m*m];                                 // 1D Connectivity Matrix.
    int R1[m/2], R2[m/2];                       // Two Regions.
    int R3[m/2], R4[m/2];                       // Starting Regions.
	// Paths Variables.
    int G, G1;
    int R5[m/2], R6[m/2];

	// Creating Counter.
    int *c = new int;
    *c = 0;

	// Reading The Connectivity Matrix.
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < m; j++)
        {
            input >> M[i][j];
            A[*c] = M[i][j];
            *c = *c + 1;
        }
    }
	// Deleting the Counter.
    delete c;
    // Closing the File.
    input.close();

	// Generate Initial Cut.
    Generate_Random_Cut(m, R1, R2, R3, R4);

	// KL Path.
    Path(m, A, R1, R2, R3, R4, G);

    // Saving Gain.
    G1 = G;

    // Checking New Path.
    while(Paths)
    {
        // Initializing for New Run.
        for(int i = 0; i < m/2; i++)
        {
            R1[i] = R3[i];
            R2[i] = R4[i];
            R5[i] = R3[i];
            R6[i] = R4[i];
        }

        // KL Path.
        Path(m, A, R1, R2, R3, R4, G);

		// Checking for New Path.
        if(G1 > G)
			Paths = false;
        else
            G1 = G;
    }

	// Writing Partitions in File.
	ofstream KL_Stream;
	KL_Stream.open(KL_File);

	// Writing Partition 1.
	KL_Stream << "Partition 1" << endl;
	for(int i = 0; i < m/2; i++)
		KL_Stream << R5[i] << endl;

	// Writing Partition 2.
	KL_Stream << "Partition 2" << endl;
	for(int i = 0; i < m/2; i++)
	{
		if (i < m/2 - 1)
			KL_Stream << R6[i] << endl;
		else
			KL_Stream << R6[i];
	}
	// Close Stream File.
	KL_Stream.close();
}

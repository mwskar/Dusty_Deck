
#include <stdio.h>
#include <math.h>

extern "C" double cputime_();
extern "C" double walltime_();

using namespace std;
#define MAXDIM 100



double conrand(double &seed);
void idcheck(int N, double &check, double AV[MAXDIM], double BV[MAXDIM], double ID[MAXDIM][MAXDIM]);
double trig (int i, int j);


int main ()
{
    int IA[MAXDIM];
    double AV[MAXDIM], BV[MAXDIM], CV[MAXDIM];
    double OP[MAXDIM][MAXDIM], ID[MAXDIM][MAXDIM];
    double AM[MAXDIM][MAXDIM], BM[MAXDIM][MAXDIM];
    double CM[MAXDIM][MAXDIM], DM[MAXDIM][MAXDIM];
    double check, BOT, TOP, HOLDA, HOLDB, TRACE3;
    float sum;
    int ival;
    int N = MAXDIM;

    double wall = walltime_();
    double cpu = cputime_();
    double seed = 1.0;


    
    //Fill arrays
    for (int i = 0; i < N; i++)
    {
        AV[i] = jn(0, (double)(conrand(seed) * pow(-1.0, int(10*conrand(seed)) % N ) ) );
    }


    for (int i = 0; i < N; i++)
    {
        BV[i] = jn(1, (double)(conrand(seed) * (double)pow(-1.0, int(10*conrand(seed)) % N ) ) );
    }


#ifdef removeIval
    check = 0.0;
    for (int i = 0; i < N; i++)
    {
        check = check + AV[i] * BV[i];
        idcheck(N, check, AV, BV, ID);
    }
#else
    check = 0.0;
    for (int i = 0; i < N; i++)
    {
        ival = N;
        check = check + AV[i] * BV[i];
        idcheck(ival, check, AV, BV, ID);
    }
#endif

    // |AV >< BV|

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            idcheck(N,check,AV,BV,ID);
            if (check > 0.5)
            {
                OP[i][j] = AV[i] * BV[j] / BV[i];
            }
            else
            {
                OP[i][j] = AV[j] * BV[i] / BV[j];
            }
        }
        IA[i] = i;
    }


    for (int i = 0; i < N; i++)
    {
        for(int j = 0; j <= (i+1); j = j + 8)
        {
            IA[i] = ((((i+1)+j) % N) % N) + 1;
        }
    }



    //Loop 20
    for (int i = 0; i < N; i++)
    {
        idcheck(N,check,AV,BV,ID);
        CV[IA[i]-1] = (AV[IA[i]-1] + BV[IA[i]-1]) / check;        
    }

    //Loop 30
    for (int i = 1; i < N; i++)
    {
        idcheck(N,check,AV,BV,ID);
        AV[i] = AV[i-1] * BV[i] + CV[i];
    }


#ifdef clean40
    for (int i = 0; i < N; i++)
    {
        idcheck(N,check,AV,BV,ID);
        for(int j = 0; j < N; j++)
        {
            BOT = OP[i][j];
            TOP = AV[j] * BV[j];
            HOLDA = AV[j];
            
            if (check > 0.5)
            {
                AV[j] = BV[j] + CV[j] / (TOP - BOT) * ID[i][i];
                BV[j] = HOLDA + CV[j] / (TOP - BOT) * ID[j][j];
                AM[i][j] = AV[j] * trig(IA[i], IA[j]);
                BM[i][j] = BV[j] * trig(IA[j], IA[i]);
            }
            else
            {
                AV[j] = BV[j] - CV[j] / (TOP - BOT) * ID[j][j];
                BV[j] = HOLDA - CV[j] / (TOP - BOT) * ID[i][i];
                AM[i][j] = AV[j] / trig(IA[i], IA[j]);
                BM[i][j] = BV[j] / trig(IA[j], IA[i]);
            }
        }
    }
#else
    //Loop 40
    for (int i = 0; i < N; i++)
    {
        idcheck(N,check,AV,BV,ID);
        for(int j = 0; j < N; j++)
        {
            if (check > 0.5)
            {
                BOT = OP[i][j];
                TOP = AV[j] * BV[j];
                HOLDA = AV[j];
                AV[j] = BV[j] + CV[j] / (TOP - BOT) * ID[i][i];
                BV[j] = HOLDA + CV[j] / (TOP - BOT) * ID[j][j];
                AM[i][j] = AV[j] * trig(IA[i], IA[j]);
                BM[i][j] = BV[j] * trig(IA[j], IA[i]);
            }
            else
            {
                BOT = OP[i][j];
                TOP = AV[j] * BV[j];
                HOLDA = AV[j];
                AV[j] = BV[j] - CV[j] / (TOP - BOT) * ID[j][j];
                BV[j] = HOLDA - CV[j] / (TOP - BOT) * ID[i][i];
                AM[i][j] = AV[j] / trig(IA[i], IA[j]);
                BM[i][j] = BV[j] / trig(IA[j], IA[i]);
            }
        }
    }
#endif

    //Loop 50
#ifdef break50
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            CM[i][j] = 0.0;
            for (int k = 0; k < N; k++)
            {
                CM[i][j] = CM[i][j] + AM[i][k] * BM[k][j] / check;
            }
        }

        for (int j = i+1; j < N; j++)
        {
            CM[i][j] = 0.0;
            for (int k = 0; k < N; k++)
            {
                CM[i][j] = CM[i][j] - AM[i][k] * BM[k][j] / check;
            }
        }
    }
#elif break50rm
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            for (int k = 0; k < N; k++)
            {
                CM[i][j] = CM[i][j] + AM[i][k] * BM[k][j] / check;
            }
        }

        for (int j = i+1; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                CM[i][j] = CM[i][j] - AM[i][k] * BM[k][j] / check;
            }
        }
    }
#elif removeSet
    for (int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                if (i < j)
                {
                    CM[i][j] = CM[i][j] - AM[i][k] * BM[k][j] / check;
                }
                else
                {
                    CM[i][j] = CM[i][j] + AM[i][k] * BM[k][j] / check;
                }
            }
        }
    }
#else
    for (int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            CM[i][j] = 0.0;
            for (int k = 0; k < N; k++)
            {
                if (i < j)
                {
                    CM[i][j] = CM[i][j] - AM[i][k] * BM[k][j] / check;
                }
                else
                {
                    CM[i][j] = CM[i][j] + AM[i][k] * BM[k][j] / check;
                }
            }
        }
    }
#endif

    //Loop 60
    for (int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            sum = 0.0;
            for (int k = 0; k < N; k++)
            {
                sum = sum + CM[i][k] * AM[j][k];
            }
            DM[i][j] = sum;
        }
    }



    for (int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            CM[i][j] = DM[i][j];
        }
    }



    //Loop 70
    for (int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            sum = 0.0;
            for (int k = 0; k < N; k++)
            {
                sum = sum - CM[i][k] * BM[j][k];
            }
            DM[i][j] = sum;
        }
    }



    HOLDA = fabs(AM[0][0]);
    HOLDB = fabs(BM[0][0]);
    for (int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            HOLDA = max(HOLDA, fabs(AM[i][j]));
            HOLDB = max(HOLDB, fabs(BM[i][j]));
        }
    }
    TRACE3 = 0.0;


    for (int i = 0; i < N; i++)
    {
        TRACE3 = TRACE3 + (AM[IA[i]-1][IA[i]-1] + BM[IA[i]-1][IA[i]-1]
                                 - DM[IA[i]-1][IA[i]-1]) / (HOLDA * HOLDB);
    }
    

    cpu = cputime_() - cpu;
    wall = walltime_() - wall;

    printf("final trace = %15.18f     IDCHECK = %15.18f\n", TRACE3, check);
    printf("-- RUNTIME -> %f seconds\n", cpu);
    

    
    return 0;
}


double trig (int i, int j)
{
    double x,y,z,pi;
    
    pi = acos(-1.0f);
    x = double(i) - double(j);
    y = double(i) + double(j);
    z = exp( sin(sqrt( pow(x,2) + pow(y,2) ) *pi ) );
    return x + y + log10(abs(1+z+(x*y*z)))/ (abs(x)+abs(y));
}

void idcheck(int N, double &check, double AV[MAXDIM], double BV[MAXDIM], double ID[MAXDIM][MAXDIM])
{
    double l2, check2, a,b,c,d;
    #ifdef moveI
        double Ihold;
    #elif unrollRemoveMove
        double Ihold;
    #endif

#ifdef removeIfs
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == j)
            {
                ID[i][j] = ((AV[i] * BV[j]) / abs(AV[i] * BV[i]));
                //printf("After ifs\n");
            }
            else if (i != j)
            {
                ID[i][j] = cos(check + (2.0f*((float)(i+1))*acos(-1.0f) ) / N) 
                                + 2.0f*sin(check+ (2.0f*((float)(j+1))*acos(-1.0f)/N) );
            }
        }
    }
#elif unrollJ
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < i; j++)
        {
            ID[i][j] = cos(check + (2.0f*((float)(i+1))*acos(-1.0f) ) / N)
                            + 2.0f*sin(check+ (2.0f*((float)(j+1))*acos(-1.0f)/N) );
        }

                if ((AV[i] < 0) && (BV[i] < 0))
                {
                    ID[i][i] = 1.0;
                }
                else if ((AV[i] < 0) && (BV[i] > 0))
                {
                    ID[i][i] = -1.0;
                }
                else if ((AV[i] > 0) && (BV[i] < 0))
                {
                    ID[i][i] = -1.0;
                }
                else
                {
                    ID[i][i] = 1.0;
                }

        for (int j = i + 1; j < N; j++)
        {
            ID[i][j] = cos(check + (2.0f*((float)(i+1))*acos(-1.0f) ) / N)
                        + 2.0f*sin(check+ (2.0f*((float)(j+1))*acos(-1.0f)/N) );
        }
    }
#elif unrollAndRemove
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < i; j++)
        {
            ID[i][j] = cos(check + (2.0f*((float)(i+1))*acos(-1.0f) ) / N)
                            + 2.0f*sin(check+ (2.0f*((float)(j+1))*acos(-1.0f)/N) );
        }

        ID[i][i] = ((AV[i] * BV[i]) / abs(AV[i] * BV[i]));
        
        for (int j = i + 1; j < N; j++)
        {
            ID[i][j] = cos(check + (2.0f*((float)(i+1))*acos(-1.0f) ) / N)
                        + 2.0f*sin(check+ (2.0f*((float)(j+1))*acos(-1.0f)/N) );
        }
    }
#elif moveI

    for (int i = 0; i < N; i++)
    {
        Ihold = cos(check + (2.0f*((float)(i+1))*acos(-1.0f) ) / N);
        for (int j = 0; j < N; j++)
        {
            if (i == j)
            {
                // ID[i][j] = 1.0 * sign(AV[i] * BV[j])
                //printf("Before ifs\n");
                if ((AV[i] < 0) && (BV[j] < 0))
                {
                    ID[i][j] = 1.0;
                }
                else if ((AV[i] < 0) && (BV[j] > 0))
                {
                    ID[i][j] = -1.0;
                }
                else if ((AV[i] > 0) && (BV[j] < 0))
                {
                    ID[i][j] = -1.0;
                }
                else
                {
                    ID[i][j] = 1.0;
                }
                //printf("After ifs\n");
            }
            else if (i != j)
            {
                ID[i][j] = Ihold + 2.0f*sin(check+ (2.0f*((float)(j+1))*acos(-1.0f)/N) );
            }
        }
    }
#elif unrollRemoveMove
    for (int i = 0; i < N; i++)
    {
        Ihold = cos(check + (2.0f*((float)(i+1))*acos(-1.0f) ) / N);
        for (int j = 0; j < i; j++)
        {
            ID[i][j] = Ihold + 2.0f*sin(check+ (2.0f*((float)(j+1))*acos(-1.0f)/N) );
        }

        ID[i][i] = ((AV[i] * BV[i]) / abs(AV[i] * BV[i]));
        
        for (int j = i + 1; j < N; j++)
        {
            ID[i][j] = Ihold + 2.0f*sin(check+ (2.0f*((float)(j+1))*acos(-1.0f)/N) );
        }
    }
#else
    //printf("Before big loops\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == j)
            {
                // ID[i][j] = 1.0 * sign(AV[i] * BV[j])
                //printf("Before ifs\n");
                if ((AV[i] < 0) && (BV[j] < 0))
                {
                    ID[i][j] = 1.0;
                }
                else if ((AV[i] < 0) && (BV[j] > 0))
                {
                    ID[i][j] = -1.0;
                }
                else if ((AV[i] > 0) && (BV[j] < 0))
                {
                    ID[i][j] = -1.0;
                }
                else
                {
                    ID[i][j] = 1.0;
                }
                //printf("After ifs\n");
            }
            else if (i != j)
            {
                ID[i][j] = cos(check + (2.0f*((float)(i+1))*acos(-1.0f) ) / N) 
                                + 2.0f*sin(check+ (2.0f*((float)(j+1))*acos(-1.0f)/N) );
            }
        }
    }

#endif

    l2 = 0.0;
    for (int i = 0; i < N; i++)
    {
        l2 = l2 + pow(AV[i],2.0);
    }
    
    l2 = sqrt(l2);
    for (int i = 0; i < N; i++)
    {
        AV[i] = AV[i] / l2;
    }
    
    l2 = 0.0;
    for (int i = 0; i < N; i++)
    {
        l2 = l2 + pow(BV[i],2.0);
    }
    
    l2 = sqrt(l2);
    for (int i = 0; i < N; i++)
    {
        BV[i] = BV[i] / l2;
    }


    a = 0.0d;
    b = 0.0d;
    c = 0.0d;
    d = 0.0d;


    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                switch (int(((i+j+k + 3) % 4) + 1))
                {
                    case 1:
                        a = a + AV[i] * BV[j] * ID[j][k];
                        check = check + a;
                        break;
                    case 2:
                        b = b + AV[j] * BV[i] * ID[k][j];
                        check = check - b;
                        break;
                    case 3:
                        c = c - AV[i] * BV[j] * ID[k][j];
                        check = sqrt(pow(b,2.0f) + pow(c,2.0f));
                        break;
                    case 4:
                        d = d - AV[j] * BV[i] * ID[j][k];
                        check2 = a + b + c + d;
                        break;
                }
            }
        }
    }


    check = fmin(abs(check2),abs(check)) / fmax(abs(check2),abs(check)) ;
}


double conrand(double &seed)
{
    double m,a, temp;
    a = 16807.0;
    m = 2147483647.0;
    temp = a *seed;
    seed = temp - m * int(temp/m);
    //printf("In function %f\n", seed / m);
    return seed / m;
}

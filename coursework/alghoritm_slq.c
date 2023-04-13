#include <stdio.h> 
#include <process.h> // for getpid(); because random doesn`t working correctly without him. 
#include <malloc.h> // for dynamic structure data.

typedef unsigned int uint; // for array size;

typedef struct Matrix {
    uint rows, cols;
    int **data;
} Matrix; 

typedef struct MatrixFloat {
    uint rows, cols;
    float **data;
} MatrixFloat; 

typedef struct Array { 
    uint size; 
    int *data; 
} Array;

typedef struct ArrayFloat { 
    uint size; 
    float *data; 
} ArrayFloat;

int TraceMode;

void Problem(int code); 
void Solve(MatrixFloat *A, ArrayFloat *B, short IsHomogeneouss); 

float f_abs(float x) 
{ 
    if ( x > 0 ) return x; 
    if ( x < 0 ) return x*(-1.0);
}

int _abs(int x) 
{ 
    if ( x > 0 ) return x; 
    if ( x < 0 ) return x*(-1);
}

uint matrix_sizes[23][2] = { 

    {2,2},    // 1
    {3,3},    // 2
    {4,4},    // 3
    {5,5},    // 4
    {6,6},    // 5
    {7,7},    // 6
    {8,8},    // 7
    {9,9},    // 8

    ////// Матрицы M x M 

    {2,3},    // 10
    {3,4},    // 11
    {5,6},    // 12
    {6,7},    // 13
    {7,8},    // 14
    {8,9},    // 15
    {9,10},   // 16
    ////// Матрицы M x M+3

}; 
///////////////////////////////////////////////// Pre Init
int iter_count = 0;

void Bench() { iter_count++; }
void PrintBench() { printf("%i\n", iter_count); } 
void BenchClear() { iter_count = 0; }

/////////////////////////////////////////////////

// Small Things; 
    Array *InitIntArray(uint size) 
    {   
        Array *self = calloc(size, sizeof(Array)); 
        self->size = size; 

        self->data = calloc(size, sizeof(int)); 
        return self; 
    }

    ArrayFloat *InitFloatArray(uint size) 
    {   
        ArrayFloat *self = calloc(size, sizeof(ArrayFloat)); 
        self->size = size; 

        self->data = calloc(size, sizeof(float)); 
        return self; 
    }

    Matrix *InitIntMatrix(uint size_arr_m, uint size_arr_n)
    {           
        Matrix *self = malloc(sizeof(Matrix)); 

        self->rows = size_arr_m; 
        self->cols = size_arr_n;  

        int **data = calloc(size_arr_m, sizeof(int*)); 

        for(int i = 0; i < size_arr_m; i++) 
        {
            data[i] = (int *)calloc(size_arr_n, sizeof(int));
        }

        self->data = data; 
        return self;
    }

    MatrixFloat *InitFloatMatrix(uint size_arr_m, uint size_arr_n)
    {           
        MatrixFloat *self = malloc(sizeof(MatrixFloat)); 

        self->rows = size_arr_m; 
        self->cols = size_arr_n;  

        float **data = calloc(size_arr_m, sizeof(float*)); 

        for(int i = 0; i < size_arr_m; i++) 
        {
            data[i] = (float *)calloc(size_arr_n, sizeof(float));
        }

        self->data = data; 
        return self;
    }

    void PrintIntArray(Array *a) 
    {  
        for ( uint i = 0; i < a->size; i++ )
            printf("%i\t", a->data[i]);

        printf("\n");
    }

    void PrintFloatArray(ArrayFloat *a) 
    {  
        for ( uint i = 0; i < a->size; i++ )
            printf("%f\t", a->data[i]);

        printf("\n");
    }

    void PrintIntMatrix(Matrix *m) 
    { 
        for( uint i = 0; i < m->rows; i++ ) 
        { 
            for ( uint k = 0; k < m->cols; k++ ) 
            { 
                printf("%i\t", m->data[i][k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    void PrintFloatMatrix(MatrixFloat *m) 
    { 
        for( uint i = 0; i < m->rows; i++ ) 
        { 
            for ( uint k = 0; k < m->cols; k++ ) 
            { 
                printf("%f\t", m->data[i][k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    void FreeMatrix(MatrixFloat *m) 
    { 
        for (uint i = 0; i < m->cols; i++)
            free(m->data[i]);

        free(m->data);        
    } 

    void swap_arr_el(ArrayFloat *a, int x, int y) 
    { 
        double temp = a->data[x];
        a->data[x] = a->data[y];
        a->data[y] = temp;
    } 
     
    void swap_arr_str(ArrayFloat *a1, ArrayFloat *a2)
    {
        int cache; 
        for(int i = 0; i < a1->size; i++) 
        { 
            cache = a1->data[i];
            a1->data[i] = a2->data[i];
            a2->data[i] = cache;  
        }
    }

    void swap_arr_col(MatrixFloat *m, ArrayFloat *a, uint column)
    {
        int cache; 
        for(int i = 0; i < a->size; i++) 
        { 
            cache = m->data[i][column];
            m->data[i][column] = a->data[i];
            a->data[i] = cache;  
            Bench();
        }
    }

    void swap_matrix_str(MatrixFloat *A, int idx, int idx2)
    {
        for (int j = 0; j < A->cols; j++) 
        {
            double temp = A->data[idx2][j];
            A->data[idx2][j] = A->data[idx][j];
            A->data[idx][j] = temp;
            Bench();
        }
    }

    int fRandom(int a, int b) { return a + rand() % ( b - a + 1 ); } 

    void GenerateLinearEquations(MatrixFloat *A, ArrayFloat *B, int a_min, int a_max, short IsHom )
    {  
        for( uint i = 0; i < A->rows; i++ )
        {  
            for ( uint j = 0; j < A->cols; j++ ) 
                A->data[i][j] = fRandom(a_min, a_max); 

            B->data[i] = (IsHom == 1) ? 0 : fRandom(a_min, a_max); 
        }
    }
////////////////////////////////////////////////////////////////////////// Stable Funcs;   

/* 
    Метод крамера. Метод крамера работает только для матриц вида M x M. Связано это с тем, что определитель матрицы определён 
    только для квадратных матриц. 
*/ 
int detMatrix(MatrixFloat *m) // разложение по строке.
{ 
    if (m->cols == 1)
        return m->data[0][0];
    else if (m->cols == 2)
        return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
    else 
    {   
        int sign = 1; 
        int d = 0;
        for (int k = 0; k < m->cols; k++) {
            MatrixFloat *b = InitFloatMatrix(m->cols-1, m->cols-1);
            for (int i = 1; i < m->cols; i++) {
                for (int j = 0; j < m->cols; j++) {
                    if (j == k)
                        continue;
                    else if (j < k)
                        b->data[i-1][j] = m->data[i][j];
                    else
                        b->data[i-1][j-1] = m->data[i][j];
                    
                    Bench();
                }
            }
             
            d += sign * m->data[0][k] * detMatrix(b);
            sign *= (-1); 
            FreeMatrix(b);
        }
        return d;
    }
}

int rankMatrix(MatrixFloat *A) // Gauss Rank
{ 
    int rank = 0;
    for(uint i = 0; i < A->rows; i++ )
    { 
        int flag = 1; 
        for (uint j = 0; j < A->cols; j++ ) 
        { 
            if ( A->data[i][j] != 0 ) { flag = 0; break; } 
        }
        if (flag == 0) 
            rank++;
    }
    return rank;
} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Solving methods
ArrayFloat *M_Kramer(MatrixFloat *A, ArrayFloat *B) 
{ 
    int detA = detMatrix(A); 
    
    if ( TraceMode ) { 
        printf("////////////////////METHOD KRAMER(A)////////////////////\n");
        printf("detA = %i\n", detA); 
    }

    ArrayFloat *solve = InitFloatArray(A->rows); 

    for(uint i = 0; i < A->rows; i++) 
    { 
        swap_arr_col(A, B, i); 
        int detXn = detMatrix(A);  

        if ( detA == 0 && detXn != 0 ) {
            printf("The system is incompatible \n");
            for(int i = 0; i < solve->size; i++ ) 
            { 
                solve->data[i] = 0; 
            }
            break;
        }

        solve->data[i] = detXn * 1.0 / detA; ;

        if ( TraceMode ) 
        { 
            printf("TraceSwap\n"); 
            PrintFloatMatrix(A); 
            printf("detA[%i] = %i\n", i, detXn); 
        }
        
        swap_arr_col(A, B, i); 

        Bench();
    }

    if ( TraceMode ) 
        printf("////////////////////METHOD KRAMER(A)////////////////////\n");
        
    return solve;
}

ArrayFloat *M_Gauss(MatrixFloat *A, ArrayFloat *B) 
{
    if ( TraceMode ) { 
        printf("////////////////////METHOD GAUSSA(A)////////////////////\n");
    }

    ArrayFloat *solve = InitFloatArray(A->rows);
    float max;

    int k, index, flag_1;
    k = 0;
    while (k < A->rows) 
    {
        // Поиск строки с максимальным a(ij)
        max = f_abs(A->data[k][k]);
        index = k;
        for (int i = k + 1; i < A->rows; i++) 
        {
            if (f_abs(A->data[i][k]) > max)
            {
                max = f_abs(A->data[i][k]);
                index = i;
                Bench();
            }
        }
        // Перестановка строк
        if (max < 0) 
        {
            flag_1 = 1; 
            printf("Gauss failed\nIndex: %i\n", index); 
            break;
        }
        swap_matrix_str(A,index,k); 

        swap_arr_el(B, k, index);
        for (int i = k; i < A->rows; i++) 
        {
            double temp = A->data[i][k];

            for (int j = 0; j < A->rows; j++) 
                A->data[i][j] = A->data[i][j] / temp;

            B->data[i] = B->data[i] / temp;

            if (i == k)  continue; // уравнение не вычитать само из себя

            for (int j = 0; j < A->rows; j++)
            {
                A->data[i][j] = A->data[i][j] - A->data[k][j];
                Bench();
            }
              

            B->data[i] = B->data[i] - B->data[k];
        }
        k++;
        if ( TraceMode ) {  
            PrintFloatMatrix(A);
            PrintFloatArray(B); 
        } 
    }
    int matrixRank = rankMatrix(A); 
    if ( TraceMode ) {  
        printf("After straight running:\n");
        PrintFloatMatrix(A);
        PrintFloatArray(B); 
        printf("Matrix Rank: %i\n",matrixRank); 
    } 
    int flag_2 = 0; 
    for (uint i = 0; i < A->rows; i++ ) 
    { 
        if ( matrixRank < i ) 
        { 
            if ( B->data[i] != 0 ) 
            { 
                printf("The system is incompatible \n");
                return solve; 
            }
        }
        Bench();
    }
    for (uint i = 0; i < A->cols; i++ ) 
    { 
        if ( matrixRank <= i ) 
        { 
            printf("The system is have many solves \n");
        }
        Bench();
    }       
    // обратная подстановка
    for (k = A->rows - 1; k > -1; k--)
    {
        solve->data[k] = B->data[k];

        for (int i = 0; i < k; i++)
        { 
            B->data[i] = B->data[i] - A->data[i][k] * solve->data[k];
            Bench();
        }
    }

    if ( TraceMode ) { 
        printf("////////////////////METHOD GAUSSA(A)////////////////////\n");
    }
    
    return solve;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() 
{ 
    srand(getpid()); 

    printf("Trace mode? 1/0 (Y/N) "); scanf("%i",&TraceMode); // Типа для отладки

    short IsHomogeneous;
    printf("Generate a homogeneous system? 1/0 (Y/N) "); scanf("%i",&IsHomogeneous); // Однородная СЛАУ
    /////////////////////////////////////////////////////////////////////////////////////////////////// Prep; 
        MatrixFloat *A = InitFloatMatrix(matrix_sizes[3][0], matrix_sizes[3][1]); // Хранит коэф     X(n)
        ArrayFloat *B = InitFloatArray(matrix_sizes[3][0]);                       // Хранит значение B(n)

        Solve(A, B, IsHomogeneous); // Solving SLQ;

        printf("[%i x %i]",matrix_sizes[3][0], matrix_sizes[3][1]);
        PrintBench();
        BenchClear();
    return 0; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Solve(MatrixFloat *A, ArrayFloat *B, short IsHomogeneous)
{
    GenerateLinearEquations(A, B, 0, 100, IsHomogeneous);  //Generate SLQ; 

    PrintFloatMatrix(A);
    PrintFloatArray(B);

    ArrayFloat *solve; 
    
    //if( A->rows == A->cols )
    //{
    //    solve = M_Kramer(A, B);     //
    //    PrintFloatArray(solve);
    //} 

    solve = M_Gauss(A,B); 

    PrintFloatArray(solve);

    FreeMatrix(A); 
    free(B->data);
    free(solve->data);
}


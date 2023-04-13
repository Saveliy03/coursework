#include <stdio.h> 
#include <math.h> 

#define true 1 
#define false 0 
#define PI 3.14
struct Circle { 
    int x,y,r;
}; 
typedef struct Circle Circle; 

int IsIntersection(Circle c1, Circle c2) // Проверяет пересечение двух окружностей
{ 
    int distance = sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2) ); 

    if ( distance < (c1.r + c2.r) ) 
        return true; 
    else 
        return false;
}

const int N = 3; 
Circle arr[] = {    
    {7,11,3}, {10,15,4}, {11,10,3} 
};
int main() 
{   

    // Часть А
    // Используем Формулу пика 
    // Для этого нужно разметить квадрат зоны акватории(пусть будет от -1000 до 1000 в ширину и от -1000 до 1000 в длинну)

    int N1 = 0; // Количество внешних целых точек (точек, где f(x) = 0)
    int N2 = 0; // Количество внутренних точек
    for(int x = -1000; x <= 1000; x++) 
    { 
        for(int y = -1000; y <= 1000; y++) 
        { 
            for (int i = 0; i <= N; i++)
            { 
                if (pow((x - arr[i].x),2) + pow((y - arr[i].y),2 ) == pow(arr[i].r,2) ) 
                {
                    N1++;
                } 
                if (pow((x - arr[i].x),2) + pow((y - arr[i].y),2 ) <= pow(arr[i].r,2) )
                { 
                    N2++;
                }
            }
        }
    }
    
    printf("%i %i\n", N1, N2); 
    printf("Otvet(a): %i\n", (N2 + N1/2 - 1));

    // Часть B 
    int Ax = 1;
    int Ay = 0;
    int Bx = 9;
    int By = 18; 

    int vec_x = (Bx - Ax); // Сам вектор движения X // в формуле это a
    int vec_y = (By - Ay); // Сам вектор движения Y // в формуле это b

    // дальше всё по формуле 
    int summ_detect = 0; // Количество катеров, которые заметят объект
    for (int i = 0; i < N; i++) 
    { 
        int b = Ax - arr[i].x;
        int c = Ay - arr[i].y;

        int A = pow(vec_x,2) + pow(vec_y,2);
        int B = 2*(vec_x*b + vec_y*c);
        int C = pow(b,2) + pow(c,2) - pow(arr[i].r,2);
        
        if( ( pow(B,2) - 4*A*C ) >= 0 ) 
        {
            printf("Curcle %i\n",i); 
            summ_detect++;
        }
    }
    
    printf("%i\n",summ_detect);

}
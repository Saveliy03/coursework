#include <stdio.h> 
#include <math.h> 
#include <stdlib.h>

#define true 1 
#define false 0 
#define PI 3.14

struct Kater { 
    int x,y;
};
 
typedef struct Kater Kater; 

const int N = 6; 
Kater arr[] = {    
    {6,21}, {7,12}, {14,9}, {18,15}, {18,32}, {39,33} 
};
int IsKater(int x,int y) // Функция определяет. являются ли координаты координатами катера. 
{
    int b = false;
    for(int i = 0; i < N; i++) 
    { 
        if ( arr[i].x == x && arr[i].y == y ) b = true; 
    }

    return b;
}
int main() 
{    
    int patrol_v = 1; 

    int intruder_x = 5; 
    int intruder_y = 6; 
    int intruder_v = 52130; 

    // По условию задачи катер-нарушитель движется прямолинейно. 
    // Оптимальным будет тот маршрут, где катер-нарушитель будет приближаться, 
    // как можно меньше к позициям патруля
    // При этом должно учитываться, что до ближайшей точки пересечения катер-нарушитель доедет быстрее, чем патрульный катер. 
    // И соответственно, самый кратчайший путь 

    // Задача делится на два случая, где он стремится к оси y, дабы пересечь черту в y > 100 и наоборот, где ему нужно пересечь x > 100 при всех y 

    // Случай 1
    
    float min_len = 10000.0; // Определяем самый кратчайший путь. Ибо
    float min_len_cache = min_len; 
    
    int coord_x = 0; // Для сохранения пути для случая 1. 
    int coord_y = 0; // 
    // Т.е. нам к каждой прямой движения катера-нарушителя нужно построить нормаль и далее вычислить её длинну.
    for(int x = intruder_x; x <= 100; x++) 
    { 
        int vect_i_x = x - intruder_x; 
        int vect_i_y = 100 - intruder_y; 

        int succes = false; 

        for(int i = 0; i < N; i++ ) 
        { 
            float distance = abs( (vect_i_y*(arr[i].x-intruder_x)) - (arr[i].y - intruder_y)*vect_i_x ) / sqrt(pow(vect_i_x,2)+pow(vect_i_y,2));  

            float time_patrol = distance / patrol_v*1.0;    // Время, которое нужно для преодоления минимального расстояния. 
            float time_intrud = distance / intruder_v*1.0; 
            // Если он преодолеет то же расстоение за меньшее время, то маршрут можно считать недействительным. 
            // ПОЯСНЕНИЕ: катер движется линейно -> тут не нужно учитывать горькие виляния патруля и т.д. 

            if ( time_intrud > time_patrol ) continue; 

            succes = true;
        } 
        if ( succes == true ) 
        {   
            float len = sqrt(pow(vect_i_x,2) + pow(vect_i_y,2));
            if ( min_len < len ) 
            { 
                min_len = len; 
            }
        }
    }
    
    // Случай 2 

    for(int y = intruder_x; y <= 100; y++) 
    { 
        int vect_i_x = 100 - intruder_x; 
        int vect_i_y = y - intruder_y; 

        int succes = false; 

        for(int i = 0; i < N; i++ ) 
        { 
            int distance = abs( (vect_i_y*(arr[i].x-intruder_x)) - (arr[i].y - intruder_y)*vect_i_x ) / sqrt(pow(vect_i_x,2)+pow(vect_i_y,2));  

            float time_patrol = distance / patrol_v*1.0;    // Время, которое нужно для преодоления минимального расстояния. 
            float time_intrud = distance / intruder_v*1.0; 
            // Если он преодолеет то же самое время, то маршрут можно считать недействительным. 
            // ПОЯСНЕНИЕ: катер движется линейно -> тут не нужно учитывать горькие виляния патруля и т.д. 

            if ( time_intrud > time_patrol ) continue; 

            succes = true;
        } 
        if ( succes == true ) 
        {   
            float len = sqrt(pow(vect_i_x,2) + pow(vect_i_y,2));
            if ( min_len < len ) 
            { 
                min_len = len; 
            }
        }
    }

    printf("%f\n", min_len); 

    // Ответ на вопрос B кроется в вопросе A; 
    // Если нет никакого оптимального маршрута -> катер догонят. все. 

    // Часть B 

    if ( min_len == min_len_cache ) 
    { 
        printf("Yes");
    }
}
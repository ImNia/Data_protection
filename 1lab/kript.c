#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEST 10

/* Функция быстрого возведения числа в степень по модулю.
 * На вход получает основание, степень в которую возводят
 * и модуль по которому берется.
 * На выходе результат возыедения числа в степень по модулю.
 * */
int module_power(int base, int power_exponent, int moduli)
{
    int help = base;
    int result = 1;

    while(power_exponent != 0){
        if((power_exponent & 1) == 1){
            result = result * (help % moduli);
        }
        help = help * (help % moduli);

        power_exponent = power_exponent >> 1;
    }
    result = result % moduli;

    return result;
}

/* Функция, реализующая обобщенный алгоритм Евклида. 
 * Функция принимает два значение наименьший знаменатель
 * которых нужно найти.
 * Функция возвращает указатель на массив в котором хранится НОД
 * и коффициенты для вычисления НОДа по формуле a*x+b*y = HOД
 * */
int *evklid(int _older, int _junior)
{
    int intermediate[3];
    if(_junior > _older){
        intermediate[0] = _older;
        _older = _junior;
        _junior = _older;
    }
   
    int *older = (int*)malloc(4);
    int *junior = (int*)malloc(4);
    older[0] = _older;
    older[1] = 1;
    older[2] = 0;
    junior[0] = _junior;
    junior[1] = 0;
    junior[2] = 1;

    int q;
    while(junior[0] != 0){
        q = older[0] / junior[0];
        intermediate[0] = older[0] % junior[0];
        intermediate[1] = older[1] - q * junior[1];
        intermediate[2] = older[2] - q * junior[2];
        for(int i = 0; i < 3; i++){
            older[i] = junior[i];
            junior[i] = intermediate[i];
        }
    }

    return older;
}

/* Проверка числа на простоту, с помощью теста Ферма.
 * На вход принимает число. На выходе 0 - число простое,
 * 1 - число не простое.
 * */
int test_prime(int number)
{
    int count = number - 1;
    int help = 1;
    for(int test_number = 2; test_number < TEST; test_number++){
        while(count > 0){
            help *= test_number;
            count--;
        }
        if(help % number != 1)
            return 1;
    }
    return 0;
}

/* Генерирование p числа.
 * */
int p_generation()
{
    srand(time(NULL));
    int p;
    while(1){
        p = 10 + rand() % 1000;
        if(test_prime(p) == 1)
            continue;
        else{
            p = (p*p*p - (p+1)*(p+1)*(p+1))/(p-(p+1));
            return p;
        }
    }
}

/* Генерирование g числа.
 * */
int g_generation(int p)
{
    srand(time(NULL));
    int g;
    while(1){
        g = 1 + rand() % p;
        if(test_prime(g) == 1)
            continue;
        else{
            return g;
        }
    }
}

int main()
{
    int p, g;
    p = p_generation();
    g = g_generation(p);
    printf("p = %d\n", p);
    printf("g = %d\n", g);

    int first_key_x, second_key_x;
    int first_key_y, second_key_y;
    first_key_x = 1000 + rand() % 10000;
    second_key_x = 1000 + rand() % 10000;
    printf("first_key_x = %d\n", first_key_x);
    printf("second_key_x = %d\n", second_key_x);
    
    first_key_y = module_power(g, first_key_x, p);
    second_key_y = module_power(g, second_key_x, p);

    printf("first_y = %d\n", first_key_y);
    printf("second_y = %d\n", second_key_y);
    int first_key, second_key;
    first_key = module_power(second_key_y, first_key_x, p);
    second_key = module_power(first_key_y, second_key_x, p);

    printf("first = %d\n", first_key);
    printf("second = %d\n", second_key);

    return 0;
}
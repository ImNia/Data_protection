#include "kript.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct listnode *hashtab[HASHTAB_SIZE];

/* Функция быстрого возведения числа в степень по модулю.
 * На вход получает основание, степень в которую возводят
 * и модуль по которому берется.
 * На выходе результат возыедения числа в степень по модулю.
 * */
long long module_power(long long base, long long power_exponent, long long moduli)
{
    long long help = base;
    long long result = 1;

    while(power_exponent != 0){
        if((power_exponent & 1) == 1){
            result = (result * help) % moduli;
        }
        help = (help * help) % moduli;

        power_exponent = power_exponent >> 1;
    }

    return result;
}

/* Функция, реализующая обобщенный алгоритм Евклида. 
 * Функция принимает два значение наименьший знаменатель
 * которых нужно найти.
 * Функция возвращает указатель на массив в котором хранится НОД
 * и коффициенты для вычисления НОДа по формуле a*x+b*y = HOД
 * */
long long *evklid(long long _older, long long _junior)
{
    long long intermediate[3];
    if(_junior > _older){
        intermediate[0] = _older;
        _older = _junior;
        _junior = intermediate[0];
    }
   
    long long *older = (long long*)malloc(4);
    long long *junior = (long long*)malloc(4);
    older[0] = _older;
    older[1] = 1;
    older[2] = 0;
    junior[0] = _junior;
    junior[1] = 0;
    junior[2] = 1;

    long long q;
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
int test_prime(long long number)
{
    if(number == 2)
        return 0;
    long long help;
    long long *gcd = (long long*)malloc(4);
    for(int i = 0; i < 100; i++){
        help  = (rand() % (number - 2)) + 2;
        gcd = evklid(number, help);
        if(gcd[0] != 1)
            return 1;
        if(module_power(help, number - 1, number) != 1)
            return 1;
    }

    return 0;
}

/* Генерирование p числа.
 * */
int p_generation()
{
    int p;
    while(1){
        p = 10 + rand() % 1000;
        if(test_prime(p) == 1)
            continue;
        else
            return p; 
    }
}

/* Генерирование g числа.
 * */
int g_generation(int p)
{
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

/* Функция построения общего ключа для двух абонентов по схеме
 * Диффи-Хеллмана.
 * */
void diffie_hellman()
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
}

/*static int cmp(const void *p1, const void *p2){
    return *(int*)p1 - *(int*)p2;
}
*/
/* Функция, которая решает задачу нахождения дискретного логарифма
 * при помощи алгоритма "Шаг младенца, шаг великана".
 * Функция на вход получает основание числа которое возводится в степень,
 * число от которого берется модуль и результат этого выражения.
 * На выходе число в которое возводится основание числа.
 * */
int child_giant(long long base, long long moduli, long long answer)
{
    hashtab_init(hashtab);
    int m = sqrt(moduli) + 2;
    int k = sqrt(moduli) + 1;

//    printf("%d\n", m);
//    printf("%d\n", k);

    int *row_y = (int*)malloc(m + 1);
    int *row_a = (int*)malloc(k + 1);

    int pow_ay;
    int help;
    for(int i = 0; i < m; i++){
        help = i;
        pow_ay = 1;
        while(help > 0){
            pow_ay *= base;
            help--;
        }
        row_y[i] = (pow_ay * answer) % moduli;

        hashtab_add(hashtab, i, row_y[i]);
//        printf("big: %d\n", row_y[i]);
    }
    int x;
    for(int j = 1; j < k + 1; j++){
        row_a[j] = module_power(base, j * m, moduli);
//printf("little: %d\n", row_a[j]);
        for(int i = 0; i < m; i++){
            if(hashtab_lookup(hashtab, i, row_a[j]) != NULL){
                x = j * m - i;
                return x;
            }
        }
    }
    return 1;
}

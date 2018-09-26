#include "kript.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <sys/time.h>

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
void evklid(long long _older, long long _junior, long long *older)
{
    long long intermediate[3];
    if(_junior > _older){
        intermediate[0] = _older;
        _older = _junior;
        _junior = intermediate[0];
    }
   
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

    free(junior);
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
        evklid(number, help, gcd);
        if(gcd[0] != 1){
            free(gcd);
            return 1;
        }
        if(module_power(help, number - 1, number) != 1){
            free(gcd);
            return 1;
        }
    }
    free(gcd);

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
 * q - простое число, для вычисления g. Все по учебнику
 * */
int g_generation(int p)
{
    int g;
int help;
    int q = (p - 1) / 2;
    if(test_prime(q) == 1){
        return -1;
    }else{
        while(1){
            g = 1 + rand() % (p - 1);
            help = module_power(g, q, p);
            if(help != 1){
                printf("q = %d\n", q);
                return g;
            }else{
                continue;
            }
        }
    }
}

/* Функция построения общего ключа для двух абонентов по схеме
 * Диффи-Хеллмана.
 * */
void diffie_hellman()
{
    int p, g = -1;
    while(g == -1){
        p = p_generation();
        g = g_generation(p);
    }
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

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

/* Функция, которая решает задачу нахождения дискретного логарифма
 * при помощи алгоритма "Шаг младенца, шаг великана".
 * Функция на вход получает основание числа которое возводится в степень,
 * число от которого берется модуль и результат этого выражения.
 * На выходе число в которое возводится основание числа.
 * */
int child_giant(long long base, long long moduli, long long answer)
{
    hashtab_init(hashtab);
    struct listnode *node;
    int m = sqrt(moduli) + 2;
    int k = sqrt(moduli) + 1;

    printf("%d\t%d\n", m, k);

    int *row_y = (int*)malloc(m + 1);
    int *row_a = (int*)malloc(k + 1);

    double t;
    t = wtime();
    
    for(int i = 0; i < m; i++){
        row_y[i] = ((module_power(base, i, moduli)) * (answer % moduli)) % moduli;
        hashtab_add(hashtab, row_y[i], i);
    }
    int x;
    for(int j = 1; j < k + 1; j++){
        row_a[j] = module_power(base, j * m, moduli);
//        for(int i = 0; i < m; i++){
//            if(row_a[j] == row_y[i]){
        node = hashtab_lookup(hashtab, row_a[j]);
        if(node != NULL){
                x = j * m - node->value;
                t = wtime() - t;
//                printf("Time: %.6f sec.\n", t);
                return x;
            }
//        }
    }
    return -1;
}

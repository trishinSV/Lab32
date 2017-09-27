//
//  main.cpp
//  Lab32
//
//  Created by Сергей Тришин on 27.09.17.
//  Copyright © 2017 Сергей Тришин. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Функция сравнения двух строк.
int compare(char *c1, char *c2)
{
    int i = 0, result = 0;
    
    // Пока не дошли до конца какой либо строки и символы на соответствующих позициях равны
    while(c1[i] != '\0' && c2[i] != '\0' && !( (c1[i] - c2[i]) % ('a' - 'A') ))
    {
        i++;
    }
    
    // Если дошли до конца только первой строки, то говорим, что она меньше
    if(c1[i] == '\0' && c2[i] != '\0') result = 1;
    
    // Если не дошли до конца ни одной строки, то сравниваем текущие символы
    // они и определяют отношение строк
    if(c1[i] != '\0' && c2[i] != '\0') result = (c1[i] % 'A') % ('a' - 'A') < (c2[i] % 'A') % ('a' - 'A');
    
    // Если мы дошли до конца обеих строк, то они равны и результат должен быть равным нулю
    // Если дошли до конца второй строки, то первая строка больше и результат будет равен нулю.
    
    return result;
}

// Сливает содержимое src1 и src2 в dest с сохранением порядка (по size элементов)
void merge(char *src1, char *src2, char *dest, int size)
{
    int i, j;
    char *temp1, *temp2;
    FILE *f1, *f2, *f3;
    f1 = fopen(src1, "r");
    f2 = fopen(src2, "r");
    f3 = fopen(dest, "w");
    
    temp1 = (char*)malloc(256 * sizeof(char));
    temp2 = (char*)malloc(256 * sizeof(char));
    
    // Считываем первый строки
    fgets(temp1, 256, f1);
    fgets(temp2, 256, f2);
    i = 0;
    j = 0;
    // Пока не конец обоих файлов
    while( !(feof(f1) || feof(f2)) )
    {
        while( i < size && j < size)
        {
            // Если temp1 раньше по алфавиту, чем temp2
            if(compare(temp1, temp2))
            {
                // Записываем сначала temp1 и считываем следующую строку
                fputs(temp1, f3);
                fgets(temp1, 256, f1);
                i++;
                if(feof(f1)) i += size;
            }
            else
            {
                // иначе записываем сначала temp2 и считываем следующую строку
                fputs(temp2, f3);
                fgets(temp2, 256, f2);
                j++;
                if(feof(f2)) j += size;
            }
        }
        
        // Если закончился блок в f1, то записываем остаток блока из f2 в результирующий файл
        if( j < size )
        {
            fputs(temp2, f3);
            j++;
            
            while( j < size )
            {
                fgets(temp2, 256, f2);
                j++;
                // Конец файла проверяется изза специфики feof (чтобы не дублировались последнии строки).
                if( ! feof(f2) )
                    fputs(temp2, f3);
            }
            // сразу считываем первую строку следующего блока
            fgets(temp2, 256, f2);
        }
        else
        {
            // иначе записываем остаток блока из f1 в результирующий файл
            fputs(temp1, f3);
            i++;
            
            while( i < size )
            {
                fgets(temp1, 256, f1);
                i++;
                // Конец файла проверяется изза специфики feof (чтобы не дублировались последнии строки).
                if( ! feof(f1) )
                    fputs(temp1, f3);
            }
            // сразу считываем первую строку следующего блока
            fgets(temp1, 256, f1);
        }
        // Обнуляем счётчики (считываем слудеющий блок).
        i = 0;
        j = 0;
    }
    
    // Если закончился f1, то записываем остаток f2 в результирующий файл
    if( feof(f1) && !feof(f2))
    {
        fputs(temp2, f3);
        
        while( !feof(f2) )
        {
            fgets(temp1, 256, f2);
            if( !feof(f2) )
                fputs(temp1, f3);
        }
    }
    if( feof(f2) && !feof(f1) )
    {
        // иначе записываем остаток f1 в результирующий файл
        fputs(temp1, f3);
        
        while( !feof(f1) )
        {
            fgets(temp2, 256, f1);
            if( ! feof(f1) )
                fputs(temp2, f3);
        }
    }
    
    free(temp1);
    free(temp2);
    
    fclose(f1);
    fclose(f2);
    fclose(f3);
}
// Разделяем файл src в два файла поблочно размером size
void split(char *src, char *a, char *b, int size)                    //именно это не нравится преподавателю
{
    FILE *f1, *f2, *f3, *temp;
    char *temp1;
    int i, j;
    
    f1 = fopen(src, "r+");
    f2 = fopen(a, "w");
    f3 = fopen(b, "w");
    
    temp1 = (char*)malloc(256 * sizeof(char));
    i = 0;
    
    while( !feof(f1) )
    {
        fgets(temp1, 256, f1);
        if( !feof(f1) )
            fputs(temp1, f2);
        // Когда считаем и запишем size строк счётчик обнулится
        i = (i + 1) % size;
        // когда счётчик обнулится
        if( !i)
        {
            // файловые переменные поменяются местами и следующий блок будет записан в другой файл
            temp = f3;
            f3 = f2;
            f2 = temp;
        }
    }
    
    free(temp1);
    fclose(f1);
    fclose(f2);
    fclose(f3);
}

void merge_sort(char* filename)
{
    int i, N;
    char *s;
    FILE *f = fopen(filename, "r");
    
    s = (char*)malloc(256 * sizeof(char));
    N = 0;
    
    // Подсчитываем кол-во строк в файле
    while( !feof(f) )
    {
        fgets(s, 256, f);
        N++;
    }
    
    N--;
    
    fclose(f);
    // Начальный размер блока 1 (т.е. рассматривается каждый отдельный элемент посл-ти строк как уже отсортированная посл-ть длины 1).
    i = 1;
    // пока размер блока меньше кол-ва строк в файле
    while(i < N)
    {
        // Разделяем
        split(filename, "/Users/Sergey/Documents/Developer/Lab32/Lab32/temp1.txt", "/Users/Sergey/Documents/Developer/Lab32/Lab32/temp2.txt", i);
        // Сливаем
        merge("/Users/Sergey/Documents/Developer/Lab32/Lab32/temp1.txt", "/Users/Sergey/Documents/Developer/Lab32/Lab32/temp2.txt", filename, i);
        // исходный файл теперь содержит упорядоченные подпоследовательности длины i * 2 и в следующий раз мы будем уже работать с ними
        i *= 2;
    }
    // Удаляем вспомогательные файлы.
    remove("/Users/Sergey/Documents/Developer/Lab32/Lab32/temp1.txt");
    remove("/Users/Sergey/Documents/Developer/Lab32/Lab32/temp2.txt");
}

int main()
{
    merge_sort("/Users/Sergey/Documents/Developer/Lab32/Lab32/in.txt");
    
    return 0;
}

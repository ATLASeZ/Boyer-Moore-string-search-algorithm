#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

/* Класс "Строка" */
class String
{
private:
    char* m_string;                                                     // Указатель на символьный массив, которй будет содержать строку
    int m_lenght;                                                       // Длина строки

public:
    String(int lenght = 0);                                             // Конструктор (с параметром)
    String(const char* other);                                          // Конструктор копирования
    String(String& other);                                              // Конструктор, принимающий ссылку на объект класса String

    ~String();                                                          // Деструктор

    int GetLenght();                                                    // Геттер длины строки

    String& operator=(const String& other);                             // Оператор присваивания
    bool operator==(String& other);                                     // Оператор сравнения равенства
    bool operator!=(String& other);                                     // Оператор неравенства

    char& operator[](int key);                                          // Оператор доступа к элементу
    String operator()(int startIndex, int endIndex);                    // Оператор вызова функции

    String operator+(const String& other);                              // Оператор сложения
    String& operator+=(const String& other);                            // Оператор сложения с присваиванием 

    friend istream& operator>>(istream& in, String& other);             // Оператор потокового ввода
    friend ostream& operator<<(ostream& out, String& other);            // Оператор потокового вывода

    int BoyerMoore(String& substring);                                  // Алгоритм Бойера-Мура поиска подстроки
};

/* Конструктор (с параметром) */
String::String(int lenght)                                         
{
    /* Выделяем память для хранения строки плюс один символ для нулевого символа, обозначающего конец строки */
    m_string = new char[lenght + 1];
    /* Инициализируем длину строки нулем */
    m_lenght = 0;

    /* Копируем пустую строку в выделенную память */
    strcpy(m_string, "\0");
};

/* Конструктор копирования */
String::String(const char* other)                                    
{
    /* Выделяем память для хранения строки плюс один символ для нулевого символа, обозначающего конец строки */
    m_string = new char[strlen(other) + 1];
    /* Копируем переданную строку в выделенную память */
    strcpy(m_string, other);
    /* Вычисляем длину строки и сохраняем в переменной m_length */
    m_lenght = strlen(other);
};

/* Конструктор, принимающий ссылку на объект класса String */
String::String(String& other)           
{
    /* Выделяем память для хранения строки плюс один символ для нулевого символа, обозначающего конец строки */
    m_string = new char[other.m_lenght + 1];
    /* Копируем длину строки из объекта other в переменную m_length */
    m_lenght = other.m_lenght;
    /* Копируем строку из объекта other в выделенную память */
    strcpy(m_string, other.m_string);
};

/* Деструктор */
String::~String()
{
    /* Проверяем, что m_string не является нулевым указателем */
    if (m_string)
    {
        /* Освобождаем память, выделенную для m_string */
        delete[] m_string;
    }

    /* Устанавливаем указатель m_string в значение nullptr */
    m_string = nullptr;
}

/* Геттер длины строки */
int String::GetLenght()
{
    /* Возвращаем значение длины строки */
    return m_lenght;
}

/* Оператор присваивания */
String& String::operator=(const String& other)
{
    /* Проверяем, что объект не является самим собой  */
    if (this != &other)
    {
        /* Освобождаем память, выделенную для текущей строки */
        delete[] m_string;

        /* Выделяем память для новой строки плюс один символ для нулевого символа, обозначающего конец строки */
        m_string = new char[other.m_lenght + 1];
        /* Копируем строку из другого объекта в выделенную память */
        strcpy(m_string, other.m_string);
        /* Присваиваем длину строки из другого объекта */
        m_lenght = other.m_lenght;
    }

    /* Возвращаем ссылку на текущий объект */
    return *this;
}

/* Оператор сравнения равенства */
bool String::operator==(String& other)
{
    /* Сравниваем строки, используя функцию strcmp */
    if (strcmp(m_string, other.m_string))
    {
        /* Если строки различаются, возвращаем false */
        return false;
    }

    /* Если строки совпадают, возвращаем true */
    return true;
}

/* Оператор неравенства */
bool String::operator!=(String& other)
{
    /* Сравниваем строки с помощью функции strcmp */
    if (strcmp(m_string, other.m_string))
    {
        /* Если строки различаются, возвращаем true */
        return true;
    }

    /* Если строки совпадают, возвращаем false */
    return false;
}

/* Оператор доступа к элементу */
char& String::operator[](int key)
{
    /* Проверяем, что значение key находится в допустимых границах */
    if (key < 0 || key >= m_lenght)
    {
        /* Если значение key не соответствует допустимым границам, вызываем функцию exit для завершения работы программы */
        exit(1);
    }

    /* Возвращаем ссылку на символ по индексу key */
    return m_string[key];
}

/* Оператор вызова функции */
String String::operator()(int startIndex, int endIndex)
{
    /* Проверяем, что startIndex <= endIndex и endIndex <= m_length */
    if (startIndex <= endIndex && endIndex <= m_lenght)
    {
        /* Создаем новый объект класса String с длиной endIndex - startIndex */
        String str(endIndex - startIndex);
        /* Копируем подстроку из m_string в новый объект str */
        strncpy(str.m_string, m_string + startIndex, endIndex - startIndex);

        /* Возвращаем новый объект str */
        return str;
    }

    /* Если условие не выполнено, вызываем функцию exit для завершения программы */
    exit(1);
}

/* Оператор сложения */
String String::operator+(const String& other)
{
    /* Создаем новый объект String, выделяя достаточное место для конкатенации строк и нулевого символа */
    String str(m_lenght + other.m_lenght + 1);
    /* Вычисляем длину объединенной строки */
    str.m_lenght = m_lenght + other.m_lenght;

    /* Копируем содержимое текущей строки в новый объект */
    strcpy(str.m_string, m_string);
    /* Добавляем содержимое строки из объекта other в конец нового объекта */
    strcat(str.m_string, other.m_string);

    /* Возвращаем новый объект str */
    return str;
}

/* Оператор сложения с присваиванием */
String& String::operator+=(const String& other)
{
    /* Выделяем временный массив для хранения копии текущей строки */
    char* temporaryString = new char[m_lenght + 1];
    /* Копируем содержимое текущей строки в массив temporaryString */
    strcpy(temporaryString, m_string);

    /* Удаляем старую память, выделенную под m_string */
    delete[] m_string;

    /* Вычисляем новую длину строки */
    m_lenght = m_lenght + other.m_lenght;
    /* Выделяем новую память для объединенной строки */
    m_string = new char[m_lenght + 1];
    /* Копируем содержимое из temporaryString обратно в m_string */
    strcpy(m_string, temporaryString);
    /* Добавляем содержимое other.m_string в конец m_string */
    strcat(m_string, other.m_string);

    /* Освобождаем память, выделенную для temporaryString */
    delete[] temporaryString;

    /* Возвращаем ссылку на текущий объект */
    return *this;
}

/* Оператор потокового ввода */
istream& operator>>(istream& in, String& other)
{
    /* Считываем строку из входного потока в m_string */
    in >> other.m_string;
    /* Вычисляем длину считанной строки */
    other.m_lenght = strlen(other.m_string);

    /* Возвращаем ссылку на входной поток */
    return in;
}

/* Оператор потокового вывода */
ostream& operator<<(ostream& out, String& other)
{
    /* Выводим строку в выходной поток */
    cout << other.m_string;

    /* Возвращаем ссылку на выходной поток */
    return out;
}

/* Алгоритм Бойера-Мура поиска подстроки */
int String::BoyerMoore(String& substring)
{
    /* Длина подстроки substring */
    int lengthSubstring = substring.m_lenght;
    /* Длина строки m_string */
    int lengthString = m_lenght;  

    /* Массив для хранения таблицы смещений */
    int tableShifts[256];
    /* Заполнение массива tableShifts значением длины подстроки */
    fill_n(tableShifts, 256, lengthSubstring);

    /* Заполнение таблицы смещений для каждого символа в подстроке */
    for (int i = 0; i < lengthSubstring - 1; i++)
    { 
        tableShifts[(int)substring.m_string[i]] = lengthSubstring - 1 - i;
    }                            

    /* Индекс строки */
    int stringIndex = lengthSubstring - 1;
    /* Индекс подстроки */
    int substringIndex = lengthSubstring - 1; 

    /* Поиск подстроки в строке */
    while (stringIndex < lengthString && substringIndex >= 0)
    {
        /* Временный индекс строки */
        int temporaryStringIndex = stringIndex; 
        substringIndex = lengthSubstring - 1; 

        /* Проходимся по символам подстроки */
        while (substringIndex >= 0)
        {
            /* Если символы не равны */
            if (m_string[temporaryStringIndex] != substring.m_string[substringIndex])
            {
                /* Узнаём (по таблице) на сколько нужно сместиться в строке */
                stringIndex += tableShifts[m_string[stringIndex]]; 

                break;
            }

            /* Смещаемся в подстроке */
            substringIndex--;
            /* Смещаемся в строке */
            temporaryStringIndex--; 
        }
    }

    /* Проверка, была ли найдена подстрока */
    if (substringIndex >= 0)
    {
        /* Если подстрока не найдена */
        return -1;
    }
    else
    {
        /* Иначе возвращение позиции найденной подстроки в строке */
        return stringIndex + 1 - lengthSubstring;
    }
}

int main()
{
    setlocale(LC_ALL, "Rus");

    cout << "Проверка работы конструкторов класса:" << endl;
    /* Первый конструктор */
    String firstString;
    cout << "firstString: " << firstString << endl;
    /* Второй конструктор */
    String secondString("12345");
    cout << "secondString: " << secondString << endl;
    cout << "Длина secondString: " << secondString.GetLenght() << endl;
    /* Третий конструктор */
    String thirdString = secondString;
    cout << "thirdString: " << thirdString << endl;

    cout << endl;

    cout << "Проверка работы операторов класса: " << endl;
    String stringOperators;
    /* Оператор сложения */
    stringOperators = secondString + "6789";
    cout << "Оператор +: " << stringOperators << endl;
    /* Оператор сравнения равенства */
    cout << "Оператор ==: " << (secondString == thirdString) << endl;
    /* Оператор неравенства */
    cout << "Оператор !=: " << (secondString != thirdString) << endl;
    /* Оператор сложения */
    stringOperators = secondString + thirdString;
    /* Оператор сложения с присваиванием */
    stringOperators += "00";
    cout << "Операторы +, +=: " << stringOperators << endl;
    /* Оператор доступа к элементу */
    cout << "stringOperators[3]: " << stringOperators[3] << endl;

    cout << endl;

    cout << "Проверка работы алгоритма Бойера-Мура поиска подстроки: " << endl;
    /* Исходный текст */
    String text("Hello world!");
    /* Шаблон (подстрока), которую нужно найти в исходном тексте */
    String pattern("orl");
    cout << text.BoyerMoore(pattern) << endl;

    return 0;
}
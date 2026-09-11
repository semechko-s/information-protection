#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <map>
using namespace std;

// 1. Быстрое возведение в степень по модулю
long long powerMod(long long a, long long n, long long p)
{
    long long result = 1;

    while (n > 0)
    {
        if (n % 2 == 1)
            result = (result * a) % p;

        a = (a * a) % p;
        n = n / 2;
    }

    return result;
}

// 2. Тест простоты Ферма
bool isPrime(long long n)
{
    if (n < 2)
        return false;

    if (n == 2)
        return true;

    // Несколько попыток теста Ферма
    for (int i = 0; i < 10; i++)
    {
        long long a = 2 + rand() % (n - 2);

        // Если a^(n-1) mod n != 1, число точно составное
        if (powerMod(a, n - 1, n) != 1)
            return false;
    }

    return true; // с высокой вероятностью простое
}

// 3. Обобщённый алгоритм Евклида
long long gcdExtended(long long a, long long b, long long &x, long long &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }

    long long x1, y1;

    long long d = gcdExtended(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return d;
}

// Генерация случайного числа
long long generateNumber()
{
    return 10 + rand() % 91; // от 10 до 100
}

// Генерация простого числа
long long generatePrime()
{
    long long n;

    do
    {
        n = 10 + rand() % 91;
    }
    while (!isPrime(n));

    return n;
}

long long modInverse(long long a, long long p)
{
    long long x, y;

    long long d = gcdExtended(a, p, x, y);

    if (d != 1)
        return -1;

    x %= p;

    if (x < 0)
        x += p;

    return x;
}

long long discreteLogBSGS(long long a, long long y, long long p)
{
    // Для алгоритма нужен простой модуль
    if (!isPrime(p))
    {
        cout << "Ошибка: p должно быть простым числом!" << endl;
        return -1;
    }

    // a должно иметь обратный элемент по модулю p
    if (a % p == 0)
    {
        cout << "Ошибка: a не должно быть кратно p!" << endl;
        return -1;
    }

    a %= p;
    y %= p;

    // m = ceil(sqrt(p - 1))
    long long m = (long long)ceil(sqrt((double)(p - 1)));

    cout << endl;
    cout << "Параметры алгоритма:" << endl;
    cout << "m = " << m << endl;

    // ШАГ МЛАДЕНЦА
    // Таблица:
    // a^0, a^1, a^2, ..., a^(m-1)
    // map хранит: значение -> показатель степени
    
    map<long long, long long> babySteps;

    long long current = 1;

    for (long long j = 0; j < m; j++)
    {
        // Сохраняем только первое вхождение
        if (babySteps.find(current) == babySteps.end())
            babySteps[current] = j;

        current = (current * a) % p;
    }

    // Вычисляем a^(-m)
    // Сначала находим a^m
    // Затем находим обратный элемент
    
    long long aM = powerMod(a, m, p);

    long long inverseA_M = modInverse(aM, p);

    if (inverseA_M == -1)
    {
        cout << "Не удалось найти обратный элемент." << endl;
        return -1;
    }

    // ШАГ ВЕЛИКАНА
    // y * (a^(-m))^i
    // Ищем совпадение с babySteps.

    long long giantValue = y;

    for (long long i = 0; i <= m; i++)
    {
        auto it = babySteps.find(giantValue);

        if (it != babySteps.end())
        {
            long long j = it->second;

            long long x = i * m + j;

            // Проверяем найденный результат
            if (powerMod(a, x, p) == y)
            {
                return x;
            }
        }

        giantValue = (giantValue * inverseA_M) % p;
    }

    return -1;
}

void generateDiscreteLogParameters(long long &a,
                                   long long &y,
                                   long long &p,
                                   long long &secretX)
{
    p = generatePrime();

    // a от 2 до p-1
    a = 2 + rand() % (p - 2);

    // Случайный показатель
    secretX = rand() % (p - 1);

    // Получаем y
    y = powerMod(a, secretX, p);
}

// Проверка, является ли число первообразным корнем по модулю p.
bool isPrimitiveRoot(long long g, long long p)
{
    if (!isPrime(p))
        return false;

    long long q = (p - 1) / 2;

    if (g <= 1 || g >= p)
        return false;

    // Проверяем, что порядок g не делит 2.
    if (powerMod(g, 2, p) == 1)
        return false;

    // Проверяем, что порядок g не делит q.
    if (powerMod(g, q, p) == 1)
        return false;

    return true;
}


// Генерация безопасного простого числа.
long long generateSafePrime()
{
    long long q;
    long long p;

    do
    {
        // Генерируем простое q.
        q = generatePrime();

        // Строим p = 2q + 1.
        p = 2 * q + 1;
    }
    while (!isPrime(p));

    return p;
}


// Генерация первообразного корня для безопасного простого p.
long long generatePrimitiveRoot(long long p)
{
    for (long long g = 2; g < p; g++)
    {
        if (isPrimitiveRoot(g, p))
            return g;
    }

    return -1;
}

void diffieHellman()
{
    long long p;
    long long g;
    long long Xa;
    long long Xb;
    long long Ya;
    long long Yb;
    long long keyAlice;
    long long keyBob;

    int choice;

    cout << "АЛГОРИТМ ДИФФИ-ХЕЛЛМАНА" << endl;

    cout << "1 - Ввести p, g, Xa, Xb" << endl;
    cout << "2 - Сгенерировать параметры" << endl;
    cout << "Выберите вариант: ";

    cin >> choice;

    if (choice == 1)
    {
        cout << endl;

        cout << "Введите p (безопасное простое число): ";
        cin >> p;

        cout << "Введите g (первообразный корень по модулю p): ";
        cin >> g;

        cout << "Введите закрытый ключ Алисы Xa: ";
        cin >> Xa;

        cout << "Введите закрытый ключ Боба Xb: ";
        cin >> Xb;

        // Проверяем, что p действительно простое.
        if (!isPrime(p))
        {
            cout << endl;
            cout << "Ошибка: p должно быть простым числом!" << endl;
            return;
        }

        if (!isPrimitiveRoot(g, p))
        {
            cout << endl;
            cout << "Ошибка: g не является первообразным корнем по модулю p!" << endl;
            return;
        }

        if (Xa <= 0 || Xa >= p - 1)
        {
            cout << endl;
            cout << "Ошибка: Xa должно находиться в диапазоне 1 ... p-2!" << endl;
            return;
        }

        if (Xb <= 0 || Xb >= p - 1)
        {
            cout << endl;
            cout << "Ошибка: Xb должно находиться в диапазоне 1 ... p-2!" << endl;
            return;
        }
    }

    else if (choice == 2)
    {
        p = generateSafePrime();

        g = generatePrimitiveRoot(p);

        if (g == -1)
        {
            cout << "Ошибка генерации первообразного корня!" << endl;
            return;
        }

        Xa = 1 + rand() % (p - 2);

        Xb = 1 + rand() % (p - 2);

        cout << endl;
        cout << "Параметры сгенерированы автоматически." << endl;
    }

    else
    {
        cout << "Ошибка!" << endl;
        return;
    }

    Ya = powerMod(g, Xa, p);
    Yb = powerMod(g, Xb, p);

    keyAlice = powerMod(Yb, Xa, p);
    keyBob = powerMod(Ya, Xb, p);

    cout << endl;
    cout << "----------------------------------------" << endl;
    cout << "Общие параметры:" << endl;
    cout << "p = " << p << endl;
    cout << "g = " << g << endl;

    cout << endl;
    cout << "Закрытые ключи:" << endl;
    cout << "Xa (Алиса) = " << Xa << endl;
    cout << "Xb (Боб)   = " << Xb << endl;

    cout << endl;
    cout << "Открытые ключи:" << endl;
    cout << "Ya (Алиса) = " << Ya << endl;
    cout << "Yb (Боб)   = " << Yb << endl;

    cout << endl;
    cout << "Общие секретные ключи:" << endl;
    cout << "Ключ Алисы = " << keyAlice << endl;
    cout << "Ключ Боба  = " << keyBob << endl;

    // ========================================
    // Проверка результата
    // ========================================

    cout << endl;

    if (keyAlice == keyBob)
    {
        cout << "УСПЕХ!" << endl;
        cout << "Общие ключи совпадают." << endl;
        cout << "Общий секретный ключ = " << keyAlice << endl;
    }
    else
    {
        // Теоретически при корректных параметрах
        // такого произойти не должно.
        cout << "ОШИБКА!" << endl;
        cout << "Общие ключи не совпадают." << endl;
    }

    cout << "----------------------------------------" << endl;
}

int main()
{
    srand(time(0));

    // int choice;

    // cout << "1 - Ввести a и b с клавиатуры" << endl;
    // cout << "2 - Сгенерировать a и b" << endl;
    // cout << "3 - Сгенерировать простые a и b" << endl;
    // cout << "Выберите вариант: ";

    // cin >> choice;

    // long long a, b;

    // if (choice == 1)
    // {
    //     cout << "Введите a: ";
    //     cin >> a;

    //     cout << "Введите b: ";
    //     cin >> b;
    // }
    // else if (choice == 2)
    // {
    //     a = generateNumber();
    //     b = generateNumber();

    //     cout << "Сгенерировано:" << endl;
    //     cout << "a = " << a << endl;
    //     cout << "b = " << b << endl;
    // }
    // else if (choice == 3)
    // {
    //     a = generatePrime();
    //     b = generatePrime();

    //     cout << "Сгенерировано:" << endl;
    //     cout << "Простое a = " << a << endl;
    //     cout << "Простое b = " << b << endl;
    // }
    // else
    // {
    //     cout << "Ошибка!" << endl;
    //     return 0;
    // }

    // // Проверка простоты
    
    // cout << endl;
    // cout << "Проверка Ферма:" << endl;

    // if (isPrime(a))
    //     cout << "a = " << a << " вероятно простое" << endl;
    // else
    //     cout << "a = " << a << " составное" << endl;

    // if (isPrime(b))
    //     cout << "b = " << b << " вероятно простое" << endl;
    // else
    //     cout << "b = " << b << " составное" << endl;


    // // Обобщённый алгоритм Евклида

    // long long x, y;

    // long long d = gcdExtended(a, b, x, y);

    // cout << endl;
    // cout << "Обобщённый алгоритм Евклида:" << endl;
    // cout << "НОД(" << a << ", " << b << ") = " << d << endl;
    // cout << "x = " << x << endl;
    // cout << "y = " << y << endl;

    // cout << endl;
    // cout << a << " * " << x << " + "
    //      << b << " * " << y << " = " << d << endl;

    // // Быстрое возведение в степень

    // long long n, p;

    // cout << endl;
    // cout << "Быстрое возведение в степень по модулю" << endl;

    // cout << "Введите число a: ";
    // cin >> a;

    // cout << "Введите степень n: ";
    // cin >> n;

    // cout << "Введите модуль p: ";
    // cin >> p;

    // cout << a << "^" << n << " mod " << p
    //      << " = " << powerMod(a, n, p) << endl;

    // // ДИСКРЕТНЫЙ ЛОГАРИФМ
    // cout << endl;
    // cout << "Дискретный логарифм. Алгоритм: Шаг младенца, шаг великана" << endl;

    // int logChoice;

    // cout << endl;
    // cout << "1 - Ввести a, y, p" << endl;
    // cout << "2 - Сгенерировать a, y, p" << endl;
    // cout << "Выберите вариант: ";

    // cin >> logChoice;


    // long long base;
    // long long value;
    // long long prime;
    // long long generatedX = -1;


    // if (logChoice == 1)
    // {
    //     cout << endl;
    //     cout << "Введите a: ";
    //     cin >> base;

    //     cout << "Введите y: ";
    //     cin >> value;

    //     cout << "Введите p (простое): ";
    //     cin >> prime;
    // }
    // else if (logChoice == 2)
    // {
    //     generateDiscreteLogParameters(
    //         base,
    //         value,
    //         prime,
    //         generatedX
    //     );

    //     cout << endl;
    //     cout << "Сгенерированные параметры:" << endl;
    //     cout << "a = " << base << endl;
    //     cout << "y = " << value << endl;
    //     cout << "p = " << prime << endl;

    //     // cout << "Скрытое x = " << generatedX << endl;
    // }
    // else
    // {
    //     cout << "Ошибка!" << endl;
    //     return 0;
    // }

    // // Поиск дискретного логарифма

    // long long resultX = discreteLogBSGS(base, value, prime);

    // if (resultX == -1)
    // {
    //     cout << endl;
    //     cout << "Решение не найдено." << endl;
    // }
    // else
    // {
    //     cout << endl;
    //     cout << "Результат:" << endl;

    //     cout << "x = " << resultX << endl;

    //     cout << endl;
    //     cout << "Проверка:" << endl;

    //     cout << base << "^" << resultX
    //          << " mod " << prime
    //          << " = "
    //          << powerMod(base, resultX, prime)
    //          << endl;

    //     cout << "Ожидалось y = " << value << endl;
    // }

    diffieHellman();

    return 0;
}
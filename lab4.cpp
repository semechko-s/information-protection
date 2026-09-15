#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

// Быстрое возведение в степень
long long power(long long a, long long b, long long p)
{
    long long r = 1;

    while (b > 0)
    {
        if (b % 2 == 1)
            r = r * a % p;

        a = a * a % p;
        b /= 2;
    }

    return r;
}

// НОД
long long gcd(long long a, long long b)
{
    while (b != 0)
    {
        long long t = a % b;
        a = b;
        b = t;
    }

    return a;
}

// Обратное число
long long inverse(long long a, long long m)
{
    for (long long x = 1; x < m; x++)
    {
        if ((a * x) % m == 1)
            return x;
    }

    return 0;
}

// Проверка на простое число
bool prime(long long n)
{
    if (n < 2)
        return false;

    for (long long i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    long long p, Ca, Cb, Da, Db;
    int mode, action;

    cout << "Шифр Шамира\n\n";

    // Ввод или генерация параметров
    cout << "1 - ввести параметры\n";
    cout << "2 - сгенерировать параметры\n";
    cin >> mode;

    if (mode == 1)
    {
        cout << "p = ";
        cin >> p;

        cout << "Ca = ";
        cin >> Ca;

        cout << "Cb = ";
        cin >> Cb;

        Da = inverse(Ca, p - 1);
        Db = inverse(Cb, p - 1);
    }
    else
    {
        // Простые параметры для автоматической генерации
        // Можно заменить на любые подходящие значения

        p = 257;

        Ca = 3;
        Cb = 5;

        Da = inverse(Ca, p - 1);
        Db = inverse(Cb, p - 1);

        cout << "\nПараметры сгенерированы:\n";
    }

    cout << "\np  = " << p;
    cout << "\nCa = " << Ca;
    cout << "\nDa = " << Da;
    cout << "\nCb = " << Cb;
    cout << "\nDb = " << Db << "\n";

    // Выбор действия
    cout << "\n1 - шифрование\n";
    cout << "2 - расшифрование\n";
    cin >> action;

    string input, output;

    cout << "Входной файл: ";
    cin >> input;

    cout << "Выходной файл: ";
    cin >> output;

    ifstream fin(input, ios::binary);
    ofstream fout(output, ios::binary);

    if (!fin || !fout)
    {
        cout << "Ошибка открытия файла!";
        return 1;
    }

    unsigned char byte;

    // ШИФРОВАНИЕ
    if (action == 1)
    {
        while (fin.read((char*)&byte, 1))
        {
            long long m = byte;

            // Алиса
            long long x = power(m, Ca, p);

            // Боб
            x = power(x, Cb, p);

            // Алиса снимает свою степень
            x = power(x, Da, p);

            // Записываем число в 2 байта
            fout.put(x / 256);
            fout.put(x % 256);
        }

        cout << "\nФайл зашифрован!";
    }

    // РАСШИФРОВАНИЕ
    else
    {
        unsigned char a, b;

        while (fin.read((char*)&a, 1))
        {
            fin.read((char*)&b, 1);

            long long x = a * 256 + b;

            // Боб снимает свою степень
            x = power(x, Db, p);

            fout.put((unsigned char)x);
        }

        cout << "\nФайл расшифрован!";
    }

    fin.close();
    fout.close();

    return 0;
}
// SIAOD2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#pragma warning(disable:4996)
using namespace std;

int q = 1;

struct book
{
    string isbn = "";
    string author = "";
    string name = "";
};
class hashTable
{
private:
    int count;
    bool t;
    string* table;
    book* array;
public:
    hashTable(int count, int n);
    unsigned int hash(string key);
    unsigned int rehash(string key);
    void readPaste(string s);
    void remove(string key);
    int search(string key);
    void tableOutput();
    void searchOutput(string key);
};

book divide(string s)// Нашу строку с данными делим и по частям записываем в свойства структуры book
{
    book a;
    int i = 0;
    while (i < 9)// Отделяем isbn
    {
        a.isbn += s[i];
        i++;
    }
    i++;
    while (s[i] != ' ') // Отделяем название книги
    {
        a.author += +s[i];
        i++;
    }
    i++;
    while (i < 41) // Отделяем Фамилию автора
    {
        if (i > s.length())
            break;
        a.name += +s[i];
        i++;
    }
    return(a);
}
string randomBook() // Создание рандомной строки с данными книги
{
    string isbn = "";
    string author = "";
    string name = "";
    string book = "";
    for (int i = 0; i < 9; i++)
        isbn += char(rand() % 9 + 48);
    author = char(rand() % 26 + 65);
    for (int i = 0; i < 9; i++)
        author += char(rand() % 26 + 97);
    name = char(rand() % 26 + 65);
    for (int i = 0; i < 19; i++)
        name += char(rand() % 26 + 97);
    book = isbn + " " + author + " " + name;
    return(book);
}
void generate(int n)// Запись в файл n строк
{
    ofstream fout("books.bin", ios::binary | ios::out);
    for (int i = 0; i < n; i++)
    {
        fout.write(randomBook().c_str(), 41);
    }
}
void input() // Запись в файл 3 строк
{
    ofstream fout("books.bin", ios::binary | ios::out);
    string s;
    getline(cin, s);
    for (int i = 0; i < 3; i++)
    {
        getline(cin, s);
        fout.write(s.c_str(), 41);
    }
    fout.close();
}

hashTable::hashTable(int c, int n) // Создаем хеш таблицу
{
    count = c;
    char s[41];
    table = new string[count];
    array = new book[count];
    book a;
    for (int i = 0; i < count; i++) // Заполняем пустышками таблицу table - содержание ячейки ХТ, array - массив книг
    {
        table[i] = "";
        array[i] = a;
    }
    ifstream file("books.bin", ios::binary | ios::in);
    for (int i = 0; i < n; i++)// Хешируем данные из файла
    {
        file.read(s, 41);
        readPaste((string)s);
    }
    file.close();
}
unsigned int hashTable::hash(string key) // Хеш функция
{
    unsigned int num = 0;
    for (int i = 0; i < key.length(); i++)
    {
        num += (int)key[i];
    }
    return(num % count);
}
unsigned int hashTable::rehash(string key) // Рехеш с помощь второй хеш функции
{
    unsigned int num = 0;
    for (int i = 0; i < key.length(); i++)
    {
        num += (int)key[i];
    }
    return(num % (count - 1));
}
void hashTable::remove(string key)// Функция удаления информации в ячейке ХТ
{
    auto begin = chrono::steady_clock::now();
    int i = search(key); // Ищем с помощью search нужную нам ячейку
    if (i == -1)
    {
        auto end = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end - begin);
        cout << "Записи не существует" << endl;
        return;
        if (!t)
            cout << " Время выполнения: " << elapsed.count() << " ns";
        cout << "\n";
        return;
    }
    book a;// Создаем пустышку, которой заполним ячейку массива книг
    table[i] = "";
    array[i] = a;
    auto end = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end - begin);
    cout << "Запись удалена" << endl;
    if (!t)
        cout << " Время выполнения : " << elapsed.count() << " ns";
    cout << "\n";
}
void hashTable::tableOutput() // Вывод ХТ
{
    for (int i = 0; i < count; i++)
        cout << i << ": " << array[i].isbn + " " + array[i].author + " " + array[i].name << endl;
}
void hashTable::readPaste(string s) // Хешируем ключ и проверяем корректность хеша
{
    book a = divide(s);
    int i;
    i = hash(a.isbn);
    cout << "Хеш для " << a.isbn << "= " << i << endl;
    while (q < count)
    {
        if (table[i] == "")
        {
            cout << "Адрес " << i << " свободен. Записываем." << endl;;
            table[i] = a.isbn;
            array[i] = a;
            return;
        }
        if (table[i] == a.isbn)
        {
            cout << "Данная книга уже существует." << endl;
            return;
        }
        cout << "Адрес занят. Найдем новый." << endl;
        q++;
        i = hash(a.isbn) + q * rehash(a.isbn);
        i %= count;
    }
    i = hash(a.isbn);
    q = 1;
    while (true)
    {
        if (table[i] == "")
        {
            cout << "Адрес " << i << " свободен. Записываем." << endl;
            table[i] = a.isbn;
            array[i] = a;
            return;
        }
        if (table[i] == a.isbn)
        {
            cout << "Данная книга уже существует." << endl;
            return;
        }
        q++;
        i = hash(a.isbn) + q * rehash(a.isbn);
        i %= count;
        table[i] = a.isbn;
        array[i] = a;
    }
}
int hashTable::search(string key) // Поиск по ключу, возвращаем либо индекс ячейки, либо -1, если не найдено
{
    int i = hash(key) + q * rehash(key);
    q = 1;
    while (q < count)
    {
        if (table[i] == key)
            return(i);
        q++;
        i = hash(key) + q * rehash(key);
        i %= count;
    }
    q = 1;
    while (q < count)
    {
        if (table[i] == key)
            return(i);
        q++;
        i = hash(key) + q * rehash(key);
        i %= count;
    }
    cout << "Книги не существует." << endl;
    return(-1);
}
void hashTable::searchOutput(string key) // Вывод результата поиска по ключу
{
    auto begin = chrono::steady_clock::now();
    int i = search(key);
    if (i == -1)
    {
        auto end = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end - begin);
        cout << "Студент не найден.";
        if (!t)
            cout << " Время выполнения: " << elapsed.count() << " ns";
        cout << "\n";
        return;
    }
    book a = array[i];
    auto end = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end - begin);
    cout << "ISBN книги: " << a.isbn << endl;
    cout << "Автор: " << a.author << endl;
    cout << "Название: " << a.name << endl;
    if (!t)
        cout << "Время выполнения: " << elapsed.count() << " ns\n";
}

int main()
{
    setlocale(LC_ALL, "rus");
    srand(time(0));
    int choice;
    cout << "Выберите способ ввода: " << endl;
    cout << "1=Ввести данные вручную, 2=Сгенерировать данные" << endl;
    cin >> choice;
    if (choice == 1)
    {
        string s;
        cout << "Введите 3 строки. Пример:" << endl;
        cout << "111111111 Pushkin EvegeniyOnegin" << endl;
        input();
        cout << endl;
        hashTable tab(3, 3);
        cout << endl << "Хеш-таблица" << endl;
        tab.tableOutput();
        cout << endl;
        while (true)
        {
            cout << "1. Найти книгу по ключу." << endl;
            cout << "2. Удалить книгу по ключу." << endl;
            cout << "3. Вывод таблицы." << endl;
            cin >> choice;
            switch (choice)
            {
            case 1:
            {
                cout << endl;
                getline(cin, s);
                cout << "Введите ISBN: ";
                getline(cin, s);
                tab.searchOutput(s);
                cout << endl;
                break;
            }
            case 2:
            {
                cout << endl;
                getline(cin, s);
                cout << "Введите ISBN: ";
                getline(cin, s);
                tab.remove(s);
                cout << endl;
                break;
            }
            case 3:
            {
                cout << endl;
                tab.tableOutput();
                cout << endl;
                break;
            }
            }
        }
    }
    else if (choice == 2)
    {
        int n;
        cout << "Введите количество книг: ";
        cin >> n;
        generate(n);
        cout << "Для рабочей таблицы в хеш-функции будем искать остаток от деления на 117." << endl;
        hashTable tab(117, n);
        cout << endl << "Хеш-таблица" << endl;
        tab.tableOutput();
        cout << endl;
        while (true)
        {
            string s;
            cout << "1. Найти книгу по ключу." << endl;
            cout << "2. Удалить книгу по ключу" << endl;
            cout << "3. Вывод таблицы." << endl;
            cin >> choice;
            switch (choice)
            {
            case 1:
            {
                cout << endl;
                getline(cin, s);
                cout << "Введите ISBN: ";
                getline(cin, s);
                tab.searchOutput(s);
                cout << endl;
                break;
            }
            case 2:
            {
                cout << endl;
                getline(cin, s);
                cout << "Введите ISBN: ";
                getline(cin, s);
                tab.remove(s);
                cout << endl;
                break;
            }
            case 3:
            {
                cout << endl;
                tab.tableOutput();
                cout << endl;
                break;
            }
            }
        }
    }
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

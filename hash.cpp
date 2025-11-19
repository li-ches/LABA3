#include "hash.h"

using namespace std;

// [modernize-use-trailing-return-type]
auto hash_str(const string& key, int mod) -> int
{
    int hashVal = 0;

    // [readability-identifier-length]: 'ch' -> 'character'
    for(char character: key)
    {
        // Возвращено число 31 вместо константы
        // [readability-redundant-casting]: убран лишний (int) перед abs
        hashVal = (hashVal * 31 + std::abs((int)character)) % mod;
    }

    return hashVal;
}

ChainHash::ChainHash(int buckCount) : table(nullptr), cap(buckCount)
{
    if(cap < 1)
    {
        cap=1;
    }
    table = new Node*[cap];
    for(int i = 0; i< cap; i++)
    {
        table[i] = nullptr;
    }
}

ChainHash::~ChainHash()
{
    for(int i=0; i < cap; i++)
    {
        Node* cur = table[i];

        while(cur != nullptr)
        {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    delete[] table;
    table = nullptr;
    cap = 0;
}

void ChainHash::insert(const string& key, const string& value) {
    int idx = hash_str(key, cap);

    cout << "Цепочки вставка: " << key << " -> " << value
         << "  в бакет " << idx << "\n";

    Node* head = table[idx];

    //пытаемся найти ключ и просто обновить значение
    Node* cur = head;
    while (cur != nullptr) {
        if (cur->key == key) {
            cur->val = value;
            show();
            return;
        }
        cur = cur->next;
    }

    //вставляем в голову списка 
    // [readability-identifier-length]: 'nd' -> 'newNode'
    Node* newNode = new Node(key, value);
    newNode->next = head;
    table[idx] = newNode;

    show();
}

// [modernize-use-trailing-return-type]
auto ChainHash::erase(const string& key) -> bool
{
    int idx = hash_str(key, cap);
    cout << "Цепочки удаление: " << key << " из бакета " << idx << "\n";

    Node* cur = table[idx];
    Node* prv = nullptr;

    while(cur != nullptr)
    {
        if(cur->key == key)
        {
            if(prv != nullptr)
            {
                prv->next = cur->next;
            } else{
                table[idx] = cur ->next;
            }
            delete cur;
            show();
            return true;
        }
        prv = cur;
        cur = cur->next;
    }

    cout << "Ключ не найден" << "\n";
    return false;
}

// [modernize-use-trailing-return-type]
auto ChainHash::find(const string& key) -> string
{
    int idx = hash_str(key, cap);

    cout << "Цепочки поиск: " << key << " в бакете " << idx << "\n";

    Node* cur = table[idx];

    while (cur != nullptr) {
        if (cur->key == key) {
            cout << "Найдено: " << cur->val << "\n";
            return cur->val;
        }
        cur = cur->next;
    }

    cout << "Не найдено" << "\n";
    return "";
}

//печать непустых бакетов
void ChainHash::show() {
    cout << "цепочки: ";

    bool any = false;

    for (int i = 0; i < cap; i++) {
        if (table[i] != nullptr) {
            any = true;

            cout << "[" << i << "]" << ": ";

            Node* cur = table[i];
            while (cur != nullptr) {
                cout << cur->key << "->" << cur->val;

                if (cur->next != nullptr) {
                    cout << " -> ";
                }

                cur = cur->next;
            }

            cout << "  ";
        }
    }

    if (!any) {
        cout << "пусто";
    }

    cout << "\n";
}

OpenHash::OpenHash(int size) : table(nullptr), cap(size)
{
    if(cap < 1)
    {
        cap=1;
    }
    table = new Entry[cap];

    for (int i = 0; i < cap; ++i) {
        table[i].used    = false;
        table[i].deleted = false;
    }
}

OpenHash::~OpenHash()
{
    delete[] table;
    table = nullptr;
    cap = 0;
}

void OpenHash::insert(const string& key, const string& value)
{
    int idx = hash_str(key, cap);
    int start = idx;

    cout << "Открытая адр. Вставка: " << key << " -> " << value << " старт " << idx << "\n";
    int probe = 0;

    while(true)
    {
        cout << "Проба " << (probe+1) << ": ячейка " << idx << "\n";

        if(!table[idx].used && !table[idx].deleted) 
        {
            table[idx].key = key;
            table[idx].val = value;
            table[idx].used = true;
            table[idx].deleted = false;
            show();

            return;
        }
            if (table[idx].used && table[idx].key == key) {
            table[idx].val = value;
            show();
            return;
        }
            if (table[idx].deleted) {
            table[idx].key     = key;
            table[idx].val     = value;
            table[idx].used    = true;
            table[idx].deleted = false;

            show();
            return;
        }

        idx = (idx+1) % cap;
        probe +=1 ;

        if(idx == start)
        {
            cout << "Таблица переполнена" << "\n";
            return;
        }
    }
}

// [modernize-use-trailing-return-type]
auto OpenHash::erase(const string& key) -> bool
{
    int idx = hash_str(key, cap);
    int start = idx;

    cout << "Откр.Адр Удаление:" << key << " старт " << idx << "\n";

    int probe = 0;

    while(true)
    {
        cout << "Проба " << (probe + 1) << ": ячейка " << idx << "\n";

        // Нашли ключ — помечаем как удалённый
        if (table[idx].used && table[idx].key == key) {
            table[idx].used    = false;
            table[idx].deleted = true;

            show();
            return true;
        }

        // Пустая и не удалённая — дальше смысла искать нет
        if (!table[idx].used && !table[idx].deleted) {
            cout << "Ключ не найден" << "\n";
            return false;
        }

        // Иначе продолжаем пробирование
        idx = (idx + 1) % cap;
        probe += 1;

        if (idx == start) {
            cout << "Ключ не найден" << "\n";
            return false;
        }
    }
}

// [modernize-use-trailing-return-type]
auto OpenHash::find(const string& key) const -> string {
    int idx   = hash_str(key, cap);
    int start = idx;

    cout << "ОТКР.АДР ПОИСК: " << key
         << "  старт " << idx << "\n";

    int probe = 0;

    while (true) {
        cout << "Проба " << (probe + 1) << ": ячейка " << idx << "\n";

        //совпало - найдено
        if (table[idx].used && table[idx].key == key) {
            cout << "Найдено: " << table[idx].val << "\n";
            return table[idx].val;
        }

        //пустая и не удалённая — ключа точно нет
        if (!table[idx].used && !table[idx].deleted) {
            cout << "Не найдено" << "\n";
            return "";
        }

        //иначе продолжаем
        idx = (idx + 1) % cap;
        probe += 1;

        if (idx == start) {
            cout << "Не найдено" << "\n";
            return "";
        }
    }
}


//Печатает занятые и удалённые ячейки. 
void OpenHash::show() {
    cout << "ОТКР.АДР: ";

    bool any = false;

    for (int i = 0; i < cap; i++) {
        if (table[i].used) {
            any = true;
            cout << "[" << i << "]" << table[i].key << "->" << table[i].val << "  ";
        } else if (table[i].deleted) {
            any = true;
            cout << "[" << i << "]" << "(удалено)  ";
        }
    }

    if (!any) {
        cout << "пусто";
    }

    cout << "\n";
}

// [modernize-use-trailing-return-type]
// [readability-identifier-length]: 'c' -> 'charToCheck'
auto is_space_char(char charToCheck) -> bool {
    return charToCheck == ' ' || charToCheck == '\t';
}

void parse_cmd(const string& line, string& cmd, string& arg1, string& rest) {
    cmd.clear();
    arg1.clear();
    rest.clear();

    // [readability-identifier-length]: 'n' -> 'lineLength'
    int lineLength = (int)line.size();
    // [readability-identifier-length]: 'i' -> 'idx'
    int idx = 0;


    //читаем команду до пробела
    // [readability-braces-around-statements]: добавлен блок {}
    while (idx < lineLength && !is_space_char(line[idx])) {
        cmd.push_back(line[idx]);
        idx++;
    }

    //пропускаем пробелы после команды
    // [readability-braces-around-statements]: добавлен блок {}
    while (idx < lineLength && is_space_char(line[idx])) 
    {
        idx++;
    }

    //читаем первый аргумент (ключ) до пробела
    // [readability-braces-around-statements]: добавлен блок {}
    while (idx < lineLength && !is_space_char(line[idx])) {
        arg1.push_back(line[idx]);
        idx++;
    }

    //пропускаем пробелы перед остатком
    // [readability-braces-around-statements]: добавлен блок {}
    while (idx < lineLength && is_space_char(line[idx])) 
    {
        idx++;
    }

    // остаток может быть пустым
    // [readability-braces-around-statements]: добавлен блок {}
    if (idx < lineLength) 
    {
        rest = line.substr(idx);
    }
}

void hash_man() {
    cout << "УПРАВЛЕНИЕ ХЕШ-ТАБЛИЦАМИ" << "\n";

    // Возвращено число 10 вместо константы
    ChainHash htChain(10);
    OpenHash  htOpen(10);

    while (true) {
        cout << "\n";
        cout << "Команды: INSERT ключ значение | DELETE ключ | SEARCH ключ | SHOW | BACK" << "\n";
        cout << "Введите команду: ";

        string line;
        // [readability-braces-around-statements]: добавлен блок {}
        if (!getline(cin, line)) { break; }

        // [readability-isolate-declaration]: разделение на отдельные строки
        string cmd;
        string arg1;
        string rest;
        parse_cmd(line, cmd, arg1, rest);

        // Пустой ввод
        if (cmd.empty()) {
            cout << "Пустая команда" << "\n";
            continue;
        }

        if (cmd == "BACK") {
            cout << "Выход из меню" << "\n";
            break;
        }

        if (cmd == "SHOW") {
            cout << "\n";
            htChain.show();
            cout << "\n";
            htOpen.show();
            continue;
        }

        if (cmd == "INSERT") {
            // key обязателен, value обязателен (всё, что осталось)
            if (arg1.empty() || rest.empty()) {
                cout << "Ошибка формата. Пример: INSERT key value" << "\n";
                continue;
            }

            cout << "\n";
            htChain.insert(arg1, rest);

            cout << "\n";
            htOpen.insert(arg1, rest);
            continue;
        }

        if (cmd == "DELETE") {
            if (arg1.empty()) {
                cout << "Ошибка формата. Пример: DELETE key" << "\n";
                continue;
            }

            cout << "\n";
            htChain.erase(arg1);

            cout << "\n";
            htOpen.erase(arg1);
            continue;
        }

        if (cmd == "SEARCH") {
            if (arg1.empty()) {
                cout << "Ошибка формата. Пример: SEARCH key" << "\n";
                continue;
            }

            cout << "\n";
            (void)htChain.find(arg1);

            cout << "\n";
            (void)htOpen.find(arg1);
            continue;
        }

        cout << "Неизвестная команда" << "\n";
    }
}
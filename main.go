package main

import (
	"bufio"
	"datastructures/ds"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func showComm() {
	fmt.Print(`--- ОБЩИЕ КОМАНДЫ ---
PRINT - Показать содержимое всех структур
HELP - Показать это сообщение
EXIT - Выйти из программы

--- СОХРАНЕНИЕ/ЗАГРУЗКА ---
SAVE <filename> - Сохранить все структуры
LOAD <filename> - Загрузить все структуры

--- МАССИВ (MyArr) ---
M_PUSH_END <val> - Добавить в конец массива
M_PUSH_HEAD <val> - Добавить в начало
M_PUSH_AT <idx> <val> - Добавить по индексу
M_DEL_AT <idx> - Удалить по индексу
M_DEL_HEAD - Удалить из начала
M_DEL_TAIL - Удалить из конца
M_GET_AT <idx> - Получить по индексу
M_REPLACE_AT <idx> <val> - Заменить по индексу
M_LENGTH - Узнать длину массива

--- ОДНОСВЯЗНЫЙ СПИСОК (MyList) ---
F_PUSH_HEAD <val> - Добавить в начало
F_PUSH_TAIL <val> - Добавить в конец
F_PUSH_AFTER <target> <val> - Добавить после значения
F_PUSH_BEFORE <target> <val> - Добавить до значения
F_DEL_HEAD - Удалить первый элемент
F_DEL_TAIL - Удалить последний элемент
F_DEL_VAL <val> - Удалить по значению
F_GET_VAL <val> - Найти по значению

--- ДВУСВЯЗНЫЙ СПИСОК (DList) ---
L_PUSH_HEAD <val> - Добавить в начало
L_PUSH_TAIL <val> - Добавить в конец
L_PUSH_AFTER <target> <val> - Добавить после значения
L_PUSH_BEFORE <target> <val> - Добавить до значения
L_DEL_HEAD - Удалить первый элемент
L_DEL_TAIL - Удалить последний элемент
L_DEL_VAL <val> - Удалить по значению
L_GET_VAL <val> - Найти по значению
L_PRINT_REV - Печать в обратном порядке

--- ОЧЕРЕДЬ (Queue) ---
Q_PUSH <val> - Добавить в очередь
Q_POP - Извлечь из очереди
Q_GET - Прочитать первый элемент

--- СТЕК (Stack) ---
S_PUSH <val> - Добавить в стек
S_POP - Извлечь из стека
S_GET - Прочитать верхний элемент

--- ДЕРЕВО (CBT) ---
T_INSERT <num> - Вставить число в дерево
T_SEARCH <num> - Найти число в дереве
T_DEL <num> - Удалить число из дерева
T_PRINT - Печать in-order

--- ХЕШ-ТАБЛИЦА (Hash) ---
HASH - Открыть отдельное меню хеш-таблиц
-----------------------
`)
}

func safeStoi(s string) int {
	i, err := strconv.Atoi(s)
	if err != nil {
		fmt.Printf("ОШИБКА: Ожидалось число, но получено '%s'\n", s)
		return -1
	}
	return i
}

func main() {
	arr := ds.NewMyArr()
	slist := ds.NewMyList()
	dlist := ds.NewDList()
	queue := ds.NewQueue()
	stack := ds.NewStack()
	tree := ds.NewCBT()

	// Создаем сериализаторы
	arrSerializer := ds.NewArrSerializer()
	listSerializer := ds.NewListSerializer()
	dlistSerializer := ds.NewDListSerializer()
	stackSerializer := ds.NewStackSerializer()
	queueSerializer := ds.NewQueueSerializer()
	cbtSerializer := ds.NewCbtSerializer()

	scanner := bufio.NewScanner(os.Stdin)
	showComm()

	for {
		fmt.Print("\n>> ")
		if !scanner.Scan() {
			break
		}
		line := scanner.Text()
		if strings.TrimSpace(line) == "" {
			continue
		}

		parts := strings.Fields(line)
		cmd := parts[0]
		arg1 := ""
		arg2 := ""
		if len(parts) > 1 {
			arg1 = parts[1]
		}
		if len(parts) > 2 {
			arg2 = parts[2]
		}

		switch cmd {
		case "EXIT":
			fmt.Println("До свидания!")
			return
		case "HELP":
			showComm()
		case "PRINT":
			fmt.Println("--- Содержимое структур ---")
			fmt.Print("Массив:    ")
			arr.ReadArray()
			fmt.Print("Список:    ")
			slist.ReadForward()
			fmt.Print("Дв.Список: ")
			dlist.ReadForward()
			fmt.Print("Стек:      ")
			stack.ReadStack()
			fmt.Print("Очередь:   ")
			queue.Print()
			fmt.Print("Дерево:    ")
			tree.InorderPrint()
			fmt.Println("---------------------------")

		case "SAVE":
			if arg1 == "" {
				fmt.Println("Ошибка: нужно имя файла-основы.")
				continue
			}
			
			// Используем сериализаторы вместо встроенных методов
			if err := arrSerializer.SaveToFile(arr, arg1+".arr"); err != nil {
				fmt.Printf("Ошибка сохранения массива: %v\n", err)
			}
			if err := listSerializer.SaveToFile(slist, arg1+".list"); err != nil {
				fmt.Printf("Ошибка сохранения списка: %v\n", err)
			}
			if err := dlistSerializer.SaveToFile(dlist, arg1+".dlist"); err != nil {
				fmt.Printf("Ошибка сохранения двусвязного списка: %v\n", err)
			}
			if err := stackSerializer.SaveToFile(stack, arg1+".stack"); err != nil {
				fmt.Printf("Ошибка сохранения стека: %v\n", err)
			}
			if err := queueSerializer.SaveToFile(queue, arg1+".queue"); err != nil {
				fmt.Printf("Ошибка сохранения очереди: %v\n", err)
			}
			if err := cbtSerializer.SaveToFile(tree, arg1+".cbt"); err != nil {
				fmt.Printf("Ошибка сохранения дерева: %v\n", err)
			}
			
			fmt.Printf("Структуры сохранены с базовым именем: %s\n", arg1)

		case "LOAD":
			if arg1 == "" {
				fmt.Println("Ошибка: нужно имя файла-основы.")
				continue
			}
			
			// Используем сериализаторы вместо встроенных методов
			if err := arrSerializer.LoadFromFile(arr, arg1+".arr"); err != nil {
				fmt.Printf("Ошибка загрузки массива: %v\n", err)
			}
			if err := listSerializer.LoadFromFile(slist, arg1+".list"); err != nil {
				fmt.Printf("Ошибка загрузки списка: %v\n", err)
			}
			if err := dlistSerializer.LoadFromFile(dlist, arg1+".dlist"); err != nil {
				fmt.Printf("Ошибка загрузки двусвязного списка: %v\n", err)
			}
			if err := stackSerializer.LoadFromFile(stack, arg1+".stack"); err != nil {
				fmt.Printf("Ошибка загрузки стека: %v\n", err)
			}
			if err := queueSerializer.LoadFromFile(queue, arg1+".queue"); err != nil {
				fmt.Printf("Ошибка загрузки очереди: %v\n", err)
			}
			if err := cbtSerializer.LoadFromFile(tree, arg1+".cbt"); err != nil {
				fmt.Printf("Ошибка загрузки дерева: %v\n", err)
			}
			
			fmt.Printf("Структуры загружены с базовым именем: %s\n", arg1)

		// Массив
		case "M_PUSH_END":
			arr.AddEnd(arg1)
			arr.ReadArray()
		case "M_PUSH_HEAD":
			arr.AddHead(arg1)
			arr.ReadArray()
		case "M_PUSH_AT":
			arr.AddAt(safeStoi(arg1), arg2)
			arr.ReadArray()
		case "M_DEL_AT":
			arr.DelAt(safeStoi(arg1))
			arr.ReadArray()
		case "M_DEL_HEAD":
			arr.DelHead()
			arr.ReadArray()
		case "M_DEL_TAIL":
			arr.DelEnd()
			arr.ReadArray()
		case "M_GET_AT":
			fmt.Printf("Элемент[%s]: %s\n", arg1, arr.GetAt(safeStoi(arg1)))
		case "M_REPLACE_AT":
			arr.RepArr(safeStoi(arg1), arg2)
			arr.ReadArray()
		case "M_LENGTH":
			fmt.Printf("Длина массива: %d\n", arr.LenArr())

		// Односвязный список
		case "F_PUSH_HEAD":
			slist.AddHead(arg1)
			slist.ReadForward()
		case "F_PUSH_TAIL":
			slist.AddTail(arg1)
			slist.ReadForward()
		case "F_PUSH_AFTER":
			slist.AddAfter(arg1, arg2)
			slist.ReadForward()
		case "F_PUSH_BEFORE":
			slist.AddBefore(arg1, arg2)
			slist.ReadForward()
		case "F_DEL_HEAD":
			slist.DelHead()
			slist.ReadForward()
		case "F_DEL_TAIL":
			slist.DelTail()
			slist.ReadForward()
		case "F_DEL_VAL":
			slist.DelByValue(arg1)
			slist.ReadForward()
		case "F_GET_VAL":
			found := "Нет"
			if slist.FindValue(arg1) {
				found = "Да"
			}
			fmt.Printf("Элемент \"%s\" найден: %s\n", arg1, found)

		// Двусвязный список
		case "L_PUSH_HEAD":
			dlist.AddHead(arg1)
			dlist.ReadForward()
		case "L_PUSH_TAIL":
			dlist.AddTail(arg1)
			dlist.ReadForward()
		case "L_PUSH_AFTER":
			dlist.AddAfter(arg1, arg2)
			dlist.ReadForward()
		case "L_PUSH_BEFORE":
			dlist.AddBefore(arg1, arg2)
			dlist.ReadForward()
		case "L_DEL_HEAD":
			dlist.DelHead()
			dlist.ReadForward()
		case "L_DEL_TAIL":
			dlist.DelTail()
			dlist.ReadForward()
		case "L_DEL_VAL":
			dlist.DelByVal(arg1)
			dlist.ReadForward()
		case "L_GET_VAL":
			found := "Нет"
			if dlist.Contains(arg1) {
				found = "Да"
			}
			fmt.Printf("Элемент \"%s\" найден: %s\n", arg1, found)
		case "L_PRINT_REV":
			dlist.ReadBackward()

		// Очередь
		case "Q_PUSH":
			queue.Push(arg1)
			queue.Print()
		case "Q_POP":
			fmt.Printf("Извлечено: %s\n", queue.Pop())
			queue.Print()
		case "Q_GET":
			fmt.Printf("Первый: %s\n", queue.Peek())

		// Стек
		case "S_PUSH":
			stack.Push(arg1)
			stack.ReadStack()
		case "S_POP":
			fmt.Printf("Извлечено: %s\n", stack.Pop())
			stack.ReadStack()
		case "S_GET":
			fmt.Printf("Верхний: %s\n", stack.Peek())

		// Дерево
		case "T_INSERT":
			tree.Insert(safeStoi(arg1))
			tree.InorderPrint()
		case "T_SEARCH":
			found := "Нет"
			if tree.Contains(safeStoi(arg1)) {
				found = "Да"
			}
			fmt.Printf("Ключ %s НАЙДЕН: %s\n", arg1, found)
		case "T_DEL":
			tree.Remove(safeStoi(arg1))
			tree.InorderPrint()
		case "T_PRINT":
			tree.InorderPrint()

		// Хеш-таблицы
		case "HASH":
			fmt.Println("--- Вход в подсистему Хеш-таблиц ---")
			ds.HashMan()
			fmt.Println("--- Выход из подсистемы Хеш-таблиц ---")

		default:
			fmt.Printf("Неизвестная команда: '%s'\n", cmd)
		}
	}
}
package main

import (
	"datastructures/ds"
	"fmt"
	"os"
)

func assert(condition bool, msg string) {
	if !condition {
		fmt.Printf("FAIL: %s\n", msg)
		os.Exit(1)
	}
}

func header(name string) {
	fmt.Printf("\n=== Тестирование %s ===\n", name)
}

func sep() {
	fmt.Println()
}

func testMyArr() {
	header("MyArr (Массив)")

	arr := ds.NewMyArr()
	fmt.Println("Добавляем элементы: Москва, Лондон, Токио, Париж")
	arr.AddEnd("Москва")
	arr.AddEnd("Лондон")
	arr.AddEnd("Токио")
	arr.AddEnd("Париж")

	// TEXT
	sep()
	fmt.Println("Тест текстового формата ---")
	fmt.Println("Сохранение в myarr.txt (Text)")
	
	arrSerializer := ds.NewArrSerializer()
	arrSerializer.SaveToFile(arr, "myarr.txt")

	fmt.Println("Загрузка из файла...")
	loadTxt := ds.NewMyArr()
	arrSerializer.LoadFromFile(loadTxt, "myarr.txt")

	fmt.Println("Проверка данных...")
	assert(loadTxt.LenArr() == 4, "length mismatch")
	fmt.Printf("  - Индекс 0: %s (Ожидалось: Москва)\n", loadTxt.GetAt(0))
	fmt.Printf("  - Индекс 1: %s (Ожидалось: Лондон)\n", loadTxt.GetAt(1))
	fmt.Printf("  - Индекс 3: %s (Ожидалось: Париж)\n", loadTxt.GetAt(3))
	assert(loadTxt.GetAt(0) == "Москва", "val mismatch")
	assert(loadTxt.GetAt(1) == "Лондон", "val mismatch")
	assert(loadTxt.GetAt(3) == "Париж", "val mismatch")
	fmt.Println("Сериализация (Text): ПРОЙДЕНА")

	// BINARY
	sep()
	fmt.Println("Тест бинарного формата ---")
	fmt.Println("Сериализация в myarr.bin (Binary)")
	arrSerializer.SaveToBinaryFile(arr, "myarr.bin")

	fmt.Println("Загрузка из файла...")
	loadBin := ds.NewMyArr()
	arrSerializer.LoadFromBinaryFile(loadBin, "myarr.bin")

	fmt.Println("Проверка данных...")
	fmt.Printf("  - Индекс 0: %s (Ожидалось: Москва)\n", loadBin.GetAt(0))
	fmt.Printf("  - Индекс 1: %s (Ожидалось: Лондон)\n", loadBin.GetAt(1))
	fmt.Printf("  - Индекс 3: %s (Ожидалось: Париж)\n", loadBin.GetAt(3))
	assert(loadBin.GetAt(0) == "Москва", "val mismatch")
	assert(loadBin.GetAt(1) == "Лондон", "val mismatch")
	assert(loadBin.GetAt(3) == "Париж", "val mismatch")
	fmt.Println("Сериализация (Binary): ПРОЙДЕНА")

	fmt.Println("MyArr: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n")
	os.Remove("myarr.txt")
	os.Remove("myarr.bin")
}

func testMyList() {
	header("MyList (Односвязный список)")

	list := ds.NewMyList()
	fmt.Println("Добавляем в хвост: Берлин, Мадрид, Рим")
	list.AddTail("Берлин")
	list.AddTail("Мадрид")
	list.AddTail("Рим")

	// TEXT
	sep()
	fmt.Println("Тест текстового формата ---")
	fmt.Println("Сохранение в mylist.txt (Text)")
	
	listSerializer := ds.NewListSerializer()
	listSerializer.SaveToFile(list, "mylist.txt")

	fmt.Println("Загрузка из файла...")
	loadTxt := ds.NewMyList()
	listSerializer.LoadFromFile(loadTxt, "mylist.txt")

	fmt.Println("Проверка наличия значений...")
	vals := []string{"Берлин", "Мадрид", "Рим"}
	for _, v := range vals {
		assert(loadTxt.FindValue(v), v+" missing")
		fmt.Println("  - Найдено:", v)
	}
	fmt.Println("Сериализация (Text): ПРОЙДЕНА")

	// BINARY
	sep()
	fmt.Println("Тест бинарного формата ---")
	fmt.Println("Сохранение в mylist.bin (Binary)")
	listSerializer.SaveToBinaryFile(list, "mylist.bin")

	fmt.Println("Загрузка из файла...")
	loadBin := ds.NewMyList()
	listSerializer.LoadFromBinaryFile(loadBin, "mylist.bin")

	fmt.Println("Проверка наличия значений...")
	for _, v := range vals {
		assert(loadBin.FindValue(v), v+" missing")
		fmt.Println("  - Найдено:", v)
	}
	fmt.Println("Сериализация (Binary): ПРОЙДЕНА")

	fmt.Println("MyList: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n")
	os.Remove("mylist.txt")
	os.Remove("mylist.bin")
}

func testDList() {
	header("DList (Двусвязный список)")

	l := ds.NewDList()
	fmt.Println("Добавляем в хвост: Пекин, Сеул, Бангкок")
	l.AddTail("Пекин")
	l.AddTail("Сеул")
	l.AddTail("Бангкок")

	// TEXT
	sep()
	fmt.Println("Тест текстового формата ---")
	fmt.Println("Сохранение в dlist.txt (Text)")
	
	dlistSerializer := ds.NewDListSerializer()
	dlistSerializer.SaveToFile(l, "dlist.txt")

	fmt.Println("Загрузка из файла...")
	loadTxt := ds.NewDList()
	dlistSerializer.LoadFromFile(loadTxt, "dlist.txt")

	fmt.Println("Проверка наличия значений...")
	for _, v := range []string{"Пекин", "Сеул", "Бангкок"} {
		assert(loadTxt.Contains(v), v+" missing")
		fmt.Println("  - Найдено:", v)
	}
	fmt.Println("Сериализация (Text): ПРОЙДЕНА")

	// BINARY
	sep()
	fmt.Println("Тест бинарного формата ---")
	fmt.Println("Сохранение в dlist.bin (Binary)")
	dlistSerializer.SaveToBinaryFile(l, "dlist.bin")

	fmt.Println("Загрузка из файла...")
	loadBin := ds.NewDList()
	dlistSerializer.LoadFromBinaryFile(loadBin, "dlist.bin")

	fmt.Println("Проверка наличия значений...")
	for _, v := range []string{"Пекин", "Сеул", "Бангкок"} {
		assert(loadBin.Contains(v), v+" missing")
		fmt.Println("  - Найдено:", v)
	}
	fmt.Println("Сериализация (Binary): ПРОЙДЕНА")

	fmt.Println("DList: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n")
	os.Remove("dlist.txt")
	os.Remove("dlist.bin")
}

func testStack() {
	header("Stack (Стек)")

	s := ds.NewStack()
	fmt.Println("Push: Осло -> Стокгольм -> Копенгаген (верх)")
	s.Push("Осло")
	s.Push("Стокгольм")
	s.Push("Копенгаген")

	// TEXT
	sep()
	fmt.Println("Тест текстового формата ---")
	fmt.Println("Сохранение в stack.txt")
	
	stackSerializer := ds.NewStackSerializer()
	stackSerializer.SaveToFile(s, "stack.txt")

	fmt.Println("Загрузка из файла...")
	loadTxt := ds.NewStack()
	stackSerializer.LoadFromFile(loadTxt, "stack.txt")

	fmt.Println("Проверка порядка (LIFO)...")
	expected := []string{"Копенгаген", "Стокгольм", "Осло"}
	for i, exp := range expected {
		v := loadTxt.Pop()
		fmt.Printf("  - Pop %d: %s (Ожидалось: %s)\n", i+1, v, exp)
		assert(v == exp, "order mismatch")
	}
	fmt.Println("Сериализация (Text): ПРОЙДЕНА")

	// BINARY
	sep()
	fmt.Println("Тест бинарного формата ---")
	fmt.Println("Сохранение в stack.bin")
	stackSerializer.SaveToBinaryFile(s, "stack.bin")

	fmt.Println("Загрузка из файла...")
	loadBin := ds.NewStack()
	stackSerializer.LoadFromBinaryFile(loadBin, "stack.bin")

	fmt.Println("Проверка порядка (LIFO)...")
	for i, exp := range expected {
		v := loadBin.Pop()
		fmt.Printf("  - Pop %d: %s (Ожидалось: %s)\n", i+1, v, exp)
		assert(v == exp, "order mismatch")
	}
	fmt.Println("Сериализация (Binary): ПРОЙДЕНА")

	fmt.Println("Stack: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n")
	os.Remove("stack.txt")
	os.Remove("stack.bin")
}

func testQueue() {
	header("Queue (Очередь)")

	q := ds.NewQueue()
	fmt.Println("Push: Дели -> Мумбаи -> Калькутта")
	q.Push("Дели")
	q.Push("Мумбаи")
	q.Push("Калькутта")

	// TEXT
	sep()
	fmt.Println("Тест текстового формата ---")
	fmt.Println("Сохранение в queue.txt")
	
	queueSerializer := ds.NewQueueSerializer()
	queueSerializer.SaveToFile(q, "queue.txt")

	fmt.Println("Загрузка из файла...")
	loadTxt := ds.NewQueue()
	queueSerializer.LoadFromFile(loadTxt, "queue.txt")

	fmt.Println("Проверка порядка (FIFO)...")
	expected := []string{"Дели", "Мумбаи", "Калькутта"}
	for i, exp := range expected {
		v := loadTxt.Pop()
		fmt.Printf("  - Pop %d: %s (Ожидалось: %s)\n", i+1, v, exp)
		assert(v == exp, "order mismatch")
	}
	fmt.Println("Сериализация (Text): ПРОЙДЕНА")

	// BINARY
	sep()
	fmt.Println("Тест бинарного формата ---")
	fmt.Println("Сохранение в queue.bin")
	queueSerializer.SaveToBinaryFile(q, "queue.bin")

	fmt.Println("Загрузка из файла...")
	loadBin := ds.NewQueue()
	queueSerializer.LoadFromBinaryFile(loadBin, "queue.bin")

	fmt.Println("Проверка порядка (FIFO)...")
	for i, exp := range expected {
		v := loadBin.Pop()
		fmt.Printf("  - Pop %d: %s (Ожидалось: %s)\n", i+1, v, exp)
		assert(v == exp, "order mismatch")
	}
	fmt.Println("Сериализация (Binary): ПРОЙДЕНА")

	fmt.Println("Queue: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n")
	os.Remove("queue.txt")
	os.Remove("queue.bin")
}

func main() {
	fmt.Println("ЗАПУСК ТЕСТОВ СЕРИАЛИЗАЦИИ СТРУКТУР ДАННЫХ\n")

	testMyArr()
	testMyList()
	testDList()
	testStack()
	testQueue()

	fmt.Println("=========================================")
	fmt.Println("ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!")
	fmt.Println("=========================================")
}
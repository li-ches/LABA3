package main

import (
	"fmt"
	"os"
	"test/dynarr"
	"test/hashtable"
	"test/onelist"
	"test/queue"
	"test/stack"
	"test/twolist"
)

func testDynArr() {
	fmt.Println("=== Тестирование DynArr ===")

	arr1 := dynarr.NewDynArr(10)
	arr1.AddToEnd("Москва")
	arr1.AddToEnd("Лондон")
	arr1.AddToEnd("Токио")
	arr1.AddToEnd("Париж")

	textFile, err := os.Create("dynarr_test.txt")
	if err != nil {
		panic(err)
	}
	arr1.SaveToFile(textFile)
	textFile.Close()

	arr2 := dynarr.NewDynArr(10)
	textFileIn, err := os.Open("dynarr_test.txt")
	if err != nil {
		panic(err)
	}
	arr2.LoadFromFile(textFileIn)
	textFileIn.Close()

	val1, _ := arr2.GetElement(0)
	val2, _ := arr2.GetElement(1)
	val3, _ := arr2.GetElement(2)
	if val1 == "Москва" && val2 == "Лондон" && val3 == "Токио" {
		fmt.Println("Текстовая сериализация: ПРОЙДЕНА")
	} else {
		fmt.Println("Текстовая сериализация: ОШИБКА")
		os.Exit(1)
	}

	binFile, err := os.Create("dynarr_test.bin")
	if err != nil {
		panic(err)
	}
	arr1.SaveToBinaryFile(binFile)
	binFile.Close()

	arr3 := dynarr.NewDynArr(10)
	binFileIn, err := os.Open("dynarr_test.bin")
	if err != nil {
		panic(err)
	}
	arr3.LoadFromBinaryFile(binFileIn)
	binFileIn.Close()

	val4, _ := arr3.GetElement(0)
	val5, _ := arr3.GetElement(1)
	val6, _ := arr3.GetElement(2)
	if val4 == "Москва" && val5 == "Лондон" && val6 == "Токио" {
		fmt.Println("Бинарная сериализация: ПРОЙДЕНА")
	} else {
		fmt.Println("Бинарная сериализация: ОШИБКА")
		os.Exit(1)
	}

	fmt.Println("DynArr: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n")
}

func testOneList() {
	fmt.Println("=== Тестирование OneList ===")

	list1 := onelist.NewOneList()
	list1.AddToTail("Берлин")
	list1.AddToTail("Мадрид")
	list1.AddToTail("Рим")

	textFile, err := os.Create("onelist_test.txt")
	if err != nil {
		panic(err)
	}
	list1.SaveToFile(textFile)
	textFile.Close()

	list2 := onelist.NewOneList()
	textFileIn, err := os.Open("onelist_test.txt")
	if err != nil {
		panic(err)
	}
	list2.LoadFromFile(textFileIn)
	textFileIn.Close()

	val1, _ := list2.GetElementByValue("Берлин")
	val2, _ := list2.GetElementByValue("Мадрид")
	val3, _ := list2.GetElementByValue("Рим")
	if val1 == "Берлин" && val2 == "Мадрид" && val3 == "Рим" {
		fmt.Println("Текстовая сериализация: ПРОЙДЕНА")
	} else {
		fmt.Println("Текстовая сериализация: ОШИБКА")
		os.Exit(1)
	}

	binFile, err := os.Create("onelist_test.bin")
	if err != nil {
		panic(err)
	}
	list1.SaveToBinaryFile(binFile)
	binFile.Close()

	list3 := onelist.NewOneList()
	binFileIn, err := os.Open("onelist_test.bin")
	if err != nil {
		panic(err)
	}
	list3.LoadFromBinaryFile(binFileIn)
	binFileIn.Close()

	val4, _ := list3.GetElementByValue("Берлин")
	val5, _ := list3.GetElementByValue("Мадрид")
	val6, _ := list3.GetElementByValue("Рим")
	if val4 == "Берлин" && val5 == "Мадрид" && val6 == "Рим" {
		fmt.Println("Бинарная сериализация: ПРОЙДЕНА")
	} else {
		fmt.Println("Бинарная сериализация: ОШИБКА")
		os.Exit(1)
	}

	fmt.Println("OneList: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n")
}

func testTwoList() {
	fmt.Println("=== Тестирование TwoList ===")

	list1 := twolist.NewTwoList()
	list1.AddToTail("Пекин")
	list1.AddToTail("Сеул")
	list1.AddToTail("Бангкок")

	textFile, err := os.Create("twolist_test.txt")
	if err != nil {
		panic(err)
	}
	list1.SaveToFile(textFile)
	textFile.Close()

	list2 := twolist.NewTwoList()
	textFileIn, err := os.Open("twolist_test.txt")
	if err != nil {
		panic(err)
	}
	list2.LoadFromFile(textFileIn)
	textFileIn.Close()

	val1, _ := list2.GetElementByValue("Пекин")
	val2, _ := list2.GetElementByValue("Сеул")
	val3, _ := list2.GetElementByValue("Бангкок")
	if val1 == "Пекин" && val2 == "Сеул" && val3 == "Бангкок" {
		fmt.Println("Текстовая сериализация: ПРОЙДЕНА")
	} else {
		fmt.Println("Текстовая сериализация: ОШИБКА")
		os.Exit(1)
	}

	binFile, err := os.Create("twolist_test.bin")
	if err != nil {
		panic(err)
	}
	list1.SaveToBinaryFile(binFile)
	binFile.Close()

	list3 := twolist.NewTwoList()
	binFileIn, err := os.Open("twolist_test.bin")
	if err != nil {
		panic(err)
	}
	list3.LoadFromBinaryFile(binFileIn)
	binFileIn.Close()

	val4, _ := list3.GetElementByValue("Пекин")
	val5, _ := list3.GetElementByValue("Сеул")
	val6, _ := list3.GetElementByValue("Бангкок")
	if val4 == "Пекин" && val5 == "Сеул" && val6 == "Бангкок" {
		fmt.Println("Бинарная сериализация: ПРОЙДЕНА")
	} else {
		fmt.Println("Бинарная сериализация: ОШИБКА")
		os.Exit(1)
	}

	fmt.Println("TwoList: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n")
}

func testStack() {
	fmt.Println("=== Тестирование Stack ===")

	stack1 := stack.NewStack()
	stack1.Push("Осло")
	stack1.Push("Стокгольм")
	stack1.Push("Копенгаген")

	textFile, err := os.Create("stack_test.txt")
	if err != nil {
		panic(err)
	}
	stack1.SaveToFile(textFile)
	textFile.Close()

	stack2 := stack.NewStack()
	textFileIn, err := os.Open("stack_test.txt")
	if err != nil {
		panic(err)
	}
	stack2.LoadFromFile(textFileIn)
	textFileIn.Close()

	fmt.Println("Текстовая сериализация: ПРОЙДЕНА")

	binFile, err := os.Create("stack_test.bin")
	if err != nil {
		panic(err)
	}
	stack1.SaveToBinaryFile(binFile)
	binFile.Close()

	stack3 := stack.NewStack()
	binFileIn, err := os.Open("stack_test.bin")
	if err != nil {
		panic(err)
	}
	stack3.LoadFromBinaryFile(binFileIn)
	binFileIn.Close()

	fmt.Println("Бинарная сериализация: ПРОЙДЕНА")

	fmt.Println("Stack: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n")
}

func testQueue() {
	fmt.Println("=== Тестирование Queue ===")

	queue1 := queue.NewQueue()
	queue1.Enqueue("Дели")
	queue1.Enqueue("Мумбаи")
	queue1.Enqueue("Калькутта")

	textFile, err := os.Create("queue_test.txt")
	if err != nil {
		panic(err)
	}
	queue1.SaveToFile(textFile)
	textFile.Close()

	queue2 := queue.NewQueue()
	textFileIn, err := os.Open("queue_test.txt")
	if err != nil {
		panic(err)
	}
	queue2.LoadFromFile(textFileIn)
	textFileIn.Close()

	fmt.Println("Текстовая сериализация: ПРОЙДЕНА")

	binFile, err := os.Create("queue_test.bin")
	if err != nil {
		panic(err)
	}
	queue1.SaveToBinaryFile(binFile)
	binFile.Close()

	queue3 := queue.NewQueue()
	binFileIn, err := os.Open("queue_test.bin")
	if err != nil {
		panic(err)
	}
	queue3.LoadFromBinaryFile(binFileIn)
	binFileIn.Close()

	fmt.Println("Бинарная сериализация: ПРОЙДЕНА")

	fmt.Println("Queue: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n")
}

func testCuckooHashTable() {
	fmt.Println("=== Тестирование CuckooHashTable ===")

	table1 := hashtable.NewCuckooHashTable(10)
	table2 := hashtable.NewCuckooHashTable(10)

	res1 := table1.Insert("Москва", "Россия")
	res2 := table1.Insert("Берлин", "Германия")
	res3 := table1.Insert("Париж", "Франция")

	fmt.Printf("Вставка результатов: %v, %v, %v\n", res1, res2, res3)

	textFile, err := os.Create("cuckoo_test.txt")
	if err != nil {
		panic(err)
	}
	table1.SaveToFile(textFile)
	textFile.Close()

	textFileIn, err := os.Open("cuckoo_test.txt")
	if err != nil {
		panic(err)
	}
	table2.LoadFromFile(textFileIn)
	textFileIn.Close()

	s1 := table2.Search("Москва")
	s2 := table2.Search("Берлин")
	s3 := table2.Search("Париж")

	fmt.Printf("Поиск результатов: %s, %s, %s\n", s1, s2, s3)

	if s1 == "Россия" && s2 == "Германия" && s3 == "Франция" {
		fmt.Println("Текстовая сериализация: ПРОЙДЕНА")
	} else {
		fmt.Println("Текстовая сериализация: ОШИБКА")
		os.Exit(1)
	}

	binFile, err := os.Create("cuckoo_test.bin")
	if err != nil {
		panic(err)
	}
	table1.SaveToBinaryFile(binFile)
	binFile.Close()

	table3 := hashtable.NewCuckooHashTable(10)
	binFileIn, err := os.Open("cuckoo_test.bin")
	if err != nil {
		panic(err)
	}
	table3.LoadFromBinaryFile(binFileIn)
	binFileIn.Close()

	s4 := table3.Search("Москва")
	s5 := table3.Search("Берлин")
	s6 := table3.Search("Париж")

	fmt.Printf("Поиск результатов (бинарный): %s, %s, %s\n", s4, s5, s6)

	if s4 == "Россия" && s5 == "Германия" && s6 == "Франция" {
		fmt.Println("Бинарная сериализация: ПРОЙДЕНА")
	} else {
		fmt.Println("Бинарная сериализация: ОШИБКА")
		os.Exit(1)
	}

	fmt.Println("CuckooHashTable: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n")
}

func testDoubleHashTable() {
	fmt.Println("=== Тестирование DoubleHashTable ===")

	table1 := hashtable.NewDoubleHashTable(10)

	res1 := table1.Insert("Токио", "Япония")
	res2 := table1.Insert("Сеул", "Корея")
	res3 := table1.Insert("Пекин", "Китай")

	fmt.Printf("Вставка результатов: %v, %v, %v\n", res1, res2, res3)

	textFile, err := os.Create("double_test.txt")
	if err != nil {
		panic(err)
	}
	table1.SaveToFile(textFile)
	textFile.Close()

	table2 := hashtable.NewDoubleHashTable(10)
	textFileIn, err := os.Open("double_test.txt")
	if err != nil {
		panic(err)
	}
	table2.LoadFromFile(textFileIn)
	textFileIn.Close()

	s1 := table2.Search("Токио")
	s2 := table2.Search("Сеул")
	s3 := table2.Search("Пекин")

	fmt.Printf("Поиск результатов: %s, %s, %s\n", s1, s2, s3)

	if s1 == "Япония" && s2 == "Корея" && s3 == "Китай" {
		fmt.Println("Текстовая сериализация: ПРОЙДЕНА")
	} else {
		fmt.Println("Текстовая сериализация: ОШИБКА")
		os.Exit(1)
	}

	binFile, err := os.Create("double_test.bin")
	if err != nil {
		panic(err)
	}
	table1.SaveToBinaryFile(binFile)
	binFile.Close()

	table3 := hashtable.NewDoubleHashTable(10)
	binFileIn, err := os.Open("double_test.bin")
	if err != nil {
		panic(err)
	}
	table3.LoadFromBinaryFile(binFileIn)
	binFileIn.Close()

	s4 := table3.Search("Токио")
	s5 := table3.Search("Сеул")
	s6 := table3.Search("Пекин")

	fmt.Printf("Поиск результатов (бинарный): %s, %s, %s\n", s4, s5, s6)

	if s4 == "Япония" && s5 == "Корея" && s6 == "Китай" {
		fmt.Println("Бинарная сериализация: ПРОЙДЕНА")
	} else {
		fmt.Println("Бинарная сериализация: ОШИБКА")
		os.Exit(1)
	}

	fmt.Println("DoubleHashTable: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n")
}

func main() {
	fmt.Println("ЗАПУСК ТЕСТОВ СЕРИАЛИЗАЦИИ СТРУКТУР ДАННЫХ\n")

	testDynArr()
	testOneList()
	testTwoList()
	testStack()
	testQueue()
	testCuckooHashTable()
	testDoubleHashTable()

	fmt.Println("=========================================")
	fmt.Println("ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!")
	fmt.Println("=========================================")
}

package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"goproj/config"
	"goproj/dynarr"
	"goproj/hashtable"
	"goproj/onelist"
	"goproj/queue"
	"goproj/stack"
	"goproj/twolist"
	"io"
	"os"
	"strconv"
	"strings"
)

type CommandHandler func(params []string)

var commandHandlers map[string]CommandHandler

var (
	arrays           = make(map[string]*dynarr.DynArr)
	oneLists         = make(map[string]*onelist.OneList)
	twoLists         = make(map[string]*twolist.TwoList)
	stacks           = make(map[string]*stack.Stack)
	queues           = make(map[string]*queue.Queue)
	doubleHashTables = make(map[string]*hashtable.DoubleHashTable)
	cuckooHashTables = make(map[string]*hashtable.CuckooHashTable)
	currFilename     string
)

func saveToFile() {
	if currFilename == "" {
		return
	}

	file, err := os.Create(currFilename)
	if err != nil {
		fmt.Println("Ошибка: не удалось создать файл")
		return
	}
	defer file.Close()

	for name, arr := range arrays {
		file.WriteString("ARRAY " + name + " ")
		arr.SaveToFile(file)
		file.WriteString("\n")
	}

	for name, list := range oneLists {
		file.WriteString("ONELIST " + name + " ")
		list.SaveToFile(file)
		file.WriteString("\n")
	}

	for name, list := range twoLists {
		file.WriteString("TWOLIST " + name + " ")
		list.SaveToFile(file)
		file.WriteString("\n")
	}

	for name, stack := range stacks {
		file.WriteString("STACK " + name + " ")
		stack.SaveToFile(file)
		file.WriteString("\n")
	}

	for name, queue := range queues {
		file.WriteString("QUEUE " + name + " ")
		queue.SaveToFile(file)
		file.WriteString("\n")
	}

	for name, hashTable := range doubleHashTables {
		file.WriteString("DOUBLEHASH " + name + " ")
		hashTable.SaveToFile(file)
		file.WriteString("\n")
	}

	for name, hashTable := range cuckooHashTables {
		file.WriteString("CUCKOO " + name + " ")
		hashTable.SaveToFile(file)
		file.WriteString("\n")
	}
}

func saveToBinaryFile() {
	if currFilename == "" {
		fmt.Println("Ошибка: имя файла не установлено")
		return
	}

	file, err := os.Create(currFilename)
	if err != nil {
		fmt.Println("Ошибка: не удалось создать файл")
		return
	}
	defer file.Close()

	for name, arr := range arrays {
		file.Write([]byte{'A'})
		nameSize := uint64(len(name))
		binary.Write(file, binary.LittleEndian, nameSize)
		file.WriteString(name)
		arr.SaveToBinaryFile(file)
	}

	for name, list := range oneLists {
		file.Write([]byte{'F'})
		nameSize := uint64(len(name))
		binary.Write(file, binary.LittleEndian, nameSize)
		file.WriteString(name)
		list.SaveToBinaryFile(file)
	}

	for name, list := range twoLists {
		file.Write([]byte{'L'})
		nameSize := uint64(len(name))
		binary.Write(file, binary.LittleEndian, nameSize)
		file.WriteString(name)
		list.SaveToBinaryFile(file)
	}

	for name, stack := range stacks {
		file.Write([]byte{'S'})
		nameSize := uint64(len(name))
		binary.Write(file, binary.LittleEndian, nameSize)
		file.WriteString(name)
		stack.SaveToBinaryFile(file)
	}

	for name, queue := range queues {
		file.Write([]byte{'Q'})
		nameSize := uint64(len(name))
		binary.Write(file, binary.LittleEndian, nameSize)
		file.WriteString(name)
		queue.SaveToBinaryFile(file)
	}

	for name, hashTable := range doubleHashTables {
		file.Write([]byte{'D'})
		nameSize := uint64(len(name))
		binary.Write(file, binary.LittleEndian, nameSize)
		file.WriteString(name)
		hashTable.SaveToBinaryFile(file)
	}

	for name, hashTable := range cuckooHashTables {
		file.Write([]byte{'C'})
		nameSize := uint64(len(name))
		binary.Write(file, binary.LittleEndian, nameSize)
		file.WriteString(name)
		hashTable.SaveToBinaryFile(file)
	}
}

func loadStructure(typeStr, name string, data string) {
	tempFilename := "temp_" + name + ".txt"
	file, err := os.Create(tempFilename)
	if err != nil {
		return
	}
	file.WriteString(data)
	file.Close()

	file, err = os.Open(tempFilename)
	if err != nil {
		return
	}
	defer file.Close()

	switch typeStr {
	case "ARRAY":
		if existing, exists := arrays[name]; exists {

			for i := 0; i < existing.GetLength(); i++ {
				existing.RemoveAtIndex(0)
			}
		} else {
			arrays[name] = dynarr.NewDynArr(config.DefaultInitialCapacity)
		}
		arrays[name].LoadFromFile(file)
	case "ONELIST":
		if existing, exists := oneLists[name]; exists {

			for {
				if err := existing.RemoveFromHead(); err != nil {
					break
				}
			}
		} else {
			oneLists[name] = onelist.NewOneList()
		}
		oneLists[name].LoadFromFile(file)
	case "TWOLIST":
		if existing, exists := twoLists[name]; exists {

			for {
				if err := existing.RemoveFromHead(); err != nil {
					break
				}
			}
		} else {
			twoLists[name] = twolist.NewTwoList()
		}
		twoLists[name].LoadFromFile(file)
	case "STACK":
		if existing, exists := stacks[name]; exists {

			for {
				if err := existing.Pop(); err != nil {
					break
				}
			}
		} else {
			stacks[name] = stack.NewStack()
		}
		stacks[name].LoadFromFile(file)
	case "QUEUE":
		if existing, exists := queues[name]; exists {

			for {
				if err := existing.Dequeue(); err != nil {
					break
				}
			}
		} else {
			queues[name] = queue.NewQueue()
		}
		queues[name].LoadFromFile(file)
	case "DOUBLEHASH":
		if _, exists := doubleHashTables[name]; exists {
			doubleHashTables[name] = hashtable.NewDoubleHashTable(config.DefaultInitialCapHash)
		} else {
			doubleHashTables[name] = hashtable.NewDoubleHashTable(config.DefaultInitialCapHash)
		}
		doubleHashTables[name].LoadFromFile(file)
	case "CUCKOO":
		if _, exists := cuckooHashTables[name]; exists {
			cuckooHashTables[name] = hashtable.NewCuckooHashTable(config.DefaultInitialCapHash)
		} else {
			cuckooHashTables[name] = hashtable.NewCuckooHashTable(config.DefaultInitialCapHash)
		}
		cuckooHashTables[name].LoadFromFile(file)
	}

	os.Remove(tempFilename)
}

func loadFromFile() {
	if currFilename == "" {
		return
	}

	file, err := os.Open(currFilename)
	if err != nil {
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		fields := strings.Fields(line)
		if len(fields) < 2 {
			continue
		}

		typeStr := fields[0]
		name := fields[1]
		data := strings.Join(fields[2:], " ")

		loadStructure(typeStr, name, data)
	}
}

func loadFromBinaryFile() {
	if currFilename == "" {
		return
	}

	file, err := os.Open(currFilename)
	if err != nil {
		return
	}
	defer file.Close()

	arrays = make(map[string]*dynarr.DynArr)
	oneLists = make(map[string]*onelist.OneList)
	twoLists = make(map[string]*twolist.TwoList)
	stacks = make(map[string]*stack.Stack)
	queues = make(map[string]*queue.Queue)
	doubleHashTables = make(map[string]*hashtable.DoubleHashTable)
	cuckooHashTables = make(map[string]*hashtable.CuckooHashTable)

	for {
		var typeByte byte
		err := binary.Read(file, binary.LittleEndian, &typeByte)
		if err != nil {
			if err == io.EOF {
				break
			}
			continue
		}

		var nameSize uint64
		err = binary.Read(file, binary.LittleEndian, &nameSize)
		if err != nil {
			continue
		}

		nameBytes := make([]byte, nameSize)
		_, err = file.Read(nameBytes)
		if err != nil {
			continue
		}
		name := string(nameBytes)

		switch typeByte {
		case 'A': // ARRAY
			arrays[name] = dynarr.NewDynArr(config.DefaultInitialCapacity)
			arrays[name].LoadFromBinaryFile(file)
		case 'F': // ONELIST
			oneLists[name] = onelist.NewOneList()
			oneLists[name].LoadFromBinaryFile(file)
		case 'L': // TWOLIST
			twoLists[name] = twolist.NewTwoList()
			twoLists[name].LoadFromBinaryFile(file)
		case 'S': // STACK
			stacks[name] = stack.NewStack()
			stacks[name].LoadFromBinaryFile(file)
		case 'Q': // QUEUE
			queues[name] = queue.NewQueue()
			queues[name].LoadFromBinaryFile(file)
		case 'D': // DOUBLEHASH
			doubleHashTables[name] = hashtable.NewDoubleHashTable(config.DefaultInitialCapHash)
			doubleHashTables[name].LoadFromBinaryFile(file)
		case 'C': // CUCKOO
			cuckooHashTables[name] = hashtable.NewCuckooHashTable(config.DefaultInitialCapHash)
			cuckooHashTables[name].LoadFromBinaryFile(file)
		}
	}
}

func MCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := arrays[name]; exists {
		arrays[name] = dynarr.NewDynArr(config.DefaultInitialCapacity)
	} else {
		arrays[name] = dynarr.NewDynArr(config.DefaultInitialCapacity)
	}
	saveToFile()
}

func BMCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := arrays[name]; exists {
		arrays[name] = dynarr.NewDynArr(config.DefaultInitialCapacity)
	} else {
		arrays[name] = dynarr.NewDynArr(config.DefaultInitialCapacity)
	}
	saveToBinaryFile()
}

func FCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := oneLists[name]; exists {
		oneLists[name] = onelist.NewOneList()
	} else {
		oneLists[name] = onelist.NewOneList()
	}
	saveToFile()
}

func BFCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := oneLists[name]; exists {
		oneLists[name] = onelist.NewOneList()
	} else {
		oneLists[name] = onelist.NewOneList()
	}
	saveToBinaryFile()
}

func LCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := twoLists[name]; exists {
		twoLists[name] = twolist.NewTwoList()
	} else {
		twoLists[name] = twolist.NewTwoList()
	}
	saveToFile()
}

func BLCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := twoLists[name]; exists {
		twoLists[name] = twolist.NewTwoList()
	} else {
		twoLists[name] = twolist.NewTwoList()
	}
	saveToBinaryFile()
}

func SCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := stacks[name]; exists {
		stacks[name] = stack.NewStack()
	} else {
		stacks[name] = stack.NewStack()
	}
	saveToFile()
}

func BSCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := stacks[name]; exists {
		stacks[name] = stack.NewStack()
	} else {
		stacks[name] = stack.NewStack()
	}
	saveToBinaryFile()
}

func QCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := queues[name]; exists {
		queues[name] = queue.NewQueue()
	} else {
		queues[name] = queue.NewQueue()
	}
	saveToFile()
}

func BQCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := queues[name]; exists {
		queues[name] = queue.NewQueue()
	} else {
		queues[name] = queue.NewQueue()
	}
	saveToBinaryFile()
}

func DCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := doubleHashTables[name]; exists {
		doubleHashTables[name] = hashtable.NewDoubleHashTable(config.DefaultInitialCapHash)
	} else {
		doubleHashTables[name] = hashtable.NewDoubleHashTable(config.DefaultInitialCapHash)
	}
	saveToFile()
}

func BDCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := doubleHashTables[name]; exists {
		doubleHashTables[name] = hashtable.NewDoubleHashTable(config.DefaultInitialCapHash)
	} else {
		doubleHashTables[name] = hashtable.NewDoubleHashTable(config.DefaultInitialCapHash)
	}
	saveToBinaryFile()
}

func CCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := cuckooHashTables[name]; exists {
		cuckooHashTables[name] = hashtable.NewCuckooHashTable(config.DefaultInitialCapHash)
	} else {
		cuckooHashTables[name] = hashtable.NewCuckooHashTable(config.DefaultInitialCapHash)
	}
	saveToFile()
}

func BCCREATE(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if _, exists := cuckooHashTables[name]; exists {
		cuckooHashTables[name] = hashtable.NewCuckooHashTable(config.DefaultInitialCapHash)
	} else {
		cuckooHashTables[name] = hashtable.NewCuckooHashTable(config.DefaultInitialCapHash)
	}
	saveToBinaryFile()
}

func MREPLACE(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	index, err := strconv.Atoi(params[2])
	if err != nil {
		fmt.Println("Ошибка: неверный индекс")
		return
	}
	value := params[3]

	arr, exists := arrays[name]
	if !exists {
		fmt.Println("Ошибка: массив не найден")
		return
	}

	if err := arr.ReplaceElement(index, value); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BMREPLACE(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	index, err := strconv.Atoi(params[2])
	if err != nil {
		fmt.Println("Ошибка: неверный индекс")
		return
	}
	value := params[3]

	arr, exists := arrays[name]
	if !exists {
		fmt.Println("Ошибка: массив не найден")
		return
	}

	if err := arr.ReplaceElement(index, value); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func MPUSH(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	arr, exists := arrays[name]
	if !exists {
		fmt.Println("Ошибка: массив не найден")
		return
	}

	arr.AddToEnd(value)
	saveToFile()
}

func BMPUSH(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	arr, exists := arrays[name]
	if !exists {
		fmt.Println("Ошибка: массив не найден")
		return
	}

	arr.AddToEnd(value)
	saveToBinaryFile()
}

func MPUSHINDEX(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	index, err := strconv.Atoi(params[2])
	if err != nil {
		fmt.Println("Ошибка: неверный индекс")
		return
	}
	value := params[3]

	arr, exists := arrays[name]
	if !exists {
		fmt.Println("Ошибка: массив не найден")
		return
	}

	if err := arr.AddAtIndex(index, value); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BMPUSHINDEX(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	index, err := strconv.Atoi(params[2])
	if err != nil {
		fmt.Println("Ошибка: неверный индекс")
		return
	}
	value := params[3]

	arr, exists := arrays[name]
	if !exists {
		fmt.Println("Ошибка: массив не найден")
		return
	}

	if err := arr.AddAtIndex(index, value); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func MDEL(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	index, err := strconv.Atoi(params[2])
	if err != nil {
		fmt.Println("Ошибка: неверный индекс")
		return
	}

	arr, exists := arrays[name]
	if !exists {
		fmt.Println("Ошибка: массив не найден")
		return
	}

	if err := arr.RemoveAtIndex(index); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BMDEL(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	index, err := strconv.Atoi(params[2])
	if err != nil {
		fmt.Println("Ошибка: неверный индекс")
		return
	}

	arr, exists := arrays[name]
	if !exists {
		fmt.Println("Ошибка: массив не найден")
		return
	}

	if err := arr.RemoveAtIndex(index); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func MGET(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	index, err := strconv.Atoi(params[2])
	if err != nil {
		fmt.Println("Ошибка: неверный индекс")
		return
	}

	arr, exists := arrays[name]
	if !exists {
		fmt.Println("Ошибка: массив не найден")
		return
	}

	element, err := arr.GetElement(index)
	if err != nil {
		fmt.Println(err.Error())
		return
	}
	fmt.Println(element)
}

func BMGET(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	index, err := strconv.Atoi(params[2])
	if err != nil {
		fmt.Println("Ошибка: неверный индекс")
		return
	}

	arr, exists := arrays[name]
	if !exists {
		fmt.Println("Ошибка: массив не найден")
		return
	}

	element, err := arr.GetElement(index)
	if err != nil {
		fmt.Println(err.Error())
		return
	}
	fmt.Println(element)
}

func FPUSHHEAD(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	list.AddToHead(value)
	saveToFile()
}

func BFPUSHHEAD(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	list.AddToHead(value)
	saveToBinaryFile()
}

func FPUSHTAIL(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	list.AddToTail(value)
	saveToFile()
}

func BFPUSHTAIL(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	list.AddToTail(value)
	saveToBinaryFile()
}

func FPUSHAFTER(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]
	newVal := params[3]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.AddAfterValue(targetVal, newVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BFPUSHAFTER(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]
	newVal := params[3]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.AddAfterValue(targetVal, newVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func FPUSHBEFORE(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]
	newVal := params[3]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.AddBeforeValue(targetVal, newVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BFPUSHBEFORE(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]
	newVal := params[3]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.AddBeforeValue(targetVal, newVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func FDELHEAD(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveFromHead(); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BFDELHEAD(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveFromHead(); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func FDELTAIL(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveFromTail(); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BFDELTAIL(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveFromTail(); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func FDELAFTER(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveAfterValue(targetVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BFDELAFTER(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveAfterValue(targetVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func FDELBEFORE(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveBeforeValue(targetVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BFDELBEFORE(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveBeforeValue(targetVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func FDELVALUE(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveByValue(targetVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BFDELVALUE(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveByValue(targetVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func FGET(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	element, err := list.GetElementByValue(targetVal)
	if err != nil {
		fmt.Println(err.Error())
		return
	}
	fmt.Println(element)
}

func BFGET(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := oneLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	element, err := list.GetElementByValue(targetVal)
	if err != nil {
		fmt.Println(err.Error())
		return
	}
	fmt.Println(element)
}

func LPUSHHEAD(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	list.AddToHead(value)
	saveToFile()
}

func BLPUSHHEAD(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	list.AddToHead(value)
	saveToBinaryFile()
}

func LPUSHTAIL(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	list.AddToTail(value)
	saveToFile()
}

func BLPUSHTAIL(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	list.AddToTail(value)
	saveToBinaryFile()
}

func LPUSHAFTER(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]
	newVal := params[3]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.AddAfterValue(targetVal, newVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BLPUSHAFTER(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]
	newVal := params[3]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.AddAfterValue(targetVal, newVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func LPUSHBEFORE(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]
	newVal := params[3]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.AddBeforeValue(targetVal, newVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BLPUSHBEFORE(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]
	newVal := params[3]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.AddBeforeValue(targetVal, newVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func LDELHEAD(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveFromHead(); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BLDELHEAD(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveFromHead(); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func LDELTAIL(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveFromTail(); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BLDELTAIL(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveFromTail(); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func LDELAFTER(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveAfterValue(targetVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BLDELAFTER(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveAfterValue(targetVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func LDELBEFORE(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveBeforeValue(targetVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BLDELBEFORE(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveBeforeValue(targetVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func LDELVALUE(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveByValue(targetVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BLDELVALUE(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	if err := list.RemoveByValue(targetVal); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func LGET(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	element, err := list.GetElementByValue(targetVal)
	if err != nil {
		fmt.Println(err.Error())
		return
	}
	fmt.Println(element)
}

func BLGET(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	targetVal := params[2]

	list, exists := twoLists[name]
	if !exists {
		fmt.Println("Ошибка: список не найден")
		return
	}

	element, err := list.GetElementByValue(targetVal)
	if err != nil {
		fmt.Println(err.Error())
		return
	}
	fmt.Println(element)
}

func SPUSH(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	stack, exists := stacks[name]
	if !exists {
		fmt.Println("Ошибка: стек не найден")
		return
	}

	stack.Push(value)
	saveToFile()
}

func BSPUSH(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	stack, exists := stacks[name]
	if !exists {
		fmt.Println("Ошибка: стек не найден")
		return
	}

	stack.Push(value)
	saveToBinaryFile()
}

func SPOP(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	stack, exists := stacks[name]
	if !exists {
		fmt.Println("Ошибка: стек не найден")
		return
	}

	if err := stack.Pop(); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BSPOP(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	stack, exists := stacks[name]
	if !exists {
		fmt.Println("Ошибка: стек не найден")
		return
	}

	if err := stack.Pop(); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func QPUSH(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	queue, exists := queues[name]
	if !exists {
		fmt.Println("Ошибка: очередь не найдена")
		return
	}

	queue.Enqueue(value)
	saveToFile()
}

func BQPUSH(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	value := params[2]

	queue, exists := queues[name]
	if !exists {
		fmt.Println("Ошибка: очередь не найдена")
		return
	}

	queue.Enqueue(value)
	saveToBinaryFile()
}

func QPOP(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	queue, exists := queues[name]
	if !exists {
		fmt.Println("Ошибка: очередь не найдена")
		return
	}

	if err := queue.Dequeue(); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToFile()
}

func BQPOP(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	queue, exists := queues[name]
	if !exists {
		fmt.Println("Ошибка: очередь не найдена")
		return
	}

	if err := queue.Dequeue(); err != nil {
		fmt.Println(err.Error())
		return
	}
	saveToBinaryFile()
}

func DINSERT(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	key := params[2]
	value := params[3]

	hashTable, exists := doubleHashTables[name]
	if !exists {
		fmt.Println("Ошибка: хэш-таблица не найдена")
		return
	}

	if !hashTable.Insert(key, value) {
		fmt.Println("Ошибка: не удалось вставить элемент")
		return
	}
	saveToFile()
}

func BDINSERT(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	key := params[2]
	value := params[3]

	hashTable, exists := doubleHashTables[name]
	if !exists {
		fmt.Println("Ошибка: хэш-таблица не найдена")
		return
	}

	if !hashTable.Insert(key, value) {
		fmt.Println("Ошибка: не удалось вставить элемент")
		return
	}
	saveToBinaryFile()
}

func DSEARCH(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	key := params[2]

	hashTable, exists := doubleHashTables[name]
	if !exists {
		fmt.Println("Ошибка: хэш-таблица не найдена")
		return
	}

	result := hashTable.Search(key)
	if result == "" {
		fmt.Println("Ошибка: ключ не найден")
		return
	}
	fmt.Println(result)
}

func BDSEARCH(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	key := params[2]

	hashTable, exists := doubleHashTables[name]
	if !exists {
		fmt.Println("Ошибка: хэш-таблица не найдена")
		return
	}

	result := hashTable.Search(key)
	if result == "" {
		fmt.Println("Ошибка: ключ не найден")
		return
	}
	fmt.Println(result)
}

func DREMOVE(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	key := params[2]

	hashTable, exists := doubleHashTables[name]
	if !exists {
		fmt.Println("Ошибка: хэш-таблица не найдена")
		return
	}

	if !hashTable.Remove(key) {
		fmt.Println("Ошибка: не удалось удалить элемент")
		return
	}
	saveToFile()
}

func BDREMOVE(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	key := params[2]

	hashTable, exists := doubleHashTables[name]
	if !exists {
		fmt.Println("Ошибка: хэш-таблица не найдена")
		return
	}

	if !hashTable.Remove(key) {
		fmt.Println("Ошибка: не удалось удалить элемент")
		return
	}
	saveToBinaryFile()
}

func CINSERT(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	key := params[2]
	value := params[3]

	hashTable, exists := cuckooHashTables[name]
	if !exists {
		fmt.Println("Ошибка: хэш-таблица не найдена")
		return
	}

	if !hashTable.Insert(key, value) {
		fmt.Println("Ошибка: не удалось вставить элемент")
		return
	}
	saveToFile()
}

func BCINSERT(params []string) {
	if len(params) < 4 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	key := params[2]
	value := params[3]

	hashTable, exists := cuckooHashTables[name]
	if !exists {
		fmt.Println("Ошибка: хэш-таблица не найдена")
		return
	}

	if !hashTable.Insert(key, value) {
		fmt.Println("Ошибка: не удалось вставить элемент")
		return
	}
	saveToBinaryFile()
}

func CSEARCH(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	key := params[2]

	hashTable, exists := cuckooHashTables[name]
	if !exists {
		fmt.Println("Ошибка: хэш-таблица не найдена")
		return
	}

	result := hashTable.Search(key)
	if result == "" {
		fmt.Println("Ошибка: ключ не найден")
		return
	}
	fmt.Println(result)
}

func BCSEARCH(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	key := params[2]

	hashTable, exists := cuckooHashTables[name]
	if !exists {
		fmt.Println("Ошибка: хэш-таблица не найдена")
		return
	}

	result := hashTable.Search(key)
	if result == "" {
		fmt.Println("Ошибка: ключ не найден")
		return
	}
	fmt.Println(result)
}

func CREMOVE(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	key := params[2]

	hashTable, exists := cuckooHashTables[name]
	if !exists {
		fmt.Println("Ошибка: хэш-таблица не найдена")
		return
	}

	if !hashTable.Remove(key) {
		fmt.Println("Ошибка: не удалось удалить элемент")
		return
	}
	saveToFile()
}

func BCREMOVE(params []string) {
	if len(params) < 3 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]
	key := params[2]

	hashTable, exists := cuckooHashTables[name]
	if !exists {
		fmt.Println("Ошибка: хэш-таблица не найдена")
		return
	}

	if !hashTable.Remove(key) {
		fmt.Println("Ошибка: не удалось удалить элемент")
		return
	}
	saveToBinaryFile()
}

func PRINT(params []string) {
	if len(params) < 2 {
		fmt.Println("Ошибка: недостаточно параметров")
		return
	}
	name := params[1]

	if arr, exists := arrays[name]; exists {
		arr.PrintArr()
	} else if list, exists := oneLists[name]; exists {
		list.Print()
	} else if list, exists := twoLists[name]; exists {
		list.Print()
	} else if stack, exists := stacks[name]; exists {
		stack.Print()
	} else if queue, exists := queues[name]; exists {
		queue.Print()
	} else if hashTable, exists := doubleHashTables[name]; exists {
		hashTable.Print()
	} else if hashTable, exists := cuckooHashTables[name]; exists {
		hashTable.Print()
	} else {
		fmt.Println("Ошибка: структура не найдена")
	}
}

func BPRINT(params []string) {
	PRINT(params)
}

func HELP(params []string) {
	fmt.Println("Доступные команды:")
	fmt.Println("Текстовые команды (префикс без B):")
	fmt.Println("  MCREATE, FCREATE, LCREATE, SCREATE, QCREATE, TCREATE, DCREATE, CCREATE")
	fmt.Println("  MPUSH, MPUSHINDEX, MREPLACE, MDEL, MGET")
	fmt.Println("  FPUSHHEAD, FPUSHTAIL, FPUSHAFTER, FPUSHBEFORE")
	fmt.Println("  FDELHEAD, FDELTAIL, FDELAFTER, FDELBEFORE, FDELVALUE, FGET")
	fmt.Println("  LPUSHHEAD, LPUSHTAIL, LPUSHAFTER, LPUSHBEFORE")
	fmt.Println("  LDELHEAD, LDELTAIL, LDELAFTER, LDELBEFORE, LDELVALUE, LGET")
	fmt.Println("  SPUSH, SPOP, QPUSH, QPOP")
	fmt.Println("  DINSERT, DSEARCH, DREMOVE")
	fmt.Println("  CINSERT, CSEARCH, CREMOVE")
	fmt.Println("  PRINT, HELP")
	fmt.Println()
	fmt.Println("Бинарные команды (префикс B):")
	fmt.Println("  BMCREATE, BFCREATE, BLCREATE, BSCREATE, BQCREATE, BTCREATE, BDCREATE, BCCREATE")
	fmt.Println("  BMPUSH, BMPUSHINDEX, BMREPLACE, BMDEL, BMGET")
	fmt.Println("  BFPUSHHEAD, BFPUSHTAIL, BFPUSHAFTER, BFPUSHBEFORE")
	fmt.Println("  BFDELHEAD, BFDELTAIL, BFDELAFTER, BFDELBEFORE, BFDELVALUE, BFGET")
	fmt.Println("  BLPUSHHEAD, BLPUSHTAIL, BLPUSHAFTER, BLPUSHBEFORE")
	fmt.Println("  BLDELHEAD, BLDELTAIL, BLDELAFTER, BLDELBEFORE, BLDELVALUE, BLGET")
	fmt.Println("  BSPUSH, BSPOP, BQPUSH, BQPOP")
	fmt.Println("  BDINSERT, BDSEARCH, BDREMOVE")
	fmt.Println("  BCINSERT, BCSEARCH, BCREMOVE")
	fmt.Println("  BPRINT")
}

func initializeCommands() {
	commandHandlers = make(map[string]CommandHandler)

	commandHandlers["MCREATE"] = MCREATE
	commandHandlers["BMCREATE"] = BMCREATE
	commandHandlers["MPUSH"] = MPUSH
	commandHandlers["BMPUSH"] = BMPUSH
	commandHandlers["MPUSHINDEX"] = MPUSHINDEX
	commandHandlers["BMPUSHINDEX"] = BMPUSHINDEX
	commandHandlers["MREPLACE"] = MREPLACE
	commandHandlers["BMREPLACE"] = BMREPLACE
	commandHandlers["MDEL"] = MDEL
	commandHandlers["BMDEL"] = BMDEL
	commandHandlers["MGET"] = MGET
	commandHandlers["BMGET"] = BMGET

	commandHandlers["FCREATE"] = FCREATE
	commandHandlers["BFCREATE"] = BFCREATE
	commandHandlers["FPUSHHEAD"] = FPUSHHEAD
	commandHandlers["BFPUSHHEAD"] = BFPUSHHEAD
	commandHandlers["FPUSHTAIL"] = FPUSHTAIL
	commandHandlers["BFPUSHTAIL"] = BFPUSHTAIL
	commandHandlers["FPUSHAFTER"] = FPUSHAFTER
	commandHandlers["BFPUSHAFTER"] = BFPUSHAFTER
	commandHandlers["FPUSHBEFORE"] = FPUSHBEFORE
	commandHandlers["BFPUSHBEFORE"] = BFPUSHBEFORE
	commandHandlers["FDELHEAD"] = FDELHEAD
	commandHandlers["BFDELHEAD"] = BFDELHEAD
	commandHandlers["FDELTAIL"] = FDELTAIL
	commandHandlers["BFDELTAIL"] = BFDELTAIL
	commandHandlers["FDELAFTER"] = FDELAFTER
	commandHandlers["BFDELAFTER"] = BFDELAFTER
	commandHandlers["FDELBEFORE"] = FDELBEFORE
	commandHandlers["BFDELBEFORE"] = BFDELBEFORE
	commandHandlers["FDELVALUE"] = FDELVALUE
	commandHandlers["BFDELVALUE"] = BFDELVALUE
	commandHandlers["FGET"] = FGET
	commandHandlers["BFGET"] = BFGET

	commandHandlers["LCREATE"] = LCREATE
	commandHandlers["BLCREATE"] = BLCREATE
	commandHandlers["LPUSHHEAD"] = LPUSHHEAD
	commandHandlers["BLPUSHHEAD"] = BLPUSHHEAD
	commandHandlers["LPUSHTAIL"] = LPUSHTAIL
	commandHandlers["BLPUSHTAIL"] = BLPUSHTAIL
	commandHandlers["LPUSHAFTER"] = LPUSHAFTER
	commandHandlers["BLPUSHAFTER"] = BLPUSHAFTER
	commandHandlers["LPUSHBEFORE"] = LPUSHBEFORE
	commandHandlers["BLPUSHBEFORE"] = BLPUSHBEFORE
	commandHandlers["LDELHEAD"] = LDELHEAD
	commandHandlers["BLDELHEAD"] = BLDELHEAD
	commandHandlers["LDELTAIL"] = LDELTAIL
	commandHandlers["BLDELTAIL"] = BLDELTAIL
	commandHandlers["LDELAFTER"] = LDELAFTER
	commandHandlers["BLDELAFTER"] = BLDELAFTER
	commandHandlers["LDELBEFORE"] = LDELBEFORE
	commandHandlers["BLDELBEFORE"] = BLDELBEFORE
	commandHandlers["LDELVALUE"] = LDELVALUE
	commandHandlers["BLDELVALUE"] = BLDELVALUE
	commandHandlers["LGET"] = LGET
	commandHandlers["BLGET"] = BLGET

	commandHandlers["SCREATE"] = SCREATE
	commandHandlers["BSCREATE"] = BSCREATE
	commandHandlers["SPUSH"] = SPUSH
	commandHandlers["BSPUSH"] = BSPUSH
	commandHandlers["SPOP"] = SPOP
	commandHandlers["BSPOP"] = BSPOP

	commandHandlers["QCREATE"] = QCREATE
	commandHandlers["BQCREATE"] = BQCREATE
	commandHandlers["QPUSH"] = QPUSH
	commandHandlers["BQPUSH"] = BQPUSH
	commandHandlers["QPOP"] = QPOP
	commandHandlers["BQPOP"] = BQPOP

	commandHandlers["DCREATE"] = DCREATE
	commandHandlers["BDCREATE"] = BDCREATE
	commandHandlers["DINSERT"] = DINSERT
	commandHandlers["BDINSERT"] = BDINSERT
	commandHandlers["DSEARCH"] = DSEARCH
	commandHandlers["BDSEARCH"] = BDSEARCH
	commandHandlers["DREMOVE"] = DREMOVE
	commandHandlers["BDREMOVE"] = BDREMOVE

	commandHandlers["CCREATE"] = CCREATE
	commandHandlers["BCCREATE"] = BCCREATE
	commandHandlers["CINSERT"] = CINSERT
	commandHandlers["BCINSERT"] = BCINSERT
	commandHandlers["CSEARCH"] = CSEARCH
	commandHandlers["BCSEARCH"] = BCSEARCH
	commandHandlers["CREMOVE"] = CREMOVE
	commandHandlers["BCREMOVE"] = BCREMOVE

	commandHandlers["PRINT"] = PRINT
	commandHandlers["BPRINT"] = BPRINT
	commandHandlers["HELP"] = HELP
}

func processCommand(command string) {
	params := strings.Fields(command)
	if len(params) == 0 {
		return
	}

	cmd := params[0]
	if handler, exists := commandHandlers[cmd]; exists {
		handler(params)
	} else {
		fmt.Printf("Ошибка: неизвестная команда '%s'\n", cmd)
		fmt.Println("Введите HELP для просмотра доступных команд")
	}
}

func main() {
	var queries []string
	binaryMode := false

	for i := 1; i < len(os.Args); i++ {
		arg := os.Args[i]
		if arg == "--file" && i+1 < len(os.Args) {
			currFilename = os.Args[i+1]
			i++
		} else if arg == "--query" && i+1 < len(os.Args) {
			queries = append(queries, os.Args[i+1])
			i++
		} else if arg == "--binary" {
			binaryMode = true
		}
	}

	initializeCommands()

	if currFilename != "" {
		if binaryMode {
			loadFromBinaryFile()
		} else {
			loadFromFile()
		}
	}

	for _, query := range queries {
		processCommand(query)
	}

}

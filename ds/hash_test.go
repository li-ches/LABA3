package ds

import (
	"os"
	"testing"
)

func TestHash_ChainHash_Coverage(t *testing.T) {
	// 1. Создаем таблицу с 1 бакетом, чтобы форсировать коллизии
	ch := NewChainHash(1)
	ch.Show() // Empty show

	// Вставка 3 элементов в 1 бакет (цепочка)
	ch.Insert("A", "1")
	ch.Insert("B", "2")
	ch.Insert("C", "3")

	// Проверка поиска в глубине цепочки
	if val := ch.Find("B"); val != "2" {
		t.Errorf("Find failed deep in chain. Expected 2, got %s", val)
	}
	if val := ch.Find("C"); val != "3" {
		t.Errorf("Find failed at end of chain. Expected 3, got %s", val)
	}

	// Обновление значения
	ch.Insert("B", "222")
	if val := ch.Find("B"); val != "222" {
		t.Errorf("Update failed. Expected 222, got %s", val)
	}

	// Удаление из середины цепочки (B)
	// Цепочка сейчас: C -> B -> A (т.к. вставка в начало)
	ch.Erase("B")
	if ch.Find("B") != "" {
		t.Error("B should be erased")
	}
	// Проверяем, что цепочка не порвалась
	if ch.Find("A") != "1" || ch.Find("C") != "3" {
		t.Error("Chain broken after erase")
	}

	// Удаление головы
	ch.Erase("C")
	if ch.Find("C") != "" {
		t.Error("C (head) should be erased")
	}
	
	// Удаление последнего
	ch.Erase("A")
	if ch.Find("A") != "" {
		t.Error("A should be erased")
	}

	// Удаление несуществующего
	if ch.Erase("Z") {
		t.Error("Erase Z should return false")
	}
}

func TestHash_OpenHash_Coverage(t *testing.T) {
	// Маленький размер для коллизий
	oh := NewOpenHash(2)
	oh.Show()

	// Заполняем полностью
	oh.Insert("A", "1")
	oh.Insert("B", "2")
	
	// Попытка вставить в переполненную таблицу
	// (В текущей реализации выводит в консоль и выходит, не падает)
	oh.Insert("C", "3") 

	// Проверка поиска
	if oh.Find("A") != "1" || oh.Find("B") != "2" {
		t.Error("Find basic failed")
	}
	if oh.Find("Z") != "" {
		t.Error("Found non-existent Z")
	}

	// Проверка обновления
	oh.Insert("A", "111")
	if oh.Find("A") != "111" {
		t.Error("Update failed")
	}

	// Проверка удаления и флага DELETED
	oh.Erase("A")
	if oh.Find("A") != "" {
		t.Error("A should be deleted")
	}
	// Слот A теперь DELETED. Попробуем вставить туда новое значение
	oh.Insert("A", "NewA")
	if oh.Find("A") != "NewA" {
		t.Error("Insert into deleted slot failed")
	}
	
	// Удаление несуществующего
	if oh.Erase("Z") {
		t.Error("Erase non-existent should return false")
	}
}

func TestHash_SaveLoad(t *testing.T) {
	filename := "hash_test.bin"
	defer os.Remove(filename)

	// Chain
	ch := NewChainHash(5)
	ch.Insert("key1", "val1")
	ch.SaveToFile(filename)
	
	ch2 := NewChainHash(1)
	ch2.LoadFromFile(filename)
	if ch2.Find("key1") != "val1" {
		t.Error("ChainHash Save/Load failed")
	}

	// Open
	oh := NewOpenHash(5)
	oh.Insert("k1", "v1")
	oh.SaveToFile(filename)

	oh2 := NewOpenHash(1)
	oh2.LoadFromFile(filename)
	if oh2.Find("k1") != "v1" {
		t.Error("OpenHash Save/Load failed")
	}
}

func TestHashMan_Simulation(t *testing.T) {
	// Этот тест просто вызывает функцию HashMan, но т.к. она блокирует ввод, 
	// мы эмулируем ввод через пайп, чтобы покрыть ветки парсинга команд.
	r, w, _ := os.Pipe()
	origStdin := os.Stdin
	os.Stdin = r
	defer func() { os.Stdin = origStdin }()

	go func() {
		w.Write([]byte("INSERT k1 v1\nSHOW\nSEARCH k1\nDELETE k1\nSEARCH k1\nBAD_CMD\nBACK\n"))
		w.Close()
	}()

	HashMan()
}
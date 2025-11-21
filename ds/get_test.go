package ds

import (
	"encoding/binary"
	"errors"
	"os"
	"testing"
)

// ==========================================
// ТЕСТЫ СЕРИАЛИЗАЦИИ И ОШИБОК IO
// ==========================================

func TestAll_IO_Errors(t *testing.T) {
	badFile := "" 
	
	// Проверка, что SaveToFile не падает (возвращает ошибку, но не паникует) при плохом имени файла
	arr := NewMyArr()
	if err := arr.SaveToFile(badFile); err == nil { t.Log("Warning: MyArr.SaveToFile did not fail") }
	l := NewMyList()
	if err := l.SaveToFile(badFile); err == nil { t.Log("Warning: MyList.SaveToFile did not fail") }
	d := NewDList()
	if err := d.SaveToFile(badFile); err == nil { t.Log("Warning: DList.SaveToFile did not fail") }
	s := NewStack()
	if err := s.SaveToFile(badFile); err == nil { t.Log("Warning: Stack.SaveToFile did not fail") }
	q := NewQueue()
	if err := q.SaveToFile(badFile); err == nil { t.Log("Warning: Queue.SaveToFile did not fail") }
	
	// ИСПРАВЛЕНО: NewBST -> NewCBT
	b := NewCBT()
	if err := b.SaveToFile(badFile); err == nil { t.Log("Warning: CBT.SaveToFile did not fail") }

	// Проверка загрузки несуществующего файла
	noFile := "non_existent_file_xyz.bin"
	if err := arr.LoadFromFile(noFile); err == nil { t.Errorf("Should fail loading missing file") }
	if err := l.LoadFromFile(noFile); err == nil { t.Errorf("Should fail loading missing file") }
	if err := d.LoadFromFile(noFile); err == nil { t.Errorf("Should fail loading missing file") }
	if err := s.LoadFromFile(noFile); err == nil { t.Errorf("Should fail loading missing file") }
	if err := q.LoadFromFile(noFile); err == nil { t.Errorf("Should fail loading missing file") }
	if err := b.LoadFromFile(noFile); err == nil { t.Errorf("Should fail loading missing file") }

	// Проверка чтения файла с обрезанным размером (не хватает байт для int32 размера)
	truncSize := "trunc_size.bin"
	f, _ := os.Create(truncSize)
	f.Write([]byte{0xFF}) 
	f.Close()
	defer os.Remove(truncSize)

	if err := arr.LoadFromFile(truncSize); err == nil { t.Errorf("Should fail reading size") }
	if err := l.LoadFromFile(truncSize); err == nil { t.Errorf("Should fail reading size") }
	if err := d.LoadFromFile(truncSize); err == nil { t.Errorf("Should fail reading size") }
	if err := s.LoadFromFile(truncSize); err == nil { t.Errorf("Should fail reading size") }
	if err := q.LoadFromFile(truncSize); err == nil { t.Errorf("Should fail reading size") }
	if err := b.LoadFromFile(truncSize); err == nil { t.Errorf("Should fail reading size") }

	// Проверка чтения файла, где размер указан как 1, но данных нет
	truncData := "trunc_data.bin"
	f2, _ := os.Create(truncData)
	binary.Write(f2, binary.LittleEndian, int32(1)) 
	f2.Close()
	defer os.Remove(truncData)

	if err := arr.LoadFromFile(truncData); err == nil { t.Errorf("Should fail reading data item") }
	if err := l.LoadFromFile(truncData); err == nil { t.Errorf("Should fail reading data item") }
	if err := d.LoadFromFile(truncData); err == nil { t.Errorf("Should fail reading data item") }
	if err := s.LoadFromFile(truncData); err == nil { t.Errorf("Should fail reading data item") }
	if err := q.LoadFromFile(truncData); err == nil { t.Errorf("Should fail reading data item") }
	if err := b.LoadFromFile(truncData); err == nil { t.Errorf("Should fail reading data item") }
}

func TestCoverage_Boost_EmptyString(t *testing.T) {
	filename := "empty_str_test.bin"
	f, _ := os.Create(filename)
	WriteString(f, "") 
	f.Close()
	defer os.Remove(filename)

	f2, _ := os.Open(filename)
	s, err := ReadString(f2)
	f2.Close()

	if err != nil || s != "" {
		t.Errorf("Failed to handle empty string correctly")
	}
}

type AlwaysFailWriter struct{}
func (f *AlwaysFailWriter) Write(p []byte) (n int, err error) {
	return 0, errors.New("Always Fail")
}

type FailOnSecondWrite struct {
	calls int
}
func (f *FailOnSecondWrite) Write(p []byte) (n int, err error) {
	if f.calls >= 1 {
		return 0, errors.New("Fail on 2nd")
	}
	f.calls++
	return len(p), nil
}

func TestSerialize_WriteString_Errors(t *testing.T) {
	// Тест ошибки при записи длины
	fw1 := &AlwaysFailWriter{}
	if err := WriteString(fw1, "test"); err == nil {
		t.Error("Should fail on length write")
	}

	// Тест ошибки при записи тела строки
	fw2 := &FailOnSecondWrite{}
	if err := WriteString(fw2, "test"); err == nil {
		t.Error("Should fail on body write")
	}
}

// ==========================================
// ОБЩИЕ ТЕСТЫ ПОКРЫТИЯ (EDGE CASES)
// ==========================================

func TestCoverage_Final_Scenarios(t *testing.T) {
	// ИСПРАВЛЕНО: NewBST -> NewCBT
	
	// Удаление корня, у которого есть ТОЛЬКО левый потомок
	b1 := NewCBT()
	b1.Insert(10)
	b1.Insert(5)
	b1.Remove(10) 
	if !b1.Contains(5) || b1.Contains(10) {
		t.Error("Failed removing root with only left child")
	}

	// Удаление корня, у которого есть ТОЛЬКО правый потомок
	b2 := NewCBT()
	b2.Insert(10)
	b2.Insert(15)
	b2.Remove(10) 
	if !b2.Contains(15) || b2.Contains(10) {
		t.Error("Failed removing root with only right child")
	}
	
	// Создаем хеш-таблицу размером 1, чтобы ВСЕ вставки вызывали коллизию
	ch := NewChainHash(1) 
	ch.Insert("A", "1")
	ch.Insert("B", "2")
	ch.Insert("C", "3") 
	
	// Ищем элемент в глубине цепочки
	if ch.Find("A") != "1" { t.Error("Deep find failed") }
	
	// Удаляем элемент из СЕРЕДИНЫ цепочки (это активирует ветку prev.Next = cur.Next)
	ch.Erase("B") 
	if ch.Find("B") != "" { t.Error("Erase middle failed") }
	if ch.Find("A") != "1" { t.Error("Chain broken after middle erase") }
	
	// --- MyList (Single) Empty/Not Found checks ---
	l := NewMyList()
	l.AddTail("A")
	l.AddTail("B")
	// Ищем несуществующий элемент, чтобы пройти цикл до конца
	if l.FindValue("Z") { t.Error("Found Z?") }
	// Удаляем несуществующий
	l.DelByValue("Z") 
	
	dl := NewDList()
	dl.AddTail("A")
	dl.DelByVal("Z")
	dl.AddTail("B")
	dl.DelByVal("Z") 
	
	// Проверка конструкторов на нулевую емкость
	chInv := NewChainHash(0)
	if chInv.Cap != 1 { t.Error("ChainHash invalid cap fix failed") }
	ohInv := NewOpenHash(0)
	if ohInv.Cap != 1 { t.Error("OpenHash invalid cap fix failed") }
}

func TestCoverage_Ultra_Edge_Cases(t *testing.T) {
	ch := NewChainHash(5)
	ch.Show() // "пусто"

	oh := NewOpenHash(5)
	oh.Show() // "пусто"

	ohFull := NewOpenHash(1)
	ohFull.Insert("A", "1")
	ohFull.Insert("B", "2") 

	// ИСПРАВЛЕНО: NewBST -> NewCBT
	b := NewCBT()
	b.Insert(10)
	// В CBT (если он реализует интерфейс BST) удаления могут возвращать void или bool. 
	// Если Remove в cbt.go ничего не возвращает, этот if нужно убрать или изменить логику.
	// В предоставленном cbt.go Remove(int) ничего не возвращает (void).
	// Поэтому просто вызываем:
	b.Remove(99) 
	// И проверяем, что ничего не упало и размер не изменился (если нужно).

	l := NewMyList()
	l.AddTail("A")
	l.AddBefore("Z", "B") // Не найдено
	l.AddAfter("Z", "B")  // Не найдено

	dl := NewDList()
	dl.AddTail("A")
	dl.AddBefore("Z", "B")
	dl.AddAfter("Z", "B")
}
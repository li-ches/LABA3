package ds

import (
	"fmt"
	"testing"
)

// --- MyArr (Динамический массив) ---
// Измеряем скорость добавления в конец
func BenchmarkArr_AddEnd(b *testing.B) {
	// b.N автоматически подбирается Go для получения точных замеров
	for i := 0; i < b.N; i++ {
		arr := NewMyArr()
		// Эмулируем пакетную вставку 1000 элементов (как в bench.cpp Range(1000))
		for j := 0; j < 1000; j++ {
			arr.AddEnd("X")
		}
	}
}

// --- MyList (Односвязный список) ---
func BenchmarkList_AddTail(b *testing.B) {
	for i := 0; i < b.N; i++ {
		l := NewMyList()
		for j := 0; j < 1000; j++ {
			l.AddTail("X")
		}
	}
}

// --- DList (Двусвязный список) ---
func BenchmarkDList_AddTail(b *testing.B) {
	for i := 0; i < b.N; i++ {
		dl := NewDList()
		for j := 0; j < 1000; j++ {
			dl.AddTail("Y")
		}
	}
}

// --- Stack (Стек) ---
// Измеряем пару операций Push + Pop
func BenchmarkStack_PushPop(b *testing.B) {
	// Здесь мы можем измерять операции напрямую без пересоздания структуры на каждой итерации,
	// так как стек возвращается в исходное состояние.
	s := NewStack()
	b.ResetTimer() // Сбрасываем время подготовки
	for i := 0; i < b.N; i++ {
		s.Push("A")
		s.Pop()
	}
}

// --- Queue (Очередь) ---
func BenchmarkQueue_PushPop(b *testing.B) {
	q := NewQueue()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		q.Push("A")
		q.Pop()
	}
}

// --- CBT (Complete Binary Tree) ---

// Вставка (Batch insert)
func BenchmarkCBT_Insert(b *testing.B) {
	for i := 0; i < b.N; i++ {
		t := NewCBT()
		for j := 0; j < 1000; j++ {
			t.Insert(j)
		}
	}
}

// Поиск (Search)
func BenchmarkCBT_Search(b *testing.B) {
	// Подготовка: создаем дерево один раз
	t := NewCBT()
	for j := 0; j < 5000; j++ {
		t.Insert(j)
	}
	
	b.ResetTimer() // Запускаем таймер только для поиска
	for i := 0; i < b.N; i++ {
		// Ищем элемент в середине
		t.Contains(2500) 
	}
}

// --- Hash Tables ---

// ChainHash Вставка
func BenchmarkChainHash_Insert(b *testing.B) {
	for i := 0; i < b.N; i++ {
		// Создаем новую таблицу на каждой итерации, чтобы проверить чистую вставку
		h := NewChainHash(100) 
		for j := 0; j < 1000; j++ {
			key := fmt.Sprintf("%d", j)
			h.Insert(key, "X")
		}
	}
}

// OpenHash Вставка
func BenchmarkOpenHash_Insert(b *testing.B) {
	for i := 0; i < b.N; i++ {
		h := NewOpenHash(2000) // Достаточный размер, чтобы избежать рехеширования (если оно есть) или вечного цикла
		for j := 0; j < 1000; j++ {
			key := fmt.Sprintf("%d", j)
			h.Insert(key, "X")
		}
	}
}
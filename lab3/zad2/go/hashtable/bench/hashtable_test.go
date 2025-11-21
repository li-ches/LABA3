package bench

import (
	"os"
	"testing"
)

func BenchmarkCuckooInsert(b *testing.B) {
	table := NewCuckooHashTable(100)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.Insert(string(rune(i)), "value")
	}
}

func BenchmarkCuckooInsertWithResize(b *testing.B) {
	table := NewCuckooHashTable(1)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.Insert(string(rune(i)), "value")
	}
}

func BenchmarkCuckooSearch(b *testing.B) {
	table := NewCuckooHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.Search(string(rune(i % 100)))
	}
}

func BenchmarkCuckooSearchNotFound(b *testing.B) {
	table := NewCuckooHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.Search("nonexistent")
	}
}

func BenchmarkCuckooRemove(b *testing.B) {
	table := NewCuckooHashTable(b.N + 1)
	for i := 0; i < b.N; i++ {
		table.Insert(string(rune(i)), "value")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.Remove(string(rune(i)))
	}
}

func BenchmarkDoubleInsert(b *testing.B) {
	table := NewDoubleHashTable(100)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.Insert(string(rune(i)), "value")
	}
}

func BenchmarkDoubleInsertWithResize(b *testing.B) {
	table := NewDoubleHashTable(1)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.Insert(string(rune(i)), "value")
	}
}

func BenchmarkDoubleSearch(b *testing.B) {
	table := NewDoubleHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.Search(string(rune(i % 100)))
	}
}

func BenchmarkDoubleSearchNotFound(b *testing.B) {
	table := NewDoubleHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.Search("nonexistent")
	}
}

func BenchmarkDoubleRemove(b *testing.B) {
	table := NewDoubleHashTable(b.N + 1)
	for i := 0; i < b.N; i++ {
		table.Insert(string(rune(i)), "value")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.Remove(string(rune(i)))
	}
}

func BenchmarkCuckooSaveToFile(b *testing.B) {
	table := NewCuckooHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	file, _ := os.CreateTemp("", "benchmark")
	defer os.Remove(file.Name())
	defer file.Close()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.SaveToFile(file)
	}
}

func BenchmarkCuckooLoadFromFile(b *testing.B) {
	file, _ := os.CreateTemp("", "benchmark")
	table := NewCuckooHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	table.SaveToFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table := NewCuckooHashTable(10)
		file, _ := os.Open(file.Name())
		table.LoadFromFile(file)
		file.Close()
	}
}

func BenchmarkDoubleSaveToFile(b *testing.B) {
	table := NewDoubleHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	file, _ := os.CreateTemp("", "benchmark")
	defer os.Remove(file.Name())
	defer file.Close()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.SaveToFile(file)
	}
}

func BenchmarkDoubleLoadFromFile(b *testing.B) {
	file, _ := os.CreateTemp("", "benchmark")
	table := NewDoubleHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	table.SaveToFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table := NewDoubleHashTable(10)
		file, _ := os.Open(file.Name())
		table.LoadFromFile(file)
		file.Close()
	}
}

func BenchmarkCuckooSaveToBinaryFile(b *testing.B) {
	table := NewCuckooHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	file, _ := os.CreateTemp("", "benchmark")
	defer os.Remove(file.Name())
	defer file.Close()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.SaveToBinaryFile(file)
	}
}

func BenchmarkCuckooLoadFromBinaryFile(b *testing.B) {
	file, _ := os.CreateTemp("", "benchmark")
	table := NewCuckooHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	table.SaveToBinaryFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table := NewCuckooHashTable(10)
		file, _ := os.Open(file.Name())
		table.LoadFromBinaryFile(file)
		file.Close()
	}
}

func BenchmarkDoubleSaveToBinaryFile(b *testing.B) {
	table := NewDoubleHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	file, _ := os.CreateTemp("", "benchmark")
	defer os.Remove(file.Name())
	defer file.Close()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.SaveToBinaryFile(file)
	}
}

func BenchmarkDoubleLoadFromBinaryFile(b *testing.B) {
	file, _ := os.CreateTemp("", "benchmark")
	table := NewDoubleHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	table.SaveToBinaryFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table := NewDoubleHashTable(10)
		file, _ := os.Open(file.Name())
		table.LoadFromBinaryFile(file)
		file.Close()
	}
}

func BenchmarkGetAllElements(b *testing.B) {
	table := NewCuckooHashTable(100)
	for i := 0; i < 100; i++ {
		table.Insert(string(rune(i)), "value")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		table.GetAllElements()
	}
}

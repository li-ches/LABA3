package bench

import (
	"os"
	"testing"
)

func BenchmarkPush(b *testing.B) {
	stack := NewStack()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		stack.Push(values[i%5])
	}
}

func BenchmarkPop(b *testing.B) {
	for i := 0; i < b.N; i++ {
		b.StopTimer()
		stack := NewStack()
		values := []string{"test1", "test2", "test3", "test4", "test5"}
		for j := 0; j < 100; j++ {
			stack.Push(values[j%5])
		}
		b.StartTimer()
		for j := 0; j < 100; j++ {
			stack.Pop()
		}
	}
}

func BenchmarkSaveToFile(b *testing.B) {
	stack := NewStack()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 100; i++ {
		stack.Push(values[i%5])
	}
	file, _ := os.CreateTemp("", "benchmark")
	defer os.Remove(file.Name())
	defer file.Close()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		stack.SaveToFile(file)
	}
}

func BenchmarkLoadFromFile(b *testing.B) {
	file, _ := os.CreateTemp("", "benchmark")
	stack := NewStack()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 100; i++ {
		stack.Push(values[i%5])
	}
	stack.SaveToFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		stack := NewStack()
		file, _ := os.Open(file.Name())
		stack.LoadFromFile(file)
		file.Close()
	}
}

func BenchmarkSaveToBinaryFile(b *testing.B) {
	stack := NewStack()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 100; i++ {
		stack.Push(values[i%5])
	}
	file, _ := os.CreateTemp("", "benchmark")
	defer os.Remove(file.Name())
	defer file.Close()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		stack.SaveToBinaryFile(file)
	}
}

func BenchmarkLoadFromBinaryFile(b *testing.B) {
	file, _ := os.CreateTemp("", "benchmark")
	stack := NewStack()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 100; i++ {
		stack.Push(values[i%5])
	}
	stack.SaveToBinaryFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		stack := NewStack()
		file, _ := os.Open(file.Name())
		stack.LoadFromBinaryFile(file)
		file.Close()
	}
}

func BenchmarkPushPopSequence(b *testing.B) {
	stack := NewStack()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		stack.Push(values[i%5])
		if i%3 == 0 && stack.top != nil {
			stack.Pop()
		}
	}
}

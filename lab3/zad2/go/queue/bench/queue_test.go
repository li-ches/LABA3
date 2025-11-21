package bench

import (
	"os"
	"testing"
)

func BenchmarkEnqueue(b *testing.B) {
	queue := NewQueue()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		queue.Enqueue(values[i%5])
	}
}

func BenchmarkDequeue(b *testing.B) {
	for i := 0; i < b.N; i++ {
		b.StopTimer()
		queue := NewQueue()
		values := []string{"test1", "test2", "test3", "test4", "test5"}
		for j := 0; j < 100; j++ {
			queue.Enqueue(values[j%5])
		}
		b.StartTimer()
		for j := 0; j < 100; j++ {
			queue.Dequeue()
		}
	}
}

func BenchmarkPrint(b *testing.B) {
	queue := NewQueue()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 10; i++ {
		queue.Enqueue(values[i%5])
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		queue.Print()
	}
}

func BenchmarkSaveToFile(b *testing.B) {
	queue := NewQueue()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 1000; i++ {
		queue.Enqueue(values[i%5])
	}
	file, _ := os.CreateTemp("", "benchmark")
	defer os.Remove(file.Name())
	defer file.Close()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		queue.SaveToFile(file)
	}
}

func BenchmarkLoadFromFile(b *testing.B) {
	file, _ := os.CreateTemp("", "benchmark")
	queue := NewQueue()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 1000; i++ {
		queue.Enqueue(values[i%5])
	}
	queue.SaveToFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		queue := NewQueue()
		file, _ := os.Open(file.Name())
		queue.LoadFromFile(file)
		file.Close()
	}
}

func BenchmarkSaveToBinaryFile(b *testing.B) {
	queue := NewQueue()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 1000; i++ {
		queue.Enqueue(values[i%5])
	}
	file, _ := os.CreateTemp("", "benchmark")
	defer os.Remove(file.Name())
	defer file.Close()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		queue.SaveToBinaryFile(file)
	}
}

func BenchmarkLoadFromBinaryFile(b *testing.B) {
	file, _ := os.CreateTemp("", "benchmark")
	queue := NewQueue()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 1000; i++ {
		queue.Enqueue(values[i%5])
	}
	queue.SaveToBinaryFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		queue := NewQueue()
		file, _ := os.Open(file.Name())
		queue.LoadFromBinaryFile(file)
		file.Close()
	}
}

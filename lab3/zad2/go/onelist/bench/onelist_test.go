package bench

import (
	"os"
	"testing"
)

func BenchmarkAddToHead(b *testing.B) {
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.AddToHead(values[i%5])
	}
}

func BenchmarkAddToTail(b *testing.B) {
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.AddToTail(values[i%5])
	}
}

func BenchmarkAddAfterValue(b *testing.B) {
	list := NewOneList()
	targets := []string{"target1", "target2", "target3", "target4", "target5"}
	values := []string{"new1", "new2", "new3", "new4", "new5"}

	for _, target := range targets {
		list.AddToTail(target)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.AddAfterValue(targets[i%5], values[i%5])
	}
}

func BenchmarkAddBeforeValue(b *testing.B) {
	list := NewOneList()
	targets := []string{"target1", "target2", "target3", "target4", "target5"}
	values := []string{"new1", "new2", "new3", "new4", "new5"}

	for _, target := range targets {
		list.AddToTail(target)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.AddBeforeValue(targets[i%5], values[i%5])
	}
}

func BenchmarkRemoveFromHead(b *testing.B) {
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < b.N; i++ {
		list.AddToHead(values[i%5])
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.RemoveFromHead()
	}
}

func BenchmarkRemoveFromTail(b *testing.B) {
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < b.N; i++ {
		list.AddToTail(values[i%5])
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.RemoveFromTail()
	}
}

func BenchmarkRemoveAfterValue(b *testing.B) {
	list := NewOneList()
	targets := []string{"target1", "target2", "target3", "target4", "target5"}
	values := []string{"test1", "test2", "test3", "test4", "test5"}

	for _, target := range targets {
		list.AddToTail(target)
		for _, val := range values {
			list.AddToTail(val)
		}
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.RemoveAfterValue(targets[i%5])
	}
}

func BenchmarkRemoveBeforeValue(b *testing.B) {
	list := NewOneList()
	targets := []string{"target1", "target2", "target3", "target4", "target5"}
	values := []string{"test1", "test2", "test3", "test4", "test5"}

	for _, target := range targets {
		for _, val := range values {
			list.AddToHead(val)
		}
		list.AddToTail(target)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.RemoveBeforeValue(targets[i%5])
	}
}

func BenchmarkRemoveByValue(b *testing.B) {
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < b.N; i++ {
		list.AddToTail(values[i%5])
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.RemoveByValue(values[i%5])
	}
}

func BenchmarkGetElementByValue(b *testing.B) {
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 1000; i++ {
		list.AddToTail(values[i%5])
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.GetElementByValue(values[i%5])
	}
}

func BenchmarkFindByValue(b *testing.B) {
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 1000; i++ {
		list.AddToTail(values[i%5])
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.findByVal(values[i%5])
	}
}

func BenchmarkFindByValueNotFound(b *testing.B) {
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	notFound := []string{"none1", "none2", "none3", "none4", "none5"}
	for i := 0; i < 1000; i++ {
		list.AddToTail(values[i%5])
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.findByVal(notFound[i%5])
	}
}

func BenchmarkPrint(b *testing.B) {
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 100; i++ {
		list.AddToTail(values[i%5])
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.Print()
	}
}

func BenchmarkSaveToFile(b *testing.B) {
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 1000; i++ {
		list.AddToTail(values[i%5])
	}
	file, _ := os.CreateTemp("", "benchmark")
	defer os.Remove(file.Name())
	defer file.Close()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.SaveToFile(file)
	}
}

func BenchmarkLoadFromFile(b *testing.B) {
	file, _ := os.CreateTemp("", "benchmark")
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 1000; i++ {
		list.AddToTail(values[i%5])
	}
	list.SaveToFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list := NewOneList()
		file, _ := os.Open(file.Name())
		list.LoadFromFile(file)
		file.Close()
	}
}

func BenchmarkSaveToBinaryFile(b *testing.B) {
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 1000; i++ {
		list.AddToTail(values[i%5])
	}
	file, _ := os.CreateTemp("", "benchmark")
	defer os.Remove(file.Name())
	defer file.Close()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.SaveToBinaryFile(file)
	}
}

func BenchmarkLoadFromBinaryFile(b *testing.B) {
	file, _ := os.CreateTemp("", "benchmark")
	list := NewOneList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 1000; i++ {
		list.AddToTail(values[i%5])
	}
	list.SaveToBinaryFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list := NewOneList()
		file, _ := os.Open(file.Name())
		list.LoadFromBinaryFile(file)
		file.Close()
	}
}

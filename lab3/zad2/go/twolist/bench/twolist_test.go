package bench

import (
	"os"
	"testing"
)

func BenchmarkAddToHead(b *testing.B) {
	list := NewTwoList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.AddToHead(values[i%5])
	}
}

func BenchmarkAddToTail(b *testing.B) {
	list := NewTwoList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.AddToTail(values[i%5])
	}
}

func BenchmarkAddAfterValue(b *testing.B) {
	list := NewTwoList()
	targets := []string{"target1", "target2", "target3", "target4", "target5"}
	for _, target := range targets {
		list.AddToTail(target)
	}
	values := []string{"new1", "new2", "new3", "new4", "new5"}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.AddAfterValue(targets[i%5], values[i%5])
	}
}

func BenchmarkAddBeforeValue(b *testing.B) {
	list := NewTwoList()
	targets := []string{"target1", "target2", "target3", "target4", "target5"}
	for _, target := range targets {
		list.AddToTail(target)
	}
	values := []string{"new1", "new2", "new3", "new4", "new5"}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.AddBeforeValue(targets[i%5], values[i%5])
	}
}

func BenchmarkRemoveFromHead(b *testing.B) {
	for i := 0; i < b.N; i++ {
		b.StopTimer()
		list := NewTwoList()
		values := []string{"test1", "test2", "test3", "test4", "test5"}
		for j := 0; j < 100; j++ {
			list.AddToHead(values[j%5])
		}
		b.StartTimer()
		for j := 0; j < 100 && list.head != nil; j++ {
			list.RemoveFromHead()
		}
	}
}

func BenchmarkRemoveFromTail(b *testing.B) {
	for i := 0; i < b.N; i++ {
		b.StopTimer()
		list := NewTwoList()
		values := []string{"test1", "test2", "test3", "test4", "test5"}
		for j := 0; j < 100; j++ {
			list.AddToTail(values[j%5])
		}
		b.StartTimer()
		for j := 0; j < 100 && list.head != nil; j++ {
			list.RemoveFromTail()
		}
	}
}

func BenchmarkRemoveAfterValue(b *testing.B) {
	list := NewTwoList()
	targets := []string{"target1", "target2", "target3", "target4", "target5"}
	for i := 0; i < 5; i++ {
		list.AddToTail(targets[i])
		list.AddToTail("temp")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.RemoveAfterValue(targets[i%5])
		list.AddToTail("temp")
	}
}

func BenchmarkRemoveBeforeValue(b *testing.B) {
	list := NewTwoList()
	targets := []string{"target1", "target2", "target3", "target4", "target5"}
	for i := 0; i < 5; i++ {
		list.AddToTail("temp")
		list.AddToTail(targets[i])
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.RemoveBeforeValue(targets[i%5])
		list.AddToHead("temp")
	}
}

func BenchmarkRemoveByValue(b *testing.B) {
	for i := 0; i < b.N; i++ {
		b.StopTimer()
		list := NewTwoList()
		values := []string{"test1", "test2", "test3", "test4", "test5"}
		for j := 0; j < 10; j++ {
			list.AddToTail(values[j%5])
		}
		b.StartTimer()
		for j := 0; j < 10 && list.head != nil; j++ {
			list.RemoveByValue(values[j%5])
		}
	}
}

func BenchmarkGetElementByValue(b *testing.B) {
	list := NewTwoList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 100; i++ {
		list.AddToTail(values[i%5])
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.GetElementByValue(values[i%5])
	}
}

func BenchmarkFindByValue(b *testing.B) {
	list := NewTwoList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 100; i++ {
		list.AddToTail(values[i%5])
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.findByVal(values[i%5])
	}
}

func BenchmarkFindByValueNotFound(b *testing.B) {
	list := NewTwoList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 100; i++ {
		list.AddToTail(values[i%5])
	}
	notFound := []string{"none1", "none2", "none3", "none4", "none5"}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.findByVal(notFound[i%5])
	}
}

func BenchmarkSaveToFile(b *testing.B) {
	list := NewTwoList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 50; i++ {
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
	list := NewTwoList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 50; i++ {
		list.AddToTail(values[i%5])
	}
	list.SaveToFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list := NewTwoList()
		file, _ := os.Open(file.Name())
		list.LoadFromFile(file)
		file.Close()
	}
}

func BenchmarkSaveToBinaryFile(b *testing.B) {
	list := NewTwoList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 50; i++ {
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
	list := NewTwoList()
	values := []string{"test1", "test2", "test3", "test4", "test5"}
	for i := 0; i < 50; i++ {
		list.AddToTail(values[i%5])
	}
	list.SaveToBinaryFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list := NewTwoList()
		file, _ := os.Open(file.Name())
		list.LoadFromBinaryFile(file)
		file.Close()
	}
}

package bench

import (
	"os"
	"testing"
)

func BenchmarkAddToEnd(b *testing.B) {
	d := NewDynArr(10)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d.AddToEnd("test")
	}
}

func BenchmarkAddToEndWithResize(b *testing.B) {
	d := NewDynArr(1)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d.AddToEnd("test")
	}
}

func BenchmarkAddAtIndex(b *testing.B) {
	d := NewDynArr(b.N + 1)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d.AddAtIndex(0, "test")
	}
}

func BenchmarkAddAtIndexWithResize(b *testing.B) {
	d := NewDynArr(1)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d.AddAtIndex(0, "test")
	}
}

func BenchmarkGetElement(b *testing.B) {
	d := NewDynArr(1000)
	for i := 0; i < 1000; i++ {
		d.AddToEnd("test")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d.GetElement(i % 1000)
	}
}

func BenchmarkReplaceElement(b *testing.B) {
	d := NewDynArr(1000)
	for i := 0; i < 1000; i++ {
		d.AddToEnd("test")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d.ReplaceElement(i%1000, "new")
	}
}

func BenchmarkRemoveAtIndex(b *testing.B) {
	d := NewDynArr(b.N + 1)
	for i := 0; i < b.N; i++ {
		d.AddToEnd("test")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d.RemoveAtIndex(0)
	}
}

func BenchmarkRemoveAtEnd(b *testing.B) {
	for i := 0; i < b.N; i++ {
		d := NewDynArr(100)
		for j := 0; j < 100; j++ {
			d.AddToEnd("test")
		}
		b.StartTimer()
		for j := 0; j < 100; j++ {
			if d.len > 0 {
				d.RemoveAtIndex(d.len - 1)
			}
		}
		b.StopTimer()
	}
}

func BenchmarkPrintArr(b *testing.B) {
	d := NewDynArr(100)
	for i := 0; i < 100; i++ {
		d.AddToEnd("test")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d.PrintArr()
	}
}

func BenchmarkSaveToFile(b *testing.B) {
	d := NewDynArr(1000)
	for i := 0; i < 1000; i++ {
		d.AddToEnd("test")
	}
	file, _ := os.CreateTemp("", "benchmark")
	defer os.Remove(file.Name())
	defer file.Close()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d.SaveToFile(file)
	}
}

func BenchmarkLoadFromFile(b *testing.B) {
	file, _ := os.CreateTemp("", "benchmark")
	for i := 0; i < 1000; i++ {
		file.WriteString("test ")
	}
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d := NewDynArr(10)
		file, _ := os.Open(file.Name())
		d.LoadFromFile(file)
		file.Close()
	}
}

func BenchmarkSaveToBinaryFile(b *testing.B) {
	d := NewDynArr(1000)
	for i := 0; i < 1000; i++ {
		d.AddToEnd("test")
	}
	file, _ := os.CreateTemp("", "benchmark")
	defer os.Remove(file.Name())
	defer file.Close()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d.SaveToBinaryFile(file)
	}
}

func BenchmarkLoadFromBinaryFile(b *testing.B) {
	file, _ := os.CreateTemp("", "benchmark")
	d := NewDynArr(1000)
	for i := 0; i < 1000; i++ {
		d.AddToEnd("test")
	}
	d.SaveToBinaryFile(file)
	file.Close()
	defer os.Remove(file.Name())

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d := NewDynArr(10)
		file, _ := os.Open(file.Name())
		d.LoadFromBinaryFile(file)
		file.Close()
	}
}

func BenchmarkGetLength(b *testing.B) {
	d := NewDynArr(1000)
	for i := 0; i < 1000; i++ {
		d.AddToEnd("test")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		d.GetLength()
	}
}

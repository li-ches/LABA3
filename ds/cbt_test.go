package ds

import (
	"os"
	"testing"
)

func TestBST_FullCoverage(t *testing.T) {
	tree := NewCBT() 
	
	// 1. Тест пустого дерева
	tree.Print()
	tree.Remove(10) // Удаление из пустого
	if tree.Search(10) {
		t.Error("Found in empty tree")
	}

	// 2. Вставка и дубликаты
	tree.Insert(50)
	tree.Insert(30)
	tree.Insert(70)
	tree.Insert(50) // Дубликат
	if tree.Size != 3 {
		t.Errorf("Size incorrect after duplicate insert. Expected 3, got %d", tree.Size)
	}

	// 3. Удаление листа (нет детей)
	tree.Remove(30)
	if tree.Contains(30) {
		t.Error("30 should be removed")
	}

	// 4. Удаление узла с ОДНИМ ребенком
	// Строим дерево: 50 -> right 70 -> right 80
	tree.Insert(80)
	tree.Remove(70) // Удаляем 70, 80 должно подтянуться
	if tree.Contains(70) {
		t.Error("70 should be removed")
	}
	if !tree.Contains(80) {
		t.Error("80 should remain")
	}

	// 5. Удаление узла с ДВУМЯ детьми
	tree.Clear()

	tree.Insert(50)
	tree.Insert(30)
	tree.Insert(70)
	tree.Insert(60)
	tree.Insert(80)

	// Удаляем 70. Его преемник (минимальный справа) - это 80 =
	tree.Remove(70)
	
	if tree.Contains(70) {
		t.Error("70 should be removed")
	}
	if !tree.Contains(60) || !tree.Contains(80) {
		t.Error("Children should remain")
	}

	// 6. Удаление КОРНЯ (с двумя детьми)
	tree.Remove(50)
	if tree.Contains(50) {
		t.Error("Root 50 should be removed")
	}
	// Проверяем целостность
	if !tree.Contains(30) || !tree.Contains(60) || !tree.Contains(80) {
		t.Error("Tree damaged after root removal")
	}

	// 7. Поиск несуществующего
	if tree.Search(999) {
		t.Error("Found 999?")
	}
	tree.Remove(999) // Удаление несуществующего
}

func TestBST_EdgeCases(t *testing.T) {
	tree := NewCBT()
	
	// Удаление корня, у которого только ЛЕВЫЙ ребенок
	tree.Insert(10)
	tree.Insert(5)
	tree.Remove(10)
	if tree.Contains(10) || !tree.Contains(5) {
		t.Error("Failed removing root with left child only")
	}

	tree.Clear()
	// Удаление корня, у которого только ПРАВЫЙ ребенок
	tree.Insert(10)
	tree.Insert(15)
	tree.Remove(10)
	if tree.Contains(10) || !tree.Contains(15) {
		t.Error("Failed removing root with right child only")
	}
}

// TestBST_Visuals_And_Traversal - новый тест для покрытия Print, Search и Contains
func TestBST_Visuals_And_Traversal(t *testing.T) {
	tree := NewCBT()
	tree.Insert(50)
	tree.Insert(30) // Left
	tree.Insert(70) // Right
	tree.Insert(20) // Left-Left
	tree.Insert(80) // Right-Right

	// 1. Покрытие метода Search (с выводом в консоль)
	// Ищем корень
	if !tree.Search(50) { t.Error("Search 50 failed") }
	// Ищем слева
	if !tree.Search(30) { t.Error("Search 30 failed") }
	// Ищем справа
	if !tree.Search(70) { t.Error("Search 70 failed") }
	
	// 2. Покрытие Contains (без вывода, только логика обхода)
	if !tree.Contains(20) { t.Error("Contains 20 failed") }
	if !tree.Contains(80) { t.Error("Contains 80 failed") }

	// 3. Покрытие метода Print для непустого дерева
	// Это заставит сработать рекурсию в printTreeUtil
	tree.Print()

	// 4. Покрытие метода Clear для непустого дерева
	tree.Clear()
	if tree.Size != 0 || tree.Root != nil {
		t.Error("Clear failed")
	}
}

func TestBST_SaveLoad(t *testing.T) {
	filename := "cbt_test.bin"
	defer os.Remove(filename)

	tree := NewCBT()
	tree.Insert(50)
	tree.Insert(30)
	tree.Insert(70)

	if err := tree.SaveToFile(filename); err != nil {
		t.Fatalf("Save failed: %v", err)
	}

	tree2 := NewCBT()
	if err := tree2.LoadFromFile(filename); err != nil {
		t.Fatalf("Load failed: %v", err)
	}

	if tree2.Size != 3 {
		t.Errorf("Size mismatch after load. Expected 3, got %d", tree2.Size)
	}
	if !tree2.Contains(50) || !tree2.Contains(30) || !tree2.Contains(70) {
		t.Error("Content mismatch")
	}
}
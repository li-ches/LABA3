package ds

import (
	"os"
	"testing"
)

func TestBST_FullCoverage(t *testing.T) {
	tree := NewCBT() 
	
	tree.Print()
	tree.Remove(10) 
	if tree.Search(10) {
		t.Error("Found in empty tree")
	}

	tree.Insert(50)
	tree.Insert(30)
	tree.Insert(70)
	tree.Insert(50) 
	if tree.Size != 3 {
		t.Errorf("Size incorrect after duplicate insert. Expected 3, got %d", tree.Size)
	}

	tree.Remove(30)
	if tree.Contains(30) {
		t.Error("30 should be removed")
	}

	tree.Insert(80)
	tree.Remove(70)
	if tree.Contains(70) {
		t.Error("70 should be removed")
	}
	if !tree.Contains(80) {
		t.Error("80 should remain")
	}

	tree.Clear()

	tree.Insert(50)
	tree.Insert(30)
	tree.Insert(70)
	tree.Insert(60)
	tree.Insert(80)

	tree.Remove(70)
	
	if tree.Contains(70) {
		t.Error("70 should be removed")
	}
	if !tree.Contains(60) || !tree.Contains(80) {
		t.Error("Children should remain")
	}

	tree.Remove(50)
	if tree.Contains(50) {
		t.Error("Root 50 should be removed")
	}
	if !tree.Contains(30) || !tree.Contains(60) || !tree.Contains(80) {
		t.Error("Tree damaged after root removal")
	}

	if tree.Search(999) {
		t.Error("Found 999?")
	}
	tree.Remove(999)
}

func TestBST_EdgeCases(t *testing.T) {
	tree := NewCBT()
	
	tree.Insert(10)
	tree.Insert(5)
	tree.Remove(10)
	if tree.Contains(10) || !tree.Contains(5) {
		t.Error("Failed removing root with left child only")
	}

	tree.Clear()
	tree.Insert(10)
	tree.Insert(15)
	tree.Remove(10)
	if tree.Contains(10) || !tree.Contains(15) {
		t.Error("Failed removing root with right child only")
	}
}

func TestBST_Visuals_And_Traversal(t *testing.T) {
	tree := NewCBT()
	tree.Insert(50)
	tree.Insert(30) 
	tree.Insert(70) 
	tree.Insert(20) 
	tree.Insert(80) 

	if !tree.Search(50) { t.Error("Search 50 failed") }
	if !tree.Search(30) { t.Error("Search 30 failed") }
	if !tree.Search(70) { t.Error("Search 70 failed") }
	
	if !tree.Contains(20) { t.Error("Contains 20 failed") }
	if !tree.Contains(80) { t.Error("Contains 80 failed") }

	tree.Print()

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

	serializer := NewCbtSerializer()
	if err := serializer.SaveToFile(tree, filename); err != nil {
		t.Fatalf("Save failed: %v", err)
	}

	tree2 := NewCBT()
	if err := serializer.LoadFromFile(tree2, filename); err != nil {
		t.Fatalf("Load failed: %v", err)
	}

	if tree2.Size != 3 {
		t.Errorf("Size mismatch after load. Expected 3, got %d", tree2.Size)
	}
	if !tree2.Contains(50) || !tree2.Contains(30) || !tree2.Contains(70) {
		t.Error("Content mismatch")
	}
}

func TestCBT_DeepCoverage(t *testing.T) {
	tree := NewCBT()
	
	tree.Print()
	tree.InorderPrint()
	tree.Remove(100)
	if tree.Contains(100) { t.Error("Empty tree contains 100?") }
	if tree.Search(100) { t.Error("Empty tree search failed") }
	
	serializer := NewCbtSerializer()
	serializer.SaveToBinaryFile(tree, "cbt_empty.bin") 
	defer os.Remove("cbt_empty.bin")

	tree.Insert(50)
	tree.Insert(30) 
	tree.Insert(70) 
	tree.Insert(20) 
	tree.Insert(40) 
	tree.Insert(60) 
	tree.Insert(80) 
	
	if !tree.Search(40) { t.Error("Search 40 failed") }
	if !tree.Search(80) { t.Error("Search 80 failed") }
	if tree.Search(99) { t.Error("Found 99?") }

	tree.Remove(20) 
	if tree.Contains(20) { t.Error("Remove leaf failed") }
	
	tree.Remove(30)
	if tree.Contains(30) { t.Error("Remove 30 failed") }
	if !tree.Contains(40) { t.Error("Child 40 lost") }

	tree.Remove(50)
	if tree.Contains(50) { t.Error("Remove root failed") }
	if !tree.Contains(60) || !tree.Contains(70) { t.Error("Tree structure broken") }

	serializer.SaveToFile(tree, "cbt.txt")
	defer os.Remove("cbt.txt")
	t2 := NewCBT()
	serializer.LoadFromFile(t2, "cbt.txt")
	if t2.Size == 0 { t.Error("Load Text failed") } 

	serializer.SaveToBinaryFile(tree, "cbt.bin")
	defer os.Remove("cbt.bin")
	t3 := NewCBT()
	serializer.LoadFromBinaryFile(t3, "cbt.bin")
	if !t3.Contains(60) { t.Error("Load Bin failed") }
}

func TestCBT_EdgeCases(t *testing.T) {
	tree := NewCBT()
	tree.Insert(10)
	tree.Insert(5) 
	tree.Remove(10)
	if tree.Root.Data != 5 { t.Error("Root remove (left only) failed") }

	tree.Clear()
	tree.Insert(10)
	tree.Insert(15) 
	tree.Remove(10)
	if tree.Root.Data != 15 { t.Error("Root remove (right only) failed") }
}
package ds

import (
	"os"
	"testing"
)

func TestMyList_Branch_Deletions(t *testing.T) {
	list := NewMyList()
	list.DelTail() 
	list.AddHead("A")
	list.DelTail() 
	if list.Head != nil {
		t.Errorf("Expected empty list")
	}
	list.AddTail("A")
	list.AddTail("B")
	list.DelTail()
	if list.Head.Value != "A" {
		t.Errorf("Expected A")
	}
	list.DelByValue("Z")
	list.DelByValue("A")
	if list.Head != nil {
		t.Errorf("Expected empty list")
	}
	list.AddTail("X")
	list.AddTail("Y")
	list.DelByValue("Y")
	if list.FindValue("Y") {
		t.Errorf("Y should be deleted")
	}
}

func TestMyList_Branch_AdvancedOps(t *testing.T) {
	list := NewMyList()
	list.DelAfterValue("A")
	list.AddTail("A")
	list.DelAfterValue("A")
	list.AddTail("B")
	list.DelAfterValue("Z")
	list.DelAfterValue("A")
	if list.FindValue("B") {
		t.Errorf("B should be deleted")
	}
	list.DelBeforeValue("A")
	list.AddHead("A")
	list.DelBeforeValue("A")

	l2 := NewMyList()
	l2.AddTail("1")
	l2.AddTail("2")
	l2.AddTail("3")
	l2.DelBeforeValue("2")
	if l2.Head.Value != "2" {
		t.Errorf("Expected head 2")
	}

	l3 := NewMyList()
	l3.AddTail("X")
	l3.AddTail("Y")
	l3.AddTail("Z")
	l3.AddTail("W")
	l3.DelBeforeValue("W")
	if l3.FindValue("Z") {
		t.Errorf("Z should be deleted")
	}
	l3.AddAfter("W", "End")
	l3.AddBefore("X", "Start")
	l3.AddAfter("Missing", "No")
	l3.AddBefore("Missing", "No")
	l3.ReadBack()
	
	// Empty list checks
	lEmpty := NewMyList()
	lEmpty.AddBefore("A", "B")
	lEmpty.AddAfter("A", "B")
	lEmpty.ReadBack()
	lEmpty.ReadForward()
}

func TestMyList_SaveAndLoad(t *testing.T) {
	l := NewMyList()
	l.AddTail("A")
	l.AddTail("B")
	filename := "list_test.bin"
	l.SaveToFile(filename)
	defer os.Remove(filename)
	l2 := NewMyList()
	l2.LoadFromFile(filename)
	if !l2.FindValue("A") || !l2.FindValue("B") {
		t.Errorf("List load failed")
	}
}
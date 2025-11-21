package ds

import (
	"os"
	"testing"
)

func TestDList_Branch_AllCases(t *testing.T) {
	dl := NewDList()
	dl.DelHead()
	dl.DelTail()
	dl.ReadForward()
	dl.ReadBackward()
	
	// Empty add checks
	dl.AddBefore("A", "B")
	dl.AddAfter("A", "B")

	dl.AddHead("A")
	dl.DelHead()
	if dl.Head != nil {
		t.Errorf("Expected empty")
	}
	dl.AddTail("B")
	dl.DelTail()
	if dl.Tail != nil {
		t.Errorf("Expected empty tail")
	}
	dl.AddHead("2")
	dl.AddHead("1")
	dl.AddTail("3")
	dl.AddAfter("1", "1.5")
	dl.AddBefore("3", "2.5")
	dl.DelByVal("1")
	dl.DelByVal("3")
	dl.DelByVal("2")
	dl.DelAfterValue("NotExists")
	dl.DelBeforeValue("NotExists")
	dl.AddTail("X")
	dl.AddTail("Y")
	dl.DelAfterValue("X")
	dl.DelBeforeValue("X")
	if !dl.Contains("X") {
		t.Errorf("Should contain X")
	}
}

func TestDList_SaveAndLoad(t *testing.T) {
	dl := NewDList()
	dl.AddTail("X")
	dl.AddTail("Y")
	filename := "dlist_test.bin"
	dl.SaveToFile(filename)
	defer os.Remove(filename)
	dl2 := NewDList()
	dl2.LoadFromFile(filename)
	if !dl2.Contains("X") || !dl2.Contains("Y") {
		t.Errorf("DList load failed")
	}
}
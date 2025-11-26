package ds

import (
	"testing"
)

func TestMyArr_GetOperations(t *testing.T) {
	arr := NewMyArr()
	
	if val := arr.GetAt(0); val != "[INVALID_INDEX]" {
		t.Errorf("Expected [INVALID_INDEX] for empty array, got %s", val)
	}
	if val := arr.GetAt(-1); val != "[INVALID_INDEX]" {
		t.Errorf("Expected [INVALID_INDEX] for negative index, got %s", val)
	}
	if val := arr.GetAt(100); val != "[INVALID_INDEX]" {
		t.Errorf("Expected [INVALID_INDEX] for out-of-bounds index, got %s", val)
	}

	arr.AddEnd("first")
	arr.AddEnd("second")
	arr.AddEnd("third")

	if val := arr.GetAt(0); val != "first" {
		t.Errorf("Expected 'first' at index 0, got %s", val)
	}
	if val := arr.GetAt(1); val != "second" {
		t.Errorf("Expected 'second' at index 1, got %s", val)
	}
	if val := arr.GetAt(2); val != "third" {
		t.Errorf("Expected 'third' at index 2, got %s", val)
	}

	if length := arr.LenArr(); length != 3 {
		t.Errorf("Expected length 3, got %d", length)
	}
}

func TestMyList_GetOperations(t *testing.T) {
	list := NewMyList()
	
	if found := list.FindValue("any"); found {
		t.Error("Expected false for empty list search")
	}

	list.AddTail("A")
	list.AddTail("B")
	list.AddTail("C")

	if !list.FindValue("A") {
		t.Error("Expected to find 'A'")
	}
	if !list.FindValue("B") {
		t.Error("Expected to find 'B'")
	}
	if !list.FindValue("C") {
		t.Error("Expected to find 'C'")
	}

	if list.FindValue("X") {
		t.Error("Should not find 'X'")
	}
}

func TestDList_GetOperations(t *testing.T) {
	dlist := NewDList()
	
	if found := dlist.Contains("any"); found {
		t.Error("Expected false for empty list search")
	}

	dlist.AddTail("X")
	dlist.AddTail("Y")
	dlist.AddTail("Z")

	if !dlist.Contains("X") {
		t.Error("Expected to find 'X'")
	}
	if !dlist.Contains("Y") {
		t.Error("Expected to find 'Y'")
	}
	if !dlist.Contains("Z") {
		t.Error("Expected to find 'Z'")
	}

	if dlist.Contains("W") {
		t.Error("Should not find 'W'")
	}
}

func TestStack_GetOperations(t *testing.T) {
	stack := NewStack()
	
	if !stack.IsEmpty() {
		t.Error("New stack should be empty")
	}
	if val := stack.Peek(); val != "[STACK_EMPTY]" {
		t.Errorf("Expected [STACK_EMPTY] for empty stack peek, got %s", val)
	}

	stack.Push("bottom")
	stack.Push("middle")
	stack.Push("top")

	if val := stack.Peek(); val != "top" {
		t.Errorf("Expected 'top' for peek, got %s", val)
	}
	if stack.IsEmpty() {
		t.Error("Stack should not be empty")
	}

	if val := stack.Pop(); val != "top" {
		t.Errorf("Expected 'top' for first pop, got %s", val)
	}
	if val := stack.Pop(); val != "middle" {
		t.Errorf("Expected 'middle' for second pop, got %s", val)
	}
	if val := stack.Pop(); val != "bottom" {
		t.Errorf("Expected 'bottom' for third pop, got %s", val)
	}
	if !stack.IsEmpty() {
		t.Error("Stack should be empty after all pops")
	}
}

func TestQueue_GetOperations(t *testing.T) {
	queue := NewQueue()
	
	if !queue.IsEmpty() {
		t.Error("New queue should be empty")
	}
	if val := queue.Peek(); val != "[QUEUE_EMPTY]" {
		t.Errorf("Expected [QUEUE_EMPTY] for empty queue peek, got %s", val)
	}

	queue.Push("first")
	queue.Push("second")
	queue.Push("third")

	if val := queue.Peek(); val != "first" {
		t.Errorf("Expected 'first' for peek, got %s", val)
	}
	if queue.IsEmpty() {
		t.Error("Queue should not be empty")
	}

	if val := queue.Pop(); val != "first" {
		t.Errorf("Expected 'first' for first pop, got %s", val)
	}
	if val := queue.Pop(); val != "second" {
		t.Errorf("Expected 'second' for second pop, got %s", val)
	}
	if val := queue.Pop(); val != "third" {
		t.Errorf("Expected 'third' for third pop, got %s", val)
	}
	if !queue.IsEmpty() {
		t.Error("Queue should be empty after all pops")
	}
}

func TestCBT_GetOperations(t *testing.T) {
	tree := NewCBT()
	
	if tree.Contains(100) {
		t.Error("Empty tree should not contain any elements")
	}
	if tree.Search(100) {
		t.Error("Empty tree search should return false")
	}

	tree.Insert(50)
	tree.Insert(30)
	tree.Insert(70)
	tree.Insert(20)
	tree.Insert(40)
	tree.Insert(60)
	tree.Insert(80)

	if !tree.Contains(50) {
		t.Error("Should contain 50")
	}
	if !tree.Contains(30) {
		t.Error("Should contain 30")
	}
	if !tree.Contains(70) {
		t.Error("Should contain 70")
	}
	if !tree.Contains(20) {
		t.Error("Should contain 20")
	}
	if !tree.Contains(40) {
		t.Error("Should contain 40")
	}
	if !tree.Contains(60) {
		t.Error("Should contain 60")
	}
	if !tree.Contains(80) {
		t.Error("Should contain 80")
	}

	if tree.Contains(25) {
		t.Error("Should not contain 25")
	}
	if tree.Contains(90) {
		t.Error("Should not contain 90")
	}

	if !tree.Search(50) {
		t.Error("Search should find 50")
	}
	if tree.Search(25) {
		t.Error("Search should not find 25")
	}
}

func TestHash_GetOperations(t *testing.T) {
	ch := NewChainHash(5)
	
	if val := ch.Find("nonexistent"); val != "" {
		t.Errorf("Expected empty string for nonexistent key, got %s", val)
	}

	ch.Insert("key1", "value1")
	ch.Insert("key2", "value2")
	ch.Insert("key3", "value3")

	if val := ch.Find("key1"); val != "value1" {
		t.Errorf("Expected 'value1' for key1, got %s", val)
	}
	if val := ch.Find("key2"); val != "value2" {
		t.Errorf("Expected 'value2' for key2, got %s", val)
	}
	if val := ch.Find("key3"); val != "value3" {
		t.Errorf("Expected 'value3' for key3, got %s", val)
	}

	if val := ch.Find("key4"); val != "" {
		t.Errorf("Expected empty string for key4, got %s", val)
	}

	oh := NewOpenHash(5)
	
	if val := oh.Find("nonexistent"); val != "" {
		t.Errorf("Expected empty string for nonexistent key, got %s", val)
	}

	oh.Insert("k1", "v1")
	oh.Insert("k2", "v2")
	oh.Insert("k3", "v3")

	if val := oh.Find("k1"); val != "v1" {
		t.Errorf("Expected 'v1' for k1, got %s", val)
	}
	if val := oh.Find("k2"); val != "v2" {
		t.Errorf("Expected 'v2' for k2, got %s", val)
	}
	if val := oh.Find("k3"); val != "v3" {
		t.Errorf("Expected 'v3' for k3, got %s", val)
	}

	if val := oh.Find("k4"); val != "" {
		t.Errorf("Expected empty string for k4, got %s", val)
	}
}

func TestMyArr_EdgeCases(t *testing.T) {
	arr := NewMyArr()

	arr.AddEnd("single")
	if val := arr.GetAt(0); val != "single" {
		t.Errorf("Expected 'single' at index 0, got %s", val)
	}

	arr.DelHead()
	if val := arr.GetAt(0); val != "[INVALID_INDEX]" {
		t.Errorf("Expected [INVALID_INDEX] after deletion, got %s", val)
	}

	arr.AddHead("new")
	if val := arr.GetAt(0); val != "new" {
		t.Errorf("Expected 'new' at index 0 after AddHead, got %s", val)
	}
}

func TestMyList_EdgeCases(t *testing.T) {
	list := NewMyList()

	list.AddHead("only")
	if !list.FindValue("only") {
		t.Error("Should find the only element")
	}

	list.DelHead()
	if list.FindValue("only") {
		t.Error("Should not find element after deletion")
	}

	list.AddTail("last")
	if !list.FindValue("last") {
		t.Error("Should find element added to tail")
	}
}

func TestStack_EdgeCases(t *testing.T) {
	stack := NewStack()

	stack.Push("single")
	if val := stack.Pop(); val != "single" {
		t.Errorf("Expected 'single', got %s", val)
	}

	if val := stack.Pop(); val != "[STACK_EMPTY]" {
		t.Errorf("Expected [STACK_EMPTY], got %s", val)
	}
}

func TestQueue_EdgeCases(t *testing.T) {
	queue := NewQueue()

	queue.Push("single")
	if val := queue.Pop(); val != "single" {
		t.Errorf("Expected 'single', got %s", val)
	}

	if val := queue.Pop(); val != "[QUEUE_EMPTY]" {
		t.Errorf("Expected [QUEUE_EMPTY], got %s", val)
	}
}
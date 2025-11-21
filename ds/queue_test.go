package ds

import (
	"os"
	"testing"
)

func TestQueue_Branch_Coverage(t *testing.T) {
	q := NewQueue()
	if !q.IsEmpty() {
		t.Errorf("New queue should be empty")
	}
	if val := q.Pop(); val != "[QUEUE_EMPTY]" {
		t.Errorf("Expected empty msg")
	}
	if val := q.Peek(); val != "[QUEUE_EMPTY]" {
		t.Errorf("Expected empty msg")
	}
	q.Print() // Empty print

	q.Push("A")
	if q.Peek() != "A" {
		t.Errorf("Expected A")
	}
	if q.Pop() != "A" {
		t.Errorf("Expected A")
	}
	q.Push("1")
	q.Push("2")
	if q.Pop() != "1" {
		t.Errorf("Expected 1")
	}
	if q.Pop() != "2" {
		t.Errorf("Expected 2")
	}
	q.Print()
}

func TestQueue_SaveAndLoad(t *testing.T) {
	q := NewQueue()
	q.Push("first")
	q.Push("second")
	q.SaveToFile("queue_test.dat")
	defer os.Remove("queue_test.dat")

	q2 := NewQueue()
	q2.LoadFromFile("queue_test.dat")
	if q2.Pop() != "first" {
		t.Errorf("Expected first")
	}
	if q2.Pop() != "second" {
		t.Errorf("Expected second")
	}
}
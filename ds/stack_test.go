package ds

import (
	"os"
	"testing"
)

func TestStack_Branch_Coverage(t *testing.T) {
	s := NewStack()
	if !s.IsEmpty() {
		t.Errorf("New stack should be empty")
	}
	if s.Pop() != "[STACK_EMPTY]" {
		t.Errorf("Expected empty")
	}
	if s.Peek() != "[STACK_EMPTY]" {
		t.Errorf("Expected empty")
	}
	s.ReadStack() // Empty print

	s.Push("A")
	if s.Peek() != "A" {
		t.Errorf("Expected A")
	}
	if s.Pop() != "A" {
		t.Errorf("Expected A")
	}
	s.ReadStack()
}

func TestStack_SaveAndLoad(t *testing.T) {
	s := NewStack()
	s.Push("hello")
	s.Push("world")
	s.SaveToFile("stack_test.dat")
	defer os.Remove("stack_test.dat")

	s2 := NewStack()
	s2.LoadFromFile("stack_test.dat")
	if s2.Pop() != "world" {
		t.Errorf("Expected world")
	}
	if s2.Pop() != "hello" {
		t.Errorf("Expected hello")
	}
}
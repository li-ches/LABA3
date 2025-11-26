package ds

import "fmt"

type SNode struct {
	Value string
	Next  *SNode
}

type Stack struct {
	Top *SNode
}

func NewStack() *Stack {
	return &Stack{}
}

func (s *Stack) Push(val string) {
	s.Top = &SNode{Value: val, Next: s.Top}
}

func (s *Stack) Pop() string {
	if s.Top == nil {
		return "[STACK_EMPTY]"
	}
	val := s.Top.Value
	s.Top = s.Top.Next
	return val
}

func (s *Stack) Peek() string {
	if s.Top == nil {
		return "[STACK_EMPTY]"
	}
	return s.Top.Value
}

func (s *Stack) IsEmpty() bool {
	return s.Top == nil
}

func (s *Stack) ReadStack() {
	if s.Top == nil {
		fmt.Println("Стек пуст")
		return
	}
	fmt.Println("Содержимое стека (сверху вниз): ")
	for cur := s.Top; cur != nil; cur = cur.Next {
		fmt.Printf("  %s\n", cur.Value)
	}
}
package ds

import (
	"bufio"
	"fmt"
	"os"
)

type StackSerializer struct{}

func NewStackSerializer() *StackSerializer {
	return &StackSerializer{}
}

func (s *Stack) toSlice() []string {
	out := []string{}
	for cur := s.Top; cur != nil; cur = cur.Next {
		out = append(out, cur.Value)
	}
	return out
}

func (s *StackSerializer) SaveToFile(stack *Stack, filename string) error {
	arr := stack.toSlice()
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	fmt.Fprintln(file, len(arr))
	for i := len(arr) - 1; i >= 0; i-- {
		WriteStringText(file, arr[i])
	}
	return nil
}

func (s *StackSerializer) LoadFromFile(stack *Stack, filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	stack.Top = nil
	scanner := bufio.NewScanner(file)

	var count int
	if scanner.Scan() {
		fmt.Sscanf(scanner.Text(), "%d", &count)
	}

	for i := 0; i < count; i++ {
		val := ReadStringText(scanner)
		stack.Push(val)
	}
	return nil
}

func (s *StackSerializer) SaveToBinaryFile(stack *Stack, filename string) error {
	arr := stack.toSlice()
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	if err := WriteInt32(file, int32(len(arr))); err != nil {
		return err
	}

	for i := len(arr) - 1; i >= 0; i-- {
		if err := WriteString(file, arr[i]); err != nil {
			return err
		}
	}
	return nil
}

func (s *StackSerializer) LoadFromBinaryFile(stack *Stack, filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	stack.Top = nil

	count, err := ReadInt32(file)
	if err != nil {
		return err
	}

	for i := 0; i < int(count); i++ {
		val, err := ReadString(file)
		if err != nil {
			return err
		}
		stack.Push(val)
	}
	return nil
}
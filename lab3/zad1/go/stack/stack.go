package stack

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"
	"goproj/node"
)

type Stack struct {
	top *node.StackNode
}

func NewStack() *Stack {
	return &Stack{}
}

func (s *Stack) Push(val string) {
	newNode := &node.StackNode{Data: val}
	newNode.Next = s.top
	s.top = newNode
}

func (s *Stack) Pop() error {
	if s.top == nil {
		return errors.New("error")
	}
	s.top = s.top.Next
	return nil
}

func (s *Stack) Print() {
	if s.top == nil {
		fmt.Println("error")
		return
	}

	curr := s.top
	fmt.Print("стек: ")
	for curr != nil {
		fmt.Print(curr.Data)
		if curr.Next != nil {
			fmt.Print(" -> ")
		}
		curr = curr.Next
	}
	fmt.Println()
}

func (s *Stack) SaveToFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	curr := s.top
	for curr != nil {
		_, err := file.WriteString(curr.Data + " ")
		if err != nil {
			return err
		}
		curr = curr.Next
	}
	return nil
}

func (s *Stack) LoadFromFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	content, err := io.ReadAll(file)
	if err != nil {
		return err
	}

	values := strings.Fields(string(content))
	for i := len(values) - 1; i >= 0; i-- {
		s.Push(values[i])
	}
	return nil
}

func (s *Stack) SaveToBinaryFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	curr := s.top
	for curr != nil {
		strSize := uint64(len(curr.Data))
		err := binary.Write(file, binary.LittleEndian, strSize)
		if err != nil {
			return err
		}
		_, err = file.WriteString(curr.Data)
		if err != nil {
			return err
		}
		curr = curr.Next
	}
	return nil
}

func (s *Stack) LoadFromBinaryFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	var elements []string
	for {
		var strSize uint64
		err := binary.Read(file, binary.LittleEndian, &strSize)
		if err != nil {
			if err == io.EOF {
				break
			}
			return err
		}

		buffer := make([]byte, strSize)
		_, err = file.Read(buffer)
		if err != nil {
			return err
		}
		elements = append(elements, string(buffer))
	}

	for i := len(elements) - 1; i >= 0; i-- {
		s.Push(elements[i])
	}
	return nil
}
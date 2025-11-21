package twolist

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"
)

type TwoList struct {
	head *TwoListNode
	tail *TwoListNode
}

func NewTwoList() *TwoList {
	return &TwoList{}
}

func (tl *TwoList) AddToHead(val string) {
	newNode := &TwoListNode{Data: val}
	if tl.head == nil {
		tl.head = newNode
		tl.tail = newNode
	} else {
		newNode.Next = tl.head
		tl.head.Prev = newNode
		tl.head = newNode
	}
}

func (tl *TwoList) AddToTail(val string) {
	newNode := &TwoListNode{Data: val}
	if tl.head == nil {
		tl.head = newNode
		tl.tail = newNode
	} else {
		newNode.Prev = tl.tail
		tl.tail.Next = newNode
		tl.tail = newNode
	}
}

func (tl *TwoList) AddAfterValue(targetVal, newVal string) error {
	targetNode := tl.findByVal(targetVal)
	if targetNode == nil {
		return errors.New("error")
	}

	newNode := &TwoListNode{Data: newVal}
	newNode.Next = targetNode.Next
	newNode.Prev = targetNode

	if targetNode.Next != nil {
		targetNode.Next.Prev = newNode
	}
	targetNode.Next = newNode

	if targetNode == tl.tail {
		tl.tail = newNode
	}
	return nil
}

func (tl *TwoList) AddBeforeValue(targetVal, newVal string) error {
	targetNode := tl.findByVal(targetVal)
	if targetNode == nil {
		return errors.New("error")
	}

	if targetNode == tl.head {
		tl.AddToHead(newVal)
		return nil
	}

	newNode := &TwoListNode{Data: newVal}
	newNode.Next = targetNode
	newNode.Prev = targetNode.Prev

	if targetNode.Prev != nil {
		targetNode.Prev.Next = newNode
	}
	targetNode.Prev = newNode
	return nil
}

func (tl *TwoList) RemoveFromHead() error {
	if tl.head == nil {
		return errors.New("error")
	}
	if tl.head == tl.tail {
		tl.head = nil
		tl.tail = nil
	} else {
		tl.head = tl.head.Next
		tl.head.Prev = nil
	}
	return nil
}

func (tl *TwoList) RemoveFromTail() error {
	if tl.head == nil {
		return errors.New("error")
	}

	if tl.head == tl.tail {
		tl.head = nil
		tl.tail = nil
	} else {
		tl.tail = tl.tail.Prev
		tl.tail.Next = nil
	}
	return nil
}

func (tl *TwoList) RemoveAfterValue(targetVal string) error {
	targetNode := tl.findByVal(targetVal)
	if targetNode == nil || targetNode.Next == nil {
		return errors.New("error")
	}

	temp := targetNode.Next
	targetNode.Next = temp.Next

	if temp.Next != nil {
		temp.Next.Prev = targetNode
	}

	if temp == tl.tail {
		tl.tail = targetNode
	}
	return nil
}

func (tl *TwoList) RemoveBeforeValue(targetVal string) error {
	targetNode := tl.findByVal(targetVal)
	if targetNode == nil || targetNode.Prev == nil {
		return errors.New("error")
	}

	if targetNode.Prev == tl.head {
		return tl.RemoveFromHead()
	}

	temp := targetNode.Prev
	targetNode.Prev = temp.Prev

	if temp.Prev != nil {
		temp.Prev.Next = targetNode
	}
	return nil
}

func (tl *TwoList) RemoveByValue(targetVal string) error {
	targetNode := tl.findByVal(targetVal)
	if targetNode == nil {
		return errors.New("error")
	}

	if targetNode == tl.head {
		return tl.RemoveFromHead()
	}

	if targetNode == tl.tail {
		return tl.RemoveFromTail()
	}

	if targetNode.Prev != nil {
		targetNode.Prev.Next = targetNode.Next
	}

	if targetNode.Next != nil {
		targetNode.Next.Prev = targetNode.Prev
	}
	return nil
}

func (tl *TwoList) GetElementByValue(targetVal string) (string, error) {
	targetNode := tl.findByVal(targetVal)
	if targetNode == nil {
		return "", errors.New("error")
	}
	return targetNode.Data, nil
}

func (tl *TwoList) findByVal(val string) *TwoListNode {
	curr := tl.head
	for curr != nil {
		if curr.Data == val {
			return curr
		}
		curr = curr.Next
	}
	return nil
}

func (tl *TwoList) Print() {
	if tl.head == nil {
		fmt.Println("error")
		return
	}

	curr := tl.head
	fmt.Print("список: ")
	for curr != nil {
		fmt.Print(curr.Data)
		if curr.Next != nil {
			fmt.Print(" <-> ")
		}
		curr = curr.Next
	}
	fmt.Println()
}

func (tl *TwoList) SaveToFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	curr := tl.head
	for curr != nil {
		_, err := file.WriteString(curr.Data + " ")
		if err != nil {
			return err
		}
		curr = curr.Next
	}
	return nil
}

func (tl *TwoList) LoadFromFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	content, err := io.ReadAll(file)
	if err != nil {
		return err
	}

	values := strings.Fields(string(content))
	for _, val := range values {
		tl.AddToTail(val)
	}
	return nil
}

func (tl *TwoList) SaveToBinaryFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	curr := tl.head
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

func (tl *TwoList) LoadFromBinaryFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

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
		tl.AddToTail(string(buffer))
	}
	return nil
}

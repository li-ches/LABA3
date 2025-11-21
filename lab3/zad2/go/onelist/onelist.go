package onelist

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"
)

type OneList struct {
	head *OneListNode
	tail *OneListNode
}

func NewOneList() *OneList {
	return &OneList{}
}

func (ol *OneList) AddToHead(val string) {
	newNode := &OneListNode{Data: val}
	newNode.Next = ol.head
	ol.head = newNode
	if ol.tail == nil {
		ol.tail = newNode
	}
}

func (ol *OneList) AddToTail(val string) {
	newNode := &OneListNode{Data: val}
	if ol.tail == nil {
		ol.head = newNode
		ol.tail = newNode
	} else {
		ol.tail.Next = newNode
		ol.tail = newNode
	}
}

func (ol *OneList) AddAfterValue(targetVal, newVal string) error {
	targetNode := ol.findByVal(targetVal)
	if targetNode == nil {
		return errors.New("error")
	}

	newNode := &OneListNode{Data: newVal}
	newNode.Next = targetNode.Next
	targetNode.Next = newNode

	if targetNode == ol.tail {
		ol.tail = newNode
	}
	return nil
}

func (ol *OneList) AddBeforeValue(targetVal, newVal string) error {
	targetNode := ol.findByVal(targetVal)
	if targetNode == nil {
		return errors.New("error")
	}

	if targetNode == ol.head {
		ol.AddToHead(newVal)
		return nil
	}

	prevNode := ol.head
	for prevNode != nil && prevNode.Next != targetNode {
		prevNode = prevNode.Next
	}

	if prevNode == nil {
		return errors.New("error")
	}

	newNode := &OneListNode{Data: newVal}
	newNode.Next = targetNode
	prevNode.Next = newNode
	return nil
}

func (ol *OneList) RemoveFromHead() error {
	if ol.head == nil {
		return errors.New("error")
	}
	ol.head = ol.head.Next
	if ol.head == nil {
		ol.tail = nil
	}
	return nil
}

func (ol *OneList) RemoveFromTail() error {
	if ol.head == nil {
		return errors.New("error")
	}

	if ol.head == ol.tail {
		ol.head = nil
		ol.tail = nil
		return nil
	}

	curr := ol.head
	for curr.Next != ol.tail {
		curr = curr.Next
	}
	curr.Next = nil
	ol.tail = curr
	return nil
}

func (ol *OneList) RemoveAfterValue(targetVal string) error {
	targetNode := ol.findByVal(targetVal)
	if targetNode == nil || targetNode.Next == nil {
		return errors.New("error")
	}

	temp := targetNode.Next
	targetNode.Next = temp.Next

	if temp == ol.tail {
		ol.tail = targetNode
	}
	return nil
}

func (ol *OneList) RemoveBeforeValue(targetVal string) error {
	targetNode := ol.findByVal(targetVal)
	if targetNode == nil || targetNode == ol.head {
		return errors.New("error")
	}

	if ol.head.Next == targetNode {
		return ol.RemoveFromHead()
	}

	prevPrev := ol.head
	for prevPrev.Next != nil && prevPrev.Next.Next != targetNode {
		prevPrev = prevPrev.Next
	}

	if prevPrev.Next == nil {
		return errors.New("error")
	}

	prevPrev.Next = targetNode
	return nil
}

func (ol *OneList) RemoveByValue(targetVal string) error {
	targetNode := ol.findByVal(targetVal)
	if targetNode == nil {
		return errors.New("error")
	}

	if targetNode == ol.head {
		return ol.RemoveFromHead()
	}

	prevNode := ol.head
	for prevNode != nil && prevNode.Next != targetNode {
		prevNode = prevNode.Next
	}

	if prevNode == nil {
		return errors.New("error")
	}

	prevNode.Next = targetNode.Next
	if targetNode == ol.tail {
		ol.tail = prevNode
	}
	return nil
}

func (ol *OneList) GetElementByValue(targetVal string) (string, error) {
	targetNode := ol.findByVal(targetVal)
	if targetNode == nil {
		return "", errors.New("error")
	}
	return targetNode.Data, nil
}

func (ol *OneList) findByVal(val string) *OneListNode {
	curr := ol.head
	for curr != nil {
		if curr.Data == val {
			return curr
		}
		curr = curr.Next
	}
	return nil
}

func (ol *OneList) Print() {
	if ol.head == nil {
		fmt.Println("error")
		return
	}

	curr := ol.head
	fmt.Print("список: ")
	for curr != nil {
		fmt.Print(curr.Data)
		if curr.Next != nil {
			fmt.Print(" -> ")
		}
		curr = curr.Next
	}
	fmt.Println()
}

func (ol *OneList) SaveToFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	curr := ol.head
	for curr != nil {
		_, err := file.WriteString(curr.Data + " ")
		if err != nil {
			return err
		}
		curr = curr.Next
	}
	return nil
}

func (ol *OneList) LoadFromFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	content, err := io.ReadAll(file)
	if err != nil {
		return err
	}

	values := strings.Fields(string(content))
	for _, val := range values {
		ol.AddToTail(val)
	}
	return nil
}

func (ol *OneList) SaveToBinaryFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	curr := ol.head
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

func (ol *OneList) LoadFromBinaryFile(file *os.File) error {
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
		ol.AddToTail(string(buffer))
	}
	return nil
}

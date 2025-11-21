package bench

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"
)

type Queue struct {
	front *QueueNode
	rear  *QueueNode
}

func NewQueue() *Queue {
	return &Queue{}
}

func (q *Queue) Enqueue(val string) {
	newNode := &QueueNode{Data: val}

	if q.front == nil {
		q.front = newNode
		q.rear = newNode
	} else {
		q.rear.Next = newNode
		q.rear = newNode
	}
}

func (q *Queue) Dequeue() error {
	if q.front == nil {
		return errors.New("error")
	}

	q.front = q.front.Next
	if q.front == nil {
		q.rear = nil
	}
	return nil
}

func (q *Queue) Print() {
	if q.front == nil {
		fmt.Println("error")
		return
	}

	curr := q.front
	fmt.Print("очередь: ")
	for curr != nil {
		fmt.Print(curr.Data)
		if curr.Next != nil {
			fmt.Print(" -> ")
		}
		curr = curr.Next
	}
	fmt.Println()
}

func (q *Queue) SaveToFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	curr := q.front
	for curr != nil {
		_, err := file.WriteString(curr.Data + " ")
		if err != nil {
			return err
		}
		curr = curr.Next
	}
	return nil
}

func (q *Queue) LoadFromFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	content, err := io.ReadAll(file)
	if err != nil {
		return err
	}

	values := strings.Fields(string(content))
	for _, val := range values {
		q.Enqueue(val)
	}
	return nil
}

func (q *Queue) SaveToBinaryFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	curr := q.front
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

func (q *Queue) LoadFromBinaryFile(file *os.File) error {
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
		q.Enqueue(string(buffer))
	}
	return nil
}

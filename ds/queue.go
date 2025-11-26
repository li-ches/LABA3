package ds

import "fmt"

type QNode struct {
	Data string
	Next *QNode
}

type Queue struct {
	Front *QNode
	Rear  *QNode
}

func NewQueue() *Queue {
	return &Queue{}
}

func (q *Queue) IsEmpty() bool {
	return q.Front == nil
}

func (q *Queue) Push(val string) {
	newNode := &QNode{Data: val, Next: nil}
	if q.IsEmpty() {
		q.Front = newNode
		q.Rear = newNode
	} else {
		q.Rear.Next = newNode
		q.Rear = newNode
	}
}

func (q *Queue) Pop() string {
	if q.IsEmpty() {
		return "[QUEUE_EMPTY]"
	}
	val := q.Front.Data
	q.Front = q.Front.Next
	if q.Front == nil {
		q.Rear = nil
	}
	return val
}

func (q *Queue) Peek() string {
	if q.IsEmpty() {
		return "[QUEUE_EMPTY]"
	}
	return q.Front.Data
}

func (q *Queue) Print() {
	if q.IsEmpty() {
		fmt.Println("Очередь пуста.")
		return
	}
	fmt.Print("Очередь (начало -> конец): ")
	for cur := q.Front; cur != nil; cur = cur.Next {
		fmt.Printf("\"%s\" ", cur.Data)
	}
	fmt.Println()
}
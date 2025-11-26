package ds

type QueueSerializer struct{}

func NewQueueSerializer() *QueueSerializer {
	return &QueueSerializer{}
}

func (q *Queue) toSlice() []string {
	out := []string{}
	for cur := q.Front; cur != nil; cur = cur.Next {
		out = append(out, cur.Data)
	}
	return out
}

func (s *QueueSerializer) SaveToFile(queue *Queue, filename string) error {
	return SaveStringsText(filename, queue.toSlice())
}

func (s *QueueSerializer) LoadFromFile(queue *Queue, filename string) error {
	vals, err := LoadStringsText(filename)
	if err != nil {
		return err
	}
	queue.Front = nil
	queue.Rear = nil
	for _, v := range vals {
		queue.Push(v)
	}
	return nil
}

func (s *QueueSerializer) SaveToBinaryFile(queue *Queue, filename string) error {
	return SaveStringsBinary(filename, queue.toSlice())
}

func (s *QueueSerializer) LoadFromBinaryFile(queue *Queue, filename string) error {
	vals, err := LoadStringsBinary(filename)
	if err != nil {
		return err
	}
	queue.Front = nil
	queue.Rear = nil
	for _, v := range vals {
		queue.Push(v)
	}
	return nil
}
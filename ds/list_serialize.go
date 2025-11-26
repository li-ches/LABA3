package ds

type ListSerializer struct{}

func NewListSerializer() *ListSerializer {
	return &ListSerializer{}
}

func (l *MyList) toSlice() []string {
	out := []string{}
	for cur := l.Head; cur != nil; cur = cur.Next {
		out = append(out, cur.Value)
	}
	return out
}

func (s *ListSerializer) SaveToFile(list *MyList, filename string) error {
	return SaveStringsText(filename, list.toSlice())
}

func (s *ListSerializer) LoadFromFile(list *MyList, filename string) error {
	vals, err := LoadStringsText(filename)
	if err != nil {
		return err
	}
	list.Head = nil
	for _, v := range vals {
		list.AddTail(v)
	}
	return nil
}

func (s *ListSerializer) SaveToBinaryFile(list *MyList, filename string) error {
	return SaveStringsBinary(filename, list.toSlice())
}

func (s *ListSerializer) LoadFromBinaryFile(list *MyList, filename string) error {
	vals, err := LoadStringsBinary(filename)
	if err != nil {
		return err
	}
	list.Head = nil
	for _, v := range vals {
		list.AddTail(v)
	}
	return nil
}
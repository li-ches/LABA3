package ds

type DListSerializer struct{}

func NewDListSerializer() *DListSerializer {
	return &DListSerializer{}
}

func (d *DList) toSlice() []string {
	out := []string{}
	for cur := d.Head; cur != nil; cur = cur.Next {
		out = append(out, cur.Value)
	}
	return out
}

func (s *DListSerializer) SaveToFile(dlist *DList, filename string) error {
	return SaveStringsText(filename, dlist.toSlice())
}

func (s *DListSerializer) LoadFromFile(dlist *DList, filename string) error {
	vals, err := LoadStringsText(filename)
	if err != nil {
		return err
	}
	dlist.Head = nil
	dlist.Tail = nil
	for _, v := range vals {
		dlist.AddTail(v)
	}
	return nil
}

func (s *DListSerializer) SaveToBinaryFile(dlist *DList, filename string) error {
	return SaveStringsBinary(filename, dlist.toSlice())
}

func (s *DListSerializer) LoadFromBinaryFile(dlist *DList, filename string) error {
	vals, err := LoadStringsBinary(filename)
	if err != nil {
		return err
	}
	dlist.Head = nil
	dlist.Tail = nil
	for _, v := range vals {
		dlist.AddTail(v)
	}
	return nil
}
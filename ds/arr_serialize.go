package ds

type ArrSerializer struct{}

func NewArrSerializer() *ArrSerializer {
	return &ArrSerializer{}
}

func (s *ArrSerializer) SaveToFile(arr *MyArr, filename string) error {
	return SaveStringsText(filename, arr.data)
}

func (s *ArrSerializer) LoadFromFile(arr *MyArr, filename string) error {
	vals, err := LoadStringsText(filename)
	if err != nil {
		return err
	}
	arr.data = vals
	return nil
}

func (s *ArrSerializer) SaveToBinaryFile(arr *MyArr, filename string) error {
	return SaveStringsBinary(filename, arr.data)
}

func (s *ArrSerializer) LoadFromBinaryFile(arr *MyArr, filename string) error {
	vals, err := LoadStringsBinary(filename)
	if err != nil {
		return err
	}
	arr.data = vals
	return nil
}
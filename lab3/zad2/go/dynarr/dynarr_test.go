package dynarr

import (
	"os"
	"testing"

	"github.com/stretchr/testify/suite"
)

type DynArrTestSuite struct {
	suite.Suite
}

func TestDynArrTestSuite(t *testing.T) {
	suite.Run(t, new(DynArrTestSuite))
}

func generateRandomString(length int) string {
	const charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	result := make([]byte, length)
	for i := range result {
		result[i] = charset[i%len(charset)]
	}
	return string(result)
}

func (s *DynArrTestSuite) TestConstructor() {
	arr1 := NewDynArr(0)
	s.Equal(DefaultInitialCapacity, arr1.cap)
	s.Equal(0, arr1.len)

	arr2 := NewDynArr(20)
	s.Equal(20, arr2.cap)
	s.Equal(0, arr2.len)

	s.NotPanics(func() {
		_ = NewDynArr(10)
	})
}

func (s *DynArrTestSuite) TestAddToEnd() {
	arr := NewDynArr(2)
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	arr.AddToEnd(str1)
	s.Equal(1, arr.len)
	element, _ := arr.GetElement(0)
	s.Equal(str1, element)

	arr.AddToEnd(str2)
	s.Equal(2, arr.len)
	element, _ = arr.GetElement(1)
	s.Equal(str2, element)

	arr.AddToEnd(str3)
	s.Equal(3, arr.len)
	s.Equal(4, arr.cap)
	element, _ = arr.GetElement(2)
	s.Equal(str3, element)

	for i := 0; i < 5; i++ {
		str := generateRandomString(10)
		arr.AddToEnd(str)
		element, _ := arr.GetElement(3 + i)
		s.Equal(str, element)
	}
}

func (s *DynArrTestSuite) TestAddAtIndex() {
	arr := NewDynArr(3)
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)
	str4 := generateRandomString(10)
	str5 := generateRandomString(10)

	arr.AddAtIndex(0, str1)
	s.Equal(1, arr.len)
	element, _ := arr.GetElement(0)
	s.Equal(str1, element)

	arr.AddAtIndex(0, str2)
	s.Equal(2, arr.len)
	element, _ = arr.GetElement(0)
	s.Equal(str2, element)
	element, _ = arr.GetElement(1)
	s.Equal(str1, element)

	arr.AddAtIndex(2, str3)
	s.Equal(3, arr.len)
	element, _ = arr.GetElement(2)
	s.Equal(str3, element)

	arr.AddAtIndex(1, str4)
	s.Equal(4, arr.len)
	element, _ = arr.GetElement(1)
	s.Equal(str4, element)

	arr.AddAtIndex(2, str5)
	s.Equal(5, arr.len)

	err := arr.AddAtIndex(-1, generateRandomString(10))
	s.Error(err)

	err = arr.AddAtIndex(10, generateRandomString(10))
	s.Error(err)
}

func (s *DynArrTestSuite) TestGetElement() {
	arr := NewDynArr(2)
	testStrings := []string{}

	for i := 0; i < 5; i++ {
		str := generateRandomString(10)
		testStrings = append(testStrings, str)
		arr.AddToEnd(str)
	}

	for i := 0; i < 5; i++ {
		element, err := arr.GetElement(i)
		s.NoError(err)
		s.Equal(testStrings[i], element)
	}

	_, err := arr.GetElement(-1)
	s.Error(err)

	_, err = arr.GetElement(5)
	s.Error(err)

	_, err = arr.GetElement(100)
	s.Error(err)
}

func (s *DynArrTestSuite) TestReplaceElement() {
	arr := NewDynArr(3)
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)
	new1 := generateRandomString(10)
	new2 := generateRandomString(10)
	new3 := generateRandomString(10)

	arr.AddToEnd(str1)
	arr.AddToEnd(str2)
	arr.AddToEnd(str3)

	arr.ReplaceElement(0, new1)
	element, _ := arr.GetElement(0)
	s.Equal(new1, element)

	arr.ReplaceElement(1, new2)
	element, _ = arr.GetElement(1)
	s.Equal(new2, element)

	arr.ReplaceElement(2, new3)
	element, _ = arr.GetElement(2)
	s.Equal(new3, element)

	err := arr.ReplaceElement(-1, generateRandomString(10))
	s.Error(err)

	err = arr.ReplaceElement(3, generateRandomString(10))
	s.Error(err)
}

func (s *DynArrTestSuite) TestRemoveAtIndex() {
	arr := NewDynArr(5)
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)
	str4 := generateRandomString(10)
	str5 := generateRandomString(10)

	arr.AddToEnd(str1)
	arr.AddToEnd(str2)
	arr.AddToEnd(str3)
	arr.AddToEnd(str4)
	arr.AddToEnd(str5)

	arr.RemoveAtIndex(2)
	s.Equal(4, arr.len)
	element, _ := arr.GetElement(0)
	s.Equal(str1, element)
	element, _ = arr.GetElement(1)
	s.Equal(str2, element)
	element, _ = arr.GetElement(2)
	s.Equal(str4, element)
	element, _ = arr.GetElement(3)
	s.Equal(str5, element)

	arr.RemoveAtIndex(3)
	s.Equal(3, arr.len)
	element, _ = arr.GetElement(0)
	s.Equal(str1, element)
	element, _ = arr.GetElement(1)
	s.Equal(str2, element)
	element, _ = arr.GetElement(2)
	s.Equal(str4, element)

	arr.RemoveAtIndex(0)
	s.Equal(2, arr.len)
	element, _ = arr.GetElement(0)
	s.Equal(str2, element)
	element, _ = arr.GetElement(1)
	s.Equal(str4, element)

	arr.RemoveAtIndex(0)
	s.Equal(1, arr.len)
	element, _ = arr.GetElement(0)
	s.Equal(str4, element)

	arr.RemoveAtIndex(0)
	s.Equal(0, arr.len)

	err := arr.RemoveAtIndex(-1)
	s.Error(err)

	err = arr.RemoveAtIndex(0)
	s.Error(err)
}

func (s *DynArrTestSuite) TestPrintArr() {
	arr := NewDynArr(2)
	s.NotPanics(arr.PrintArr)

	arr.AddToEnd(generateRandomString(10))
	arr.AddToEnd(generateRandomString(10))
	s.NotPanics(arr.PrintArr)
}

func (s *DynArrTestSuite) TestFileOperations() {
	filename := "test_dynarr.txt"
	arr1 := NewDynArr(2)
	arr2 := NewDynArr(2)

	testData := []string{}
	for i := 0; i < 5; i++ {
		str := generateRandomString(10)
		testData = append(testData, str)
		arr1.AddToEnd(str)
	}

	file, err := os.Create(filename)
	s.NoError(err)

	err = arr1.SaveToFile(file)
	s.NoError(err)
	file.Close()

	file, err = os.Open(filename)
	s.NoError(err)

	err = arr2.LoadFromFile(file)
	s.NoError(err)
	file.Close()

	s.Equal(arr1.GetLength(), arr2.GetLength())
	for i := 0; i < arr1.GetLength(); i++ {
		element1, _ := arr1.GetElement(i)
		element2, _ := arr2.GetElement(i)
		s.Equal(element1, element2)
	}

	os.Remove(filename)
}

func (s *DynArrTestSuite) TestBinaryFileOperations() {
	filename := "test_dynarr_binary.bin"
	arr1 := NewDynArr(2)
	arr2 := NewDynArr(2)

	testData := []string{}
	for i := 0; i < 5; i++ {
		str := generateRandomString(10)
		testData = append(testData, str)
		arr1.AddToEnd(str)
	}

	file, err := os.Create(filename)
	s.NoError(err)

	err = arr1.SaveToBinaryFile(file)
	s.NoError(err)
	file.Close()

	file, err = os.Open(filename)
	s.NoError(err)

	err = arr2.LoadFromBinaryFile(file)
	s.NoError(err)
	file.Close()

	s.Equal(arr1.GetLength(), arr2.GetLength())
	for i := 0; i < arr1.GetLength(); i++ {
		element1, _ := arr1.GetElement(i)
		element2, _ := arr2.GetElement(i)
		s.Equal(element1, element2)
	}

	os.Remove(filename)
}

func (s *DynArrTestSuite) TestExceptionSafety() {
	arr := NewDynArr(2)

	_, err := arr.GetElement(0)
	s.Error(err)

	err = arr.ReplaceElement(0, generateRandomString(10))
	s.Error(err)

	err = arr.RemoveAtIndex(0)
	s.Error(err)

	testStr := generateRandomString(10)
	arr.AddToEnd(testStr)

	_, err = arr.GetElement(-1)
	s.Error(err)

	_, err = arr.GetElement(1)
	s.Error(err)

	err = arr.ReplaceElement(-1, generateRandomString(10))
	s.Error(err)

	err = arr.ReplaceElement(1, generateRandomString(10))
	s.Error(err)

	err = arr.RemoveAtIndex(-1)
	s.Error(err)

	err = arr.RemoveAtIndex(1)
	s.Error(err)

	err = arr.AddAtIndex(-1, generateRandomString(10))
	s.Error(err)

	err = arr.AddAtIndex(2, generateRandomString(10))
	s.Error(err)
}

func (s *DynArrTestSuite) TestNilFile() {
	arr := NewDynArr(2)
	arr.AddToEnd("test")

	err := arr.SaveToFile(nil)
	s.Error(err)

	err = arr.LoadFromFile(nil)
	s.Error(err)

	err = arr.SaveToBinaryFile(nil)
	s.Error(err)

	err = arr.LoadFromBinaryFile(nil)
	s.Error(err)
}

package onelist

import (
	"crypto/rand"
	"math/big"
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"github.com/stretchr/testify/suite"
)

const alphanum = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

func generateRandomString(length int) string {
	b := make([]byte, length)
	for i := range b {
		n, _ := rand.Int(rand.Reader, big.NewInt(int64(len(alphanum))))
		b[i] = alphanum[n.Int64()]
	}
	return string(b)
}

type OneListTestSuite struct {
	suite.Suite
}

func TestOneListTestSuite(t *testing.T) {
	suite.Run(t, new(OneListTestSuite))
}

func (s *OneListTestSuite) TestConstructorDestructor() {
	list1 := NewOneList()
	assert.NotNil(s.T(), list1)
	assert.NotPanics(s.T(), func() {
		list1.Print()
	})

	list2 := NewOneList()
	assert.NotNil(s.T(), list2)
	assert.NotPanics(s.T(), func() {
		list2.Print()
	})
}

func (s *OneListTestSuite) TestAddToHead() {
	list := NewOneList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	assert.NotPanics(s.T(), func() {
		list.AddToHead(str1)
	})
	assert.NotPanics(s.T(), func() {
		list.AddToHead(str2)
	})
	assert.NotPanics(s.T(), func() {
		list.AddToHead(str3)
	})
	assert.NotPanics(s.T(), func() {
		list.Print()
	})
}

func (s *OneListTestSuite) TestAddToTail() {
	list := NewOneList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	assert.NotPanics(s.T(), func() {
		list.AddToTail(str1)
	})
	val, err := list.GetElementByValue(str1)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str1, val)

	assert.NotPanics(s.T(), func() {
		list.AddToTail(str2)
	})
	val, err = list.GetElementByValue(str2)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str2, val)

	assert.NotPanics(s.T(), func() {
		list.AddToTail(str3)
	})
	val, err = list.GetElementByValue(str3)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str3, val)

	assert.NotPanics(s.T(), func() {
		list.Print()
	})
}

func (s *OneListTestSuite) TestAddAfterValue() {
	list := NewOneList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)
	str4 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)

	err := list.AddAfterValue(str2, str4)
	assert.NoError(s.T(), err)
	val, err := list.GetElementByValue(str4)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str4, val)

	str5 := generateRandomString(10)
	err = list.AddAfterValue(str3, str5)
	assert.NoError(s.T(), err)
	val, err = list.GetElementByValue(str5)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str5, val)

	assert.NotPanics(s.T(), func() {
		list.Print()
	})

	err = list.AddAfterValue("stop", generateRandomString(10))
	assert.Error(s.T(), err)
}

func (s *OneListTestSuite) TestAddBeforeValue() {
	list := NewOneList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)
	str4 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)

	err := list.AddBeforeValue(str2, str4)
	assert.NoError(s.T(), err)
	val, err := list.GetElementByValue(str4)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str4, val)

	str5 := generateRandomString(10)
	err = list.AddBeforeValue(str1, str5)
	assert.NoError(s.T(), err)
	val, err = list.GetElementByValue(str5)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str5, val)

	assert.NotPanics(s.T(), func() {
		list.Print()
	})

	err = list.AddBeforeValue("stop", generateRandomString(10))
	assert.Error(s.T(), err)
}

func (s *OneListTestSuite) TestRemoveFromHead() {
	list := NewOneList()
	err := list.RemoveFromHead()
	assert.Error(s.T(), err)

	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)

	err = list.RemoveFromHead()
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str1)
	assert.Error(s.T(), err)
	val, err := list.GetElementByValue(str2)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str2, val)

	err = list.RemoveFromHead()
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str2)
	assert.Error(s.T(), err)
	val, err = list.GetElementByValue(str3)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str3, val)

	err = list.RemoveFromHead()
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str3)
	assert.Error(s.T(), err)

	err = list.RemoveFromHead()
	assert.Error(s.T(), err)
}

func (s *OneListTestSuite) TestRemoveFromTail() {
	list := NewOneList()
	err := list.RemoveFromTail()
	assert.Error(s.T(), err)

	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)

	err = list.RemoveFromTail()
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str3)
	assert.Error(s.T(), err)
	val, err := list.GetElementByValue(str2)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str2, val)

	err = list.RemoveFromTail()
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str2)
	assert.Error(s.T(), err)
	val, err = list.GetElementByValue(str1)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str1, val)

	err = list.RemoveFromTail()
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str1)
	assert.Error(s.T(), err)

	err = list.RemoveFromTail()
	assert.Error(s.T(), err)
}

func (s *OneListTestSuite) TestRemoveAfterValue() {
	list := NewOneList()
	err := list.RemoveAfterValue("danil")
	assert.Error(s.T(), err)

	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)

	err = list.RemoveAfterValue(str1)
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str2)
	assert.Error(s.T(), err)
	val, err := list.GetElementByValue(str3)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str3, val)

	err = list.RemoveAfterValue(str3)
	assert.Error(s.T(), err)

	err = list.RemoveAfterValue("stop")
	assert.Error(s.T(), err)

	str4 := generateRandomString(10)
	str5 := generateRandomString(10)
	list.AddAfterValue(str1, str4)
	list.AddAfterValue(str4, str5)

	err = list.RemoveAfterValue(str4)
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str5)
	assert.Error(s.T(), err)
}

func (s *OneListTestSuite) TestRemoveBeforeValue() {
	list := NewOneList()
	err := list.RemoveBeforeValue("danil")
	assert.Error(s.T(), err)

	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)
	str4 := generateRandomString(10)
	str5 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)
	list.AddToTail(str4)
	list.AddToTail(str5)

	err = list.RemoveBeforeValue(str1)
	assert.Error(s.T(), err)

	err = list.RemoveBeforeValue(str2)
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str1)
	assert.Error(s.T(), err)
	val, err := list.GetElementByValue(str2)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str2, val)

	err = list.RemoveBeforeValue(str3)
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str2)
	assert.Error(s.T(), err)
	val, err = list.GetElementByValue(str3)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str3, val)

	err = list.RemoveBeforeValue(str5)
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str4)
	assert.Error(s.T(), err)
	val, err = list.GetElementByValue(str3)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str3, val)
	val, err = list.GetElementByValue(str5)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str5, val)

	err = list.RemoveBeforeValue("stop")
	assert.Error(s.T(), err)
}

func (s *OneListTestSuite) TestRemoveByValue() {
	list := NewOneList()
	err := list.RemoveByValue("danil")
	assert.Error(s.T(), err)

	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)
	str4 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)
	list.AddToTail(str4)

	err = list.RemoveByValue(str1)
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str1)
	assert.Error(s.T(), err)
	val, err := list.GetElementByValue(str2)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str2, val)

	err = list.RemoveByValue(str4)
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str4)
	assert.Error(s.T(), err)
	val, err = list.GetElementByValue(str3)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str3, val)

	err = list.RemoveByValue(str2)
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str2)
	assert.Error(s.T(), err)
	val, err = list.GetElementByValue(str3)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str3, val)

	err = list.RemoveByValue(str3)
	assert.NoError(s.T(), err)
	_, err = list.GetElementByValue(str3)
	assert.Error(s.T(), err)

	err = list.RemoveByValue("stop")
	assert.Error(s.T(), err)
}

func (s *OneListTestSuite) TestGetElementByValue() {
	list := NewOneList()
	_, err := list.GetElementByValue("danil")
	assert.Error(s.T(), err)

	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)

	val, err := list.GetElementByValue(str1)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str1, val)

	val, err = list.GetElementByValue(str2)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str2, val)

	val, err = list.GetElementByValue(str3)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str3, val)

	_, err = list.GetElementByValue("stop")
	assert.Error(s.T(), err)
}

func (s *OneListTestSuite) TestPrintEmpty() {
	list := NewOneList()
	assert.NotPanics(s.T(), func() {
		list.Print()
	})
}

func (s *OneListTestSuite) TestFileOperations() {
	filename := "test_onelist.txt"
	list1 := NewOneList()
	list2 := NewOneList()

	for i := 0; i < 5; i++ {
		str := generateRandomString(10)
		list1.AddToTail(str)
	}

	outFile, err := os.Create(filename)
	require.NoError(s.T(), err)
	err = list1.SaveToFile(outFile)
	assert.NoError(s.T(), err)
	outFile.Close()

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	err = list2.LoadFromFile(inFile)
	assert.NoError(s.T(), err)
	inFile.Close()

	assert.NotPanics(s.T(), func() {
		list1.Print()
	})
	assert.NotPanics(s.T(), func() {
		list2.Print()
	})

	os.Remove(filename)
}

func (s *OneListTestSuite) TestBinaryFileOperations() {
	filename := "test_onelist_binary.bin"
	list1 := NewOneList()
	list2 := NewOneList()

	testStrings := make([]string, 5)
	for i := 0; i < 5; i++ {
		str := generateRandomString(10)
		testStrings[i] = str
		list1.AddToTail(str)
	}

	outFile, err := os.Create(filename)
	require.NoError(s.T(), err)
	err = list1.SaveToBinaryFile(outFile)
	assert.NoError(s.T(), err)
	outFile.Close()

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	err = list2.LoadFromBinaryFile(inFile)
	assert.NoError(s.T(), err)
	inFile.Close()

	assert.NotPanics(s.T(), func() {
		list1.Print()
	})
	assert.NotPanics(s.T(), func() {
		list2.Print()
	})

	for _, str := range testStrings {
		val, err := list2.GetElementByValue(str)
		assert.NoError(s.T(), err)
		assert.Equal(s.T(), str, val)
	}

	os.Remove(filename)
}

func (s *OneListTestSuite) TestFileOperationsErrors() {
	list := NewOneList()

	err := list.SaveToFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	err = list.LoadFromFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	err = list.SaveToBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	err = list.LoadFromBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	filename := "test_nonexistent.txt"
	file, err := os.Open(filename)
	require.Error(s.T(), err)
	if file != nil {
		err = list.LoadFromFile(file)
		assert.Error(s.T(), err)
		file.Close()
	}

	filename = "test_corrupted.bin"
	corruptedData := []byte{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	err = os.WriteFile(filename, corruptedData, 0644)
	require.NoError(s.T(), err)

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	err = list.LoadFromBinaryFile(inFile)
	assert.Error(s.T(), err)
	inFile.Close()

	os.Remove(filename)
}

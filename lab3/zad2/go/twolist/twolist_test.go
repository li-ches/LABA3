package twolist

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

type TwoListTestSuite struct {
	suite.Suite
}

func TestTwoListTestSuite(t *testing.T) {
	suite.Run(t, new(TwoListTestSuite))
}

func (s *TwoListTestSuite) TestConstructorDestructor() {
	list1 := NewTwoList()
	assert.NotNil(s.T(), list1)
	assert.NotPanics(s.T(), func() {
		list1.Print()
	})

	list2 := NewTwoList()
	assert.NotNil(s.T(), list2)
}

func (s *TwoListTestSuite) TestAddToHead() {
	list := NewTwoList()
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

func (s *TwoListTestSuite) TestAddToTail() {
	list := NewTwoList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	assert.NotPanics(s.T(), func() {
		list.AddToTail(str1)
	})
	assert.NotPanics(s.T(), func() {
		list.AddToTail(str2)
	})
	assert.NotPanics(s.T(), func() {
		list.AddToTail(str3)
	})

	assert.NotPanics(s.T(), func() {
		list.Print()
	})
}

func (s *TwoListTestSuite) TestAddAfterValue() {
	list := NewTwoList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)
	str4 := generateRandomString(10)
	str5 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)

	err := list.AddAfterValue(str2, str4)
	assert.NoError(s.T(), err)
	assert.NotPanics(s.T(), func() {
		list.Print()
	})

	err = list.AddAfterValue(str3, str5)
	assert.NoError(s.T(), err)
	assert.NotPanics(s.T(), func() {
		list.Print()
	})
}

func (s *TwoListTestSuite) TestAddBeforeValue() {
	list := NewTwoList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)
	str4 := generateRandomString(10)
	str5 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)

	err := list.AddBeforeValue(str2, str4)
	assert.NoError(s.T(), err)
	assert.NotPanics(s.T(), func() {
		list.Print()
	})

	err = list.AddBeforeValue(str1, str5)
	assert.NoError(s.T(), err)
	assert.NotPanics(s.T(), func() {
		list.Print()
	})
}

func (s *TwoListTestSuite) TestRemoveFromHead() {
	list := NewTwoList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)

	err := list.RemoveFromHead()
	assert.NoError(s.T(), err)
	assert.NotPanics(s.T(), func() {
		list.Print()
	})

	err = list.RemoveFromHead()
	assert.NoError(s.T(), err)

	err = list.RemoveFromHead()
	assert.Error(s.T(), err)
}

func (s *TwoListTestSuite) TestRemoveFromTail() {
	list := NewTwoList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)

	err := list.RemoveFromTail()
	assert.NoError(s.T(), err)
	assert.NotPanics(s.T(), func() {
		list.Print()
	})

	err = list.RemoveFromTail()
	assert.NoError(s.T(), err)

	err = list.RemoveFromTail()
	assert.Error(s.T(), err)
}

func (s *TwoListTestSuite) TestRemoveAfterValue() {
	list := NewTwoList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)

	err := list.RemoveAfterValue(str1)
	assert.NoError(s.T(), err)
	assert.NotPanics(s.T(), func() {
		list.Print()
	})

	err = list.RemoveAfterValue(str3)
	assert.Error(s.T(), err)
}

func (s *TwoListTestSuite) TestRemoveBeforeValue() {
	list := NewTwoList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)
	str4 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)
	list.AddToTail(str4)

	err := list.RemoveBeforeValue(str2)
	assert.NoError(s.T(), err)
	assert.NotPanics(s.T(), func() {
		list.Print()
	})

	err = list.RemoveBeforeValue(str4)
	assert.NoError(s.T(), err)
	assert.NotPanics(s.T(), func() {
		list.Print()
	})

	err = list.RemoveBeforeValue(str1)
	assert.Error(s.T(), err)
}

func (s *TwoListTestSuite) TestRemoveByValue() {
	list := NewTwoList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)

	err := list.RemoveByValue(str2)
	assert.NoError(s.T(), err)
	assert.NotPanics(s.T(), func() {
		list.Print()
	})

	err = list.RemoveByValue(str1)
	assert.NoError(s.T(), err)
	assert.NotPanics(s.T(), func() {
		list.Print()
	})

	err = list.RemoveByValue(str3)
	assert.NoError(s.T(), err)
	assert.NotPanics(s.T(), func() {
		list.Print()
	})
}

func (s *TwoListTestSuite) TestGetElementByValue() {
	list := NewTwoList()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	list.AddToTail(str1)
	list.AddToTail(str2)
	list.AddToTail(str3)

	val, err := list.GetElementByValue(str2)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str2, val)

	val, err = list.GetElementByValue(str1)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str1, val)

	val, err = list.GetElementByValue(str3)
	assert.NoError(s.T(), err)
	assert.Equal(s.T(), str3, val)
}

func (s *TwoListTestSuite) TestPrintEmpty() {
	list := NewTwoList()
	assert.NotPanics(s.T(), func() {
		list.Print()
	})
}

func (s *TwoListTestSuite) TestFileOperations() {
	filename := "test_twolist.txt"
	list1 := NewTwoList()
	list2 := NewTwoList()

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

func (s *TwoListTestSuite) TestBinaryFileOperations() {
	filename := "test_twolist_binary.bin"
	list1 := NewTwoList()
	list2 := NewTwoList()

	for i := 0; i < 5; i++ {
		str := generateRandomString(10)
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

	os.Remove(filename)
}

func (s *TwoListTestSuite) TestFileOperationsErrors() {
	list := NewTwoList()

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

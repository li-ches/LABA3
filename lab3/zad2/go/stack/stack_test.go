package stack

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

type StackTestSuite struct {
	suite.Suite
}

func TestStackTestSuite(t *testing.T) {
	suite.Run(t, new(StackTestSuite))
}

func (s *StackTestSuite) TestConstructorDestructor() {
	stack1 := NewStack()
	assert.NotNil(s.T(), stack1)
	assert.NotPanics(s.T(), func() {
		stack1.Print()
	})

	stack2 := NewStack()
	assert.NotNil(s.T(), stack2)
}

func (s *StackTestSuite) TestPushPop() {
	stack := NewStack()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	assert.NotPanics(s.T(), func() {
		stack.Push(str1)
	})
	assert.NotPanics(s.T(), func() {
		stack.Push(str2)
	})
	assert.NotPanics(s.T(), func() {
		stack.Push(str3)
	})

	assert.NotPanics(s.T(), func() {
		stack.Print()
	})

	err := stack.Pop()
	assert.NoError(s.T(), err)
	err = stack.Pop()
	assert.NoError(s.T(), err)
	err = stack.Pop()
	assert.NoError(s.T(), err)

	err = stack.Pop()
	assert.Error(s.T(), err)
}

func (s *StackTestSuite) TestPrintEmpty() {
	stack := NewStack()
	assert.NotPanics(s.T(), func() {
		stack.Print()
	})
}

func (s *StackTestSuite) TestFileOperations() {
	filename := "test_stack.txt"
	stack1 := NewStack()
	stack2 := NewStack()

	testData := make([]string, 5)
	for i := 0; i < 5; i++ {
		str := generateRandomString(10)
		testData[i] = str
		stack1.Push(str)
	}

	outFile, err := os.Create(filename)
	require.NoError(s.T(), err)
	err = stack1.SaveToFile(outFile)
	assert.NoError(s.T(), err)
	outFile.Close()

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	err = stack2.LoadFromFile(inFile)
	assert.NoError(s.T(), err)
	inFile.Close()

	assert.NotPanics(s.T(), func() {
		stack1.Print()
	})
	assert.NotPanics(s.T(), func() {
		stack2.Print()
	})

	os.Remove(filename)
}

func (s *StackTestSuite) TestBinaryFileOperations() {
	filename := "test_stack_binary.bin"
	stack1 := NewStack()
	stack2 := NewStack()

	testData := make([]string, 5)
	for i := 0; i < 5; i++ {
		str := generateRandomString(10)
		testData[i] = str
		stack1.Push(str)
	}

	outFile, err := os.Create(filename)
	require.NoError(s.T(), err)
	err = stack1.SaveToBinaryFile(outFile)
	assert.NoError(s.T(), err)
	outFile.Close()

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	err = stack2.LoadFromBinaryFile(inFile)
	assert.NoError(s.T(), err)
	inFile.Close()

	assert.NotPanics(s.T(), func() {
		stack1.Print()
	})
	assert.NotPanics(s.T(), func() {
		stack2.Print()
	})

	os.Remove(filename)
}

func (s *StackTestSuite) TestExceptionSafety() {
	stack := NewStack()

	err := stack.Pop()
	assert.Error(s.T(), err)

	testStr := generateRandomString(10)
	stack.Push(testStr)
	err = stack.Pop()
	assert.NoError(s.T(), err)
	err = stack.Pop()
	assert.Error(s.T(), err)
}

func (s *StackTestSuite) TestFileOperationsErrors() {
	stack := NewStack()

	err := stack.SaveToFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	err = stack.LoadFromFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	err = stack.SaveToBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	err = stack.LoadFromBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	filename := "test_nonexistent.txt"
	file, err := os.Open(filename)
	require.Error(s.T(), err)
	if file != nil {
		err = stack.LoadFromFile(file)
		assert.Error(s.T(), err)
		file.Close()
	}

	filename = "test_corrupted.bin"
	corruptedData := []byte{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	err = os.WriteFile(filename, corruptedData, 0644)
	require.NoError(s.T(), err)

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	err = stack.LoadFromBinaryFile(inFile)
	assert.Error(s.T(), err)
	inFile.Close()

	os.Remove(filename)
}

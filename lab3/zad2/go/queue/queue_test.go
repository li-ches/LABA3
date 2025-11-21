package queue

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

type QueueTestSuite struct {
	suite.Suite
}

func TestQueueTestSuite(t *testing.T) {
	suite.Run(t, new(QueueTestSuite))
}

func (s *QueueTestSuite) TestConstructorDestructor() {
	queue1 := NewQueue()
	assert.NotNil(s.T(), queue1)
	assert.NotPanics(s.T(), func() {
		queue1.Print()
	})

	queue2 := NewQueue()
	assert.NotNil(s.T(), queue2)
	assert.NotPanics(s.T(), func() {
		queue2.Print()
	})
}

func (s *QueueTestSuite) TestEnqueueDequeue() {
	queue := NewQueue()
	str1 := generateRandomString(10)
	str2 := generateRandomString(10)
	str3 := generateRandomString(10)

	assert.NotPanics(s.T(), func() {
		queue.Enqueue(str1)
	})
	assert.NotPanics(s.T(), func() {
		queue.Enqueue(str2)
	})
	assert.NotPanics(s.T(), func() {
		queue.Enqueue(str3)
	})
	assert.NotPanics(s.T(), func() {
		queue.Print()
	})

	err := queue.Dequeue()
	assert.NoError(s.T(), err)
	err = queue.Dequeue()
	assert.NoError(s.T(), err)
	err = queue.Dequeue()
	assert.NoError(s.T(), err)
	err = queue.Dequeue()
	assert.Error(s.T(), err)
}

func (s *QueueTestSuite) TestPrintEmpty() {
	queue := NewQueue()
	assert.NotPanics(s.T(), func() {
		queue.Print()
	})
}

func (s *QueueTestSuite) TestFileOperations() {
	filename := "test_queue.txt"
	queue1 := NewQueue()
	queue2 := NewQueue()

	for i := 0; i < 5; i++ {
		str := generateRandomString(10)
		queue1.Enqueue(str)
	}

	outFile, err := os.Create(filename)
	require.NoError(s.T(), err)
	err = queue1.SaveToFile(outFile)
	assert.NoError(s.T(), err)
	outFile.Close()

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	err = queue2.LoadFromFile(inFile)
	assert.NoError(s.T(), err)
	inFile.Close()

	assert.NotPanics(s.T(), func() {
		queue1.Print()
	})
	assert.NotPanics(s.T(), func() {
		queue2.Print()
	})

	os.Remove(filename)
}

func (s *QueueTestSuite) TestBinaryFileOperations() {
	filename := "test_queue_binary.bin"
	queue1 := NewQueue()
	queue2 := NewQueue()

	testStrings := make([]string, 5)
	for i := 0; i < 5; i++ {
		str := generateRandomString(10)
		testStrings[i] = str
		queue1.Enqueue(str)
	}

	outFile, err := os.Create(filename)
	require.NoError(s.T(), err)
	err = queue1.SaveToBinaryFile(outFile)
	assert.NoError(s.T(), err)
	outFile.Close()

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	err = queue2.LoadFromBinaryFile(inFile)
	assert.NoError(s.T(), err)
	inFile.Close()

	assert.NotPanics(s.T(), func() {
		queue1.Print()
	})
	assert.NotPanics(s.T(), func() {
		queue2.Print()
	})

	for range testStrings {
		err := queue2.Dequeue()
		assert.NoError(s.T(), err)
	}

	err = queue2.Dequeue()
	assert.Error(s.T(), err)

	os.Remove(filename)
}

func (s *QueueTestSuite) TestFileOperationsErrors() {
	queue := NewQueue()

	err := queue.SaveToFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	err = queue.LoadFromFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	err = queue.SaveToBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	err = queue.LoadFromBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	filename := "test_nonexistent.txt"
	file, err := os.Open(filename)
	require.Error(s.T(), err)
	if file != nil {
		err = queue.LoadFromFile(file)
		assert.Error(s.T(), err)
		file.Close()
	}

	filename = "test_corrupted.bin"
	corruptedData := []byte{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	err = os.WriteFile(filename, corruptedData, 0644)
	require.NoError(s.T(), err)

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	err = queue.LoadFromBinaryFile(inFile)
	assert.Error(s.T(), err)
	inFile.Close()

	os.Remove(filename)
}

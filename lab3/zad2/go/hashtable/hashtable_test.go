package hashtable

import (
	"crypto/rand"
	"fmt"
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

func generateRandomKeyValuePairs(count int) []struct {
	key   string
	value string
} {
	pairs := make([]struct {
		key   string
		value string
	}, count)
	for i := 0; i < count; i++ {
		pairs[i] = struct {
			key   string
			value string
		}{
			key:   generateRandomString(8),
			value: generateRandomString(15),
		}
	}
	return pairs
}

type HashTableTestSuite struct {
	suite.Suite
}

func TestHashTableTestSuite(t *testing.T) {
	suite.Run(t, new(HashTableTestSuite))
}

func (s *HashTableTestSuite) TestCuckooConstructorDestructor() {
	table1 := NewCuckooHashTable(0)
	assert.NotNil(s.T(), table1)
	assert.NotPanics(s.T(), func() {
		table1.Print()
	})

	table2 := NewCuckooHashTable(20)
	assert.NotNil(s.T(), table2)
	assert.NotPanics(s.T(), func() {
		table2.Print()
	})
}

func (s *HashTableTestSuite) TestDoubleConstructorDestructor() {
	table1 := NewDoubleHashTable(0)
	assert.NotNil(s.T(), table1)
	assert.NotPanics(s.T(), func() {
		table1.Print()
	})

	table2 := NewDoubleHashTable(20)
	assert.NotNil(s.T(), table2)
	assert.NotPanics(s.T(), func() {
		table2.Print()
	})
}

func (s *HashTableTestSuite) TestCuckooInsertSearch() {
	table := NewCuckooHashTable(0)
	pairs := generateRandomKeyValuePairs(5)

	for _, pair := range pairs {
		data := pair.value
		assert.True(s.T(), table.Insert(pair.key, data))
		assert.Equal(s.T(), pair.value, table.Search(pair.key))
	}

	assert.NotPanics(s.T(), func() {
		table.Print()
	})
}

func (s *HashTableTestSuite) TestDoubleInsertSearch() {
	table := NewDoubleHashTable(0)
	pairs := generateRandomKeyValuePairs(5)

	for _, pair := range pairs {
		data := pair.value
		assert.True(s.T(), table.Insert(pair.key, data))
		assert.Equal(s.T(), pair.value, table.Search(pair.key))
	}

	assert.NotPanics(s.T(), func() {
		table.Print()
	})
}

func (s *HashTableTestSuite) TestCuckooInsertDuplicate() {
	table := NewCuckooHashTable(0)
	key := generateRandomString(10)
	value1 := generateRandomString(15)
	value2 := generateRandomString(15)

	assert.True(s.T(), table.Insert(key, value1))
	assert.Equal(s.T(), value1, table.Search(key))

	assert.True(s.T(), table.Insert(key, value2))
	assert.Equal(s.T(), value2, table.Search(key))
}

func (s *HashTableTestSuite) TestDoubleInsertDuplicate() {
	table := NewDoubleHashTable(0)
	key := generateRandomString(10)
	value1 := generateRandomString(15)
	value2 := generateRandomString(15)

	assert.True(s.T(), table.Insert(key, value1))
	assert.Equal(s.T(), value1, table.Search(key))

	assert.True(s.T(), table.Insert(key, value2))
	assert.Equal(s.T(), value2, table.Search(key))
}

func (s *HashTableTestSuite) TestCuckooRemove() {
	table := NewCuckooHashTable(0)
	pairs := generateRandomKeyValuePairs(3)

	for _, pair := range pairs {
		data := pair.value
		table.Insert(pair.key, data)
	}

	assert.True(s.T(), table.Remove(pairs[0].key))
	assert.Equal(s.T(), "", table.Search(pairs[0].key))

	assert.True(s.T(), table.Remove(pairs[1].key))
	assert.Equal(s.T(), "", table.Search(pairs[1].key))

	assert.Equal(s.T(), pairs[2].value, table.Search(pairs[2].key))

	assert.NotPanics(s.T(), func() {
		table.Print()
	})
}

func (s *HashTableTestSuite) TestDoubleRemove() {
	table := NewDoubleHashTable(0)
	pairs := generateRandomKeyValuePairs(3)

	for _, pair := range pairs {
		data := pair.value
		table.Insert(pair.key, data)
	}

	assert.True(s.T(), table.Remove(pairs[0].key))
	assert.Equal(s.T(), "", table.Search(pairs[0].key))

	assert.True(s.T(), table.Remove(pairs[1].key))
	assert.Equal(s.T(), "", table.Search(pairs[1].key))

	assert.Equal(s.T(), pairs[2].value, table.Search(pairs[2].key))

	assert.NotPanics(s.T(), func() {
		table.Print()
	})
}

func (s *HashTableTestSuite) TestCuckooRehash() {
	table := NewCuckooHashTable(5)
	pairs := generateRandomKeyValuePairs(10)

	for _, pair := range pairs {
		data := pair.value
		assert.True(s.T(), table.Insert(pair.key, data))
	}

	for _, pair := range pairs {
		assert.Equal(s.T(), pair.value, table.Search(pair.key))
	}

	assert.NotPanics(s.T(), func() {
		table.Print()
	})
}

func (s *HashTableTestSuite) TestDoubleRehash() {
	table := NewDoubleHashTable(5)
	pairs := generateRandomKeyValuePairs(10)

	for _, pair := range pairs {
		data := pair.value
		assert.True(s.T(), table.Insert(pair.key, data))
	}

	for _, pair := range pairs {
		assert.Equal(s.T(), pair.value, table.Search(pair.key))
	}

	assert.NotPanics(s.T(), func() {
		table.Print()
	})
}

func (s *HashTableTestSuite) TestCuckooGetAllElements() {
	table := NewCuckooHashTable(0)
	pairs := generateRandomKeyValuePairs(5)

	for _, pair := range pairs {
		data := pair.value
		table.Insert(pair.key, data)
	}

	elements := table.GetAllElements()
	assert.Equal(s.T(), len(pairs), len(elements))

	for _, pair := range pairs {
		found := false
		for _, elem := range elements {
			if elem.Key == pair.key && elem.Data == pair.value {
				found = true
				break
			}
		}
		assert.True(s.T(), found, "Element %s:%s not found", pair.key, pair.value)
	}
}

func (s *HashTableTestSuite) TestDoubleGetAllElements() {
	table := NewDoubleHashTable(0)
	pairs := generateRandomKeyValuePairs(5)

	for _, pair := range pairs {
		data := pair.value
		table.Insert(pair.key, data)
	}

	elements := table.GetAllElements()
	assert.Equal(s.T(), len(pairs), len(elements))

	for _, pair := range pairs {
		found := false
		for _, elem := range elements {
			if elem.Key == pair.key && elem.Data == pair.value {
				found = true
				break
			}
		}
		assert.True(s.T(), found, "Element %s:%s not found", pair.key, pair.value)
	}
}

func (s *HashTableTestSuite) TestCuckooFileOperations() {
	filename := "test_cuckoo.txt"
	table1 := NewCuckooHashTable(0)
	table2 := NewCuckooHashTable(0)
	pairs := generateRandomKeyValuePairs(5)

	for _, pair := range pairs {
		data := pair.value
		table1.Insert(pair.key, data)
	}

	outFile, err := os.Create(filename)
	require.NoError(s.T(), err)
	require.NotNil(s.T(), outFile)

	err = table1.SaveToFile(outFile)
	assert.NoError(s.T(), err)
	outFile.Close()

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	require.NotNil(s.T(), inFile)

	err = table2.LoadFromFile(inFile)
	assert.NoError(s.T(), err)
	inFile.Close()

	for _, pair := range pairs {
		assert.Equal(s.T(), pair.value, table2.Search(pair.key))
	}

	err = os.Remove(filename)
	assert.NoError(s.T(), err)
}

func (s *HashTableTestSuite) TestDoubleFileOperations() {
	filename := "test_double.txt"
	table1 := NewDoubleHashTable(0)
	table2 := NewDoubleHashTable(0)
	pairs := generateRandomKeyValuePairs(5)

	for _, pair := range pairs {
		data := pair.value
		table1.Insert(pair.key, data)
	}

	outFile, err := os.Create(filename)
	require.NoError(s.T(), err)
	require.NotNil(s.T(), outFile)

	err = table1.SaveToFile(outFile)
	assert.NoError(s.T(), err)
	outFile.Close()

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	require.NotNil(s.T(), inFile)

	err = table2.LoadFromFile(inFile)
	assert.NoError(s.T(), err)
	inFile.Close()

	for _, pair := range pairs {
		assert.Equal(s.T(), pair.value, table2.Search(pair.key))
	}

	err = os.Remove(filename)
	assert.NoError(s.T(), err)
}

func (s *HashTableTestSuite) TestCuckooBinaryFileOperations() {
	filename := "test_cuckoo_binary.bin"
	table1 := NewCuckooHashTable(0)
	table2 := NewCuckooHashTable(0)
	pairs := generateRandomKeyValuePairs(5)

	for _, pair := range pairs {
		data := pair.value
		table1.Insert(pair.key, data)
	}

	outFile, err := os.Create(filename)
	require.NoError(s.T(), err)
	require.NotNil(s.T(), outFile)

	err = table1.SaveToBinaryFile(outFile)
	assert.NoError(s.T(), err)
	outFile.Close()

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	require.NotNil(s.T(), inFile)

	err = table2.LoadFromBinaryFile(inFile)
	assert.NoError(s.T(), err)
	inFile.Close()

	for _, pair := range pairs {
		assert.Equal(s.T(), pair.value, table2.Search(pair.key))
	}

	err = os.Remove(filename)
	assert.NoError(s.T(), err)
}

func (s *HashTableTestSuite) TestDoubleBinaryFileOperations() {
	filename := "test_double_binary.bin"
	table1 := NewDoubleHashTable(0)
	table2 := NewDoubleHashTable(0)
	pairs := generateRandomKeyValuePairs(5)

	for _, pair := range pairs {
		data := pair.value
		table1.Insert(pair.key, data)
	}

	outFile, err := os.Create(filename)
	require.NoError(s.T(), err)
	require.NotNil(s.T(), outFile)

	err = table1.SaveToBinaryFile(outFile)
	assert.NoError(s.T(), err)
	outFile.Close()

	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	require.NotNil(s.T(), inFile)

	err = table2.LoadFromBinaryFile(inFile)
	assert.NoError(s.T(), err)
	inFile.Close()

	for _, pair := range pairs {
		assert.Equal(s.T(), pair.value, table2.Search(pair.key))
	}

	err = os.Remove(filename)
	assert.NoError(s.T(), err)
}

func (s *HashTableTestSuite) TestCuckooLargeData() {
	table := NewCuckooHashTable(0)
	pairs := generateRandomKeyValuePairs(50)

	for _, pair := range pairs {
		data := pair.value
		assert.True(s.T(), table.Insert(pair.key, data))
	}

	for _, pair := range pairs {
		assert.Equal(s.T(), pair.value, table.Search(pair.key))
	}

	for i := 0; i < len(pairs)/2; i++ {
		assert.True(s.T(), table.Remove(pairs[i].key))
	}

	for i := 0; i < len(pairs)/2; i++ {
		assert.Equal(s.T(), "", table.Search(pairs[i].key))
	}

	for i := len(pairs) / 2; i < len(pairs); i++ {
		assert.Equal(s.T(), pairs[i].value, table.Search(pairs[i].key))
	}
}

func (s *HashTableTestSuite) TestDoubleLargeData() {
	table := NewDoubleHashTable(0)
	pairs := generateRandomKeyValuePairs(50)

	for _, pair := range pairs {
		data := pair.value
		assert.True(s.T(), table.Insert(pair.key, data))
	}

	for _, pair := range pairs {
		assert.Equal(s.T(), pair.value, table.Search(pair.key))
	}

	for i := 0; i < len(pairs)/2; i++ {
		assert.True(s.T(), table.Remove(pairs[i].key))
	}

	for i := 0; i < len(pairs)/2; i++ {
		assert.Equal(s.T(), "", table.Search(pairs[i].key))
	}

	for i := len(pairs) / 2; i < len(pairs); i++ {
		assert.Equal(s.T(), pairs[i].value, table.Search(pairs[i].key))
	}
}

func (s *HashTableTestSuite) TestCuckooEmptyKey() {
	table := NewCuckooHashTable(0)
	value := "value"

	assert.False(s.T(), table.Insert("", value))
	assert.Equal(s.T(), "", table.Search(""))
	assert.False(s.T(), table.Remove(""))
}

func (s *HashTableTestSuite) TestDoubleEmptyKey() {
	table := NewDoubleHashTable(0)
	value := "value"

	assert.False(s.T(), table.Insert("", value))
	assert.Equal(s.T(), "", table.Search(""))
	assert.False(s.T(), table.Remove(""))
}

func (s *HashTableTestSuite) TestBinaryFileOperationsNilFile() {
	cuckooTable := NewCuckooHashTable(5)
	doubleTable := NewDoubleHashTable(5)

	pairs := generateRandomKeyValuePairs(3)
	for _, pair := range pairs {
		cuckooTable.Insert(pair.key, pair.value)
		doubleTable.Insert(pair.key, pair.value)
	}

	err := cuckooTable.SaveToBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	err = doubleTable.SaveToBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	err = cuckooTable.LoadFromBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")

	err = doubleTable.LoadFromBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")
}

func (s *HashTableTestSuite) TestBinaryFileReadErrors() {
	filename := "test_corrupted.bin"

	corruptedData := []byte{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	err := os.WriteFile(filename, corruptedData, 0644)
	require.NoError(s.T(), err)

	table := NewCuckooHashTable(5)
	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)

	err = table.LoadFromBinaryFile(inFile)
	assert.Error(s.T(), err)
	inFile.Close()

	err = os.Remove(filename)
	assert.NoError(s.T(), err)
}

type ErrorWriterFile struct {
	failOnWrite bool
	failAfter   int
	writeCount  int
}

func (f *ErrorWriterFile) Write(p []byte) (n int, err error) {
	f.writeCount++
	if f.failOnWrite || (f.failAfter > 0 && f.writeCount >= f.failAfter) {
		return 0, fmt.Errorf("failed to write")
	}
	return len(p), nil
}

func (f *ErrorWriterFile) WriteString(s string) (n int, err error) {
	return f.Write([]byte(s))
}

func (f *ErrorWriterFile) Close() error {
	return nil
}

func (s *HashTableTestSuite) TestCuckooSaveToFileNil() {
	table := NewCuckooHashTable(5)
	err := table.SaveToFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")
}

func (s *HashTableTestSuite) TestCuckooLoadFromFileNil() {
	table := NewCuckooHashTable(5)
	err := table.LoadFromFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")
}

func (s *HashTableTestSuite) TestCuckooSaveToBinaryFileNil() {
	table := NewCuckooHashTable(5)
	err := table.SaveToBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")
}

func (s *HashTableTestSuite) TestCuckooLoadFromBinaryFileNil() {
	table := NewCuckooHashTable(5)
	err := table.LoadFromBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")
}

func (s *HashTableTestSuite) TestDoubleSaveToFileNil() {
	table := NewDoubleHashTable(5)
	err := table.SaveToFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")
}

func (s *HashTableTestSuite) TestDoubleLoadFromFileNil() {
	table := NewDoubleHashTable(5)
	err := table.LoadFromFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")
}

func (s *HashTableTestSuite) TestDoubleSaveToBinaryFileNil() {
	table := NewDoubleHashTable(5)
	err := table.SaveToBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")
}

func (s *HashTableTestSuite) TestDoubleLoadFromBinaryFileNil() {
	table := NewDoubleHashTable(5)
	err := table.LoadFromBinaryFile(nil)
	assert.Error(s.T(), err)
	assert.Contains(s.T(), err.Error(), "file is nil")
}

func (s *HashTableTestSuite) TestCuckooLoadFromFileReadError() {
	filename := "test_nonexistent.txt"
	table := NewCuckooHashTable(5)

	file, err := os.Open(filename)
	require.Error(s.T(), err)

	if file != nil {
		err = table.LoadFromFile(file)
		assert.Error(s.T(), err)
		file.Close()
	}
}

func (s *HashTableTestSuite) TestCuckooLoadFromBinaryFileReadErrors() {
	filename := "test_corrupted.bin"

	corruptedData := []byte{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	err := os.WriteFile(filename, corruptedData, 0644)
	require.NoError(s.T(), err)

	table := NewCuckooHashTable(5)
	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)

	err = table.LoadFromBinaryFile(inFile)
	assert.Error(s.T(), err)
	inFile.Close()

	os.Remove(filename)
}

func (s *HashTableTestSuite) TestCuckooMaxIterationsRehash() {

	table := NewCuckooHashTable(2)

	keys := []string{"abc", "bca", "cab", "acb", "bac", "cba"}
	values := []string{"v1", "v2", "v3", "v4", "v5", "v6"}

	for i := 0; i < 3; i++ {
		assert.True(s.T(), table.Insert(keys[i], values[i]))
	}

	for i := 3; i < len(keys); i++ {
		result := table.Insert(keys[i], values[i])

		fmt.Printf("Insert result for %s: %t\n", keys[i], result)
	}
	for i := 0; i < 3; i++ {
		assert.Equal(s.T(), values[i], table.Search(keys[i]))
	}
}

func (s *HashTableTestSuite) TestCuckooBinaryFileTable2Elements() {
	filename := "test_table2.bin"
	table1 := NewCuckooHashTable(3)

	keys := []string{"key1", "key2", "key3", "key4", "key5"}
	for i, key := range keys {
		table1.Insert(key, fmt.Sprintf("value%d", i+1))
	}

	outFile, err := os.Create(filename)
	require.NoError(s.T(), err)
	err = table1.SaveToBinaryFile(outFile)
	assert.NoError(s.T(), err)
	outFile.Close()

	table2 := NewCuckooHashTable(3)
	inFile, err := os.Open(filename)
	require.NoError(s.T(), err)
	err = table2.LoadFromBinaryFile(inFile)
	assert.NoError(s.T(), err)
	inFile.Close()

	for i, key := range keys {
		assert.Equal(s.T(), fmt.Sprintf("value%d", i+1), table2.Search(key))
	}

	os.Remove(filename)
}

func (s *HashTableTestSuite) TestCuckooUpdateTable2Element() {
	table := NewCuckooHashTable(5)

	key := "testkey"
	value1 := "value1"
	value2 := "value2"

	table.Insert(key, value1)

	originalPos1 := table.hash1(key)
	originalPos2 := table.hash2(key)

	table.Insert(key, value2)

	if !table.table1[originalPos1].IsEmpty && table.table1[originalPos1].Key == key {
		assert.Equal(s.T(), value2, table.table1[originalPos1].Data)
	}
	if !table.table2[originalPos2].IsEmpty && table.table2[originalPos2].Key == key {
		assert.Equal(s.T(), value2, table.table2[originalPos2].Data)
	}

	assert.Equal(s.T(), value2, table.Search(key))
}

func (s *HashTableTestSuite) TestCuckooRehashInsertFailure() {
	table := NewCuckooHashTable(2)

	for i := 0; i < 100; i++ {
		key := fmt.Sprintf("key%d", i)
		value := fmt.Sprintf("value%d", i)
		table.Insert(key, value)
	}

	result := table.Insert("final_key", "final_value")
	fmt.Printf("Final insert result: %t\n", result)
}

func (s *HashTableTestSuite) TestCuckooBinaryFileTable1WriteErrors() {
	table := NewCuckooHashTable(5)
	pairs := generateRandomKeyValuePairs(3)
	for _, pair := range pairs {
		table.Insert(pair.key, pair.value)
	}

	filename := "test_temp.bin"
	file, err := os.Create(filename)
	require.NoError(s.T(), err)
	file.Close()

	file, err = os.OpenFile(filename, os.O_RDONLY, 0644)
	require.NoError(s.T(), err)

	err = table.SaveToBinaryFile(file)
	assert.Error(s.T(), err)
	file.Close()

	os.Remove(filename)
}

package hashtable

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"
	"goproj/config"
	"goproj/node"
)

type CuckooHashTable struct {
	table1         []node.CKNode
	table2         []node.CKNode
	capacity       int
	size           int
	maxIterations  int
}

func NewCuckooHashTable(capacity int) *CuckooHashTable {
	if capacity <= 0 {
		capacity = config.DefaultInitialCapHash
	}

	table := &CuckooHashTable{
		capacity:      capacity,
		size:          0,
		maxIterations: capacity * 2,
		table1:        make([]node.CKNode, capacity),
		table2:        make([]node.CKNode, capacity),
	}

	for i := 0; i < capacity; i++ {
		table.table1[i].IsEmpty = true
		table.table1[i].IsDeleted = false
		table.table2[i].IsEmpty = true
		table.table2[i].IsDeleted = false
	}

	return table
}

func (c *CuckooHashTable) hash1(key string) int {
	hash := 0
	for _, ch := range key {
		hash = (hash*config.FirstHash + int(ch)) % c.capacity
	}
	return hash
}

func (c *CuckooHashTable) hash2(key string) int {
	hash := 0
	for _, ch := range key {
		hash = (hash*config.SecondHash + int(ch)) % c.capacity
	}
	return hash
}

func (c *CuckooHashTable) rehash(newCapacity int) bool {
	elements := c.GetAllElements()
	c.table1 = make([]node.CKNode, newCapacity)
	c.table2 = make([]node.CKNode, newCapacity)
	c.capacity = newCapacity
	c.size = 0
	c.maxIterations = newCapacity * 2

	for i := 0; i < newCapacity; i++ {
		c.table1[i].IsEmpty = true
		c.table1[i].IsDeleted = false
		c.table2[i].IsEmpty = true
		c.table2[i].IsDeleted = false
	}

	success := true
	for _, elem := range elements {
		key := elem.Key
		value := elem.Data
		if !c.Insert(key, value) {
			success = false
			break
		}
	}

	return success
}

func (c *CuckooHashTable) Insert(key string, data string) bool {
	if key == "" {
		return false
	}

	pos1 := c.hash1(key)
	pos2 := c.hash2(key)

	if !c.table1[pos1].IsEmpty && !c.table1[pos1].IsDeleted && c.table1[pos1].Key == key {
		c.table1[pos1].Data = data
		return true
	}
	if !c.table2[pos2].IsEmpty && !c.table2[pos2].IsDeleted && c.table2[pos2].Key == key {
		c.table2[pos2].Data = data
		return true
	}

	loadFactor := float64(c.size) / float64(c.capacity)
	if loadFactor > config.LoadFactorCoo {
		if !c.rehash(c.capacity * 2) {
			return false
		}
	}

	currentKey := key
	currentData := data
	iterations := 0

	for iterations < c.maxIterations {
		pos1 := c.hash1(currentKey)
		if c.table1[pos1].IsEmpty || c.table1[pos1].IsDeleted {
			c.table1[pos1].Key = currentKey
			c.table1[pos1].Data = currentData
			c.table1[pos1].IsEmpty = false
			c.table1[pos1].IsDeleted = false
			c.size++
			return true
		}

		currentKey, c.table1[pos1].Key = c.table1[pos1].Key, currentKey
		currentData, c.table1[pos1].Data = c.table1[pos1].Data, currentData

		pos2 := c.hash2(currentKey)
		if c.table2[pos2].IsEmpty || c.table2[pos2].IsDeleted {
			c.table2[pos2].Key = currentKey
			c.table2[pos2].Data = currentData
			c.table2[pos2].IsEmpty = false
			c.table2[pos2].IsDeleted = false
			c.size++
			return true
		}

		currentKey, c.table2[pos2].Key = c.table2[pos2].Key, currentKey
		currentData, c.table2[pos2].Data = c.table2[pos2].Data, currentData

		iterations++
	}

	if iterations >= c.maxIterations {
		if c.rehash(c.capacity * 2) {
			return c.Insert(key, data)
		}
	}

	return false
}

func (c *CuckooHashTable) Search(key string) string {
	if key == "" {
		return ""
	}

	pos1 := c.hash1(key)
	pos2 := c.hash2(key)

	if !c.table1[pos1].IsEmpty && !c.table1[pos1].IsDeleted && c.table1[pos1].Key == key {
		return c.table1[pos1].Data
	}
	if !c.table2[pos2].IsEmpty && !c.table2[pos2].IsDeleted && c.table2[pos2].Key == key {
		return c.table2[pos2].Data
	}
	return ""
}

func (c *CuckooHashTable) Remove(key string) bool {
	if key == "" {
		return false
	}

	pos1 := c.hash1(key)
	pos2 := c.hash2(key)

	if !c.table1[pos1].IsEmpty && !c.table1[pos1].IsDeleted && c.table1[pos1].Key == key {
		c.table1[pos1].IsDeleted = true
		c.size--
		return true
	}
	if !c.table2[pos2].IsEmpty && !c.table2[pos2].IsDeleted && c.table2[pos2].Key == key {
		c.table2[pos2].IsDeleted = true
		c.size--
		return true
	}
	return false
}

func (c *CuckooHashTable) Print() {
	fmt.Println("=== Кукушкина таблица ===")
	fmt.Printf("Размер: %d/%d (нагрузка: %.2f%%)\n", c.size, c.capacity, float64(c.size)/float64(c.capacity)*float64(config.Proc))

	fmt.Print("Таблица 1: ")
	for i := 0; i < c.capacity; i++ {
		if !c.table1[i].IsEmpty && !c.table1[i].IsDeleted {
			fmt.Printf("[%d]:'%s -> %s' ", i, c.table1[i].Key, c.table1[i].Data)
		}
	}
	fmt.Println()

	fmt.Print("Таблица 2: ")
	for i := 0; i < c.capacity; i++ {
		if !c.table2[i].IsEmpty && !c.table2[i].IsDeleted {
			fmt.Printf("[%d]:'%s -> %s' ", i, c.table2[i].Key, c.table2[i].Data)
		}
	}
	fmt.Println()
}

type KeyValuePair struct {
	Key  string
	Data string
}

func (c *CuckooHashTable) GetAllElements() []KeyValuePair {
	var elements []KeyValuePair
	for i := 0; i < c.capacity; i++ {
		if !c.table1[i].IsEmpty && !c.table1[i].IsDeleted {
			elements = append(elements, KeyValuePair{c.table1[i].Key, c.table1[i].Data})
		}
		if !c.table2[i].IsEmpty && !c.table2[i].IsDeleted {
			elements = append(elements, KeyValuePair{c.table2[i].Key, c.table2[i].Data})
		}
	}
	return elements
}

func (c *CuckooHashTable) SaveToFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	for i := 0; i < c.capacity; i++ {
		if !c.table1[i].IsEmpty && !c.table1[i].IsDeleted {
			_, err := file.WriteString(fmt.Sprintf("%s:%s ", c.table1[i].Key, c.table1[i].Data))
			if err != nil {
				return err
			}
		}
		if !c.table2[i].IsEmpty && !c.table2[i].IsDeleted {
			_, err := file.WriteString(fmt.Sprintf("%s:%s ", c.table2[i].Key, c.table2[i].Data))
			if err != nil {
				return err
			}
		}
	}
	return nil
}

func (c *CuckooHashTable) LoadFromFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	content, err := io.ReadAll(file)
	if err != nil {
		return err
	}

	keyValues := strings.Fields(string(content))
	for _, kv := range keyValues {
		colonPos := strings.Index(kv, ":")
		if colonPos != -1 {
			key := kv[:colonPos]
			value := kv[colonPos+1:]
			c.Insert(key, value)
		}
	}
	return nil
}

func (c *CuckooHashTable) SaveToBinaryFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	for i := 0; i < c.capacity; i++ {
		if !c.table1[i].IsEmpty && !c.table1[i].IsDeleted {
			keySize := uint64(len(c.table1[i].Key))
			err := binary.Write(file, binary.LittleEndian, keySize)
			if err != nil {
				return err
			}
			_, err = file.WriteString(c.table1[i].Key)
			if err != nil {
				return err
			}

			dataSize := uint64(len(c.table1[i].Data))
			err = binary.Write(file, binary.LittleEndian, dataSize)
			if err != nil {
				return err
			}
			_, err = file.WriteString(c.table1[i].Data)
			if err != nil {
				return err
			}
		}
		if !c.table2[i].IsEmpty && !c.table2[i].IsDeleted {
			keySize := uint64(len(c.table2[i].Key))
			err := binary.Write(file, binary.LittleEndian, keySize)
			if err != nil {
				return err
			}
			_, err = file.WriteString(c.table2[i].Key)
			if err != nil {
				return err
			}

			dataSize := uint64(len(c.table2[i].Data))
			err = binary.Write(file, binary.LittleEndian, dataSize)
			if err != nil {
				return err
			}
			_, err = file.WriteString(c.table2[i].Data)
			if err != nil {
				return err
			}
		}
	}
	return nil
}

func (c *CuckooHashTable) LoadFromBinaryFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	for {
		var keySize uint64
		err := binary.Read(file, binary.LittleEndian, &keySize)
		if err != nil {
			if err == io.EOF {
				break
			}
			return err
		}

		keyBuffer := make([]byte, keySize)
		_, err = file.Read(keyBuffer)
		if err != nil {
			return err
		}
		key := string(keyBuffer)

		var dataSize uint64
		err = binary.Read(file, binary.LittleEndian, &dataSize)
		if err != nil {
			return err
		}

		dataBuffer := make([]byte, dataSize)
		_, err = file.Read(dataBuffer)
		if err != nil {
			return err
		}
		value := string(dataBuffer)

		c.Insert(key, value)
	}
	return nil
}

type DoubleHashTable struct {
	table    []node.DHNode
	capacity int
	size     int
}

func NewDoubleHashTable(capacity int) *DoubleHashTable {
	if capacity <= 0 {
		capacity = config.DefaultInitialCapHash
	}

	table := &DoubleHashTable{
		capacity: capacity,
		size:     0,
		table:    make([]node.DHNode, capacity),
	}

	for i := 0; i < capacity; i++ {
		table.table[i].IsEmpty = true
		table.table[i].IsDeleted = false
	}

	return table
}

func (d *DoubleHashTable) hash1(key string) int {
	hash := 0
	for _, ch := range key {
		hash = (hash*config.FirstHash + int(ch)) % d.capacity
	}
	return hash
}

func (d *DoubleHashTable) hash2(key string) int {
	hash := 0
	for _, ch := range key {
		hash = (hash*config.SecondHash + int(ch)) % d.capacity
	}
	return hash
}

func (d *DoubleHashTable) rehash(newCapacity int) bool {
	elements := d.GetAllElements()

	d.table = make([]node.DHNode, newCapacity)
	d.capacity = newCapacity
	d.size = 0

	for i := 0; i < newCapacity; i++ {
		d.table[i].IsEmpty = true
		d.table[i].IsDeleted = false
	}

	success := true
	for _, elem := range elements {
		key := elem.Key
		value := elem.Data
		if !d.Insert(key, value) {
			success = false
			break
		}
	}

	return success
}

func (d *DoubleHashTable) Insert(key string, data string) bool {
	if key == "" {
		return false
	}

	loadFactor := float64(d.size) / float64(d.capacity)
	if loadFactor > config.LoadFactorDouble {
		if !d.rehash(d.capacity * 2) {
			return false
		}
	}

	hash1 := d.hash1(key)
	hash2 := d.hash2(key)
	index := hash1

	for i := 0; i < d.capacity; i++ {
		if d.table[index].IsEmpty || d.table[index].IsDeleted {
			d.table[index].Key = key
			d.table[index].Data = data
			d.table[index].IsEmpty = false
			d.table[index].IsDeleted = false
			d.size++
			return true
		}
		if !d.table[index].IsEmpty && !d.table[index].IsDeleted && d.table[index].Key == key {
			d.table[index].Data = data
			return true
		}

		index = (hash1 + i*hash2) % d.capacity
	}

	if d.rehash(d.capacity * 2) {
		return d.Insert(key, data)
	}

	return false
}

func (d *DoubleHashTable) Search(key string) string {
	if key == "" {
		return ""
	}

	hash1 := d.hash1(key)
	hash2 := d.hash2(key)
	index := hash1

	for i := 0; i < d.capacity; i++ {
		if d.table[index].IsEmpty && !d.table[index].IsDeleted {
			break
		}
		if !d.table[index].IsEmpty && !d.table[index].IsDeleted && d.table[index].Key == key {
			return d.table[index].Data
		}
		index = (hash1 + i*hash2) % d.capacity
	}
	return ""
}

func (d *DoubleHashTable) Remove(key string) bool {
	if key == "" {
		return false
	}

	hash1 := d.hash1(key)
	hash2 := d.hash2(key)
	index := hash1

	for i := 0; i < d.capacity; i++ {
		if d.table[index].IsEmpty && !d.table[index].IsDeleted {
			break
		}

		if !d.table[index].IsEmpty && !d.table[index].IsDeleted && d.table[index].Key == key {
			d.table[index].IsDeleted = true
			d.size--
			return true
		}

		index = (hash1 + i*hash2) % d.capacity
	}
	return false
}

func (d *DoubleHashTable) Print() {
	fmt.Println("=== Таблица двойного хеширования ===")
	fmt.Printf("Размер: %d/%d (нагрузка: %.2f%%)\n", d.size, d.capacity, float64(d.size)/float64(d.capacity)*float64(config.Proc))

	for i := 0; i < d.capacity; i++ {
		if !d.table[i].IsEmpty && !d.table[i].IsDeleted {
			fmt.Printf("[%d]:'%s -> %s' ", i, d.table[i].Key, d.table[i].Data)
		}
	}
	fmt.Println()
}

func (d *DoubleHashTable) GetAllElements() []KeyValuePair {
	var elements []KeyValuePair
	for i := 0; i < d.capacity; i++ {
		if !d.table[i].IsEmpty && !d.table[i].IsDeleted {
			elements = append(elements, KeyValuePair{d.table[i].Key, d.table[i].Data})
		}
	}
	return elements
}

func (d *DoubleHashTable) SaveToFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	for i := 0; i < d.capacity; i++ {
		if !d.table[i].IsEmpty && !d.table[i].IsDeleted {
			_, err := file.WriteString(fmt.Sprintf("%s:%s ", d.table[i].Key, d.table[i].Data))
			if err != nil {
				return err
			}
		}
	}
	return nil
}

func (d *DoubleHashTable) LoadFromFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	content, err := io.ReadAll(file)
	if err != nil {
		return err
	}

	keyValues := strings.Fields(string(content))
	for _, kv := range keyValues {
		colonPos := strings.Index(kv, ":")
		if colonPos != -1 {
			key := kv[:colonPos]
			value := kv[colonPos+1:]
			d.Insert(key, value)
		}
	}
	return nil
}

func (d *DoubleHashTable) SaveToBinaryFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	for i := 0; i < d.capacity; i++ {
		if !d.table[i].IsEmpty && !d.table[i].IsDeleted {
			keySize := uint64(len(d.table[i].Key))
			err := binary.Write(file, binary.LittleEndian, keySize)
			if err != nil {
				return err
			}
			_, err = file.WriteString(d.table[i].Key)
			if err != nil {
				return err
			}

			dataSize := uint64(len(d.table[i].Data))
			err = binary.Write(file, binary.LittleEndian, dataSize)
			if err != nil {
				return err
			}
			_, err = file.WriteString(d.table[i].Data)
			if err != nil {
				return err
			}
		}
	}
	return nil
}

func (d *DoubleHashTable) LoadFromBinaryFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	for {
		var keySize uint64
		err := binary.Read(file, binary.LittleEndian, &keySize)
		if err != nil {
			if err == io.EOF {
				break
			}
			return err
		}

		keyBuffer := make([]byte, keySize)
		_, err = file.Read(keyBuffer)
		if err != nil {
			return err
		}
		key := string(keyBuffer)

		var dataSize uint64
		err = binary.Read(file, binary.LittleEndian, &dataSize)
		if err != nil {
			return err
		}

		dataBuffer := make([]byte, dataSize)
		_, err = file.Read(dataBuffer)
		if err != nil {
			return err
		}
		value := string(dataBuffer)

		d.Insert(key, value)
	}
	return nil
}
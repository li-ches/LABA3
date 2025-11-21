package dynarr

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"
)

type DynArr struct {
	data []string
	cap  int
	len  int
}

func NewDynArr(initialCap int) *DynArr {
	if initialCap <= 0 {
		initialCap = DefaultInitialCapacity
	}
	return &DynArr{
		data: make([]string, initialCap),
		cap:  initialCap,
		len:  0,
	}
}

func (d *DynArr) AddToEnd(val string) {
	if d.len >= d.cap {
		newCap := d.cap * 2
		newData := make([]string, newCap)
		copy(newData, d.data[:d.len])
		d.data = newData
		d.cap = newCap
	}
	d.data[d.len] = val
	d.len++
}

func (d *DynArr) AddAtIndex(index int, val string) error {
	if index < 0 || index > d.len {
		return errors.New("error")
	}

	if d.len >= d.cap {
		newCap := d.cap * 2
		newData := make([]string, newCap)
		copy(newData, d.data[:d.len])
		d.data = newData
		d.cap = newCap
	}

	for i := d.len; i > index; i-- {
		d.data[i] = d.data[i-1]
	}
	d.data[index] = val
	d.len++
	return nil
}

func (d *DynArr) GetElement(index int) (string, error) {
	if index < 0 || index >= d.len {
		return "", errors.New("error")
	}
	return d.data[index], nil
}

func (d *DynArr) ReplaceElement(index int, value string) error {
	if index < 0 || index >= d.len {
		return errors.New("error")
	}
	d.data[index] = value
	return nil
}

func (d *DynArr) RemoveAtIndex(index int) error {
	if index < 0 || index >= d.len {
		return errors.New("error")
	}

	for i := index; i < d.len-1; i++ {
		d.data[i] = d.data[i+1]
	}
	d.len--
	return nil
}

func (d *DynArr) PrintArr() {
	if d.len == 0 {
		fmt.Println("error")
		return
	}

	fmt.Print("[")
	for i := 0; i < d.len; i++ {
		fmt.Print(d.data[i])
		if i < d.len-1 {
			fmt.Print(", ")
		}
	}
	fmt.Println("]")
}

func (d *DynArr) SaveToFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	for i := 0; i < d.len; i++ {
		_, err := file.WriteString(d.data[i] + " ")
		if err != nil {
			return err
		}
	}
	return nil
}

func (d *DynArr) LoadFromFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	content, err := io.ReadAll(file)
	if err != nil {
		return err
	}

	values := strings.Fields(string(content))
	for _, val := range values {
		d.AddToEnd(val)
	}
	return nil
}

func (d *DynArr) SaveToBinaryFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	for i := 0; i < d.len; i++ {
		strSize := uint64(len(d.data[i]))
		err := binary.Write(file, binary.LittleEndian, strSize)
		if err != nil {
			return err
		}
		_, err = file.WriteString(d.data[i])
		if err != nil {
			return err
		}
	}
	return nil
}

func (d *DynArr) LoadFromBinaryFile(file *os.File) error {
	if file == nil {
		return errors.New("file is nil")
	}

	for {
		var strSize uint64
		err := binary.Read(file, binary.LittleEndian, &strSize)
		if err != nil {
			if err == io.EOF {
				break
			}
			return err
		}

		buffer := make([]byte, strSize)
		_, err = file.Read(buffer)
		if err != nil {
			return err
		}
		d.AddToEnd(string(buffer))
	}
	return nil
}

func (d *DynArr) GetLength() int {
	return d.len
}

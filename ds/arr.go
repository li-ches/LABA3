package ds

import "fmt"

type MyArr struct {
	data []string
}

func NewMyArr() *MyArr {
	return &MyArr{
		data: make([]string, 0),
	}
}

func (a *MyArr) AddEnd(val string) {
	a.data = append(a.data, val)
}

func (a *MyArr) AddHead(val string) {
	a.data = append([]string{val}, a.data...)
}

func (a *MyArr) AddAt(idx int, val string) {
	if idx < 0 || idx > len(a.data) {
		return
	}
	if idx == len(a.data) {
		a.AddEnd(val)
		return
	}
	a.data = append(a.data[:idx+1], a.data[idx:]...)
	a.data[idx] = val
}

func (a *MyArr) DelHead() {
	if len(a.data) == 0 {
		return
	}
	a.data = a.data[1:]
}

func (a *MyArr) DelEnd() {
	if len(a.data) == 0 {
		return
	}
	a.data = a.data[:len(a.data)-1]
}

func (a *MyArr) DelAt(idx int) {
	if idx < 0 || idx >= len(a.data) {
		return
	}
	a.data = append(a.data[:idx], a.data[idx+1:]...)
}

func (a *MyArr) RepArr(idx int, val string) {
	if idx < 0 || idx >= len(a.data) {
		return
	}
	a.data[idx] = val
}

func (a *MyArr) GetAt(idx int) string {
	if idx >= 0 && idx < len(a.data) {
		return a.data[idx]
	}
	return "[INVALID_INDEX]"
}

func (a *MyArr) LenArr() int {
	return len(a.data)
}

func (a *MyArr) ReadArray() {
	if len(a.data) == 0 {
		fmt.Println("Массив пуст.")
		return
	}
	fmt.Printf("Содержимое массива (%d/%d): ", len(a.data), cap(a.data))
	for _, val := range a.data {
		fmt.Printf("\"%s\" ", val)
	}
	fmt.Println()
}
package ds

import (
	"bufio"
	"fmt"
	"os"
)

type HashSerializer struct{}

func NewHashSerializer() *HashSerializer {
	return &HashSerializer{}
}

func (s *HashSerializer) SaveToFile(hash interface{}, filename string) error {
	switch h := hash.(type) {
	case *ChainHash:
		return s.saveChainHash(h, filename)
	case *OpenHash:
		return s.saveOpenHash(h, filename)
	default:
		return fmt.Errorf("unsupported hash type")
	}
}

func (s *HashSerializer) LoadFromFile(hash interface{}, filename string) error {
	switch h := hash.(type) {
	case *ChainHash:
		return s.loadChainHash(h, filename)
	case *OpenHash:
		return s.loadOpenHash(h, filename)
	default:
		return fmt.Errorf("unsupported hash type")
	}
}

func (s *HashSerializer) SaveToBinaryFile(hash interface{}, filename string) error {
	switch h := hash.(type) {
	case *ChainHash:
		return s.saveChainHashBinary(h, filename)
	case *OpenHash:
		return s.saveOpenHashBinary(h, filename)
	default:
		return fmt.Errorf("unsupported hash type")
	}
}

func (s *HashSerializer) LoadFromBinaryFile(hash interface{}, filename string) error {
	switch h := hash.(type) {
	case *ChainHash:
		return s.loadChainHashBinary(h, filename)
	case *OpenHash:
		return s.loadOpenHashBinary(h, filename)
	default:
		return fmt.Errorf("unsupported hash type")
	}
}

func (s *HashSerializer) saveChainHash(ch *ChainHash, filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	fmt.Fprintln(file, ch.Cap)
	count := 0
	for _, n := range ch.Table {
		for cur := n; cur != nil; cur = cur.Next {
			count++
		}
	}
	fmt.Fprintln(file, count)

	for _, n := range ch.Table {
		for cur := n; cur != nil; cur = cur.Next {
			WriteStringText(file, cur.Key)
			WriteStringText(file, cur.Val)
		}
	}
	return nil
}

func (s *HashSerializer) loadChainHash(ch *ChainHash, filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	sc := bufio.NewScanner(file)
	var capVal, count int
	if sc.Scan() {
		fmt.Sscanf(sc.Text(), "%d", &capVal)
	}

	ch.Cap = capVal
	ch.Table = make([]*ChainNode, ch.Cap)

	if sc.Scan() {
		fmt.Sscanf(sc.Text(), "%d", &count)
	}

	for i := 0; i < count; i++ {
		k := ReadStringText(sc)
		v := ReadStringText(sc)
		ch.Insert(k, v)
	}
	return nil
}

func (s *HashSerializer) saveChainHashBinary(ch *ChainHash, filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	WriteInt32(file, int32(ch.Cap))
	count := 0
	for _, n := range ch.Table {
		for cur := n; cur != nil; cur = cur.Next {
			count++
		}
	}
	WriteInt32(file, int32(count))

	for _, n := range ch.Table {
		for cur := n; cur != nil; cur = cur.Next {
			WriteString(file, cur.Key)
			WriteString(file, cur.Val)
		}
	}
	return nil
}

func (s *HashSerializer) loadChainHashBinary(ch *ChainHash, filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	capVal, _ := ReadInt32(file)
	ch.Cap = int(capVal)
	ch.Table = make([]*ChainNode, ch.Cap)

	count, _ := ReadInt32(file)
	for i := 0; i < int(count); i++ {
		k, _ := ReadString(file)
		v, _ := ReadString(file)
		ch.Insert(k, v)
	}
	return nil
}

func (s *HashSerializer) saveOpenHash(oh *OpenHash, filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	fmt.Fprintln(file, oh.Cap)
	count := 0
	for _, e := range oh.Table {
		if e.Used {
			count++
		}
	}
	fmt.Fprintln(file, count)

	for _, e := range oh.Table {
		if e.Used {
			WriteStringText(file, e.Key)
			WriteStringText(file, e.Val)
		}
	}
	return nil
}

func (s *HashSerializer) loadOpenHash(oh *OpenHash, filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	sc := bufio.NewScanner(file)
	var capVal, count int
	if sc.Scan() {
		fmt.Sscanf(sc.Text(), "%d", &capVal)
	}
	oh.Cap = capVal
	oh.Table = make([]OpenEntry, capVal)

	if sc.Scan() {
		fmt.Sscanf(sc.Text(), "%d", &count)
	}

	for i := 0; i < count; i++ {
		k := ReadStringText(sc)
		v := ReadStringText(sc)
		oh.Insert(k, v)
	}
	return nil
}

func (s *HashSerializer) saveOpenHashBinary(oh *OpenHash, filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	WriteInt32(file, int32(oh.Cap))
	count := 0
	for _, e := range oh.Table {
		if e.Used {
			count++
		}
	}
	WriteInt32(file, int32(count))

	for _, e := range oh.Table {
		if e.Used {
			WriteString(file, e.Key)
			WriteString(file, e.Val)
		}
	}
	return nil
}

func (s *HashSerializer) loadOpenHashBinary(oh *OpenHash, filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	capVal, _ := ReadInt32(file)
	oh.Cap = int(capVal)
	oh.Table = make([]OpenEntry, oh.Cap)

	count, _ := ReadInt32(file)
	for i := 0; i < int(count); i++ {
		k, _ := ReadString(file)
		v, _ := ReadString(file)
		oh.Insert(k, v)
	}
	return nil
}
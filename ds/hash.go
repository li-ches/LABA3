package ds

import (
    "bufio"
    "fmt"
    "os"
)

// ------------------------------------------------------------
// HASH FUNCTION
// ------------------------------------------------------------

func hashStr(key string, mod int) int {
    h := 0
    for _, ch := range key {
        v := int(ch)
        if v < 0 {
            v = -v
        }
        h = (h*31 + v) % mod
    }
    return h
}

// ------------------------------------------------------------
// CHAIN HASH TABLE
// ------------------------------------------------------------

type ChainNode struct {
    Key  string
    Val  string
    Next *ChainNode
}

type ChainHash struct {
    Table []*ChainNode
    Cap   int
}

func NewChainHash(buckets int) *ChainHash {
    if buckets < 1 {
        buckets = 1
    }
    return &ChainHash{
        Table: make([]*ChainNode, buckets),
        Cap:   buckets,
    }
}

func (c *ChainHash) Insert(k, v string) {
    idx := hashStr(k, c.Cap)
    cur := c.Table[idx]

    for cur != nil {
        if cur.Key == k {
            cur.Val = v
            return
        }
        cur = cur.Next
    }

    c.Table[idx] = &ChainNode{
        Key:  k,
        Val:  v,
        Next: c.Table[idx],
    }
}

func (c *ChainHash) Find(key string) string {
    idx := hashStr(key, c.Cap)
    cur := c.Table[idx]
    for cur != nil {
        if cur.Key == key {
            return cur.Val
        }
        cur = cur.Next
    }
    return ""
}

// ------------------------------------------------------------
// TEXT SERIALIZATION (unique and correct version)
// ------------------------------------------------------------

func (c *ChainHash) SaveToFile(filename string) error {
    file, err := os.Create(filename)
    if err != nil {
        return err
    }
    defer file.Close()

    // Write capacity
    fmt.Fprintln(file, c.Cap)

    // Count elements
    cnt := 0
    for _, n := range c.Table {
        for cur := n; cur != nil; cur = cur.Next {
            cnt++
        }
    }
    fmt.Fprintln(file, cnt)

    // Write elements
    for _, n := range c.Table {
        for cur := n; cur != nil; cur = cur.Next {
            WriteStringText(file, cur.Key)
            WriteStringText(file, cur.Val)
        }
    }
    return nil
}

func (c *ChainHash) LoadFromFile(filename string) error {
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

    c.Cap = capVal
    c.Table = make([]*ChainNode, c.Cap)

    if sc.Scan() {
        fmt.Sscanf(sc.Text(), "%d", &count)
    }

    for i := 0; i < count; i++ {
        k := ReadStringText(sc)
        v := ReadStringText(sc)
        c.Insert(k, v)
    }
    return nil
}

// ------------------------------------------------------------
// BINARY SERIALIZATION
// ------------------------------------------------------------

func (c *ChainHash) SaveToBinaryFile(filename string) error {
    file, err := os.Create(filename)
    if err != nil {
        return err
    }
    defer file.Close()

    WriteInt32(file, int32(c.Cap))

    count := 0
    for _, n := range c.Table {
        for cur := n; cur != nil; cur = cur.Next {
            count++
        }
    }
    WriteInt32(file, int32(count))

    for _, n := range c.Table {
        for cur := n; cur != nil; cur = cur.Next {
            WriteString(file, cur.Key)
            WriteString(file, cur.Val)
        }
    }
    return nil
}

func (c *ChainHash) LoadFromBinaryFile(filename string) error {
    file, err := os.Open(filename)
    if err != nil {
        return err
    }
    defer file.Close()

    capVal, _ := ReadInt32(file)
    c.Cap = int(capVal)
    c.Table = make([]*ChainNode, c.Cap)

    count, _ := ReadInt32(file)
    for i := 0; i < int(count); i++ {
        k, _ := ReadString(file)
        v, _ := ReadString(file)
        c.Insert(k, v)
    }
    return nil
}


type OpenEntry struct {
    Key     string
    Val     string
    Used    bool
    Deleted bool
}

type OpenHash struct {
    Table []OpenEntry
    Cap   int
}

func NewOpenHash(size int) *OpenHash {
    if size < 1 {
        size = 1
    }
    return &OpenHash{
        Table: make([]OpenEntry, size),
        Cap:   size,
    }
}

func (o *OpenHash) Insert(k, v string) {
    idx := hashStr(k, o.Cap)
    start := idx

    for {
        e := &o.Table[idx]

        if !e.Used && !e.Deleted {
            e.Key = k
            e.Val = v
            e.Used = true
            return
        }
        if e.Used && e.Key == k {
            e.Val = v
            return
        }
        if e.Deleted {
            e.Key = k
            e.Val = v
            e.Used = true
            e.Deleted = false
            return
        }

        idx = (idx + 1) % o.Cap
        if idx == start {
            return
        }
    }
}

func (o *OpenHash) Find(k string) string {
    idx := hashStr(k, o.Cap)
    start := idx

    for {
        e := &o.Table[idx]

        if e.Used && e.Key == k {
            return e.Val
        }
        if !e.Used && !e.Deleted {
            return ""
        }

        idx = (idx + 1) % o.Cap
        if idx == start {
            return ""
        }
    }
}


func (o *OpenHash) SaveToFile(filename string) error {
    file, err := os.Create(filename)
    if err != nil {
        return err
    }
    defer file.Close()

    fmt.Fprintln(file, o.Cap)

    count := 0
    for _, e := range o.Table {
        if e.Used {
            count++
        }
    }
    fmt.Fprintln(file, count)

    for _, e := range o.Table {
        if e.Used {
            WriteStringText(file, e.Key)
            WriteStringText(file, e.Val)
        }
    }
    return nil
}

func (o *OpenHash) LoadFromFile(filename string) error {
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
    o.Cap = capVal
    o.Table = make([]OpenEntry, capVal)

    if sc.Scan() {
        fmt.Sscanf(sc.Text(), "%d", &count)
    }

    for i := 0; i < count; i++ {
        k := ReadStringText(sc)
        v := ReadStringText(sc)
        o.Insert(k, v)
    }

    return nil
}


func (o *OpenHash) SaveToBinaryFile(filename string) error {
    file, err := os.Create(filename)
    if err != nil {
        return err
    }
    defer file.Close()

    WriteInt32(file, int32(o.Cap))

    count := 0
    for _, e := range o.Table {
        if e.Used {
            count++
        }
    }
    WriteInt32(file, int32(count))

    for _, e := range o.Table {
        if e.Used {
            WriteString(file, e.Key)
            WriteString(file, e.Val)
        }
    }
    return nil
}

func (o *OpenHash) LoadFromBinaryFile(filename string) error {
    file, err := os.Open(filename)
    if err != nil {
        return err
    }
    defer file.Close()

    capVal, _ := ReadInt32(file)
    o.Cap = int(capVal)
    o.Table = make([]OpenEntry, o.Cap)

    count, _ := ReadInt32(file)
    for i := 0; i < int(count); i++ {
        k, _ := ReadString(file)
        v, _ := ReadString(file)
        o.Insert(k, v)
    }

    return nil
}

package ds

import (
	"os"
	"testing"
)

func TestHash_ChainHash_Coverage(t *testing.T) {
	ch := NewChainHash(1)
	ch.Show() 

	ch.Insert("A", "1")
	ch.Insert("B", "2")
	ch.Insert("C", "3")

	if val := ch.Find("B"); val != "2" {
		t.Errorf("Find failed deep in chain. Expected 2, got %s", val)
	}
	if val := ch.Find("C"); val != "3" {
		t.Errorf("Find failed at end of chain. Expected 3, got %s", val)
	}

	ch.Insert("B", "222")
	if val := ch.Find("B"); val != "222" {
		t.Errorf("Update failed. Expected 222, got %s", val)
	}

	
	ch.Erase("B")
	if ch.Find("B") != "" {
		t.Error("B should be erased")
	}

	if ch.Find("A") != "1" || ch.Find("C") != "3" {
		t.Error("Chain broken after erase")
	}

	ch.Erase("C")
	if ch.Find("C") != "" {
		t.Error("C (head) should be erased")
	}
	
	ch.Erase("A")
	if ch.Find("A") != "" {
		t.Error("A should be erased")
	}

	if ch.Erase("Z") {
		t.Error("Erase Z should return false")
	}
}

func TestHash_OpenHash_Coverage(t *testing.T) {
	oh := NewOpenHash(2)
	oh.Show()

	oh.Insert("A", "1")
	oh.Insert("B", "2")
	
	oh.Insert("C", "3") 

	if oh.Find("A") != "1" || oh.Find("B") != "2" {
		t.Error("Find basic failed")
	}
	if oh.Find("Z") != "" {
		t.Error("Found non-existent Z")
	}

	oh.Insert("A", "111")
	if oh.Find("A") != "111" {
		t.Error("Update failed")
	}

	oh.Erase("A")
	if oh.Find("A") != "" {
		t.Error("A should be deleted")
	}
	oh.Insert("A", "NewA")
	if oh.Find("A") != "NewA" {
		t.Error("Insert into deleted slot failed")
	}
	
	if oh.Erase("Z") {
		t.Error("Erase non-existent should return false")
	}
}

func TestHash_SaveLoad(t *testing.T) {
	filename := "hash_test.bin"
	defer os.Remove(filename)

	ch := NewChainHash(5)
	ch.Insert("key1", "val1")
	
	hashSerializer := NewHashSerializer()
	hashSerializer.SaveToFile(ch, filename)
	
	ch2 := NewChainHash(1)
	hashSerializer.LoadFromFile(ch2, filename)
	if ch2.Find("key1") != "val1" {
		t.Error("ChainHash Save/Load failed")
	}

	oh := NewOpenHash(5)
	oh.Insert("k1", "v1")
	hashSerializer.SaveToFile(oh, filename)

	oh2 := NewOpenHash(1)
	hashSerializer.LoadFromFile(oh2, filename)
	if oh2.Find("k1") != "v1" {
		t.Error("OpenHash Save/Load failed")
	}
}

func TestHashMan_Simulation(t *testing.T) {

	r, w, _ := os.Pipe()
	origStdin := os.Stdin
	os.Stdin = r
	defer func() { os.Stdin = origStdin }()

	go func() {
		w.Write([]byte("INSERT k1 v1\nSHOW\nSEARCH k1\nDELETE k1\nSEARCH k1\nBAD_CMD\nBACK\n"))
		w.Close()
	}()

	HashMan()
}

func TestHash_Chain_Coverage(t *testing.T) {
	ch := NewChainHash(1)
	
	ch.Insert("A", "1")
	ch.Insert("B", "2") 
	
	ch.Insert("A", "11")
	if ch.Find("A") != "11" { t.Error("Update A failed") }
	
	ch.Insert("C", "3") 
	ch.Erase("B")
	if ch.Find("B") != "" { t.Error("Erase B failed") }
	if ch.Find("C") != "3" || ch.Find("A") != "11" { t.Error("Chain broken") }

	ch.Erase("C")
	if ch.Find("C") != "" { t.Error("Erase C failed") }

	if ch.Erase("Z") { t.Error("Erase Z should be false") }

	hashSerializer := NewHashSerializer()
	hashSerializer.SaveToFile(ch, "ch.txt")
	defer os.Remove("ch.txt")
	ch2 := NewChainHash(1)
	hashSerializer.LoadFromFile(ch2, "ch.txt")
	if ch2.Find("A") != "11" { t.Error("Load txt failed") }

	hashSerializer.SaveToBinaryFile(ch, "ch.bin")
	defer os.Remove("ch.bin")
	ch3 := NewChainHash(1)
	hashSerializer.LoadFromBinaryFile(ch3, "ch.bin")
	if ch3.Find("A") != "11" { t.Error("Load bin failed") }
}

func TestHash_Open_Coverage(t *testing.T) {
	oh := NewOpenHash(2)
	oh.Show() 
	
	oh.Insert("A", "1")
	oh.Insert("B", "2")
	
	oh.Insert("C", "3") 
	
	if oh.Find("A") != "1" { t.Error("Find A fail") }
	if oh.Find("Z") != "" { t.Error("Find Z fail") }
	
	oh.Insert("A", "11")
	if oh.Find("A") != "11" { t.Error("Update A fail") }

	oh.Erase("A")
	if oh.Find("A") != "" { t.Error("Should be deleted") }
	
	oh.Insert("A", "22") 
	if oh.Find("A") != "22" { t.Error("Reuse slot fail") }

	if oh.Erase("ZZ") { t.Error("Erase missing fail") }

	hashSerializer := NewHashSerializer()
	hashSerializer.SaveToFile(oh, "oh.txt")
	defer os.Remove("oh.txt")
	oh2 := NewOpenHash(2)
	hashSerializer.LoadFromFile(oh2, "oh.txt")
	if oh2.Find("B") != "2" { t.Error("Load txt fail") }

	hashSerializer.SaveToBinaryFile(oh, "oh.bin")
	defer os.Remove("oh.bin")
	oh3 := NewOpenHash(2)
	hashSerializer.LoadFromBinaryFile(oh3, "oh.bin")
	if oh3.Find("B") != "2" { t.Error("Load bin fail") }
	
	bad := NewOpenHash(0)
	if bad.Cap != 1 { t.Error("Min cap fail") }
}

func TestHashMan_Run(t *testing.T) {
	r, w, _ := os.Pipe()
	origStdin := os.Stdin
	os.Stdin = r
	defer func() { os.Stdin = origStdin }()

	go func() {
		w.Write([]byte("CHAIN\nINSERT k1 v1\nSHOW\nOPEN\nINSERT k2 v2\nSEARCH k2\nDELETE k2\nBAD\nBACK\n"))
		w.Close()
	}()
	
	HashMan()
}
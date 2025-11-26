package ds

import (
	"os"
)

type CbtSerializer struct{}

func NewCbtSerializer() *CbtSerializer {
	return &CbtSerializer{}
}

func (s *CbtSerializer) SaveToFile(tree *CompleteBinaryTree, filename string) error {
	return SaveCBTText(filename, tree.Root)
}

func (s *CbtSerializer) LoadFromFile(tree *CompleteBinaryTree, filename string) error {
	root, err := LoadCBTText(filename)
	if err != nil {
		return err
	}
	tree.Root = root
	tree.Size = tree.countNodes(tree.Root)
	return nil
}

func writeBinaryRec(n *CBTNode, f *os.File) error {
	if n == nil {
		return WriteInt32(f, -1)
	}
	if err := WriteInt32(f, n.Data); err != nil {
		return err
	}
	if err := writeBinaryRec(n.Left, f); err != nil {
		return err
	}
	return writeBinaryRec(n.Right, f)
}

func (s *CbtSerializer) SaveToBinaryFile(tree *CompleteBinaryTree, filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	return writeBinaryRec(tree.Root, file)
}

func readBinaryRec(f *os.File) (*CBTNode, error) {
	val, err := ReadInt32(f)
	if err != nil {
		return nil, err
	}
	if val == -1 {
		return nil, nil
	}
	node := &CBTNode{Data: val}
	node.Left, err = readBinaryRec(f)
	if err != nil {
		return nil, err
	}
	node.Right, err = readBinaryRec(f)
	if err != nil {
		return nil, err
	}
	return node, nil
}

func (s *CbtSerializer) LoadFromBinaryFile(tree *CompleteBinaryTree, filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	tree.Clear()
	root, err := readBinaryRec(file)
	if err != nil {
		return err
	}
	tree.Root = root
	tree.Size = tree.countNodes(tree.Root)
	return nil
}
package onelist

type OneListNode struct {
	Data string
	Next *OneListNode
}

type TwoListNode struct {
	Data string
	Next *TwoListNode
	Prev *TwoListNode
}

type StackNode struct {
	Data string
	Next *StackNode
}

type QueueNode struct {
	Data string
	Next *QueueNode
}

type DHNode struct {
	Key       string
	Data      string
	IsDeleted bool
	IsEmpty   bool
}

type CKNode struct {
	Key       string
	Data      string
	IsDeleted bool
	IsEmpty   bool
}

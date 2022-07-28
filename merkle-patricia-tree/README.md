# Merkle Patricial Tree
  该文件是对Node.js平台下的merkle-patricia-tree库的理解，该库是用TypeScript语言编写并可以在ts文件中调用的库。首先在cmd命令行窗口通过npm指令下载merkle-patricia-tree库
该步骤需要在安装了nodejs后，完整的node.js的配置过程参考文章[1]。

  安装完毕merkle-patricia-tree库如下：

![image](https://github.com/HaiboYu02/img-storage/blob/main/pic14.png)

  在nodejs的以下路径中可以看到该库的所有实现源码...\nodejs\node_global\node_modules\merkle-patricia-tree\src，与上传的部分相同。该程序作为官方标注库，实现过程较为严谨和复杂。
但还是可以看到Merkle Patricial Tree的实现思路：首先在trieNode.ts文件中定义了树中四种节点类型：BranchNode、ExtensionNode、LeafNode。各种节点中定义的方法不同，其中包括对
key-value对的处理。以BranchNode节点为例:

```js
export class BranchNode {
  _branches: (EmbeddedNode | null)[]
  _value: Buffer | null

  constructor() {
    this._branches = new Array(16).fill(null)
    this._value = null
  }

  static fromArray(arr: Buffer[]): BranchNode {
    const node = new BranchNode()
    node._branches = arr.slice(0, 16)
    node._value = arr[16]
    return node
  }

  get value(): Buffer | null {
    return this._value && this._value.length > 0 ? this._value : null
  }

  set value(v: Buffer | null) {
    this._value = v
  }

  setBranch(i: number, v: EmbeddedNode | null) {
    this._branches[i] = v
  }

  raw(): (EmbeddedNode | null)[] {
    return [...this._branches, this._value]
  }

  serialize(): Buffer {
    return rlp.encode(this.raw())
  }

  hash(): Buffer {
    return keccak256(this.serialize())
  }

  getBranch(i: number) {
    const b = this._branches[i]
    if (b !== null && b.length > 0) {
      return b
    } else {
      return null
    }
  }

  getChildren(): [number, EmbeddedNode][] {
    const children: [number, EmbeddedNode][] = []
    for (let i = 0; i < 16; i++) {
      const b = this._branches[i]
      if (b !== null && b.length > 0) {
        children.push([i, b])
      }
    }
    return children
  }
}

export class ExtensionNode {
  _nibbles: Nibbles
  _value: Buffer

  constructor(nibbles: Nibbles, value: Buffer) {
    this._nibbles = nibbles
    this._value = value
  }

  static encodeKey(key: Nibbles): Nibbles {
    return addHexPrefix(key, false)
  }

  static decodeKey(key: Nibbles): Nibbles {
    return removeHexPrefix(key)
  }

  get key(): Nibbles {
    return this._nibbles.slice(0)
  }

  set key(k: Nibbles) {
    this._nibbles = k
  }

  get keyLength() {
    return this._nibbles.length
  }

  get value(): Buffer {
    return this._value
  }

  set value(v: Buffer) {
    this._value = v
  }

  encodedKey(): Nibbles {
    return ExtensionNode.encodeKey(this._nibbles.slice(0))
  }

  raw(): [Buffer, Buffer] {
    return [nibblesToBuffer(this.encodedKey()), this._value]
  }

  serialize(): Buffer {
    return rlp.encode(this.raw())
  }

  hash(): Buffer {
    return keccak256(this.serialize())
  }
}
```

  类中包含两个字段变量 **\_branches** 、**\_value** 
  
  一个简单的构造函数；
  
  若干个方法。
  
  Trie树中每个叶子节点都有Key-Value对，键不是直接保存在节点中，而是由节点在树中的位置决定。一个节点的所有子孙都有相同的前缀，节点对应的key是根节点到该节点路径上的所有节点key值前后拼接而成，节点的value值就是该key对应的值。根节点对应空字符串key。根据这些原理就可以分析出get value()获取节点的value、set value(v: Buffer | null)设置节点的value、setBranch(i: number, v: EmbeddedNode | null)设置节点的第i各分支为某个节点、raw()返回节点的所有分支和value值、serialize()返回加密后的raw()结果、getBranch(i: number)获取节点的第i个分支、getChildren()获取孩子节点。
  
  * * *
  
  另一个重要文件是baseTrie.ts，这里定义了Trie类，该文件是调用 '\_import { BaseTrie as Trie } from 'merkle-patricia-tree' 接口的内部实现。\_root字段变量代表的是根节点，异步get()是给定value取key，put()是在给定的key路径下存储一个value值，还有findPath(key: Buffer, throwIfMissing = false)寻找路径函数。其中还包括许多更高级的实现如通过哈希对节点加锁。更新和删除节点较复杂，都需要对TrieNode类型数组进行push等操作。
  
  以上均是对库的源代码进行简单的结构梳理的总结，所有的src文件均在该文章的同一个库中上传。
  
  参考文献：
  
  [1]https://cloud.tencent.com/developer/article/1572591
  
  [2]https://blog.csdn.net/ITleaks/article/details/79992072
  
  [3]https://github.com/ethereumjs/merkle-patricia-tree
  

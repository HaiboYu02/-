# Merkle Patricial Tree
  该文件是对Node.js平台下的merkle-patricia-tree库的理解，该库是用TypeScript语言编写并可以在ts文件中调用的库。首先在cmd命令行窗口通过npm指令下载merkle-patricia-tree库
该步骤需要在安装了nodejs后，完整的node.js的配置过程参考文章[1]。

  安装完毕merkle-patricia-tree库如下：

![image](https://github.com/HaiboYu02/img-storage/blob/main/pic14.png)

  在nodejs的以下路径中可以看到该库的所有实现源码...\nodejs\node_global\node_modules\merkle-patricia-tree\src，与上传的部分相同。该程序作为官方标注库，实现过程较为严谨和复杂。
但还是可以看到Merkle Patricial Tree的实现思路：首先在trieNode.ts文件中定义了树中四种节点类型：BranchNode、ExtensionNode、LeafNode。各种节点中定义的方法不同，其中包括对
key-value
对的处理。以BranchNode节点为例:

  '''javascript
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



  '''

  类中包含两个字段变量_branches

https://cloud.tencent.com/developer/article/1572591

# Meow快速哈希算法逆向求key（未完全实现）
  通过Meow_Hash.h文件中的定义，可以在cpp文件中测试哈希算法的可行性。

```c
HashTestBuffer(void)
{
    // NOTE(casey): Make a buffer with repeating numbers.

    char Buffer[] = "YuHaibo202000460019";
    int Size = strlen(Buffer);

    // NOTE(casey): Ask Meow for the hash
    meow_u128 Hash = MeowHash(MeowDefaultSeed, Size, Buffer);

    // NOTE(casey): Print the hash
    printf("  Hash of a test buffer:\n");
    PrintHash(Hash);
}
```

  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic18.png)
  
  根据哈希算法分组时每一轮的流程图，可知在Absorb Message、Finalization、Squeeze阶段都是可逆的，推导逆过程如下：
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic19.jpg)

  但编写逆向算法时出现了一些问题，由于不熟悉该头文件中的实现细节也不确定逆向求key的过程是否正确，导致求出的key替代源代码中1024bit的defaultkey时  求哈希得不到想要的摘要值，所以没有成功求出特定message的key。
  

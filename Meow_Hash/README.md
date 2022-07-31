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

  但编写逆向算法时出现了一些问题，由于不熟悉该头文件中的实现细节，所以没有成功求出特定message的key。
  

# V-Engine Package Format (VPF)


### Header (64 bytes)
| Field       | Size     |
|-------------|----------|
| Magic       | 4 bytes  |
| Version     | 2 bytes  |
| Flags       | 2 bytes  |
| FileCount   | 4 bytes  |
| TableOffset | 8 bytes  |
| DataOffset  | 8 bytes  |
| TableSize   | 8 bytes  |
| Reserved    | 28 bytes |


```c++
VPFPackage package;
package.open("assets.vpf");

auto entry = package.findFile(hashPath("textures/hero.png"));
if (entry) {
    auto data = package.readEntry(entry); 
}
```
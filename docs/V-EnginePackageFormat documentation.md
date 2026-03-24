# V-Engine Package Format (VPF)
## VPF 1.0.0
Endianness: little-endian
Checksum hash function: xxHash64
### Header (64 bytes)
Start of the file 

| Field           | Size     |
|-----------------|----------|
| Magic           | 4 bytes  |
| Version         | 4 bytes  |
| Flags           | 2 bytes  |
| DataOffset      | 8 bytes  |
| HashTableSize   | 8 bytes  |
| Checksum        | 8 bytes  |
| CompressionType | 2 bytes  |
| EncryptionType  | 2 bytes  |
| Reserved        | 26 bytes |

Magic - 0x56504600 ('V', 'P', 'F', 0)<br>
Version - 0x01000 - Version code of VPF specification<br>
Flags - Bool flags:<br>
            1: Compression
            2: Encryption
            [6]: Reserved
Checksum - Only Path Table & Header & File Entry's & Collision Entry's 

## Path Table Entry (24 bytes)
| Field      | Size     |
|------------|----------|
| HashPath   | 16 bytes |
| DataOffset | 8 bytes  |

DataOffset - Offset to data or collision list
### File Entry Header (20 bytes)
| Field    | Size    |
|----------|---------|
| Magic    | 4 bytes |
| FileSize | 8 bytes |
| Checksum | 8 bytes |

Magic - 0x88 <br>
### Collision Entry
| Field         | Size     |
|---------------|----------|
| Magic         | 4 bytes  |
| HashPath      | 16 bytes |
| RealPathSize  | 2 bytes  |
| RealPath      | relative |
| DataOffset    | 8 bytes  |
| NextCollision | 8 bytes  |

Magic - 0x87 <br>
NextCollision - Will be -1 if next collision doesn't exist
```c++
VPF::Package package;
package.open("assets.vpf");

Image::Image image;
if (package.has("textures/hazmat.png")) {
    image = Image::Image(
        package.read("textures/hazmat.png")
    )
}

```
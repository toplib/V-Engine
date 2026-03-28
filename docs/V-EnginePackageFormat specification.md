# V-Engine Package Format (VPF)
## VPF 1.0.0
Endianness: little-endian
Checksum hash function: xxHash128

### Header (64 bytes)
Start of the file

| Field           | Size     |
|-----------------|----------|
| Magic           | 4 bytes  |
| Version         | 4 bytes  |
| Flags           | 2 bytes  |
| HashTableSize   | 8 bytes  |
| FileCount       | 8 bytes  |
| Checksum        | 8 bytes  |
| CompressionType | 2 bytes  |
| EncryptionType  | 2 bytes  |
| Reserved        | 26 bytes |

Magic - 0x56504600 ('V', 'P', 'F', 0)<br>
Version - 0x00010000 - Version code of VPF specification<br>
Flags - Bool flags:<br>
0: Compression<br>
1: Encryption<br>
[2-15]: Reserved<br>
HashTableSize - Size of the Path Table in bytes. Path Table starts at offset sizeof(Header)<br>
FileCount - Number of files stored in the archive<br>
Checksum - Calculated over Header (Checksum field zeroed), Path Table, File Entry's & Collision Entry's

## Path Table Entry (24 bytes)
| Field      | Size     |
|------------|----------|
| HashPath   | 16 bytes |
| DataOffset | 8 bytes  |

HashPath - xxHash128 of the lowercase UTF-8 path with '/' separator<br>
DataOffset - Absolute offset from start of file to File Entry or Collision Entry. 0 if slot is empty.

### File Entry (20 bytes)
| Field    | Size    |
|----------|---------|
| Magic    | 4 bytes |
| FileSize | 8 bytes |
| Checksum | 8 bytes |

Magic - 0x76706666<br>
FileSize - Size of the uncompressed file data in bytes<br>
Checksum - xxHash128 of the uncompressed file data<br>
File data follows immediately after File Entry

### Collision Entry
| Field         | Size     |
|---------------|----------|
| Magic         | 4 bytes  |
| HashPath      | 16 bytes |
| RealPathSize  | 2 bytes  |
| DataOffset    | 8 bytes  |
| NextCollision | 8 bytes  |
| RealPath      | relative |

Magic - 0x76706663<br>
HashPath - xxHash128 of the lowercase UTF-8 path with '/' separator<br>
DataOffset - Absolute offset from start of file to File Entry<br>
NextCollision - Absolute offset from start of file to next Collision Entry. 0xFFFFFFFFFFFFFFFF if none.<br>
RealPath - UTF-8 encoded path, length defined by RealPathSize

```c++
VPF::Package package;
package.open("assets.vpf");

Image::Image image;
if (package.has("textures/hazmat.png")) {
    image = Image::Image(
        package.read("textures/hazmat.png"),
        
    )
}

```
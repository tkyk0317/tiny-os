#include "elf/loader.hpp"

// ELF構造体
ELF64Data ELFLoader::elf;

/**
 * ELFデータ解析
 */
void ELFLoader::analysis(const uint8_t* data)
{
    // ヘッダロード
    ELFLoader::loadHeader(data);
}

/**
 * ヘッダロード
 */
void ELFLoader::loadHeader(const uint8_t* data)
{
    uint8_t* h = const_cast<uint8_t*>(data);

    // read ident
    for (uint64_t i = 0; i < 16; i++) ELFLoader::elf.header.ident[i] = *h++;

    // read type
    ELFLoader::elf.header.type = *reinterpret_cast<uint16_t*>(h);
    h += sizeof(ELFLoader::elf.header.type);

    // read machine
    ELFLoader::elf.header.machine = *reinterpret_cast<uint16_t*>(h);
    h += sizeof(ELFLoader::elf.header.machine);

    // read version
    ELFLoader::elf.header.version = *reinterpret_cast<uint32_t*>(h);
    h += sizeof(ELFLoader::elf.header.version);

    // read entry
    ELFLoader::elf.header.entry = *reinterpret_cast<uint64_t*>(h);
    h += sizeof(ELFLoader::elf.header.entry);

    // read phoff
    ELFLoader::elf.header.phoff = *reinterpret_cast<uint64_t*>(h);
    h += sizeof(ELFLoader::elf.header.phoff);

    // read shoff
    ELFLoader::elf.header.shoff = *reinterpret_cast<uint64_t*>(h);
    h += sizeof(ELFLoader::elf.header.shoff);

    // read flags
    ELFLoader::elf.header.flags = *reinterpret_cast<uint32_t*>(h);
    h += sizeof(ELFLoader::elf.header.flags);

    // read ensize
    ELFLoader::elf.header.ehsize = *reinterpret_cast<uint16_t*>(h);
    h += sizeof(ELFLoader::elf.header.ehsize);

    // read phentsize
    ELFLoader::elf.header.phentsize = *reinterpret_cast<uint16_t*>(h);
    h += sizeof(ELFLoader::elf.header.phentsize);

    // read phnum
    ELFLoader::elf.header.phnum = *reinterpret_cast<uint16_t*>(h);
    h += sizeof(ELFLoader::elf.header.phnum);

    // read shentsize
    ELFLoader::elf.header.shentsize = *reinterpret_cast<uint16_t*>(h);
    h += sizeof(ELFLoader::elf.header.shentsize);

    // read shnum
    ELFLoader::elf.header.shnum = *reinterpret_cast<uint16_t*>(h);
    h += sizeof(ELFLoader::elf.header.shnum);

    // read shstrndx
    ELFLoader::elf.header.shstrndx = *reinterpret_cast<uint16_t*>(h);
    h += sizeof(ELFLoader::elf.header.shstrndx);
}

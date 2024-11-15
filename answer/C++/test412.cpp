#ifndef BINARYSTDIO_H
#define BINARYSTDIO_H

import std;

class BinaryStdIn
{
  private:
    std::istream &in;
    unsigned char buffer = 0;
    int bitsLeft = 0;

  public:
    explicit BinaryStdIn(std::istream &input = std::cin);
    ~BinaryStdIn() = default;

    [[nodiscard]] auto isEmpty() const -> bool;
    auto readBit() -> bool;
    auto readByte() -> unsigned char;
    void readBytes(std::span<unsigned char> buffer);
};

class BinaryStdOut
{
  private:
    std::ostream &out;
    unsigned char buffer = 0;
    int bitsLeft = 0;

  public:
    explicit BinaryStdOut(std::ostream &output = std::cout);
    ~BinaryStdOut();

    void writeBit(bool bit);
    void writeByte(unsigned char byte);
    void writeBytes(std::span<const unsigned char> buffer);
    void flush();
};

#endif // BINARYSTDIO_H

BinaryStdIn::BinaryStdIn(std::istream &input)
    : in(input)
{}

auto BinaryStdIn::isEmpty() const -> bool
{
    // 使用模块无法使用宏 EOF(-1)
    return in.peek() == std::istream::traits_type::eof();
}

auto BinaryStdIn::readBit() -> bool
{
    if (bitsLeft == 0)
    {
        buffer = in.get();
        bitsLeft = 8;
    }

    bool bit = (buffer & 0x80) != 0;
    buffer <<= 1;
    bitsLeft--;

    return bit;
}

auto BinaryStdIn::readByte() -> unsigned char
{
    unsigned char byte = 0;
    for (int i = 0; i < 8; i++)
    {
        byte = (byte << 1) | static_cast<int>(readBit());
    }
    return byte;
}

void BinaryStdIn::readBytes(std::span<unsigned char> buffer)
{
    for (auto &i : buffer)
    {
        i = readByte();
    }
}

BinaryStdOut::BinaryStdOut(std::ostream &output)
    : out(output)
{}

BinaryStdOut::~BinaryStdOut()
{
    flush();
}

void BinaryStdOut::writeBit(bool bit)
{
    buffer = (buffer << 1) | (bit ? 1 : 0);
    bitsLeft++;

    if (bitsLeft == 8)
    {
        out.put(buffer);
        buffer = 0;
        bitsLeft = 0;
    }
}

void BinaryStdOut::writeByte(unsigned char byte)
{
    for (int i = 7; i >= 0; i--)
    {
        writeBit(((byte >> i) & 1) != 0);
    }
}

void BinaryStdOut::writeBytes(std::span<const unsigned char> buffer)
{
    for (auto i : buffer)
    {
        writeByte(i);
    }
}

void BinaryStdOut::flush()
{
    if (bitsLeft > 0)
    {
        out.put(buffer << (8 - bitsLeft));
        buffer = 0;
        bitsLeft = 0;
    }
}

auto main() -> int
{
    BinaryStdOut binStdOut;

    // 写入一些二进制数据
    binStdOut.writeByte('H');
    binStdOut.writeByte('e');
    binStdOut.writeByte('l');
    binStdOut.writeByte('l');
    binStdOut.writeByte('o');
    binStdOut.writeByte(' ');

    unsigned char world[] = "world!";
    binStdOut.writeBytes(world);

    binStdOut.flush();

    BinaryStdIn binStdIn;

    // 读取并打印每个字节
    unsigned char byte;
    while (!binStdIn.isEmpty())
    {
        byte = binStdIn.readByte();
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    return 0;
}
﻿#include <gtest/gtest.h>

#include <pods/binary_serializer.h>
#include <pods/memory_storage.h>

#include "data.h"

TEST(binarySerializer, common)
{
    const TestData expected;

    pods::ResizeableWriteOnlyMemoryStorage out;
    pods::BinarySerializer<pods::ResizeableWriteOnlyMemoryStorage> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    TestData actual;

    actual.x = 0;
    actual.y = 0;
    actual.z = std::string();
    actual.a.floatData = { 0, 0, 0, 0, 0 };
    actual.b.c.b = true;
    actual.b.c.e1 = AA;
    actual.b.c.e2 = Enum2::Y;
    actual.points.clear();
    actual.dict.clear();
    actual.strings.clear();
    actual.arr = { { 0, 0 } };
    actual.map.clear();

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    pods::BinaryDeserializer<pods::ReadOnlyMemoryStorage> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x, actual.x);
    EXPECT_EQ(expected.y, actual.y);
    EXPECT_EQ(expected.z, actual.z);
    EXPECT_EQ(expected.a.floatData, actual.a.floatData);
    EXPECT_EQ(expected.b.c.b, actual.b.c.b);
    EXPECT_EQ(expected.b.c.e1, actual.b.c.e1);
    EXPECT_EQ(expected.b.c.e2, actual.b.c.e2);
    EXPECT_EQ(expected.points, actual.points);
    EXPECT_EQ(expected.dict, actual.dict);
    EXPECT_EQ(expected.strings, actual.strings);
    EXPECT_EQ(expected.arr, actual.arr);
    EXPECT_EQ(expected.map, actual.map);
}

TEST(binarySerializer, binary1)
{
    const BinData1 expected;

    pods::ResizeableWriteOnlyMemoryStorage out;
    pods::BinarySerializer<pods::ResizeableWriteOnlyMemoryStorage> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    BinData1 actual;
    std::fill(actual.x.begin(), actual.x.end(), 0);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    pods::BinaryDeserializer<pods::ReadOnlyMemoryStorage> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x, actual.x);
}

TEST(binarySerializer, binary2)
{
    int16_t buf1[BinData2::size];
    int16_t buf2[BinData2::size];

    const BinData2 expected = { std::make_unique<int16_t[]>(expected.size), buf1 };

    for (int16_t i = 0; i < expected.size; ++i)
    {
        expected.x.get()[i] = i + 1;
        expected.y[i] = i + 10;
    }

    pods::ResizeableWriteOnlyMemoryStorage out;
    pods::BinarySerializer<pods::ResizeableWriteOnlyMemoryStorage> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    BinData2 actual = { std::make_unique<int16_t[]>(expected.size), buf2 };
    std::fill_n(actual.x.get(), expected.size, 0);
    std::fill_n(actual.y, expected.size, 0);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    pods::BinaryDeserializer<pods::ReadOnlyMemoryStorage> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    for (int16_t i = 0; i < expected.size; ++i)
    {
        EXPECT_EQ(actual.x.get()[i], i + 1);
        EXPECT_EQ(actual.y[i], i + 10);
    }
}

TEST(binarySerializer, binaryLarger)
{
    const BinData1 expected;

    pods::ResizeableWriteOnlyMemoryStorage out;
    pods::BinarySerializer<pods::ResizeableWriteOnlyMemoryStorage> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    BinData1 actual;
    actual.x.clear();

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    pods::BinaryDeserializer<pods::ReadOnlyMemoryStorage> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x, actual.x);
}

TEST(binarySerializer, binaryLargerArray)
{
    const Array1 expected;

    pods::ResizeableWriteOnlyMemoryStorage out;
    pods::BinarySerializer<pods::ResizeableWriteOnlyMemoryStorage> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Array2 actual;

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    pods::BinaryDeserializer<pods::ReadOnlyMemoryStorage> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::CorruptedArchive);
}

TEST(binarySerializer, binarySmallerArray)
{
    const Array2 expected;

    pods::ResizeableWriteOnlyMemoryStorage out;
    pods::BinarySerializer<pods::ResizeableWriteOnlyMemoryStorage> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Array1 actual;

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    pods::BinaryDeserializer<pods::ReadOnlyMemoryStorage> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::CorruptedArchive);
}

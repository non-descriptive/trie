#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "Trie.hpp"


class TrieExistence: public ::testing::Test
{
protected:
    Trie<int> trie;
    static const char* KEY_EXISTS_DATA;
    static const char* KEY_EXISTS_OVERLAP_DATA;
    static const char* KEY_EXISTS_OVERLAP_NO_DATA;
    static const char* KEY_EXISTS_NO_OVERLAP_DATA;
    static const char* KEY_NOT_EXIST_NO_OVERLAP_NO_DATA;
    static const char* KEY_NOT_EXISTS;
    static const usize SIZE_EXISTS = 10;
    static const usize SIZE_OVERLAP = 8;
    static const usize SIZE_NOT_EXISTS = 255;
public:
     virtual void SetUp() override;

};



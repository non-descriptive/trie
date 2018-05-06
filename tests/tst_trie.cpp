#include "tst_trie.h"


TEST(strlen, _0_9)
{
    constexpr const char* t1 = "123456789";
    ASSERT_EQ(strlen_(t1), 9);
}
TEST(strlen, hello_world)
{
    constexpr const char* t2 = "hello, world!";
    ASSERT_EQ(strlen_(t2), 13);
}
TEST(strlen, null)
{
    constexpr const char* t3 = nullptr;
    constexpr const char* t4 = "\0";
    ASSERT_EQ(strlen_(t3), 0);
    ASSERT_EQ(strlen_(t4), 0);
}

constexpr const int VALUE_DEFAULT = -1;
constexpr const int DATA_NORMAL = 3;
constexpr const int DATA_OVERLAPPED = 0;
constexpr const int DATA_NO_OVERLAP = 4;
constexpr const int DATA_NOT_EXISTED_YET = 5;

const char* TrieExistence::KEY_EXISTS_DATA            = "1234567890";
const char* TrieExistence::KEY_EXISTS_OVERLAP_DATA    = "12345678";
const char* TrieExistence::KEY_EXISTS_OVERLAP_NO_DATA = "12345";
const char* TrieExistence::KEY_EXISTS_NO_OVERLAP_DATA = "0987654321";
const char* TrieExistence::KEY_NOT_EXIST_NO_OVERLAP_NO_DATA = "098765";

const char* TrieExistence::KEY_NOT_EXISTS             = "1234777";

void TrieExistence::SetUp()
{
    trie.insert(KEY_EXISTS_DATA, DATA_NORMAL);
    trie.insert(KEY_EXISTS_OVERLAP_DATA, DATA_OVERLAPPED);
    trie.insert(KEY_EXISTS_NO_OVERLAP_DATA, DATA_NO_OVERLAP);
}


TEST_F(TrieExistence, exists)
{
    ASSERT_TRUE(trie.exists(KEY_EXISTS_DATA));
}

TEST_F(TrieExistence, not_exists)
{
    ASSERT_FALSE(trie.exists(KEY_NOT_EXISTS));
}

TEST_F(TrieExistence, exists_after_insert)
{
    trie.insert(KEY_NOT_EXISTS, DATA_NOT_EXISTED_YET);
    ASSERT_TRUE(trie.exists(KEY_NOT_EXISTS));
}

TEST_F(TrieExistence, exists_overlap)
{
    ASSERT_TRUE(trie.exists((KEY_EXISTS_OVERLAP_NO_DATA)));
}

TEST_F(TrieExistence, exists_sz_overlap)
{
    ASSERT_TRUE(trie.exists(KEY_EXISTS_DATA, SIZE_EXISTS));
}

TEST_F(TrieExistence, not_exists_sz_overlap)
{
    ASSERT_FALSE(trie.exists(KEY_EXISTS_DATA, SIZE_NOT_EXISTS));
}

TEST_F(TrieExistence, has_data_existent_key)
{
    ASSERT_TRUE(trie.has_data(KEY_EXISTS_DATA));
}

TEST_F(TrieExistence, has_data_non_existent_key)
{
    ASSERT_TRUE(trie.has_data(KEY_EXISTS_DATA));
}

TEST_F(TrieExistence, has_no_data_overlap_key)
{
    ASSERT_FALSE(trie.has_data(KEY_EXISTS_OVERLAP_NO_DATA));
}

TEST_F(TrieExistence, has_data_overlap_key)
{
    ASSERT_TRUE(trie.has_data(KEY_EXISTS_OVERLAP_DATA));
}

TEST_F(TrieExistence, has_data_overlap_sz_partial)
{
    ASSERT_TRUE(trie.has_data(KEY_EXISTS_DATA, SIZE_OVERLAP));
}

TEST_F(TrieExistence, get_data_existent)
{
    ASSERT_EQ(trie.at(KEY_EXISTS_DATA, VALUE_DEFAULT), DATA_NORMAL);
}

TEST_F(TrieExistence, get_data_not_existent)
{
    ASSERT_EQ(trie.at(KEY_NOT_EXISTS, VALUE_DEFAULT), VALUE_DEFAULT);
}


TEST_F(TrieExistence, get_data_overlap_existent)
{
    ASSERT_EQ(trie.at(KEY_EXISTS_OVERLAP_DATA, VALUE_DEFAULT), DATA_OVERLAPPED);
}

TEST_F(TrieExistence, get_data_sz_overlap_normal)
{
    ASSERT_EQ(trie.at(KEY_EXISTS_OVERLAP_DATA, SIZE_OVERLAP, VALUE_DEFAULT), DATA_OVERLAPPED);
}

TEST_F(TrieExistence, get_data_sz_overlap_partial)
{
    ASSERT_EQ(trie.at(KEY_EXISTS_DATA, SIZE_OVERLAP, VALUE_DEFAULT), DATA_OVERLAPPED);
}

TEST_F(TrieExistence, remove_data_existent)
{
    trie.remove(KEY_EXISTS_NO_OVERLAP_DATA);
    ASSERT_FALSE(trie.has_data(KEY_EXISTS_NO_OVERLAP_DATA));
}

TEST_F(TrieExistence, remove_data_key_clear)
{
    trie.remove(KEY_EXISTS_NO_OVERLAP_DATA);
    ASSERT_FALSE(trie.exists(KEY_NOT_EXIST_NO_OVERLAP_NO_DATA));
}

TEST_F(TrieExistence, remove_data_overlap)
{
    trie.remove(KEY_EXISTS_OVERLAP_DATA);
    ASSERT_TRUE(trie.exists(KEY_EXISTS_DATA, SIZE_OVERLAP));
}

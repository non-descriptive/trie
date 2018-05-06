#ifndef TRIE_HPP
#define TRIE_HPP

typedef unsigned long usize;

usize strlen_(const char* str);

template<typename TData>
class Trie
{
    static const usize DEFAULT_SIZE = 4;
    struct KeyNode
    {
        usize     count   ; // количество детей
        usize     reserved; // зарезервировано под указатели на детей
        KeyNode** children; // дети
        char      key     ; // собственное значение
        TData     value   ; // хранимое значение
        bool      withData; // флаг данных
        KeyNode() : count(0) , reserved(0) , children(nullptr) , key('\0') , withData(false)
        {
        }
        ~KeyNode()
        {
            for (usize i = 0; i < count; ++i)
            {
                delete children[i];
            }
            delete [] children;
        }

        /**
         * @brief резервирует место под указатели на детей
         * @param size - количество резервируемого места
         */
        void reserve(usize size)
        {
            children = new KeyNode*[size];
            reserved = size;
        }

        bool exists(const char* key, usize ksize) const
        {
            bool isExists = false;
            if (key and ksize > 0 and count > 0)
            {
                const char* next_key = key + 1;
                usize next_size = ksize - 1;
                char k = *key;
                for (usize i = 0; i < count; ++i)
                {
                    const KeyNode* child = children[i];
                    isExists = child->key == k;
                    if (isExists)
                    {
                        if (next_size > 0)
                        {
                            return child->exists(next_key, next_size);
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            return isExists;
        }

        /**
         * @brief перевыделяем зарезервированную память
         */
        void relocate()
        {
            KeyNode** tmp_ptr = children;
            usize sz_new = reserved * 2;
            children = new KeyNode*[sz_new];
            for (usize i = 0; i < reserved; ++i)
            {
                children[i] = tmp_ptr[i];
            }
            delete [] tmp_ptr;
            reserved = sz_new;
        }

        void insert(const char* key, usize ksize, TData data)
        {
            if (key and ksize > 0)
            {
                usize len = 0;
                KeyNode* ptr = this;
                while (len != ksize)
                {
                    bool isExist = false;
                    char k = *(key + len);
                    for (usize i = 0; i < ptr->count; ++i)
                    {
                        KeyNode* child = ptr->children[i];
                        if (child->key == k)
                        {
                            ptr = child;
                            ++len;
                            isExist = true;
                        }
                    }
                    if (not isExist)
                    {
                        if (ptr->count == ptr->reserved)
                        {
                            ptr->relocate();
                        }
                        ptr->children[ptr->count] = new KeyNode;
                        KeyNode* child = ptr->children[ptr->count];
                        ptr->count++;
                        child->reserve(DEFAULT_SIZE);
                        child->key = k;
                        ++len;
                        ptr = child;
                    }
                }
                ptr->value = data;
                ptr->withData = true;
            }
        }

        KeyNode* findNode(const char* key, usize ksize)
        {
            if (ksize == 0)
            {
                return this;
            }
            if (exists(key, ksize))
            {
                usize len = 0;
                KeyNode* ptr = this;
                while (len != ksize)
                {
                    char k = *(key + len);
                    for (usize i = 0; i < ptr->count; ++i)
                    {
                        KeyNode* child = ptr->children[i];
                        if (child->key == k)
                        {
                            ptr = child;
                            ++len;
                            break;
                        }
                    }
                }
                return ptr;
            }
            return nullptr;
        }

        bool hasData(const char* key, usize ksize)
        {
            KeyNode* ptr = findNode(key, ksize);
            return ptr and ptr->withData;
        }

        TData getData(const char* key, usize ksize)
        {
            KeyNode* ptr = findNode(key, ksize);
            if (ptr)
            {
                return ptr->value;
            }
        }

        void remove(const char* key, usize ksize)
        {
            if (exists(key, ksize))
            {
                usize del = 0;

                usize len = 0;
                KeyNode* ptr = this;
                while (len != ksize)
                {
                    char k = *(key + len);

                    for (usize i = 0; i < ptr->count; ++i)
                    {
                        KeyNode* child = ptr->children[i];
                        if (child->key == k)
                        {
                            if (ptr->count > 1)
                            {
                                del = len;
                            }

                            ptr = child;
                            ++len;

                            break;
                        }
                    }
                }

                KeyNode* del_ptr = findNode(key, del);
                char k = *(key + del);
                bool deleted = false;
                for (usize i = 0; i < del_ptr->count; ++i)
                {
                    KeyNode* child = del_ptr->children[i];
                    if (child->key == k)
                    {
                        delete child;
                        child = nullptr;
                        del_ptr->count--;
                        deleted = true;
                    }
                    else if (deleted)
                    {
                        del_ptr->children[i-1] = child;
                    }
                }
            }

        }
    };
public:
    Trie()
    {
        _root = new KeyNode;
        _root->reserve(DEFAULT_SIZE);
    }

    /**
     * @brief вставка по обычному ключу
     * @param key - ключ
     * @param value - значение
     */
    void insert(const char* key, TData value)
    {
        const usize key_sz = strlen_(key);
        insert(key, key_sz, value);
    }

    /**
     * @brief вставка по срезу
     * @param key - указатель на строку
     * @param key_sz - размер ключа
     * @param value - значение
     */
    void insert(const char* key, usize ksize, TData value)
    {
        _root->insert(key, ksize, value);
    }

    /**
     * @brief проверка существования по ключу
     * @param key - ключ
     * @return true если существует ключ
     */
    bool exists(const char* key) const
    {
        usize sz_key = strlen_(key);
        return exists(key, sz_key);
    }

    /**
     * @brief проверка существования по ключу
     * @param key - ключ
     * @param size - размер среза ключа
     * @return true если существует ключ для среза
     */
    bool exists(const char* key, usize size) const
    {
        return _root->exists(key, size);
    }

    /**
     * @brief проверяет наличие данных по ключу
     * @param key - ключ
     * @return true если существуют данные по ключу
     */
    bool has_data(const char* key) const
    {
        usize sz_key = strlen_(key);
        return has_data(key, sz_key);
    }

    /**
     * @brief проверяет наличие данных по срезу ключа
     * @param key ключ
     * @param ksize - размер среза
     * @return true - если обнаружены данные по срезу
     */
    bool has_data(const char* key, usize size) const
    {
        return _root->hasData(key, size);
    }

    TData at(const char* key)
    {
        // Можно было бы сделать статическую проверку, но мы не используем std
#ifdef USE_STD
        static_assert(std::is_default_constructible<TData>::value,
                      "Data Structure requires default-constructible elements");
#endif
        usize sz_key = strlen_(key);
        return at(key, {});
    }
    TData at(const char* key, const TData& default_value)
    {
        usize sz_key = strlen_(key);
        return at(key, sz_key, default_value);
    }
    TData at(const char* key, usize ksize, const TData& default_value)
    {
        if (has_data(key, ksize))
        {
            return _root->getData(key, ksize);
        }
        return default_value;
    }

    void remove(const char* key)
    {
        usize sz_key = strlen_(key);
        remove(key, sz_key);
    }

    void remove(const char* key, usize ksize)
    {
        _root->remove(key, ksize);
    }

private:
    KeyNode* _root;
};

#endif // TRIE_HPP

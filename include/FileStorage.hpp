#ifndef FILESTORAGE_INCLUDED
#define FILESTORAGE_INCLUDED

#include <string>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <fstream>
#include <stdexcept>
#include <cstdio>
#include <set>

/* FILE FORMAT
   (flags) flags
   (sizeof(T) + sizeof(bool))[] data */

// Only serializers constant size objects
template <typename T>
class ConstantSizeSerializer
{
    public:
        std::unique_ptr<char> allocate_ptr() const {
            return std::unique_ptr<char>(new char[serialize_size()]);
        }
        
        std::unique_ptr<char> serialize(const T& in) const {
            std::unique_ptr<char> ret(allocate_ptr());
            serialize(in, ret.get());
            return ret;
        }

        T deserialize(const std::unique_ptr<char>& in) const {
            return deserialize(in.get());
        }

        T deserialize(const char* in) const {
            T out;
            deserialize(in, out);
            return out;
        }

        virtual size_t serialize_size() const = 0;
        virtual void serialize(const T& in, char* out) const = 0;
        virtual void deserialize(const char* in, T& out) const = 0;
};

template <typename T>
class BinarySerializer : public ConstantSizeSerializer<T> {
    public:
        using ConstantSizeSerializer<T>::deserialize;
        using ConstantSizeSerializer<T>::serialize;

        virtual size_t serialize_size() const override {
            return sizeof(T);
        }

        virtual void serialize(const T& in, char* out) const override {
            const char* data = reinterpret_cast<const char*>(&in);
            std::copy(data, data+serialize_size(), out);
        }

        virtual void deserialize(const char* in, T& out) const override {
            out = *reinterpret_cast<const T*>(in);
        }
};

template <typename T,
          class Serializer = BinarySerializer<T>> 
class FileStorage {
    private:
        const Serializer serializer;

    public:
        typedef enum FileFlags {
            CLEAN = 1,
            EMPTY = 2
        } FileFlags;

        const FileFlags DEFAULT_FLAGS = (FileFlags)(FileFlags::CLEAN | FileFlags::EMPTY);

        FileStorage(std::string path) : path(path)
        {
            static_assert(std::is_base_of<ConstantSizeSerializer<T>, Serializer>::value, 
                          "Serializer must derive from DataSerializer");

            using std::fstream;

            fstream file(path, std::ifstream::binary);
            if (!file.good()) { // if file does not exist, creates it
                file.open(path, fstream::out | fstream::binary | fstream::trunc);
                file.write(reinterpret_cast<const char*>(&DEFAULT_FLAGS), sizeof(FileFlags));
            }
        }

        FileFlags flags() {
            std::ifstream file(path, std::ifstream::in | std::ifstream::binary);
            return flags(file);
        }

        bool clean() {
            return flags() & FileFlags::CLEAN;
        }

        bool empty() {
            return flags() & FileFlags::EMPTY;
        }

        void remove(size_t index) {
            std::ofstream file(path, std::ofstream::binary);
            setOutPosition(file, index);
            if (!file.eof())
                file << false;
        }

        void write(const T& data, size_t index = -1) {
            std::ofstream file(path, std::ofstream::out | std::ofstream::binary | std::ofstream::app);
            if (index >= 0)
                setOutPosition(file, index);
            bool valid = true;
            file.write(reinterpret_cast<const char*>(&valid), sizeof(bool));
            file.write(serializer.serialize(data).get(), serializer.serialize_size());
        }

        T read(size_t index) {
            std::ifstream file(path, std::ifstream::in | std::ifstream::binary)
            std::pair<bool, T> data(readln(file, index));
            return data.second;
        }

        std::set<size_t> rewrite() {
            std::fstream file(path);
            std::string tmpFilePath = tmpnam(nullptr);
            std::fstream tmp(tmpFilePath, std::fstream::out | std::fstream::trunc);

            // copies file flags to temporary file
            tmp << (int)(flags(file) & FileFlags::EMPTY);
            // positions read cursor at first record
            setInPosition(file, 0);

            std::set<size_t> invalidatedIndexes;
            size_t index = 0;
            while (!file.eof()) {
                bool valid;
                std::unique_ptr<char> data;
                file >> valid;
                file.read(data, serializer.serialize_size());
                if (valid) {
                    tmp << true;
                    tmp.write(serializer.serialize(data), serializer.serialize_size());
                }
                else
                    invalidatedIndexes.insert(index);

                index++;
            }

            // reopens both files and copies temporary file content into other file
            file.open(path, std::fstream::out | std::fstream::trunc);
            file.open(tmpFilePath, std::fstream::in);
            file << tmp.rdbuf();

            // deletes temporary file
            tmp.close();
            remove(tmpFilePath);

            return invalidatedIndexes;
        }

        friend std::ostream& operator<<(std::ostream& os, const FileStorage<T>& s) {
            std::ifstream file(s.path, std::ifstream::in | std::ifstream::binary);
            int i;
            file.read(reinterpret_cast<char*>(&i), sizeof(int));
            os << "Flags: " << i << std::endl;
            while (!file.eof()) {
                std::pair<bool, T> data(s.readln(file));

                if (data.first)
                    os << "valid";
                else 
                    os << "invalid";

                os << " " << data.second << std::endl;
            }

            return os;
        }

    private:
        std::string path;

        FileFlags flags(std::ifstream& file) const {
            std::streampos startPos = file.tellg();
            file.seekg(0, file.beg);
            int i;
            file.read(reinterpret_cast<char*>(&i), sizeof(FileFlags));
            file.seekg(startPos, file.beg);
            return static_cast<FileFlags>(i);
        }

        std::pair<bool, T> readln(std::ifstream& file, size_t line = -1) const {
            if (line > -1)
                setInPosition(file, line);

            bool valid;
            file.read(reinterpret_cast<char*>(&valid), sizeof(bool));

            std::unique_ptr<char> bytes(serializer.allocate_ptr());
            file.read(bytes.get(), serializer.serialize_size());
            T data(serializer.deserialize(bytes));

            return std::pair<bool, T>(valid, data);
        }

        void setOutPosition(std::ofstream& file, size_t offset) {
            file.seekp(sizeof(FileFlags) + offset * (sizeof(bool) + serializer.serialize_size()), file.beg);
        }

        void setInPosition(std::ifstream& file, size_t offset) const {
            file.seekg(sizeof(FileFlags) + offset * (sizeof(bool) + serializer.serialize_size()), file.beg);
        }
};

#endif
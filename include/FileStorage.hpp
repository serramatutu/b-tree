#ifndef FILESTORAGE_INCLUDED
#define FILESTORAGE_INCLUDED

#include <string>
#include <memory>
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
        static virtual size_t serialize_size() const = 0;
        static virtual void serialize(const T& in, char* out) const = 0;
        static virtual void deserialize(const char* in, T& out) const = 0;
};

template <typename T>
class BinarySerializer : ConstantSizeSerializer {
    public:
        static virtual size_t serialize_size() const {
            return sizeof(T);
        }

        static virtual void serialize(const T& in, char* out) const {
            out = reinterpret_cast<char*>(in);
        }

        static virtual void deserialize(char* in, T& out) const {
            out = reinterpret_cast<T&>(in);
        }
};

template <typename T,
          class Serializer = BinarySerializer<T>> 
class FileStorage {

    public:
        typedef enum FileFlags {
            CLEAN = 1,
            EMPTY = 2
        } FileFlags;

        const FileFlags DEFAULT_FLAGS = (FileFlags)(FileFlags::CLEAN | FileFlags::EMPTY);

        FileStorage(std::string path) : path(path)
        {
            static_assert(std::is_base_of<ConstantSizeSerializer, Serializer>()::value, 
                          "Serializer must derive from DataSerializer")

            using std::fstream;

            fstream file(path);
            if (!file.good()) { // if file does not exist, creates it
                file.open(path, fstream::out | fstream::trunc);
                file << DEFAULT_FLAGS;
            }
        }

        FileFlags flags() {
            return flags(std::ifstream(path));
        }

        bool clean() {
            return flags() & FileFlags::CLEAN;
        }

        bool empty() {
            return flags() & FileFlags::EMPTY;
        }

        void remove(size_t index) {
            std::ofstream file(path);
            setOutPosition(file, index);
            if (!file.eof())
                file << false;
        }

        void write(const T& data, size_t index = -1) {
            std::ofstream file(path, std::ofstream::out | std::ofstream::app);
            if (index >= 0)
                setOutPosition(file, index);
            file << true;
            file.write(Serializer.serialize(data), Serializer.serialize_size());
        }

        T read(size_t index) {
            std::ifstream file(path);
            setInPosition(file, index);
            shared_ptr<char> bytes;
            file.read(bytes, Serializer.serialize_size());
            return Serializer.deserialize(bytes);
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
                T data;
                file >> valid;
                file.read(&data, Serializer.serialize_size());
                if (valid) {
                    tmp << true;
                    tmp.write(data, Serializer.serialize_size());
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
            std::ifstream file(s.path);
            int i;
            file >> i;
            os << "Flags: " << (FileFlags)i << std::endl;
            while (!file.eof()) {
                bool valid;
                T data;
                file >> valid >> data;
                if (valid)
                    os << "valid";
                else 
                    os << "invalid";

                os << " " << data << std::endl;
            }

            return os;
        }

    private:
        std::string path;

        FileFlags flags(std::ifstream& file) {
            std::streampos startPos = file.tellg();
            file.seekg(0, file.beg);
            int i;
            file >> i;
            file.seekg(startPos, file.beg);
            return static_cast<FileFlags>(i);
        }

        static void setOutPosition(std::ofstream& file, size_t offset) {
            file.seekp(sizeof(FileFlags) + offset * (sizeof(bool) + Serializer.serialize_size()), file.beg);
        }

        static void setInPosition(std::ifstream& file, size_t offset) {
            file.seekg(sizeof(FileFlags) + offset * (sizeof(bool) + Serializer.serialize_size()), file.beg);
        }
};

#endif
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
        typedef struct FileHeader {
            FileHeader(size_t size = 0, bool clean = true) 
                : size(size), clean(clean) {}

            bool clean;
            size_t size;

            friend std::ostream operator<<(std::ostream& os, const FileHeader& f) {
                if (f.clean)
                    os << "clean" << std::endl;
                else
                    os << "not clean" << std::endl;

                os << "size: " << f.size << std::endl;
            }

            operator const char*() const {
                return reinterpret_cast<const char*>(this);
            }
        } FileHeader;

        template <typename RType>
        struct Register {
            Register() : Register(RType()) {}
            Register(const T& data) : valid(true), data(data) {}

            bool valid;
            const RType& data;

            template <typename U>
            friend std::ostream operator<<(std::ostream& os, const Register<U>& r) {
                if (r.valid)
                    os << "valid";
                else
                    os << "invalid";
                os << " " << r.data;
            }

            operator const char*() const {
                Serializer serializer;
                return (std::string(1, (char)valid) + 
                       std::string(serializer.serialize(data).get())).c_str();
            }

            char* c_ptr() {
                return reinterpret_cast<char*>(&valid);
            }
        };

        FileStorage(std::string path) : path(path)
        {
            static_assert(std::is_base_of<ConstantSizeSerializer<T>, Serializer>::value, 
                          "Serializer must derive from DataSerializer");

            using std::ofstream;

            ofstream file(path, std::ifstream::binary);
            if (!file.good()) { // if file does not exist, creates it
                file.open(path, ofstream::out | ofstream::binary | ofstream::trunc);
                writeHeader(file, FileHeader());
            }
        }

        FileHeader header() {
            std::ifstream file(path, std::ifstream::in | std::ifstream::binary);
            return header(file);
        }

        bool clean() {
            return header().clean;
        }

        bool empty() {
            return header().size == 0;
        }

        void remove(size_t index) {
            std::ofstream file(path, std::ofstream::binary);
            setOutPosition(file, index);
            if (!file.eof())
                file << false;
        }

        void write(const T& data, size_t index = 0) {
            std::ofstream file(path, std::ofstream::out | std::ofstream::binary);
            setOutPosition(file, index);
            file.write(Register<T>(data), sizeof(Register<T>));
        }

        T read(size_t index) {
            std::ifstream file(path, std::ifstream::in | std::ifstream::binary);
            Register<T> r(readln(file, index));
            return r.data;
        }

        std::set<size_t> rewrite() {
            std::fstream file(path);
            std::string tmpFilePath = tmpnam(nullptr);
            std::fstream tmp(tmpFilePath, std::fstream::out | std::fstream::trunc);

            // copies file flags to temporary file
            writeHeader(tmp, FileHeader(true, header().size));
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
                Register<T> r(s.readln(file));

                if (r.valid)
                    os << "valid";
                else 
                    os << "invalid";

                os << " " << r.data << std::endl;
            }

            return os;
        }

    private:
        std::string path;

        void writeHeader(std::ofstream& file, const FileHeader& header) {
            std::streampos startPos = file.tellp();
            file.seekp(0, file.beg);
            file.write(header, sizeof(FileHeader));
            file.seekp(startPos, file.beg);
        }

        FileHeader header(std::ifstream& file) const {
            std::streampos startPos = file.tellg();
            file.seekg(0, file.beg);
            FileHeader header;
            file.read(reinterpret_cast<char*>(&header), sizeof(FileHeader));
            file.seekg(startPos, file.beg);
            return header;
        }

        Register<T> readln(std::ifstream& file, size_t line) const {
            setInPosition(file, line);
            readln(file);
        }

        Register<T> readln(std::ifstream& file) const {
            Register<T> r;
            file.read(r.c_ptr(), sizeof(Register<T>));
            return r;
        }

        void setOutPosition(std::ofstream& file, size_t offset) {
            file.seekp(sizeof(FileHeader) + offset * sizeof(Register<T>), file.beg);
        }

        void setInPosition(std::ifstream& file, size_t offset) const {
            file.seekg(sizeof(FileHeader) + offset * sizeof(Register<T>), file.beg);
        }
};

#endif
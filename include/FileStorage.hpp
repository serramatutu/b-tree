#ifndef FILESTORAGE_INCLUDED
#define FILESTORAGE_INCLUDED

#include <string>
#include <fstream>
#include <stdexcept>
#include <cstdio>
#include <set>

/* FILE FORMAT
   (flags) flags
   (sizeof(T) + sizeof(bool))[] data */

template <typename T> 
class FileStorage {
    public:
        typedef enum FileFlags {
            CLEAN = 1,
            EMPTY = 2
        } FileFlags;

        const FileFlags DEFAULT_FLAGS = (FileFlags)(FileFlags::CLEAN | FileFlags::EMPTY);

        FileStorage(std::string path) : path(path)
        {
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
            file.write(&data, sizeof(T));
        }

        T read(size_t index) {
            std::ifstream file(path);
            setInPosition(file, index);
            T data;
            file.read(&data, sizeof(T));
            return data;
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
                file.read(&data, sizeof(T));
                if (valid) {
                    tmp << true;
                    tmp.write(data, sizeof(T));
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
            file.seekp(sizeof(FileFlags) + offset * (sizeof(T) + sizeof(bool)), file.beg);
        }

        static void setInPosition(std::ifstream& file, size_t offset) {
            file.seekg(sizeof(FileFlags) + offset * (sizeof(bool) + sizeof(T)), file.beg);
        }
};

#endif
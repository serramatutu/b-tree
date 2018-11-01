#ifndef AVLDATABASE_INCLUDED
#define AVLDATABASE_INCLUDED

#include <string>
#include <filesystem>

template <typename T>
class AVLDatabase {
    private:
        FileStorage index;
        FileStorage body;

        void rewriteIndex();
        void rewriteData();

    public:
        AVLDatabase(const std::string& idxPath, const std::string& dataPath) 
            : index(idxPath), body(dataPath) 
            {}

        virtual ~AVLDatabase() {
            rewrite();
        }

        AVLDatabase(const AVLDatabase& other) {

        }

        AVLDatabase<T, Less>& operator=(AVLDatabase<T, Less> other) {

        }

        AVLDatabase(AVLDatabase&& other) {

        }

        void insert(const T& data) {

        }

        bool remove(const T& data) {

        }

        void rewrite() {

        }

        bool dataClean() {
            return isDataClean;
        }

        bool indexClean() {
            return isIndexClean;
        }

        bool clean() {
            return indexClean() && dataClean();
        }

        bool empty() const {

        }

        int height() const {

        }

        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const AVLDatabase<U>& t);
};

#endif
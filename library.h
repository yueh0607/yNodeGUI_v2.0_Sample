#pragma once


#include"book.h"
#include <string>
#include"csvfile.h"
#include"history.h"

// 图书馆类
class Library {
private:
    std::vector<Book*> books;
    std::vector<history*> lentd;
    csvfile<Book> bookdata = csvfile<Book>("data/books.csv");
    csvfile<history> records = csvfile<history>("data/records.csv");
public:
    ~Library();

    bool addBook(Book* book);

    bool removeBook(const std::string& id);

    Book* findBook(const std::string& id);


    //在用户没借书时，返回false，正确还书返回true
    bool returnBook(std::string id,std::string user);
    //在用户借书时，如果有余量返回true，没有返回false
    bool lentBook(std::string id,std::string user);

    void savedata();
    void readdata();
    bool readAddData(string path);

    vector<Book*>* GetBooks() { return &books; };
    vector<history*>* GetRecords() { return &lentd; }
};

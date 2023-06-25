#pragma once

#include<iostream>
#include<sstream>
#include"tools.h"


class history :ISerializable<history>
{
public:
    std::string user;
    std::string book;
    history(std::string book, std::string user) :user(user), book(book) {}

    history() {}

    void FromCsvRow(string str) override
    {
        vector<string>* parts = split_tovector(str, ',');
        book = (*parts)[0];
        user = (*parts)[1];
        delete parts;
    }


    string ToCsvRow() override
    {
        stringstream builder("");
        builder << book << ",";
        builder << user << ",";
        return builder.str();
    }
};

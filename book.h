#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include"csvfile.h"

#include<sstream>
#include <string>
// 书类
class Book : public ISerializable<Book>
{

public:
    //编号
    std::string id;
    //名字
    std::string name;
    //出版社
    std::string publisher;
    //分类
    std::string category;
    //价格
    float price=0;
    //剩余
    int residue=0 ;
    //总量
    int quantity=0;

    Book(const std::string& id, const std::string& name, const std::string& publisher, const std::string& category, float price, int quantity);
       
    Book();

    void FromCsvRow(string str) override;

    string ToCsvRow() override;

    
};



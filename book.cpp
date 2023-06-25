#include"book.h"
#include"tools.h"


Book::Book(const std::string& id, const std::string& name, const std::string& publisher, const std::string& category,float price, int quantity)
    : id(id), name(name), publisher(publisher), category(category), price(price), quantity(0), residue(0)
{

}
Book::Book() {}




void Book::FromCsvRow(string str)
{
    vector<string>* parts =split_tovector(str,',');

    id = (*parts)[0];
    name= (*parts)[1];
    publisher= (*parts)[2];
    category = (*parts)[3];
    price = stod((*parts)[4]);
    quantity = std::stoi((*parts)[5]);
    residue = std::stoi((*parts)[6]);
    delete parts;
}


string Book::ToCsvRow()
{
    stringstream builder("");
    builder << id << ",";
    builder << name << ",";
    builder << publisher << ",";
    builder << category<< ",";
    builder << price << ",";
    builder << quantity<< ",";
    builder << residue ;
    return builder.str();
}
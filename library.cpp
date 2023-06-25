#include "library.h"

Library::~Library() {
	// 清理内存，释放 Book 对象
	for (Book* book : books) {
		delete book;
	}
	for (history* his : lentd)
	{
		delete his;
	}

}

bool Library::addBook(Book* book) {
	// 检查是否存在相同 ID 的书籍
	for (Book* existingBook : books) {
		if (existingBook->id == book->id) {
			delete book;  // 如果存在相同 ID 的书籍，则删除传入的书籍对象
			return false;
		}
	}

	books.push_back(book);
	return true;
}

bool Library::removeBook(const std::string& id) {
	auto iter = std::find_if(books.begin(), books.end(), [id](const Book* book) {
		return book->id == id;
		});

	if (iter != books.end()) {


		delete* iter;
		books.erase(iter);
		return true;
	}

	return false;
}

Book* Library::findBook(const std::string& id)
{
	auto iter = std::find_if(books.begin(), books.end(), [id](const Book* book)
		{
	
			return book->id == id;
		});

	if (iter != books.end()) {
		return *iter;
	}

	return nullptr;
}


//在用户没借书时，返回false，正确还书返回true
bool Library::returnBook(std::string id, std::string user)
{
	auto item_it = findBook(id);
	for (int i = 0; i < lentd.size(); i++)
	{
		if (lentd[i]->user == user && lentd[i]->book == id)
		{
			item_it->residue++;
			lentd.erase(lentd.begin()+i);
			return true;
		}
	}
	return false;
}

//在用户借书时，如果有余量返回true，没有返回false,成功借书数量-1，借书记录+
bool Library::lentBook(std::string id, std::string user)
{
	auto item_it = findBook(id);
	if (item_it->residue > 0)
	{
		item_it->residue--;
		lentd.push_back(new history(id, user));
		return true;
	}
	return false;
}

void Library::savedata()
{
	bookdata.write(books);
	records.write(lentd);
}

void Library::readdata()
{
	books = bookdata.read();
	lentd = records.read();
}


bool Library::readAddData(string path)
{
	csvfile<Book> csvFile(path);
	vector<Book*> data = csvFile.read();

	// 将读取的数据添加到 books 向量中
	for (auto bookPtr : data)
	{
		books.push_back(bookPtr);
	}

	return true;
}
// ReSharper disable All
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<easyx.h>
#include<fstream>
#include<graphics.h>
#include<conio.h>
#include<list>
#include<functional>
#include <string>
#include<cmath>
#include"yNodeGUI.h"
#include"book.h"
#include"library.h"
#include<map>
using namespace std;

#pragma region Defines-定义一些全局使用的量

#define LINESIGN "Line:"  //存储文件的行标志，在course.data里看
#define ENDSIGN "End"    //存储文件的尾标志
#define FILENAME1 "course.data"   //存储文件名
#define FILENAME2 "select.data"    //已选择课程文件名
#define BGFILENAME "Resources/background.jpg"   //背景目录
#define BUTTONCOLOR RGB(63, 92, 57)     //按钮颜色
#define LINEBOXCOLOR GREEN       //按钮线框颜色
#define BUTTONFONTCOLOR WHITE      //按钮字体颜色
#define GIRDFONTCOLOR BLACK        //网格内字体颜色
#define GIRDLINECOLOR BLACK        //王国线颜色
#define FONTNAME "宋体"            //全局字体

#pragma endregion


//菜单节点管理器
Menu* menu;
//背景ID
int bg_id;
//书籍列表ID
int book_list_id, lastbtn_id,nextbtn_id,rtn_id;
//借出列表ID
int lent_list_id,  lastbtn_1_id, nextbtn_1_id;
Library library = Library();

//改变数量
void ChangeCount(Menu& menu, Canvas& canvas)
{
	MessageBoxTip("提示", "请在控制台输入要删除的书籍编号");
	system("cls");
	InputGroup group = InputGroup(0);
	char id[20];
	int c=0;
	group.Add("书籍编号(string)：", "%s", &(id));
	group.Add("变动数量(int   )：", "%d", &c);
	group.Print();
	bool result = group.GetInput();
	if (result)
	{
		Book* book = library.findBook(id);
		if (book!=nullptr)
		{
			if (c>0||book->residue >= abs(c) )
			{
				book->residue += c;
				book->quantity += c;
				MessageBoxTip("提示", "修改成功");
			}
			else
			{
				MessageBoxTip("提示", "数量不足");
			}
			
			library.savedata();
		}
		else
		{

			MessageBoxTip("提示", "没有找到对应书籍，请先添加");
		}
	}
	else
	{
		MessageBoxTip("提示", "错误输入格式");
	}
	menu.Last();
}
//找书
void FindBook(Menu& menu, Canvas& canvas)
{
	system("cls");
	InputGroup group = InputGroup(0);
	char id[20];
	group.Add("书籍编号(string)：", "%s", &(id));
	group.Print();
	bool result = group.GetInput();
	if (result)
	{
		Book* item = library.findBook(id);
		if (item != nullptr)
		{
			MessageBoxTip("查询结果", LPCSTR(item->ToCsvRow().c_str()));
		}
		else
		{
			MessageBoxTip("提示", "没有找到书籍");
		}
	}
	else
	{
		MessageBoxTip("提示", "输入格式错误");
	}
	menu.Last();
}
//添加书
void AddBook(Menu& menu, Canvas& canvas)
{
	MessageBoxTip("提示", "请在控制台输入新书信息");

	InputGroup group = InputGroup(0);
	Book* item = new Book();
	item->quantity = 0;
	
	char id[20],name[30],pub[30],cate[20];

	group.Add("书籍编号(string)：", "%s", id);
	group.Add("书籍名称(string)：", "%s", name);
	group.Add("出版社名(string)：", "%s", pub);
	group.Add("书籍价格(float )：", "%f", &(item->price));
	group.Add("书籍分类(string)：", "%s", cate);
	group.Print();
	bool result = group.GetInput();

	item->id = string(id);
	item->name = string(name);
	item->category = string(cate);
	item->publisher = string(pub);


	if (result)
	{
		if (library.addBook(item))
		{
			MessageBoxTip("提示", "添加成功");
			library.savedata();
		}
		else
		{
			MessageBoxTip("提示", "输入格式错误");
		}
	}
	else
	{
		MessageBoxTip("提示", "格式错误");
	}
	menu.Last();
}
//导入
void ImportBooks(Menu& menu, Canvas& canvas)
{
	string path = OpenFileSelectionWindow();
	bool result = library.readAddData(path);

	if (result)
	{
		MessageBoxTip("提示", "导入成功");
		library.savedata();
	}
	else
	{
		MessageBoxTip("错误", "您导入的文件格式错误！");
	}
	menu.Last();
}

//删除书
void DelBook(Menu& menu, Canvas& canvas)
{
	MessageBoxTip("提示", "请在控制台输入要删除的书籍编号");
	system("cls");
	InputGroup group = InputGroup(0);
	char id[20];
	group.Add("书籍编号(string)：", "%s", &(id));
	group.Print();
	bool result = group.GetInput();
	if (result)
	{
		if (library.removeBook(string(id)))
		{
			MessageBoxTip("提示", "移除成功");
			library.savedata();
		}
		else
		{
			MessageBoxTip("提示", "不存在书籍");
		}
	}
	else
	{
		MessageBoxTip("提示", "错误输入格式");
	}
	menu.Last();
}
//展示列表
void BookList(Menu& menu, Canvas& canvas)
{
	canvas.Env(1).Draw(book_list_id);
	canvas.Env(1).Draw(lastbtn_id);
	canvas.Env(1).Draw(nextbtn_id);
	canvas.Env(1).Draw(rtn_id);
}

void CateList(Menu& menu, Canvas& canvas)
{
	map<string, int> counts;

	auto books = library.GetBooks();
	for (auto& i : *books)
	{
		auto it = counts.find(i->category);
		if (it == counts.end())
		{
			counts.insert({ i->category, 1 });
			
		}
		else
		{
			counts[i->category]++;
		}
	}
	system("cls");
	for (auto& i : counts)
	{
		cout <<"类别：" << i.first << "  共：" << i.second << "种"<<endl;
	}
	menu.Last();
}


//借书
void BorrowBook(Menu& menu, Canvas& canvas)
{
	MessageBoxTip("提示", "请在控制台输入要借阅的书籍编号");
	system("cls");
	InputGroup group = InputGroup(0);
	char id[20], user[30];

	group.Add("书籍编号(string  )：", "%s", &(id));
	group.Add("用户标识(string  )：", "%s", &(user));
	group.Print();
	bool result = group.GetInput();
	if (result)
	{
		Book* item = library.findBook(string(id));
		if (item == nullptr)
		{
			MessageBoxTip("提示", "不存在该书籍");
		}
		else
		{
			if (library.lentBook(id, user))
			{
				MessageBoxTip("提示", "借阅成功");
				library.savedata();
			}
			else
			{
				MessageBoxTip("提示", "余量不足，请改日再来哦~");
			}
		}
	}
	else
	{
		MessageBoxTip("提示", "错误输入格式");
	}
	menu.Last();
}
//还书
void ReturnBook(Menu& menu, Canvas& canvas)
{
	MessageBoxTip("提示", "请在控制台输入要归还的书籍编号");
	system("cls");
	InputGroup group = InputGroup(0);
	char id[20], user[30];

	group.Add("书籍编号(string  )：", "%s", &(id));
	group.Add("用户标识(string  )：", "%s", &(user));
	group.Print();
	bool result = group.GetInput();
	if (result)
	{
		Book* item = library.findBook(id);
		if (item == nullptr)
		{
			MessageBoxTip("提示", "不存在该书籍");
		}
		else
		{
			if (library.returnBook(id, user))
			{
				MessageBoxTip("提示", "还书成功");
				library.savedata();
			}
			else
			{
				MessageBoxTip("提示", "该用户未借书~");
			}
		}
	}
	else
	{
		MessageBoxTip("提示", "错误输入格式");
	}
	menu.Last();
}
//展示记录列表
void LentList(Menu& menu, Canvas& canvas)
{
	canvas.Env(1).Draw(lent_list_id);
	canvas.Env(1).Draw(lastbtn_1_id);
	canvas.Env(1).Draw(nextbtn_1_id);
	canvas.Env(1).Draw(rtn_id);
}

//退出
void ExitSystem(Menu& menu, Canvas& canvas)
{
	system("cls");
	library.savedata();
	cout << "感谢您的支持！(＾▽＾)／期待与您下次再见！" << endl;
	exit(0);
}


//生命周期-Start | Canvas 消息 | Priority=0
void OnStart(Canvas& canvas)
{
#pragma region 读取数据

	try 
	{
		library.readdata();
	}
	catch (exception ex)
	{
		MessageBoxTip("提示", "数据读取失败！如果未修复请勿添加数据!否则可能导致数据丢失");
	}
#pragma endregion
#pragma region MenuNode

	//设置窗口标题
	SetWindowText(GetHWnd(), "图书馆管理系统");

	menu = new Menu(&canvas);
	//main menu
	Node* node_a1 = new Node(menu->root, "书库变更登记");
	Node* node_a2 = new Node(menu->root, "借还登记");
	Node* node_ax = new Node(menu->root, "查找书籍",FindBook);
	Node* node_av = new Node(menu->root, "数量变动", ChangeCount);
	Node* node_a3 = new Node(menu->root, "退出系统", ExitSystem);

	//lev1 - 书库变更
	Node* node_c1 = new Node(node_a1, "添加书籍", AddBook);
	Node* node_cx = new Node(node_a1, "批量导入", ImportBooks);
	Node* node_c2 = new Node(node_a1, "删除书籍", DelBook);
	Node* node_c3 = new Node(node_a1, "书籍列表", BookList);
	Node* node_cv = new Node(node_a1, "分类统计", CateList);
	Node* node_c4 = new Node(node_a1, "返回", last_menu);

	//lev1 - 登记入库
	Node* node_b1 = new Node(node_a2, "借书", BorrowBook);
	Node* node_b2 = new Node(node_a2, "还书", ReturnBook);
	Node* node_b3 = new Node(node_a2, "借出列表", LentList);
	Node* node_b4 = new Node(node_a2, "返回", last_menu);

	//注册菜单
	menu->RegisterMenuByRootNode(0, 100, 80, 300, 50, 10, BUTTONCOLOR, BUTTONFONTCOLOR, LINEBOXCOLOR, FONTNAME);

	//注册背景
	Image* background = new Image(createRectbyPoint(0, 0, 1000, 600), BGFILENAME);
	bg_id = background->InstanceId();
	canvas.Env(1).Register(background->InstanceId(), background);



#pragma endregion

#pragma region BookList

	GirdList<Book>* list = new GirdList<Book>(
		10, 7, Vector2{ 10,10 }, 140, 50, FONTNAME, GIRDLINECOLOR, GIRDFONTCOLOR
		, 20, 20);

	book_list_id = list->InstanceId();
	canvas.Env(1).Register(book_list_id, list);
	list->SetHeader(vector<string>{"编号", "名称", "出版社", "分类", "价格", "剩余", "总量"});
	list->SetOrigin(library.GetBooks());
	list->SetColumn([&](Book* book)->vector<string>
		{
			vector<string> columns;
	columns.push_back(((Book*)book)->id);
	columns.push_back(((Book*)book)->name);
	columns.push_back(((Book*)book)->publisher);
	columns.push_back(((Book*)book)->category);
	columns.push_back(to_string(((Book*)book)->price));
	columns.push_back(to_string(((Book*)book)->residue));
	columns.push_back(to_string(((Book*)book)->quantity));
	return columns;
		});


	//上页按钮注册
	Rect rc1 = createRectbyCenter({ 100,580}, 50, 20);
	Image* bg1 = new Image(rc1, BUTTONCOLOR);
	Text* txt1 = new Text("上页", rc1, FONTNAME, BUTTONFONTCOLOR);
	LineBox* lb1 = new LineBox(rc1, LINEBOXCOLOR);
	Button* ret1 = new Button(bg1, txt1, lb1);
	ret1->AddListener([list]() { list->last_page(); });
	lastbtn_id = ret1->InstanceId();
	canvas.Env(1).Register(lastbtn_id, ret1);


	//下页按钮注册
	Rect rc2 = createRectbyCenter({ 160,580 }, 50, 20);
	Image* bg2 = new Image(rc2, BUTTONCOLOR);
	Text* txt2 = new Text("下页", rc2, FONTNAME, BUTTONFONTCOLOR);
	LineBox* lb2 = new LineBox(rc2, LINEBOXCOLOR);
	Button* ret2 = new Button(bg2, txt2, lb2);
	ret2->AddListener([list]() { list->next_page(); });
	nextbtn_id = ret2->InstanceId();
	canvas.Env(1).Register(nextbtn_id, ret2);

	//返回按钮注册
	Rect rc3 = createRectbyCenter({ 220,580 }, 50, 20);
	Image* bg3 = new Image(rc3, BUTTONCOLOR);
	Text* txt3 = new Text("返回", rc3, FONTNAME, BUTTONFONTCOLOR);
	LineBox* lb3 = new LineBox(rc3 ,LINEBOXCOLOR);
	Button* ret3 = new Button(bg3, txt3, lb3);
	ret3->AddListener([&]() { menu->Last(); });
	rtn_id = ret3->InstanceId();
	canvas.Env(1).Register(rtn_id, ret3);


#pragma endregion

#pragma region LentList
	GirdList<history>* list2 = new GirdList<history>(
		10, 2, Vector2{ 10,10 }, 450, 50, FONTNAME, GIRDLINECOLOR, GIRDFONTCOLOR
		, 20, 20);

	lent_list_id = list2->InstanceId();
	canvas.Env(1).Register(lent_list_id, list2);
	list2->SetHeader(vector<string>{"书籍编号", "借走用户"});
	list2->SetOrigin(library.GetRecords());
	list2->SetColumn( [&](history* book)->vector<string>
		{
			vector<string> columns;
	columns.push_back(book->book);
	columns.push_back(book->user);

	return columns;
		});


	//上页按钮注册
	Rect rcx1 = createRectbyCenter({ 100,580 }, 50, 20);
	Image* bgx1 = new Image(rcx1, BUTTONCOLOR);
	Text* txtx1 = new Text("上页", rcx1, FONTNAME, BUTTONFONTCOLOR);
	LineBox* lbx1 = new LineBox(rcx1, LINEBOXCOLOR);
	Button* retx1 = new Button(bgx1, txtx1, lbx1);
	retx1->AddListener([list2]() { list2->last_page(); });
	lastbtn_1_id = ret1->InstanceId();
	canvas.Env(1).Register(lastbtn_1_id, retx1);


	//下页按钮注册
	Rect rcx2 = createRectbyCenter({ 160,580 }, 50, 20);
	Image* bgx2 = new Image(rcx2, BUTTONCOLOR);
	Text* txtx2 = new Text("下页", rcx2, FONTNAME, BUTTONFONTCOLOR);
	LineBox* lbx2 = new LineBox(rcx2, LINEBOXCOLOR);
	Button* retx2 = new Button(bgx2, txtx2, lbx2);
	retx2->AddListener([list2]() { list2->next_page(); });
	nextbtn_1_id = ret2->InstanceId();
	canvas.Env(1).Register(nextbtn_1_id, retx2);

#pragma endregion
}

//生命周期-OnGUI | Canvas 消息 | Priority=1
void OnGUI(Canvas& canvas)
{
	canvas.Env(1).Draw(bg_id); //渲染背景
	menu->DrawOnGUI();   //渲染菜单
}

//生命周期-OnUpdate | Canvas 消息 | Priority=2
void OnUpdate(Canvas& canvas)
{

}


int main()
{
	Canvas canvas = { 1000,600,90, RGB(255,255,255) }; //创建1000*600的窗户，背景色为白色
	canvas.Show(OnStart, OnUpdate, OnGUI, true); //初始化GUI画布并启动生命周期，阻塞程序,显示小黑窗
	return 0;
}
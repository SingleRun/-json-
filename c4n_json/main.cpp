#include <iostream>
#include <windows.h>
#include "entity/Json.h"
#include "parser/Parser.h"

using namespace std;
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    //测试Json类
    if(false)
    {
        cout<<"测试Json类\n";
        Json j1=Json();
        Json j2=Json(7);
        Json j3=Json(7.7);
        Json j4=Json("hello world");

        //拷贝构造
        Json j5=Json(j3);
        //数组
        Json j6=Json(Json::Type::Group);
        j6.pushBack(Json(10));
        j6.pushBack(Json("3123"));
        j6.pushBack(Json());
        Json j7=Json(Json::Type::Group);
        j7.pushBack(Json(753));
        j7.pushBack(Json(123));
        j6.pushBack(j7);
        j6.pushBack(Json(true));

        //对象
        Json j8=Json(Json::Type::Object);
        //传入字符串有问题,实际上是调用了析构函数,实际上是因为没有调用深拷贝构造函数,因为调用的是赋值运算符重载
        j8["name"]=Json("C4n");
        j8["phone_number"]=Json("13169281234");
        j8["age"]=Json(21);

        cout<<"j1:"<<j1.toString()<<endl;
        cout<<"j2:"<<j2.toString()<<endl;
        cout<<"j3:"<<j3.toString()<<endl;
        cout<<"j4:"<<j4.toString()<<endl;
        cout<<"j5:"<<j5.toString()<<endl;
        cout<<"j6:"<<j6.toString()<<endl;
        cout<<"j8:"<<j8.toString()<<endl;
    }

    //测试Parser类
    if(false)
    {
        cout << "测试Parser类" << endl;
        Parser parser = Parser();

        parser.setJsonString("true");
        cout << parser.parse().toString() << endl;
        parser.setJsonString("123");
        cout << parser.parse().toString() << endl;
        parser.setJsonString("123.23");
        cout << parser.parse().toString() << endl;
        parser.setJsonString("\"大哥\"");
        cout << parser.parse().toString() << endl;
        parser.setJsonString("[123,null,true,\"大哥\",[\"s\",890]]");
        cout << parser.parse().toString() << endl;
        parser.setJsonString("{\"age\":21,\"name\":\"C4n\",\"phone_number\":\"13169281234\"}");
        cout << (parser.parse()["name"]).toString() << endl;
    }


    //文件导入jsonString测试
    if(true)
    {
        Parser parser = Parser();
        parser.loadJsonFile("../jsonFile/myJson.json");
        cout << parser.parse().toString() << endl;
    }

    return 0;
}

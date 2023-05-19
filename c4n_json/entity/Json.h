//Json格式：https://www.json.org/json-zh.html
// Created by 陈浩楠1 on 2023/4/5.
//

#ifndef C4N_JSON_JSON_H
#define C4N_JSON_JSON_H
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class Json
{
public:
    enum Type
    {
        Null,
        Bool,
        Int,
        Double,
        String,
        Group,//由于array有特殊意义，我的实现是用vector实现的，所以我不希望这里使用array作为变量名导致歧义
        Object
    };
    //使用std::variant好像更好些
    union Value
    {
        int vInt;
        bool vBool;
        double vDouble=0;
        string* vString;
        vector<Json>* vGroup;
        map<string,Json>* vObject;
    };
private:
    Type type;
    Value value;
public:
    Json();
    explicit Json(const bool value);
    explicit Json(const int value);
    explicit Json(const double value);
    //不加const char* value的话，字符串会被转成bool而不是string
    explicit Json(const char* value);
    explicit Json(const string& value);
    //这两个构造函数好像并不好用，所以没用到
    explicit Json(const vector<Json>& value);
    explicit Json(const map<string,Json>& value);
    explicit Json(const Json& value);
    //给map赋值用的，不然会double dtor，因为自动生成的赋值运算符重载是浅拷贝
    Json& operator=(const Json& value);
    //用于array和object的初始化
    explicit Json(Type type);

    void pushBack(const Json& value);

    Json& operator[](const int index);
    Json& operator[](const string key);
    string toString() const;

    //将类还原初始化状态
    void clear();

    ~Json();
};


#endif //C4N_JSON_JSON_H

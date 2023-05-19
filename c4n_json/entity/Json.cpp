//
// Created by 陈浩楠1 on 2023/4/5.
//

#include "Json.h"
#include <assert.h>
#include <iostream>

Json::Json() : type(Type::Null)
{
};

Json::Json(const bool value) : type(Type::Bool)
{
    this->value.vBool = value;
}

Json::Json(const int value) : type(Type::Int)
{
    this->value.vInt = value;
}

Json::Json(const double value) : type(Type::Double)
{
    this->value.vDouble = value;
}

Json::Json(const char *value) : Json(string(value))
{
}

Json::Json(const string &value) : type(Type::String)
{
    try
    {
        this->value.vString = new string(value);
    }
    catch (const std::bad_alloc& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

Json::Json(const vector<Json> &value) : type(Type::Group)
{
    this->value.vGroup = new vector<Json>(value);
}

Json::Json(const map<string, Json> &value)
{
    this->value.vObject = new map<string, Json>(value);
}

Json::Json(Json::Type type)
{
    this->type = type;
    switch (this->type)
    {
        case Type::Null:
            break;
        case Type::Bool:
            Json(false);
            break;
        case Type::Int:
            Json(0);
            break;
        case Type::Double:
            Json(0.0);
            break;
        case Type::String:
            Json(string(""));
            break;
        case Type::Group:
            this->value.vGroup = new vector<Json>();
            break;
        case Type::Object:
            this->value.vObject = new map<std::string, Json>();
            break;
    }
}

Json::Json(const Json &value)
{
    clear();
    this->type = value.type;
    //注意不能直接调用构造函数，得手动 this->value.vString=new string (*(value.value.vString));
    switch (this->type)
    {
        case Type::Null:
            break;
        case Type::Bool:
            this->value.vBool = value.value.vBool;
            break;
        case Type::Int:
            this->value.vInt = value.value.vInt;
            break;
        case Type::Double:
            this->value.vDouble = value.value.vDouble;
            break;
        case Type::String:
            this->value.vString = new string(*(value.value.vString));
            break;
        case Type::Group:
            this->value.vGroup = new vector<Json>(*(value.value.vGroup));
            break;
        case Type::Object:
            this->value.vObject = new map<string, Json>(*(value.value.vObject));
            break;
    }
}

Json &Json::operator=(const Json &value)
{
    clear();
    this->type = value.type;
    switch (this->type)
    {
        case Type::Null:
            break;
        case Type::Bool:
            this->value.vBool = value.value.vBool;
            break;
        case Type::Int:
            this->value.vInt = value.value.vInt;
            break;
        case Type::Double:
            this->value.vDouble = value.value.vDouble;
            break;
        case Type::String:
        {
            const char *temp = value.value.vString->c_str();
            this->value.vString = nullptr;
            this->value.vString = new string(temp);
//            this->value.vString=new string (*(value.value.vString));
            break;
        }
        case Type::Group:
            this->value.vGroup = new vector<Json>(*(value.value.vGroup));
            break;
        case Type::Object:
            this->value.vObject = new map<string, Json>(*(value.value.vObject));
            break;
    }
}

void Json::pushBack(const Json &value)
{
    assert(this->type == Type::Group);
    if (this->type != Type::Group)
    {
        throw "该类型不是数组，不能使用pushBack";
    }
    //跟踪push_back进去的是什么东西
    this->value.vGroup->push_back(Json(value));
}

Json &Json::operator[](const int index)
{
    assert(this->type == Type::Group);
    if (this->type != Type::Group)
    {
        throw "该类型不是数组，不能使用[index]索引";
    }
    if (index >= this->value.vGroup->size())
    {
        throw "索引超出范围";
    }
    return (*(this->value.vGroup))[index];
}

Json &Json::operator[](const string key)
{
    //由于map的特性，该函数还可用于创建map的新元素
    assert(this->type == Type::Object);
    if (this->type != Type::Object)
    {
        throw "该类型不是对象，不能使用[key]索引";
    }
    return (*(this->value.vObject))[key];
}

string Json::toString() const
{
    switch (type)
    {
        case Type::Null:
            return string("null");
        case Type::Bool:
            switch (this->value.vBool)
            {
                case true:
                    return string("true");
                case false:
                    return string("false");
            }
        case Type::Int:
            return std::to_string(value.vInt);
        case Type::Double:
            return std::to_string(value.vDouble);
        case Type::String:
            return '\"' + string(*(this->value.vString)) + '\"';
        case Type::Group:
        {
            string ret("[");
            for (auto it = value.vGroup->begin(); it != value.vGroup->end(); it++)
            {
                if (it != value.vGroup->begin())
                {
                    ret += ',';
                }
                ret += it->toString();
            }
            ret += ']';
            return ret;
        }
        case Type::Object:
        {
            string ret("{");
            for (auto it = value.vObject->begin(); it != value.vObject->end(); it++)
            {
                if (it != value.vObject->begin())
                {
                    ret += ',';
                }
                ret += '\"' + it->first + '\"' + ':' + it->second.toString();
            }
            ret += '}';
            return ret;
        }

    }
}

void Json::clear()
{
    switch (this->type)
    {
        case Type::Null:
            break;
        case Type::Bool:
            this->type = Type::Null;
            this->value.vBool = false;
        case Type::Int:
            this->type = Type::Null;
            this->value.vInt = 0;
            break;
        case Type::Double:
            this->type = Type::Null;
            this->value.vDouble = 0.0;
            break;
        case Type::String:
            this->type = Type::Null;
            if (this->value.vString)
            {
                delete this->value.vString;
                this->value.vString = nullptr;
            }
            break;
        case Type::Group:
            this->type = Type::Null;
            if (this->value.vGroup)
            {
                for (auto &it: *this->value.vGroup)
                {
                    it.clear();
                }
                delete this->value.vGroup;
                this->value.vGroup= nullptr;
            }
            break;
        case Type::Object:
            this->type = Type::Null;
            if (this->value.vObject)
            {
                for (auto &it: *this->value.vObject)
                {
                    it.second.clear();
                }
                delete this->value.vObject;
                this->value.vObject= nullptr;
            }

            break;
    }
}

Json::~Json()
{
    clear();
}








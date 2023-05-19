//
// Created by 陈浩楠1 on 2023/4/5.
//

#include "Parser.h"
#include <assert.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <fstream>

Parser::Parser()
{

}

void Parser::setCurrentPosToBeginOfJsonString()
{
    this->currentPos=jsonString.begin();
}

void Parser::loadJsonFile(const char *filePath)
{
    jsonString.clear();

    std::ifstream fin;
    fin.open(filePath);
    if (!fin.is_open())
    {
        std::cout<<"文件打开失败"<<std::endl;
        return;
    }
    while (!fin.eof())
    {
        string temp;
        std::getline(fin,temp);
        jsonString+=temp;
    }
    fin.close();
    setCurrentPosToBeginOfJsonString();
}

void Parser::loadJsonFile(const string &filePath)
{

}

void Parser::setJsonString(const char *jsonString)
{
    //这样需要复制两次，可能会造成效率低下
    setJsonString(string(jsonString));
}

void Parser::setJsonString(const string &jsonString)
{
    this->jsonString=string(jsonString);
    setCurrentPosToBeginOfJsonString();
}

Json Parser::parse()
{
    if (this->jsonString.empty())
    {
        throw string("得先导入json字符串再解析")+std::to_string(__LINE__);
    }
    skipSpace();
    //其实不用这个if我觉得，待会去掉试试看
    if (currentPos!=jsonString.end())
    {
        switch (*currentPos)
        {
            case 'n':
                return parseNull();
            case 't':
            case 'f':
                return parseBool();
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return parseNumber();
            case '"':
                return Json(parseString());
            case '[':
                return parseArray();
            case '{':
                return parseObject();
            default:
                std::cout<<"json格式有误："<<std::endl;
                break;
        }
    }

    return Json();
}

void Parser::skipSpace()
{
    while (currentPos!=jsonString.end()&&('\\t'==*currentPos||'\\n'==*currentPos||' '==*currentPos))
    {
        currentPos++;
    }
}

bool Parser::compareTo(char *destination, int compareLength)
{
    //想过要不要返回true就不复原currentPos，但还是算了，这样结构比较清晰
    for (int i = 0; i < compareLength; i++,currentPos++,destination++)
    {
        if (currentPos==jsonString.end()||*currentPos!=*destination)
        {
            currentPos-=i;
            return false;
        }
    }
    currentPos-=compareLength;
    return true;
}

Json Parser::parseNull()
{
    skipSpace();
    assert(*currentPos=='n');
    if (compareTo("null",4))
    {
        currentPos+=4;
        return Json();
    }
    throw string("json格式有误：")+std::to_string(__LINE__);
}

Json Parser::parseBool()
{
    skipSpace();
    assert(*currentPos=='t'||*currentPos=='f');
    if (*currentPos=='t')
    {
        if (compareTo("true",4))
        {
            currentPos+=4;
            return Json(true);
        }
    }
    else if(*currentPos=='f')
    {
        if (compareTo("false",5))
        {
            currentPos+=5;
            return Json(false);
        }
    }
    throw string("json格式有误：")+std::to_string(__LINE__);
}

Json Parser::parseNumber()
{
    skipSpace();
    int has_decimal = 0;
    int i;
    string tempNumber=string();
    bool isDouble= false;
    //实际上应该判断第一个字符才能为-,中间字符才能为.
    while ('-'==*currentPos|| std::isdigit(*currentPos)||'.'==*currentPos)
    {
        if ('.'==*currentPos)   isDouble= true;
        tempNumber+=*currentPos;
        currentPos++;
    }
    if (isDouble)
    {
        return Json(strtod(tempNumber.c_str(),NULL));
    }
    else
    {
        return Json(atoi(tempNumber.c_str()));
    }
}

std::string Parser::parseString()
{
    skipSpace();
    assert('"'==*currentPos);
    //这里还得看下json字符串是否允许"出现在字符串中，如果不允许就不能这样处理
    //还得看下是否允许反转义字符在字符串中
    string str=string();
    currentPos++;
    while (currentPos!=jsonString.end()&&*currentPos!='"')
    {
        str+=*currentPos;
        currentPos++;
    }
    if (currentPos==jsonString.end())  throw string("json格式有误：")+std::to_string(__LINE__);
    currentPos++;
    return str;
}

Json Parser::parseArray()
{
    skipSpace();
    assert('['==*currentPos);
    Json array=Json(Json::Type::Group);
    //跳过[
    currentPos++;
    array.pushBack(parse());
    while(','==*currentPos)
    {
        //跳过,
        currentPos++;
        array.pushBack(parse());
    }
    if (']'!=*currentPos)
        throw string("json格式有误：")+std::to_string(__LINE__);
    //跳过]
    currentPos++;
    return Json(array);
}

Json Parser::parseObject()
{
    skipSpace();
    assert('{'==*currentPos);
    Json object=Json(Json::Type::Object);
    //跳过{
    currentPos++;
    string key=parseString();
    //跳过:
    currentPos++;
    object[key]=parse();
    while (','==*currentPos)
    {
        //跳过,
        currentPos++;
        key=parseString();
        //跳过:
        currentPos++;
        object[key]=parse();
    }
    if ('}'!=*currentPos)
        throw string("json格式有误：")+std::to_string(__LINE__);
    //跳过}
    currentPos++;
    return Json(object);
}





//
// Created by 陈浩楠1 on 2023/4/5.
//

#ifndef C4N_JSON_PARSER_H
#define C4N_JSON_PARSER_H
#include <string>
#include "../entity/Json.h"
class Parser
{
public:
    explicit Parser();

    void setCurrentPosToBeginOfJsonString();

    void loadJsonFile(const char* filePath);
    void loadJsonFile(const std::string& filePath);

    void setJsonString(const char* jsonString);
    void setJsonString(const std::string& jsonString);

    Json parse();
private:
    //json字符串
    std::string jsonString;
    //当前读取到的位置
    string::iterator currentPos;
    //跳过空格直到有意义的值
    void skipSpace();
    //比较字符串
    bool compareTo(char* destination,int compareLength);
    //各种解析
    Json parseNull();
    Json parseBool();
    Json parseNumber();
    //string可以复用在解析key哪里，所以不直接返回Json对象
    std::string parseString();
    Json parseArray();
    Json parseObject();
};


#endif //C4N_JSON_PARSER_H

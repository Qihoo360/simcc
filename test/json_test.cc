
#include "test_common.h"
#include "simcc/json/json.h"
#include "simcc/base64.h"
#include "simcc/string_util.h"

#include <iostream>
#include <fstream>


namespace {
void test_json_1() {
    const char* json_test_str = "{\"type\":\"AE,AE\\u6a21\\u677f,AE\\u63d2\\u4ef6,AE\\u6559\\u7a0b,\\u4f1a\\u58f0\\u4f1a\\u5f71,\\u5a5a\\u5e86,\\u680f\\u5305\"}";
    simcc::json::JSONObject jo0;
    H_TEST_ASSERT(jo0.Parse(json_test_str));
    std::string serialize = jo0.ToString();
    simcc::json::JSONObject jo1;
    H_TEST_ASSERT(jo1.Parse(serialize.data(), serialize.size()));
    std::string serialize1 = jo1.ToString();
    H_TEST_ASSERT(jo1.Equals(jo0));
    H_TEST_ASSERT(serialize1 == serialize);
    H_TEST_ASSERT(jo0.error() == simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo1.error() == simcc::json::JSONParser::kNoError);
}

void test_json_2() {
    const char* json_test_str = "{\"key\":\"657d9bc79cf47f406c018eb0c5abf7cd\",\"table\":\"1\",\"url\":\"http:\\/\\/www.qiyi.com\\/dianshiju\\/20100417\\/n3283.html\",\"value\":[{\"cmd\":\"merge\",\"type\":\"2\",\"attrs\":{\"bthumb\":\"\",\"title\":\"\\u5bb6\\u6709\\u513f\\u5973 \\u7b2c\\u4e00\\u90e8\",\"length\":\"1512\",\"src\":\"qiyi\",\"star\":null,\"type\":\"\\u90fd\\u5e02\\u5267,\\u60c5\\u666f\\u5267,\\u513f\\u7ae5\\u5267,\\u751f\\u6d3b\\u5267,\\u9752\\u5c11\\u5267\",\"class\":\"0\"}}]}";
    simcc::json::JSONObject jo0;
    H_TEST_ASSERT(jo0.Parse(json_test_str));
    simcc::json::JSONString* type = jo0.GetJSONString("type");
    (void)type;
    std::string serialize = jo0.ToString();
    //(void)serialize;
    //std::cout << serialize.c_str();
    simcc::json::JSONObject jo1;
    H_TEST_ASSERT(jo1.Parse(serialize.data(), serialize.size()));
    std::string serialize1 = jo1.ToString();
    //std::cout << serialize.c_str();
    H_TEST_ASSERT(serialize1 == serialize);
    H_TEST_ASSERT(jo1.Equals(jo0));
    H_TEST_ASSERT(jo0.error() == simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo1.error() == simcc::json::JSONParser::kNoError);
}

void test_json_3(const char* json_test_str) {
    simcc::json::JSONObject jo0;
    H_TEST_ASSERT(jo0.Parse(json_test_str));
    std::string serialize0 = jo0.ToString();
    simcc::json::JSONObject jo1;
    H_TEST_ASSERT(jo1.Parse(serialize0.data(), serialize0.size()));
    std::string serialize1 = jo1.ToString();
    H_TEST_ASSERT(jo1.Equals(jo0));
    H_TEST_ASSERT(serialize1 == serialize0);
    simcc::json::JSONObject jo2;
    H_TEST_ASSERT(jo2.Parse(serialize1.data(), serialize1.size()));
    std::string serialize2 = jo2.ToString();
    H_TEST_ASSERT(jo2.Equals(jo1));
    H_TEST_ASSERT(jo2.Equals(jo0));
    H_TEST_ASSERT(serialize1 == serialize2);
    H_TEST_ASSERT(serialize1 == serialize0);
    H_TEST_ASSERT(jo0.error() == simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo1.error() == simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo2.error() == simcc::json::JSONParser::kNoError);
}


void test_json_4() {
    const char* json_test_str = "{\"key\":  \"657d9bc79cf47f406c018eb0c5abf7cd\"  ,  \"table\":1,\"url\":\"http:\\/\\/www.qiyi.com\\/dianshiju\\/20100417\\/n3283.html\",\"value\":[{\"cmd\":\"merge\",\"type\":2,\"attrs\":{\"bthumb\":\"\",\"title\":\"\\u5bb6\\u6709\\u513f\\u5973 \\u7b2c\\u4e00\\u90e8\",\"length\":\"1512\",\"src\":\"qiyi\",\"star\":null,\"type\":\"\\u90fd\\u5e02\\u5267,\\u60c5\\u666f\\u5267,\\u513f\\u7ae5\\u5267,\\u751f\\u6d3b\\u5267,\\u9752\\u5c11\\u5267\",\"class\": 0.0008744  }}]}";
    simcc::json::JSONObject jo0;
    H_TEST_ASSERT(jo0.Parse(json_test_str));
    simcc::json::JSONString* type = jo0.GetJSONString("type");
    (void)type;
    std::string serialize = jo0.ToString();
    //(void)serialize;
    //std::cout << serialize.c_str();
    simcc::json::JSONObject jo1;
    H_TEST_ASSERT(jo1.Parse(serialize.data(), serialize.size()));
    std::string serialize1 = jo1.ToString();
    //std::cout << serialize.c_str();
    H_TEST_ASSERT(serialize1 == serialize);
    H_TEST_ASSERT(jo1.Equals(jo0));
    H_TEST_ASSERT(jo0.error() == simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo1.error() == simcc::json::JSONParser::kNoError);
}

void test_json_JSONObject_ToString_pretty() {
    const char* s = "{\"errno\":\"0\",\"video\":[{\"type\":\"11\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\"},{\"type\":\"11\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\"},{\"type\":\"12\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\"},{\"type\":\"11\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\"},{\"type\":\"11\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\",\"double\":0.123}]}";
    simcc::json::JSONObject jo;
    jo.Parse(s);
    std::string ss = jo.ToString(1);
    //std::cout << std::endl << ss << std::endl;
}

void testJSONParser() {
    const char* s = "{\"errno\":\"0\",\"video\":[{\"type\":\"11<\\/b>\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\"},{\"type\":\"11\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\"},{\"type\":\"12\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\"},{\"type\":\"11\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\"},{\"type\":\"11\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\",\"double\":0.123}]}";
    simcc::json::ObjectPtr jo = simcc::json::JSONParser::Load(s);
    H_TEST_ASSERT(jo);
    std::string ss = jo->ToString(1);
    //std::cout << std::endl << ss << std::endl;

    s = "[12, 11.1, {\"type\":\"11\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\"},{\"type\":\"11\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\"},{\"type\":\"12\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\"},{\"type\":\"11\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\"},{\"type\":\"11\",\"img\":\"http:\\/\\/p6.qhimg.com\\/t01e9b78b922480d701.jpg\",\"url\":\"http:\\/\\/v.360.cn\\/_0h\\/gqXlZUYnQXP7Tx.html\",\"name\":\"\\u91cd\\u5e86\\u68ee\\u6797\",\"actor\":[\"\\u5f20\\u4e09\",\"\\u674e\\u56db\",\"\\u738b\\u4e94\"],\"year\":\"2011\",\"double\":0.123}]";
    jo = simcc::json::JSONParser::Load(s);
    H_TEST_ASSERT(jo);
    H_TEST_ASSERT(jo->IsTypeOf(simcc::json::kJSONArray));
    simcc::json::JSONArray* ja = simcc::json::cast<simcc::json::JSONArray>(jo);
    H_TEST_ASSERT(ja);
    H_TEST_ASSERT(ja->GetInteger(0) == 12);
    H_TEST_ASSERT(ja->GetDecimal(0) == 12);
    H_TEST_ASSERT(ja->GetInteger(1) == 0);
    H_TEST_ASSERT(ja->GetDouble(1) == 11.1);
    H_TEST_ASSERT(ja->GetDecimal(1) == 11.1);
}

}

TEST_UNIT(json_unicode_test_1) {
    test_json_1();
    test_json_2();
    test_json_4();
    test_json_JSONObject_ToString_pretty();
    testJSONParser();
}

namespace {
void parse_json_file(const char* path) {
    std::fstream fs;
    fs.open(path);
    H_TEST_ASSERT(fs.good());
    H_TEST_ASSERT(!fs.fail());
    std::string line;
    size_t text_length = 0;
    size_t binary_length = 0;
    simcc::uint64 text_parse_time = 0;
    simcc::uint64 binary_parse_time = 0;
    while (std::getline(fs, line).good()) {
        simcc::json::JSONObject jo0;
        H_TEST_ASSERT(jo0.Parse(line.data(), line.size()));
        simcc::uint64 t1begin = simcc::utcmicrosecond();
        std::string serialize0 = jo0.ToString();
        simcc::json::JSONObject jo1;
        H_TEST_ASSERT(jo1.Parse(serialize0.data(), serialize0.size()));
        simcc::uint64 t1end = simcc::utcmicrosecond();
        text_parse_time += t1end - t1begin;
        std::string serialize1 = jo1.ToString();
        H_TEST_ASSERT(serialize1 == serialize0);
        H_TEST_ASSERT(jo1.Equals(jo0));
        simcc::json::JSONObject jo2;
        H_TEST_ASSERT(jo2.Parse(serialize1.data(), serialize1.size()));
        std::string serialize2 = jo2.ToString();
        H_TEST_ASSERT(jo2.Equals(jo1));
        H_TEST_ASSERT(jo2.Equals(jo0));
        H_TEST_ASSERT(serialize1 == serialize2);
        H_TEST_ASSERT(serialize1 == serialize0);

        text_length += serialize0.length();

        simcc::uint64 t2begin = simcc::utcmicrosecond();
        simcc::DataStream buf;
        buf << jo2;
        simcc::json::JSONObject jo3;
        buf >> jo3;
        simcc::uint64 t2end = simcc::utcmicrosecond();
        binary_parse_time += t2end - t2begin;
        binary_length += buf.size();
        H_TEST_ASSERT(jo3.Equals(jo2));

        std::string serialize4 = jo3.ToString(1);
        simcc::json::JSONObject jo4;
        H_TEST_ASSERT(jo4.Parse(serialize4.data(), serialize4.size()));
        H_TEST_ASSERT(jo4.Equals(jo1));

        //std::string serialize5 = jo4.ToString(1);
        //simcc::json::JSONObject jo5;
        //H_TEST_ASSERT(jo5.Parse(serialize5.data(), serialize5.size()));
        //H_TEST_ASSERT(jo5.Equals(jo1));
        //H_TEST_ASSERT(jo5.Equals(jo4));
        //H_TEST_ASSERT(serialize5 == serialize4);

        H_TEST_ASSERT(jo0.error() == simcc::json::JSONParser::kNoError);
        H_TEST_ASSERT(jo1.error() == simcc::json::JSONParser::kNoError);
        H_TEST_ASSERT(jo2.error() == simcc::json::JSONParser::kNoError);
        H_TEST_ASSERT(jo3.error() == simcc::json::JSONParser::kNoError);
        H_TEST_ASSERT(jo4.error() == simcc::json::JSONParser::kNoError);
        //H_TEST_ASSERT(jo5.error() == simcc::json::JSONParser::kNoError);
    }
    fs.close();
}
}

TEST_UNIT(json_unicode_and_binary_test_2) {
    const char* path = "../test/test_data/json/unicode.json.txt";
    parse_json_file(path);
    path = "../test/test_data/json/mid_json.txt";
    parse_json_file(path);


    //std::cout << "\nbinary/text parse time=" << (double)binary_parse_time/(double)text_parse_time << std::endl;//0.336
    //std::cout << "binary_length/text_length=" << double(binary_length)/double(text_length) << std::endl; //0.885
}

TEST_UNIT(json_gbk_chinese_test_3) {
    const char* path = "../test/test_data/json/gbk-chinese.json.txt";
    std::fstream fs(path);
    H_TEST_ASSERT(fs.good());
    H_TEST_ASSERT(!fs.fail());
    std::string line;
    while (std::getline(fs, line).good()) {
        simcc::json::JSONObject jo0;
        H_TEST_ASSERT(jo0.Parse(line.data(), line.size()));
        std::string serialize0 = jo0.ToString();
        simcc::json::JSONObject jo1;
        H_TEST_ASSERT(jo1.Parse(serialize0.data(), serialize0.size()));
        std::string serialize1 = jo1.ToString();
        H_TEST_ASSERT(serialize1 == serialize0);
        H_TEST_ASSERT(jo1.Equals(jo0));
        simcc::json::JSONObject jo2;
        H_TEST_ASSERT(jo2.Parse(serialize1.data(), serialize1.size()));
        std::string serialize2 = jo2.ToString();
        H_TEST_ASSERT(jo2.Equals(jo1));
        H_TEST_ASSERT(jo2.Equals(jo0));
        H_TEST_ASSERT(serialize1 == serialize2);
        H_TEST_ASSERT(serialize1 == serialize0);


        H_TEST_ASSERT(jo0.error() == simcc::json::JSONParser::kNoError);
        H_TEST_ASSERT(jo1.error() == simcc::json::JSONParser::kNoError);
        H_TEST_ASSERT(jo2.error() == simcc::json::JSONParser::kNoError);

        test_json_3(line.c_str());
    }
}

TEST_UNIT(json_error_test_4) {
    std::string line = "{\"list\":[{\"attrs\":{\"content\":\"\u606d\u559c\u3010\u674e\u5c0f\u59d0\u25c6\u25c6\u25c6\u3011\u7b2c\u4e00\u8f6e24\u780110\u4e2d8\u8fbe\u5230\u52a0\u5a01\u671b\u6807\u51c6\uff0c\u52a0\u5a01\u671b1000\u5e76\u664b\u5347\u4e3a\u4f18\u79c0\u9ad8\u624b\uff0c\u611f\u8c22\u4f60\u5bf9\u5409\u5229\u7684\u652f\u6301\uff0c\u5409\u5229\u6709\u4f60\u66f4\u7cbe\u5f69\uff01\u3010\u7c89\u58a8\u3011\u606d\u559c\u3010\u674e\u5c0f\u59d0\u25c6\u25c6\u25c6\u3011\u5728\u7b2c\u4e00\u8f6e\u4e8c\u56db\u7801\u4e2d\u5341\u671f\u4e2d\u516b\uff0c\u8fbe\u5230\u52a0\u5a01\u671b\u6807\u51c6\uff0c\u52a0\u5a01\u671b1000\u5e76\u664b\u5347\u4e3a\u91d1\u6e2f\u4f18\u79c0\u53d1\u8868\u9ad8\u624b\uff0c\u91d1\u6e2f\u6709\u4f60\u66f4\u7cbe\u5f69\uff01\u3010\u7ba1\u7406\u5458\u3011\u606d\u559c\u3010\u674e\u5c0f\u59d0\u25c6\u25c6\u25c6\u3011\u7b2c\u4e00\u8f6e\u4e00\u6ce2\u8fde\u4e2d\u4e94\u671f\u8fbe\u5230\u52a0\u5a01\u671b\u6807\u51c6\uff0c\u52a0\u5a01\u671b1000\u5e76\u664b\u5347\u4e3a\u8363\u8a89\u9ad8\u624b\uff0c\u611f\u8c22\u4f60\u5bf9\u5409\u5229\u7684\u652f\u6301\uff0c\u5409\u5229\u6709\u4f60\u66f4\u7cbe\u5f69\uff01 \u3010\u7c89\u58a8\u3011\u606d\u559c\u3010\u674e\",\"title\":\"026\u671f:\u4e00\u6ce220\u7801\u5927\u4e2d\u5df2\u7ecf\u66f4\u65b0\u4e86\uff01\u2588\u2588\u3010\u674e\u5c0f\u59d0\u4eba\u7f8e\u6599\u66f4\u9753\uff01\u5168\u80fd\u51a0\u519b\uff01\u3011\u2588\"},\"type\":\"99\"}],\"url\":\"http://ji2010.com/bbs/htm_data/2/1203/672810.html\"}";
    simcc::json::JSONObject jo0;
    H_TEST_ASSERT(jo0.Parse(line.data(), line.size()));
    std::string serialize0 = jo0.ToString();
    //H_TEST_ASSERT(serialize0 == line);
    simcc::json::JSONObject jo1;
    H_TEST_ASSERT(jo1.Parse(serialize0.data(), serialize0.size()));
    std::string serialize1 = jo1.ToString();
    H_TEST_ASSERT(serialize1 == serialize0);
    H_TEST_ASSERT(jo1.Equals(jo0));
    simcc::json::JSONObject jo2;
    H_TEST_ASSERT(jo2.Parse(serialize1.data(), serialize1.size()));
    std::string serialize2 = jo2.ToString();
    H_TEST_ASSERT(jo2.Equals(jo1));
    H_TEST_ASSERT(jo2.Equals(jo0));
    H_TEST_ASSERT(serialize1 == serialize2);
    H_TEST_ASSERT(serialize1 == serialize0);


    H_TEST_ASSERT(jo0.error() == simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo1.error() == simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo2.error() == simcc::json::JSONParser::kNoError);
}

TEST_UNIT(json_error_test_5) {
    std::string line = "{\"type\":{\"0\":\"\u8d44\u8baf\",\"1\":\"\u56fe\u7247\",\"2\":\"\u89c6\u9891\",\"3\":\"\u56e2\u8d2d\",\"4\":\"\u8f6f\u4ef6\",\"5\":\"\u8d2d\u7269\",\"99\":\"\u5176\u4ed6\"}}";
    simcc::json::JSONObject jo0;
    H_TEST_ASSERT(jo0.Parse(line.data(), line.size()));
    std::string serialize0 = jo0.ToString();
    //H_TEST_ASSERT(serialize0 == line);
    simcc::json::JSONObject jo1;
    H_TEST_ASSERT(jo1.Parse(serialize0.data(), serialize0.size()));
    std::string serialize1 = jo1.ToString();
    H_TEST_ASSERT(serialize1 == serialize0);
    H_TEST_ASSERT(jo1.Equals(jo0));
    simcc::json::JSONObject jo2;
    H_TEST_ASSERT(jo2.Parse(serialize1.data(), serialize1.size()));
    std::string serialize2 = jo2.ToString();
    H_TEST_ASSERT(jo2.Equals(jo1));
    H_TEST_ASSERT(jo2.Equals(jo0));
    H_TEST_ASSERT(serialize1 == serialize2);
    H_TEST_ASSERT(serialize1 == serialize0);
    H_TEST_ASSERT(jo0.error() == simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo1.error() == simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo2.error() == simcc::json::JSONParser::kNoError);
}


TEST_UNIT(json_array_complex_test_6) {
    const char* path = "../test/test_data/json/more_json_testcase.txt";
    std::fstream fs(path);
    H_TEST_ASSERT(fs.good());
    H_TEST_ASSERT(!fs.fail());
    std::string line;
    bool r = false;
    while (std::getline(fs, line).good()) {
        simcc::json::JSONArray jo0;
        H_TEST_ASSERT(jo0.Parse(line.data(), line.size()));
        std::string serialize0 = jo0.ToString();
        simcc::json::JSONArray jo1;
        H_TEST_ASSERT(jo1.Parse(serialize0.data(), serialize0.size()));
        std::string serialize1 = jo1.ToString();
        H_TEST_ASSERT(serialize1 == serialize0);
        r = jo1.Equals(jo0);
        H_TEST_ASSERT(r);
        simcc::json::JSONArray jo2;
        H_TEST_ASSERT(jo2.Parse(serialize1.data(), serialize1.size()));
        std::string serialize2 = jo2.ToString();
        H_TEST_ASSERT(jo2.Equals(jo1));
        H_TEST_ASSERT(jo2.Equals(jo0));
        H_TEST_ASSERT(serialize1 == serialize2);
        H_TEST_ASSERT(serialize1 == serialize0);


        H_TEST_ASSERT(jo0.error() == simcc::json::JSONParser::kNoError);
        H_TEST_ASSERT(jo1.error() == simcc::json::JSONParser::kNoError);
        H_TEST_ASSERT(jo2.error() == simcc::json::JSONParser::kNoError);
    }
}

namespace {
void json_error_test_case_1() {
    std::string line = "{\"type\":{\"0\">\"\u8d44\u8baf\"}}";
    simcc::json::JSONObject jo;
    H_TEST_ASSERT(0 == jo.Parse(line.data(), line.size()));
    H_TEST_ASSERT(jo.error() == simcc::json::JSONObject::kKeyValueSeperatorError);
    H_TEST_ASSERT((line.data() + jo.error_location())[0] == '>');
}

void json_error_test_case_2() {
    std::string line = "[\"type\":{\"0\">\"\u8d44\u8baf\"}}";
    simcc::json::JSONObject jo;
    H_TEST_ASSERT(0 == jo.Parse(line.data(), line.size()));
    H_TEST_ASSERT(jo.error() == simcc::json::JSONObject::kJSONObjectNotBeginWithBraces);
    H_TEST_ASSERT(jo.error_location() == 0);
}

void json_error_test_case_3() {
    std::string line = "{type\":{\"0\">\"\u8d44\u8baf\"}}";
    simcc::json::JSONObject jo;
    H_TEST_ASSERT(0 == jo.Parse(line.data(), line.size()));
    H_TEST_ASSERT(jo.error() != simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo.error_location() == 1);
}


void json_error_test_case_4() {
    std::string line = "{\"\":{\"0\">\"\u8d44\u8baf\"}}";
    simcc::json::JSONObject jo;
    H_TEST_ASSERT(0 == jo.Parse(line.data(), line.size()));
    H_TEST_ASSERT(jo.error() != simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo.error_location() > 1);
}
}

TEST_UNIT(json_error_test_6) {
    json_error_test_case_1();
    json_error_test_case_2();
    json_error_test_case_3();
    json_error_test_case_4();
}




TEST_UNIT(json_error_debug_1) {
    const char* path = "../test/test_data/json/browser_relative.json";
    simcc::DataStream buf;
    H_TEST_ASSERT(buf.ReadFile(path));
    simcc::json::JSONObject jo0;
    H_TEST_ASSERT(jo0.Parse(buf.data(), buf.size()));


    std::string serialize4 = jo0.ToString(1);
    simcc::json::JSONObject jo4;
    H_TEST_ASSERT(jo4.Parse(serialize4.data(), serialize4.size()));
    H_TEST_ASSERT(jo4.Equals(jo0));

    std::string serialize5 = jo4.ToString(1);
    simcc::json::JSONObject jo5;
    H_TEST_ASSERT(jo5.Parse(serialize5.data(), serialize5.size()));
    H_TEST_ASSERT(jo5.Equals(jo0));
    H_TEST_ASSERT(jo5.Equals(jo4));
    H_TEST_ASSERT(serialize5 == serialize4);
}

TEST_UNIT(json_utf8_punctuation_test) {
    //std::string punctuation = "[\"\u300a\u62dc\u6258\u5c0f\u59d0\u300b\"]";
    std::string punctuation = "[\"\\u300a\\u300b\"]"; //《》
    simcc::json::JSONArray ja;
    H_TEST_ASSERT(ja.Parse(punctuation.data(), punctuation.size()));
    H_TEST_ASSERT(ja.error() == simcc::json::JSONParser::kNoError);
    std::string s = ja.ToString();
    simcc::json::JSONArray ja1;
    H_TEST_ASSERT(ja1.Parse(s.data(), s.size()));
    H_TEST_ASSERT(ja1.error() == simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(ja1.Equals(ja));
}



TEST_UNIT(test_json_parse_error_commit_badcase) {
    const char* path = "../test/test_data/json/browser_relative2.json";
    simcc::DataStream buf;
    H_TEST_ASSERT(buf.ReadFile(path));
    simcc::json::JSONObject jo0;
    H_TEST_ASSERT(jo0.Parse(buf.data(), buf.size()));


    std::string serialize4 = jo0.ToString(1);
    simcc::json::JSONObject jo4;
    H_TEST_ASSERT(jo4.Parse(serialize4.data(), serialize4.size()));
    H_TEST_ASSERT(jo4.Equals(jo0));

    std::string serialize5 = jo4.ToString(1);
    simcc::json::JSONObject jo5;
    H_TEST_ASSERT(jo5.Parse(serialize5.data(), serialize5.size()));
    H_TEST_ASSERT(jo5.Equals(jo0));
    H_TEST_ASSERT(jo5.Equals(jo4));
    H_TEST_ASSERT(serialize5 == serialize4);
}

TEST_UNIT(test_json_JSONString_ValueType_operator) {
    simcc::json::JSONString js("jane");
    std::string& Ref = (std::string&)js;
    Ref = "new_jane";
    H_TEST_ASSERT(js == "new_jane");

    std::string val = (std::string)js;
    val = "other_jane";
    H_TEST_ASSERT(js == "new_jane");
}




















namespace {


std::string encode_unicode_snprintf(simcc::uint32 codepoint) {
    char seq[13] = {};
    int  seq_len = 0; //The length of data written into seq
    /* codepoint is in BMP */
    if (codepoint < 0x10000) {
        snprintf(seq, sizeof(seq), "\\u%04x", codepoint);
        seq_len = 6;
    } else { /* not in BMP -> construct a UTF-16 surrogate pair */
        int32_t first, last;

        codepoint -= 0x10000;
        first = 0xD800 | ((codepoint & 0xffc00) >> 10);
        last = 0xDC00 | (codepoint & 0x003ff);

        snprintf(seq, sizeof(seq), "\\u%04x\\u%04x", first, last);
        seq_len = 12;
    }

    (void)seq_len;
    return seq;
}

void encode_unicode(simcc::uint32 codepoint, simcc::DataStream& stream) {
    stream.Expand(12);
    char* begin = (char*)stream.GetCurrentWriteBuffer();

    /* codepoint is in BMP */
    if (codepoint < 0x10000) {
        begin[0] = '\\';
        begin[1] = 'u';

        //uint16-t high byte
        char _0h = ((codepoint & 0xF000) >> 12);
        if (_0h <= 9) {
            begin[2] = _0h + '0';
        } else {
            begin[2] = _0h + 'a' - 10;
        }

        char _0l = ((codepoint & 0x0F00) >> 8);
        if (_0l <= 9) {
            begin[3] = _0l + '0';
        } else {
            begin[3] = _0l + 'a' - 10;
        }


        //uint16-t low byte
        char _1h = ((codepoint & 0xF0) >> 4);
        if (_1h <= 9) {
            begin[4] = _1h + '0';
        } else {
            begin[4] = _1h + 'a' - 10;
        }

        char _1l = (codepoint & 0x0F);
        if (_1l <= 9) {
            begin[5] = _1l + '0';
        } else {
            begin[5] = _1l + 'a' - 10;
        }

        stream.seekp(6);
    } else { /* not in BMP -> construct a UTF-16 surrogate pair */
        codepoint -= 0x10000;
        int32_t first = 0xD800 | ((codepoint & 0xffc00) >> 10);
        int32_t last = 0xDC00 | (codepoint & 0x003ff);

        //Write First
        {

            begin[0] = '\\';
            begin[1] = 'u';

            //uint16-t high byte
            char _0h = ((first & 0xF000) >> 12);
            if (_0h <= 9) {
                begin[2] = _0h + '0';
            } else {
                begin[2] = _0h + 'a' - 10;
            }

            char _0l = ((first & 0x0F00) >> 8);
            if (_0l <= 9) {
                begin[3] = _0l + '0';
            } else {
                begin[3] = _0l + 'a' - 10;
            }


            //uint16-t low byte
            char _1h = ((first & 0xF0) >> 4);
            if (_1h <= 9) {
                begin[4] = _1h + '0';
            } else {
                begin[4] = _1h + 'a' - 10;
            }

            char _1l = (first & 0x0F);
            if (_1l <= 9) {
                begin[5] = _1l + '0';
            } else {
                begin[5] = _1l + 'a' - 10;
            }
        }


        //Write last
        {
            begin[6] = '\\';
            begin[7] = 'u';

            //uint16-t high byte
            char _0h = ((last & 0xF000) >> 12);
            if (_0h <= 9) {
                begin[8] = _0h + '0';
            } else {
                begin[8] = _0h + 'a' - 10;
            }

            char _0l = ((last & 0x0F00) >> 8);
            if (_0l <= 9) {
                begin[9] = _0l + '0';
            } else {
                begin[9] = _0l + 'a' - 10;
            }


            //uint16-t low byte
            char _1h = ((last & 0xF0) >> 4);
            if (_1h <= 9) {
                begin[10] = _1h + '0';
            } else {
                begin[10] = _1h + 'a' - 10;
            }

            char _1l = (last & 0x0F);
            if (_1l <= 9) {
                begin[11] = _1l + '0';
            } else {
                begin[11] = _1l + 'a' - 10;
            }
        }

        stream.seekp(12);
    }
}

std::string encode_unicode(simcc::uint32 codepoint) {
    simcc::DataStream buf;
    encode_unicode(codepoint, buf);
    return std::string(buf.data(), buf.size());
}
}

TEST_UNIT(test_json_utf16_encode_unicode) {
    simcc::uint32 codepoint = 0x10000 - 1;
    std::string utf16s   = encode_unicode_snprintf(codepoint);
    std::string utf16s_m = encode_unicode(codepoint);
    H_TEST_ASSERT(utf16s == "\\uffff");
    H_TEST_ASSERT(utf16s == utf16s_m);

    codepoint = 0x8912;
    utf16s   = encode_unicode_snprintf(codepoint);
    utf16s_m = encode_unicode(codepoint);
    H_TEST_ASSERT(utf16s == "\\u8912");
    H_TEST_ASSERT(utf16s == utf16s_m);

    codepoint = 0x5e058912;
    utf16s   = encode_unicode_snprintf(codepoint);
    utf16s_m = encode_unicode(codepoint);
    H_TEST_ASSERT(utf16s == "\\ud922\\udd12");
    H_TEST_ASSERT(utf16s == utf16s_m);

}


TEST_UNIT(test_json_iterator_compile) {
    simcc::json::JSONArray ja;
    {
        simcc::json::JSONArray::iterator it(ja.begin());
        simcc::json::JSONArray::iterator ite(ja.begin());
        for (; it != ite; ++it) {
            (void)it;
        }
    }
    {
        simcc::json::JSONArray::iterator it(ja.begin());
        simcc::json::JSONArray::iterator ite(ja.begin());
        for (; it != ite; ++it) {
            (void)it;
        }
    }

    simcc::json::JSONObject jo;
    {
        simcc::json::JSONObject::Iterator it(jo.begin());
        simcc::json::JSONObject::Iterator ite(jo.begin());
        for (; it != ite; ++it) {
            (void)it;
        }
    }
    {
        simcc::json::JSONObject::iterator it(jo.begin());
        simcc::json::JSONObject::iterator ite(jo.begin());
        for (; it != ite; ++it) {
            (void)it;
        }
    }
}



TEST_UNIT(test_json_effective_1) {
    const char* json_test_str = "{\"type\":\"AE,AE\\u6a21\\u677f,AE\\u63d2\\u4ef6,AE\\u6559\\u7a0b,\\u4f1a\\u58f0\\u4f1a\\u5f71,\\u5a5a\\u5e86,\\u680f\\u5305\"}";
    simcc::json::JSONObject jo0;
    H_TEST_ASSERT(jo0.Parse(json_test_str));
    std::string serialize = jo0.ToString();
    simcc::json::JSONObject jo1;
    H_TEST_ASSERT(jo1.Parse(serialize.data(), serialize.size()));
    std::string serialize1 = jo1.ToString();
    H_TEST_ASSERT(jo1.Equals(jo0));
    H_TEST_ASSERT(serialize1 == serialize);
    H_TEST_ASSERT(jo0.error() == simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo1.error() == simcc::json::JSONParser::kNoError);
}







TEST_UNIT(json_in_json_problem_test) {
    std::string outjson = simcc::Base64::Decode("eyJlcnJvcl9jb2RlIjowLCJwbHVnaW5fcmVzdWx0Ijp7InBsdWdpbnMiOlt7ImlkIjowLCJtYWluX2luZm8iOiJ7XCJjaWRcIjozNSxcImxldmVsXCI6NixcImZsYWdcIjowLFwic2NvcmVcIjotNDIsXCJuYW1lXCI6XCJDbm5pY1xcdTRlMmRcXHU2NTg3XFx1NGUwYVxcdTdmNTFcIixcImNvbXBhbnlcIjpcIlxcdTRlMmRcXHU1NmZkXFx1NGU5MlxcdTgwNTRcXHU3ZjUxXFx1N2VkY1xcdTRmZTFcXHU2MDZmXFx1NGUyZFxcdTVmYzNcIixcImluZm9cIjpcIlxcdTU3MzBcXHU1NzQwXFx1NjgwZlxcdTY0MWNcXHU3ZDIyXFx1NjNkMlxcdTRlZjZcXHVmZjBjXFx1NTE3N1xcdTY3MDlcXHU1ZjNhXFx1NTIzNlxcdTViODlcXHU4OGM1XFx1MzAwMVxcdTY1ZTBcXHU2Y2Q1XFx1NWY3YlxcdTVlOTVcXHU1MjIwXFx1OTY2NCBcXHUzMDAxXFx1NWU3MlxcdTYyNzBcXHU1MTc2XFx1NGVkNlxcdThmNmZcXHU0ZWY2XFx1OGZkMFxcdTg4NGNcXHUzMDAxXFx1NTJhYlxcdTYzMDFcXHU2ZDRmXFx1ODljOFxcdTU2NjhcXHU3YjQ5XFx1ODg0Y1xcdTRlM2FcXHUzMDAyXCIsXCJyYXRpb1wiOjQ5LFwiYm94dGV4dFwiOlwiXCIsXCJmbGFnX2V4dFwiOjAsXCJ2ZXJcIjoxfSIsIm1hdGNoIjoxLCJyZW1vdmUiOiJbe1wib3AxXCI6e1wib3Bjb2RlXCI6MyxcImRhdGFcIjp7XCJzdWJtYXRjaFwiOjYzLFwiaW5mbzNcIjpcIjw8W3hhbGxyZWddPj5cIixcInJlbW92ZVwiOjB9fSxcImNvbmRcIjpcIm9wMVwifV0iLCJzY2FuIjoiW3tcIm9wMVwiOntcIm9wY29kZVwiOjMsXCJkYXRhXCI6e1wiaW5mbzNcIjpcIjw8W3hhbGxyZWddPj5cIn19LFwiY29uZFwiOlwib3AxXCJ9XSJ9LHsiaWQiOjEsIm1haW5faW5mbyI6Int9IiwibWF0Y2giOjAsInJlbW92ZSI6Int9Iiwic2NhbiI6Int9In0seyJpZCI6MiwibWFpbl9pbmZvIjoie30iLCJtYXRjaCI6MCwicmVtb3ZlIjoie30iLCJzY2FuIjoie30ifV19fQ==");
    simcc::json::JSONObject joutjson;
    joutjson.Parse(outjson);//1. 第一次JSON解析
    H_TEST_ASSERT(joutjson.ok());

    //2. 取得main_info这个JSONString对象
    simcc::json::JSONString* jmain_info = joutjson.GetJSONObject("plugin_result")->GetJSONArray("plugins")->GetJSONObject(0)->GetJSONString("main_info");
    H_TEST_ASSERT(jmain_info);

    //3. 取得main_info这个JSONString对象的实际内存的字符串value
    std::string main_info_string = jmain_info->value();
    //    std::cout << "base64(main_info_string)=" << simcc::Base64::encode(main_info_string) << "\n";

    //4. 最后进行JSON解析
    simcc::json::JSONObject jmain_info_obj;
    jmain_info_obj.Parse(main_info_string);
    H_TEST_ASSERT(jmain_info_obj.ok());
    //bool readable_print = true;
    //     std::cout << "main_info_json=\n"
    //         << jmain_info_obj.ToString(readable_print);

}



TEST_UNIT(json_parse_tab_test1) {
    /*
    json串为：
    {
    "exts":
    [
    {
    "key":"candidate",
    "val" : "原阳县粮食局\t原阳县公共资源交易中心"
    }
    ]
    }
    */
    std::string s = simcc::Base64::Decode("eyJleHRzIjpbeyJ2YWwiOiJcdTUzOWZcdTk2MzNcdTUzYmZcdTdjYWVcdTk4ZGZcdTVjNDBcdFx1NTM5Zlx1OTYzM1x1NTNiZlx1NTE2Y1x1NTE3MVx1OGQ0NFx1NmU5MFx1NGVhNFx1NjYxM1x1NGUyZFx1NWZjMyIsImtleSI6ImNhbmRpZGF0ZSJ9XX0=");
    simcc::json::JSONObject jo;
    jo.Parse(s);
    H_TEST_ASSERT(jo.ok());
    simcc::json::JSONArray* jexts = jo.GetJSONArray("exts");
    H_TEST_ASSERT(jexts);

    simcc::json::JSONObject* jext = jexts->GetJSONObject(0);
    H_TEST_ASSERT(jext);
    simcc::json::JSONString* jval = jext->GetJSONString("val");
    H_TEST_ASSERT(jval);

    std::string utf8val = simcc::Base64::Decode("5Y6f6Ziz5Y6/57Ku6aOf5bGACeWOn+mYs+WOv+WFrOWFsei1hOa6kOS6pOaYk+S4reW/gw==");
    std::string val = jval->value();
    H_TEST_ASSERT(val == utf8val);
    //#ifdef H_OS_WINDOWS
    //    std::string utf8val = simcc::StringUtil::mbsToUtf8("原阳县粮食局\t原阳县公共资源交易中心");
    //    std::string val = jval->value();
    //    H_TEST_ASSERT(val == utf8val);
    //#else
    //#endif
}

TEST_UNIT(json_parse_tab_test2) {
    std::string escape_strings = simcc::Base64::Decode("eyAiYSIgOiAiYVx0YlxuY1xiZFxmZVxyIiB9DQ=="); // { "a" : "a\tb\nc\bd\fe\r" }
    simcc::json::JSONObject jo;
    H_TEST_ASSERT(jo.Parse(escape_strings));
    H_TEST_ASSERT(jo.ok());
    simcc::json::JSONString* jext = jo.GetJSONString("a");
    H_TEST_ASSERT(jext);
    H_TEST_ASSERT(jext->value() == "a\tb\nc\bd\fe\r");
}






TEST_UNIT(json_parse_tab_test3) {
    std::string s = simcc::Base64::Decode("ewoJImlkIjowLAoJIm1haW5faW5mbyI6IntcXFwiY2lkXFxcIjo0MDg5LFxcXCJsZXZlbFxcXCI6MixcXFwiZmxhZ1xcXCI6MCxcXFwic2NvcmVcXFwiOjg4LFxcXCJuYW1lXFxcIjpcXFwiXFxcXHU1MzcwXFxcXHU4YzYxXFxcXHU3YjE0XFxcXHU4YmIwKEV2ZXJub3RlKVxcXFx1OTY0NFxcXFx1NWUyNlxcXFx1NmQ0ZlxcXFx1ODljOFxcXFx1NTY2OFxcXFx1NjNkMlxcXFx1NGVmNlxcXCIsXFxcImNvbXBhbnlcXFwiOlxcXCJFdmVybm90ZS5jb21cXFwiLFxcXCJpbmZvXFxcIjpcXFwiXFxcXHU0ZTAwXFxcXHU2YjNlXFxcXHU1M2VmXFxcXHU0ZWU1XFxcXHU1ZTJlXFxcXHU1MmE5XFxcXHU2MGE4XFxcXHU4YmIwXFxcXHU0ZjRmXFxcXHU3NmY4XFxcXHU1MTczXFxcXHU0ZmUxXFxcXHU2MDZmXFxcXHUzMDAxXFxcXHU1ZmViXFxcXHU5MDFmXFxcXHU1NDJmXFxcXHU1MmE4RXZlcm5vdGVcXFxcdTY3MGRcXFxcdTUyYTFcXFxcdTc2ODRcXFxcdTZkNGZcXFxcdTg5YzhcXFxcdTU2NjhcXFxcdTYzMDlcXFxcdTk0YWVcXFxcdTMwMDJcXFwiLFxcXCJyYXRpb1xcXCI6MyxcXFwiYm94dGV4dFxcXCI6XFxcIlxcXCIsXFxcImZsYWdfZXh0XFxcIjowLFxcXCJ2ZXJcXFwiOjF9IiwKCSJtYXRjaCI6MQp9Cgo=");
    simcc::json::JSONObject jo;
    jo.Parse(s);
    H_TEST_ASSERT(jo.ok());
    simcc::json::JSONString* jval = jo.GetJSONString("main_info");
    H_TEST_ASSERT(jval);
}

TEST_UNIT(json_parse_tab_test4) {
    std::string escape_strings = simcc::Base64::Decode("eyAiYSIgOiAiYVx0YlxuY1xiZFxmZVxyIiB9DQ=="); // { "a" : "a\tb\nc\bd\fe\r" }
    simcc::json::JSONObject jo;
    H_TEST_ASSERT(jo.Parse(escape_strings));
    H_TEST_ASSERT(jo.ok());
    simcc::json::JSONString* jext = jo.GetJSONString("a");
    H_TEST_ASSERT(jext);
    H_TEST_ASSERT(jext->value() == "a\tb\nc\bd\fe\r");
}



TEST_UNIT(testJSONDoubleSerialize) {
    const char* json_test_str = "{\"cost\":1.0}";
    simcc::json::JSONObject jo0;
    H_TEST_ASSERT(jo0.Parse(json_test_str));
    std::string serialize = jo0.ToString();
    simcc::json::JSONObject jo1;
    H_TEST_ASSERT(jo1.Parse(serialize.data(), serialize.size()));
    std::string serialize1 = jo1.ToString();
    H_TEST_ASSERT(jo1.Equals(jo0));
    H_TEST_ASSERT(serialize1 == serialize);
    H_TEST_ASSERT(jo0.error() == simcc::json::JSONParser::kNoError);
    H_TEST_ASSERT(jo1.error() == simcc::json::JSONParser::kNoError);

    simcc::json::JSONDouble* d = jo0.GetJSONDouble("cost");
    H_TEST_ASSERT(d);
    std::string s = d->ToString();
    H_TEST_ASSERT(simcc::StringUtil::Contains(s, ".0"));
}

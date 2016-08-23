#include "test_common.h"

#include "wcpp/ini_parser.h"
#include "wcpp/data_stream.h"
#include "wcpp/file_util.h"
#include "wcpp/base64.h"
#include "wcpp/timestamp.h"

namespace {
void test_ini_parser_1() {
    const char* rawdata = "mid=ac9219aa5232c4e519ae5fcb4d77ae5b\r\n"
                          "product=xxxxxxse\r\n"
                          "combo=sedl\r\n"
                          "version=4.4\r\n"
                          "appext=";
    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser;
    parser.Parse(rawdata, rawdatalen, "\r\n", "=");
    bool found = false;
    {
        {
            std::string key = "mid";
            std::string value = "ac9219aa5232c4e519ae5fcb4d77ae5b";
            H_TEST_ASSERT(value == parser.Get(key));
            H_TEST_ASSERT(value == parser.Get(key, &found));
            H_TEST_ASSERT(found);
        }

        {
            std::string key = "product";
            std::string value = "xxxxxxse";
            H_TEST_ASSERT(value == parser.Get(key));
            H_TEST_ASSERT(value == parser.Get(key, &found));
            H_TEST_ASSERT(found);
        }

        {
            std::string key = "combo";
            std::string value = "sedl";
            H_TEST_ASSERT(value == parser.Get(key));
        }

        {
            std::string key = "version";
            std::string value = "4.4";
            H_TEST_ASSERT(value == parser.Get(key));
        }

        {
            std::string key = "appext";
            std::string value = "";
            H_TEST_ASSERT(value == parser.Get(key));
        }
    }

    const wcpp::INIParser::ssmap& kvmap = parser.GetDefaultKeyValueMap();
    wcpp::INIParser::ssmap::const_iterator it(kvmap.begin());
    wcpp::INIParser::ssmap::const_iterator ite(kvmap.end());
    for (; it != ite; ++it) {

    }

}

void test_ini_parser_section_6() {
    const char* rawdata =
        "             \r\n"
        " mid=ac9219aa5232c4e519ae5fcb4d77ae5b\r\n"
        " product=xxxxxxse\r\n"
        "             \r\n"
        " [xxxxxxse] \r\n"
        "             \r\n"
        "combo=sedl\r\n"
        "version=4.4\r\n"
        "             \r\n"
        "             \r\n"
        "[a ]  \r\n"
        "             \r\n"
        "appext=0\r\n"
        "appext1=1\r\n"
        "             \r\n"
        "[b] \r\n"
        "             \r\n"
        "appext=b0\r\n"
        "appext1=b1\r\n"
        "             \r\n"
        "[ c]\r\n"
        "             \r\n"
        "appext=c0\r\n"
        "appext1=c1\r\n"
        "             \r\n"
        "             \r\n"
        "             \r\n";

    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser;
    auto f = [](::wcpp::INIParser & p, const ::std::string & section, const ::std::string & key, const ::std::string & value) {
        (void)section;
        (void)key;
        (void)value;
        size_t c = p.GetDefaultKeyValueMap().size();
        size_t b = c;
        (void)b;
    };
    parser.SetParseListener(f);

    parser.Parse(rawdata, rawdatalen, "\r\n", "=");

    bool found = false;

    {
        std::string key = "mid";
        std::string value = "ac9219aa5232c4e519ae5fcb4d77ae5b";
        H_TEST_ASSERT(value == parser.Get(key));
    }

    {
        std::string key = "product";
        std::string value = "xxxxxxse";
        H_TEST_ASSERT(value == parser.Get(key));
    }

    {
        std::string key = "combo";
        std::string value = "sedl";
        H_TEST_ASSERT(value == parser.Get("xxxxxxse", key));
    }

    {
        std::string key = "version";
        std::string value = "4.4";
        H_TEST_ASSERT(value == parser.Get("xxxxxxse", key));
        H_TEST_ASSERT(value == parser.Get("xxxxxxse", key, &found));
        H_TEST_ASSERT(found);
    }

    {
        std::string section = "a";
        std::string key = "appext";
        std::string value = "0";
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);

        key = "appext1";
        value = "1";
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);
    }

    {
        std::string section = "b";
        std::string key = "appext";
        std::string value = "b0";
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);

        key = "appext1";
        value = "b1";
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);
    }

    {
        std::string section = "c";
        std::string key = "appext";
        std::string value = "c0";
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);

        key = "appext1";
        value = "c1";
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);
    }

    {
        std::string section = "c";
        std::string key = "apadft";
        std::string value = "";
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(!found);
    }
}

void test_ini_parser_case_senstive_7() {
    const char* rawdata =
        "             \r\n"
        " Mid=ac9219aa5232c4e519ae5fcb4d77ae5b\r\n"
        " prOduct=xxxxxxse\r\n"
        "             \r\n"
        " [xxxxXxsE] \r\n"
        "             \r\n"
        "combo=sedl\r\n"
        "version=4.4\r\n"
        "             \r\n"
        "             \r\n"
        "[A ]  \r\n"
        "appext=0\r\n"
        "appext1=1\r\n"
        "             \r\n"
        "[B]  \r\n"
        "             \r\n"
        "appext=b0\r\n"
        "appext1=b1\r\n"
        "             \r\n"
        "[ c]  \r\n"
        "             \r\n"
        "appext=c0\r\n"
        "appext1=c1\r\n"
        "             \r\n"
        "             \r\n"
        "[hold住]\r\n"
        "有木有=不想上班的有木有!!!!\r\n";

    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser(false);
    parser.Parse(rawdata, rawdatalen, "\r\n", "=");

    bool found = false;
    {
        std::string key = "mId";
        std::string value = "ac9219aa5232c4e519ae5fcb4d77ae5b";
        H_TEST_ASSERT(value == parser.Get(key));
    }

    {
        std::string key = "proDuct";
        std::string value = "xxxxxxse";
        H_TEST_ASSERT(value == parser.Get(key));
    }

    {
        std::string key = "comBo";
        std::string value = "sedl";
        H_TEST_ASSERT(value == parser.Get("xxxxxxse", key));
    }

    {
        std::string key = "veRsioN";
        std::string value = "4.4";
        H_TEST_ASSERT(value == parser.Get("xxxxxxse", key));
    }

    {
        std::string section = "A";
        std::string key = "appext";
        std::string value = "0";
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);

        key = "appeXt1";
        value = "1";
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);
    }

    {
        std::string section = "b";
        std::string key = "apPext";
        std::string value = "b0";
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);

        key = "appext1";
        value = "b1";
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);
    }

    {
        std::string section = "c";
        std::string key = "appExt";
        std::string value = "c0";
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);

        key = "appexT1";
        value = "c1";
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);
    }

    {

        if (parser.case_sensitive()) {
            std::string section = "hold住";
            std::string key = "有木有";
            std::string value = "不想上班的有木有!!!!";
            H_TEST_ASSERT(value == parser.Get(section, key));
        } else {
            wcpp::INIParser parser_case_senstive(true);
            parser_case_senstive.Parse(rawdata, rawdatalen, "\r\n", "=");
            H_TEST_ASSERT(parser_case_senstive.case_sensitive());
            std::string section = "hold住";
            std::string key = "有木有";
            std::string value = "不想上班的有木有!!!!";
            H_TEST_ASSERT(value == parser_case_senstive.Get(section, key));
        }
    }
}


void test_ini_parser_set_8() {
    wcpp::INIParser parser;
    bool found = false;
    {
        std::string key = "mid";
        std::string value = "ac9219aa5232c4e519ae5fcb4d77ae5b";
        parser.Set("", key, value);
        H_TEST_ASSERT(value == parser.Get(key));
    }

    {
        std::string key = "product";
        std::string value = "xxxxxxse";
        parser.Set("", key, value);
        H_TEST_ASSERT(value == parser.Get(key));
    }

    {
        std::string section = "xxxxxxse";
        std::string key = "combo";
        std::string value = "sedl";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));
        H_TEST_ASSERT(value == parser.Get(section, key, &found));
        H_TEST_ASSERT(found);
    }

    {
        std::string section = "xxxxxxse";
        std::string key = "version";
        std::string value = "4.4";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    {
        std::string section = "a";
        std::string key = "appext";
        std::string value = "0";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));

        key = "appext1";
        value = "1";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));


        key = "appext1";
        value = "3";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));


        key = "appext1";
        value = "3   ddddssaa     dddd";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));


        key = "appext1";
        value = "3   FFFFFFFFFFFF     dddd";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    {
        std::string section = "b";
        std::string key = "appext";
        std::string value = "b0";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));

        key = "appext1";
        value = "b1";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));


        value = "bccccccccccccccccccccc1";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));

        value = "你懂的cccccc";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));

        value = "不管你信不信，反正我信了aaaa";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));

        value = "浮云bbb";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    {
        std::string section = "c";
        std::string key = "appext";
        std::string value = "c0";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));

        key = "appext1";
        value = "c1";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    {
        std::string section = "不要仰视哥";
        std::string key = "哥是一个神话";
        std::string value = "hold住";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));

        key = "appext1";
        value = "c1";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));
    }
}


void test_ini_parser_output_9() {
    wcpp::INIParser parser;

    {
        std::string key = "mid";
        std::string value = "ac9219aa5232c4e519ae5fcb4d77ae5b";
        parser.Set("", key, value);
        H_TEST_ASSERT(value == parser.Get(key));
    }

    {
        std::string key = "product";
        std::string value = "xxxxxxse";
        parser.Set("", key, value);
        H_TEST_ASSERT(value == parser.Get(key));
    }

    {
        std::string section = "xxxxxxse";
        std::string key = "combo";
        std::string value = "sedl";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    {
        std::string section = "xxxxxxse";
        std::string key = "version";
        std::string value = "4.4";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    {
        std::string section = "a";
        std::string key = "appext";
        std::string value = "0";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));

        key = "appext1";
        value = "1";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    {
        std::string section = "b";
        std::string key = "appext";
        std::string value = "b0";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));

        key = "appext1";
        value = "b1";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    {
        std::string section = "c";
        std::string key = "appext";
        std::string value = "c0";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));

        key = "appext1";
        value = "c1";
        parser.Set(section, key, value);
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    parser.SetKVSeparator("=");
    parser.SetLineSeparator("\n");
    std::string output = parser.Serialize();
    wcpp::INIParser test_ini_parser_parser;
    H_TEST_ASSERT(test_ini_parser_parser.Parse(output.data(), output.length()));

    {
        std::ostringstream oss;
        parser.Serialize(oss);
        H_TEST_ASSERT(oss.str() == output);
    }

    {

        {
            std::string key = "mid";
            std::string value = "ac9219aa5232c4e519ae5fcb4d77ae5b";
            H_TEST_ASSERT(value == parser.Get(key));
        }

        {
            std::string key = "product";
            std::string value = "xxxxxxse";
            H_TEST_ASSERT(value == parser.Get(key));
        }

        {
            std::string key = "combo";
            std::string value = "sedl";
            H_TEST_ASSERT(value == parser.Get("xxxxxxse", key));
        }

        {
            std::string key = "version";
            std::string value = "4.4";
            H_TEST_ASSERT(value == parser.Get("xxxxxxse", key));
        }

        {
            std::string section = "a";
            std::string key = "appext";
            std::string value = "0";
            H_TEST_ASSERT(value == parser.Get(section, key));

            key = "appext1";
            value = "1";
            H_TEST_ASSERT(value == parser.Get(section, key));
        }

        {
            std::string section = "b";
            std::string key = "appext";
            std::string value = "b0";
            H_TEST_ASSERT(value == parser.Get(section, key));

            key = "appext1";
            value = "b1";
            H_TEST_ASSERT(value == parser.Get(section, key));
        }

        {
            std::string section = "c";
            std::string key = "appext";
            std::string value = "c0";
            H_TEST_ASSERT(value == parser.Get(section, key));

            key = "appext1";
            value = "c1";
            H_TEST_ASSERT(value == parser.Get(section, key));
        }
    }
}



void test_ini_parser_chinese_10() {
    const char* rawdata =
        "             \r\n"
        " 伤不起=学法语的人你伤不起啊\r\n"
        " 卖萌=当我们都去卖萌，都去装可爱的时候，说明这个世界还是萌的，还是可爱的。\r\n"
        "             \r\n"
        " [坑爹] \r\n"
        "             \r\n"
        "天龙八部=你妹啊!\r\n"
        "你妹=你妹是个很深奥的问题！\r\n"
        "             \r\n"
        "             \r\n"
        "[悲催 ]  \r\n"
        "             \r\n"
        " 悲惨得催人泪下=一般表示不称意，不顺心，失败，伤心，悔恨等意思   \r\n"
        "appext1=1\r\n"
        "             \r\n"
        "[普通青年] \r\n"
        "             \r\n"
        "文艺青年=2B青年\r\n";

    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser;
    parser.Parse(rawdata, rawdatalen, "\r\n", "=");

    {
        std::string key = "伤不起";
        std::string value = "学法语的人你伤不起啊";
        H_TEST_ASSERT(value == parser.Get(key));
    }

    {
        std::string key = "卖萌";
        std::string value = "当我们都去卖萌，都去装可爱的时候，说明这个世界还是萌的，还是可爱的。";
        H_TEST_ASSERT(value == parser.Get(key));
    }


    {
        std::string section = "坑爹";
        std::string key = "天龙八部";
        std::string value = "你妹啊!";
        H_TEST_ASSERT(value == parser.Get(section, key));

        key = "你妹";
        value = "你妹是个很深奥的问题！";
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    {
        std::string section = "悲催";
        std::string key = "悲惨得催人泪下";
        std::string value = "一般表示不称意，不顺心，失败，伤心，悔恨等意思";
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    {
        std::string section = "普通青年";
        std::string key = "文艺青年";
        std::string value = "2B青年";
        H_TEST_ASSERT(value == parser.Get(section, key));
    }
}

void test_ini_parser_11() {
    const char* rawdata = "mid=ac9219aa5232c4e519ae5fcb4d77ae5b&"
                          "product=xxxxxxse&"
                          "combo=sedl&"
                          "version=4.4&"
                          "appext=aa";
    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser;
    parser.Parse(rawdata, rawdatalen, "&", "=");

    {
        {
            std::string key = "mid";
            std::string value = "ac9219aa5232c4e519ae5fcb4d77ae5b";
            H_TEST_ASSERT(value == parser.Get(key));
        }

        {
            std::string key = "product";
            std::string value = "xxxxxxse";
            H_TEST_ASSERT(value == parser.Get(key));
        }

        {
            std::string key = "combo";
            std::string value = "sedl";
            H_TEST_ASSERT(value == parser.Get(key));
        }

        {
            std::string key = "version";
            std::string value = "4.4";
            H_TEST_ASSERT(value == parser.Get(key));
        }

        {
            std::string key = "appext";
            std::string value = "aa";
            H_TEST_ASSERT(value == parser.Get(key));
        }
    }

    const wcpp::INIParser::ssmap& kvmap = parser.GetDefaultKeyValueMap();
    wcpp::INIParser::ssmap::const_iterator it(kvmap.begin());
    wcpp::INIParser::ssmap::const_iterator ite(kvmap.end());
    for (; it != ite; ++it) {

    }

}

class MyParseListener {
public:
    void OnValue(wcpp::INIParser& parser, const std::string& section, const std::string& key, const std::string& value) {
        parser.StopParsing(true);
    }
};


void test_ini_parser_stop_parse_11() {
    const char* rawdata =
        "             \r\n"
        " Mid=ac9219aa5232c4e519ae5fcb4d77ae5b\r\n"
        " prOduct=xxxxxxse\r\n"
        "             \r\n"
        " [360sE] \r\n"
        "             \r\n"
        "combo=sedl\r\n"
        "version=4.4\r\n"
        "             \r\n"
        "             \r\n"
        "[A ]  \r\n"
        "appext=0\r\n"
        "appext1=1\r\n"
        "             \r\n"
        "[B]  \r\n"
        "             \r\n"
        "appext=b0\r\n"
        "appext1=b1\r\n"
        "             \r\n"
        "[ c]  \r\n"
        "             \r\n"
        "appext=c0\r\n"
        "appext1=c1\r\n"
        "             \r\n"
        "             \r\n"
        "[hold住]\r\n"
        "有木有=不想上班的有木有!!!!\r\n";

    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser(false);
    auto f = [](wcpp::INIParser & p, const std::string & section, const std::string & key, const std::string & value) {
        p.StopParsing(true);
    };
    parser.SetParseListener(f);
    parser.Parse(rawdata, rawdatalen, "\r\n", "=");
}


void test_ini_parser_bug_string_12() {
    const char* data = "files=librun.dat\t1.0.1.3557|libtask.dat\t1.0.1.3557|deepscan\\cloudsec2.dll\t3.2.8.1002\r\nmodules=360hotfix\t1.0.0.100|deepscan\t2.0.2.200\r\ntags=tencent|kingsoft\r\npid=123\r\nuid=234\r\nver=3.2.8.1002\r\nsysver=6.1.7600\r\npa=32\r\ntype=updated\r\nrt=2\r\nlt=1.5\r\nue=0\r\nlang=zh_CN\r\nvdays=365\r\nuname=zhangshan\r\nrate=360hotfix\t3|deepscan\t10\r\nproduct=360safe\r\ncombo=quick\r\nmid=a16cf365149a8aed21fdd04ae2545824\r\n";
    wcpp::INIParser parser(false);
    parser.Parse(data, strlen(data), "\r\n", "=");
}



void test_ini_parser_serialize_13_internal(wcpp::INIParser& parser) {

    {
        std::string key = "mid";
        std::string value = "ac9219aa5232c4e519ae5fcb4d77ae5b";
        H_TEST_ASSERT(value == parser.Get(key));
    }

    {
        std::string key = "product";
        std::string value = "xxxxxxse";
        H_TEST_ASSERT(value == parser.Get(key));
    }

    {
        std::string key = "combo";
        std::string value = "sedl";
        H_TEST_ASSERT(value == parser.Get("xxxxxxse", key));
    }

    {
        std::string key = "version";
        std::string value = "4.4";
        H_TEST_ASSERT(value == parser.Get("xxxxxxse", key));
    }

    {
        std::string section = "a";
        std::string key = "appext";
        std::string value = "0";
        H_TEST_ASSERT(value == parser.Get(section, key));

        key = "appext1";
        value = "1";
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    {
        std::string section = "b";
        std::string key = "appext";
        std::string value = "b0";
        H_TEST_ASSERT(value == parser.Get(section, key));

        key = "appext1";
        value = "b1";
        H_TEST_ASSERT(value == parser.Get(section, key));
    }

    {
        std::string section = "c";
        std::string key = "appext";
        std::string value = "c0";
        H_TEST_ASSERT(value == parser.Get(section, key));

        key = "appext1";
        value = "c1";
        H_TEST_ASSERT(value == parser.Get(section, key));
    }
}

class MyFastVisitor {
public:
    MyFastVisitor(const char* ini_string) {
        init(ini_string);
    }

    virtual void visit(const wcpp::INIParser& parser, const std::string& section, const std::string& key, const std::string& value) {
        H_TEST_ASSERT(value == parser_.Get(section, key));
    }

    void init(const char* ini_string) {
        H_TEST_ASSERT(parser_.Parse(ini_string, strlen(ini_string)));
    }

public:
    wcpp::INIParser parser_;
};

class MySequenceVisitor {
public:
    MySequenceVisitor(const char* ini_string) {
        init(ini_string);
    }

    void init(const char* ini_string) {
        H_TEST_ASSERT(parser_.Parse(ini_string, strlen(ini_string)));

        null_section_vector_.push_back("mid");
        null_section_vector_.push_back("product");

        c_section_vector_.push_back("appext");
        c_section_vector_.push_back("appext1");

        xxxxxxse_section_vector_.push_back("version");
        xxxxxxse_section_vector_.push_back("combo");

        b_section_vector_.push_back("appext");
        b_section_vector_.push_back("appext1");
        b_section_vector_.push_back("appbxt2");
        b_section_vector_.push_back("appaxt0");
        b_section_vector_.push_back("appbxt8");
        b_section_vector_.push_back("appaxt6");
        b_section_vector_.push_back("appbxt9");
        b_section_vector_.push_back("appaxt5");

        a_section_vector_.push_back("appext");
        a_section_vector_.push_back("appext1");
        a_section_vector_.push_back("appext3");
        a_section_vector_.push_back("appext2");

        null_index_ = 0;
        c_index_ = 0;
        xxxxxxse_index_ = 0;
        b_index_ = 0;
        a_index_ = 0;
    }

    virtual void visit(const wcpp::INIParser& parser, const std::string& section, const std::string& key, const std::string& value) {
        H_TEST_ASSERT(value == parser_.Get(section, key));

        if (section == "") {
            H_TEST_ASSERT(null_section_vector_[null_index_++] == key);
            H_TEST_ASSERT(c_index_ == 0 && xxxxxxse_index_ == 0 && b_index_ == 0 && a_index_ == 0);
        } else if (section == "c") {
            H_TEST_ASSERT(null_index_ != 0);
            H_TEST_ASSERT(c_section_vector_[c_index_++] == key);
            H_TEST_ASSERT(xxxxxxse_index_ == 0 && b_index_ == 0 && a_index_ == 0);
        } else if (section == "xxxxxxse") {
            H_TEST_ASSERT(null_index_ != 0 && c_index_ != 0);
            H_TEST_ASSERT(xxxxxxse_section_vector_[xxxxxxse_index_++] == key);
            H_TEST_ASSERT(b_index_ == 0 && a_index_ == 0);
        } else if (section == "b") {
            H_TEST_ASSERT(null_index_ != 0 && c_index_ != 0 && xxxxxxse_index_ != 0);
            H_TEST_ASSERT(b_section_vector_[b_index_++] == key);
            H_TEST_ASSERT(a_index_ == 0);
        } else if (section == "a") {
            H_TEST_ASSERT(null_index_ != 0 && c_index_ != 0 && xxxxxxse_index_ != 0 && b_index_ != 0);
            H_TEST_ASSERT(a_section_vector_[a_index_++] == key);
        } else {
            H_TEST_ASSERT(false);
        }
    }

public:
    wcpp::INIParser parser_;

    typedef std::vector< std::string > stringvector;

    stringvector null_section_vector_;
    stringvector c_section_vector_;
    stringvector xxxxxxse_section_vector_;
    stringvector b_section_vector_;
    stringvector a_section_vector_;

    int null_index_;
    int c_index_;
    int xxxxxxse_index_;
    int b_index_;
    int a_index_;
};

void test_ini_parser_serialize_13() {
    const char* rawdata =
        "mid=ac9219aa5232c4e519ae5fcb4d77ae5b\r\n"
        "product=xxxxxxse\r\n"
        "[c]\r\n"
        "appext=c0\r\n"
        "appext1=c1\r\n"
        "[xxxxxxse]\r\n"
        "version=4.4\r\n"
        "combo=sedl\r\n"
        "[b]\r\n"
        "appext=b0\r\n"
        "appext1=b1\r\n"
        "appbxt2=b0\r\n"
        "appaxt0=b1\r\n"
        "appbxt8=b0\r\n"
        "appaxt6=b1\r\n"
        "appbxt9=b0\r\n"
        "appaxt5=b1\r\n"
        "[a]\r\n"
        "appext=0\r\n"
        "appext1=1\r\n"
        "appext3=3\r\n"
        "appext2=2\r\n";

    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser1(true, false, true);
    H_TEST_ASSERT(parser1.Parse(rawdata, rawdatalen, "\r\n", "="));
    test_ini_parser_serialize_13_internal(parser1);

    std::string sequence_str = parser1.Serialize(true);
    H_TEST_ASSERT(sequence_str == rawdata);

    wcpp::INIParser parser2(true, false, true);
    H_TEST_ASSERT(parser2.Parse(sequence_str.data(), sequence_str.size(), "\r\n", "="));
    test_ini_parser_serialize_13_internal(parser2);
    std::string sequence_str2 = parser2.Serialize(true);
    H_TEST_ASSERT(sequence_str2 == sequence_str);

    wcpp::INIParser parser3(true, false, true);
    H_TEST_ASSERT(parser3.Parse(sequence_str2.data(), sequence_str2.size(), "\r\n", "="));
    test_ini_parser_serialize_13_internal(parser3);
    std::string sequence_str3 = parser3.Serialize(false);


    wcpp::INIParser parser4(true, false, true);
    H_TEST_ASSERT(parser4.Parse(sequence_str3.data(), sequence_str3.size(), "\r\n", "="));
    test_ini_parser_serialize_13_internal(parser4);
    std::string sequence_str4 = parser4.Serialize(true);
    H_TEST_ASSERT(sequence_str4 == sequence_str3);

    {
        MyFastVisitor vistor(sequence_str3.data());
        wcpp::INIParser::Visitor f = std::bind(&MyFastVisitor::visit, &vistor, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        parser4.Visit("a", f, false);
        parser4.Visit("b", f, false);
        parser4.Visit("c", f, false);
        parser4.Visit(f, false);
    }

    {
        MySequenceVisitor vistor(rawdata);
        wcpp::INIParser::Visitor f = std::bind(&MySequenceVisitor::visit, &vistor, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        parser1.Visit(f, true);
    }

    {
        MySequenceVisitor vistor(rawdata);
        wcpp::INIParser::Visitor f = std::bind(&MySequenceVisitor::visit, &vistor, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        parser1.Visit("", f, true);
        parser1.Visit("c", f, true);
        parser1.Visit("xxxxxxse", f, true);
        parser1.Visit("b", f, true);
        parser1.Visit("a", f, true);
    }
}


void test_ini_parser_no_trim_15() {
    const char* rawdata = "mid=ac9219aa5232c4e519ae5fcb4d77ae5b\t\r\n"
                          "product=xxxxxxse\r\n"
                          "combo=sedl\tx\r\n"
                          "version=4.4 \r\n"
                          "appext=aa \r\n";
    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser;
    parser.set_trim_chars("");
    parser.Parse(rawdata, rawdatalen, "\r\n", "=");

    {
        {
            std::string key = "mid";
            std::string value = "ac9219aa5232c4e519ae5fcb4d77ae5b\t";
            H_TEST_ASSERT(value == parser.Get(key));
        }

        {
            std::string key = "product";
            std::string value = "xxxxxxse";
            H_TEST_ASSERT(value == parser.Get(key));
        }

        {
            std::string key = "combo";
            std::string value = "sedl\tx";
            H_TEST_ASSERT(value == parser.Get(key));
        }

        {
            std::string key = "version";
            std::string value = "4.4 ";
            H_TEST_ASSERT(value == parser.Get(key));
        }

        {
            std::string key = "appext";
            std::string value = "aa ";
            H_TEST_ASSERT(value == parser.Get(key));
        }
    }

}

void test_ini_parser_wrong_format_1() {
    const char* rawdata =
        "0\r\n"
        "mid=ac9219aa5232c4e519ae5fcb4d77ae5b\r\n"
        "product=xxxxxxse\r\n"
        "[c]\r\n"
        "appext=c0\r\n";

    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser1;
    H_TEST_ASSERT(!parser1.Parse(rawdata, rawdatalen, "\r\n", "="));
}

void test_ini_parser_wrong_format_2() {
    const char* rawdata =
        "mid=ac9219aa5232c4e519ae5fcb4d77ae5b\r\n"
        "product=xxxxxxse\r\n"
        "[c]\r\n"
        "appext=c0\r\n"
        "0\r\n";

    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser1;
    H_TEST_ASSERT(!parser1.Parse(rawdata, rawdatalen, "\r\n", "="));
}

void test_ini_parser_wrong_format_3() {
    const char* rawdata =
        "mid=ac9219aa5232c4e519ae5fcb4d77ae5b\r\n"
        "product=xxxxxxse\r\n"
        "[c\r\n"
        "appext=c0\r\n"
        "0\r\n";

    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser1;
    H_TEST_ASSERT(!parser1.Parse(rawdata, rawdatalen, "\r\n", "="));
    H_TEST_ASSERT(parser1.error() == wcpp::INIParser::kErrorSectionFormatWrong);
}

void test_ini_parser_compatible_wrong_format_1() {
    const char* rawdata =
        "45\r\n"
        "mid=ac9219aa5232c4e519ae5fcb4d77ae5b\r\n"
        "product=xxxxxxse\r\n"
        "[c]\r\n"
        "appext=c0\r\n";

    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser1(true, true);
    H_TEST_ASSERT(parser1.Parse(rawdata, rawdatalen, "\r\n", "="));
    H_TEST_ASSERT(parser1.Get("mid") == "ac9219aa5232c4e519ae5fcb4d77ae5b");
    H_TEST_ASSERT(parser1.Get("product") == "xxxxxxse");
    H_TEST_ASSERT(parser1.Get("c", "appext") == "c0");
    H_TEST_ASSERT(parser1.error() != wcpp::INIParser::kNoError);
}

void test_ini_parser_compatible_wrong_format_2() {
    const char* rawdata =
        "mid=ac9219aa5232c4e519ae5fcb4d77ae5b\r\n"
        "product=xxxxxxse\r\n"
        "[c]\r\n"
        "appext=c0\r\n"
        "0\r\n";

    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser1(true, true);
    H_TEST_ASSERT(parser1.Parse(rawdata, rawdatalen, "\r\n", "="));
    H_TEST_ASSERT(parser1.error() != wcpp::INIParser::kNoError);
}

void test_ini_parser_compatible_wrong_format_3() {
    const char* rawdata =
        "mid=ac9219aa5232c4e519ae5fcb4d77ae5b\r\n"
        "product=xxxxxxse\r\n"
        "[c\r\n"
        "appext=c0\r\n"
        "0\r\n";

    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser(true, true);
    H_TEST_ASSERT(parser.Parse(rawdata, rawdatalen, "\r\n", "="));
    H_TEST_ASSERT(parser.error() != wcpp::INIParser::kNoError);
    H_TEST_ASSERT(parser.Get("mid") == "ac9219aa5232c4e519ae5fcb4d77ae5b");
    H_TEST_ASSERT(parser.Get("c", "appext") == "c0");
}

void test_ini_parser_compatible_wrong_format_4() {
    const char* rawdata =
        "mid=ac9219aa5232c4e519ae5fcb4d77ae5b\r\n"
        "[b]\r\n"
        "product=xxxxxxse\r\n"
        "[\r\n"
        "appext=c0\r\n"
        "[  \r\n"
        "appext1=c1\r\n"
        "44\r\n";

    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser(true, true);
    H_TEST_ASSERT(parser.Parse(rawdata, rawdatalen, "\r\n", "="));
    H_TEST_ASSERT(parser.error() != wcpp::INIParser::kNoError);
    H_TEST_EQUAL(parser.Get("mid"), "ac9219aa5232c4e519ae5fcb4d77ae5b");
    H_TEST_EQUAL(parser.Get("b", "product"), "xxxxxxse");
    H_TEST_EQUAL(parser.Get("appext"), "c0");
    H_TEST_EQUAL(parser.Get("appext1"), "c1");
}
}

TEST_UNIT(ini_parser) {
    test_ini_parser_1();
    test_ini_parser_11();
    test_ini_parser_bug_string_12();
}

TEST_UNIT(ini_parser_more_unit_test) {
    test_ini_parser_section_6();
    test_ini_parser_case_senstive_7();
    test_ini_parser_set_8();
    test_ini_parser_output_9();
    test_ini_parser_chinese_10();
    test_ini_parser_stop_parse_11();
    test_ini_parser_serialize_13();
    test_ini_parser_no_trim_15();
}

TEST_UNIT(test_ini_parser_wrong_format_1) {
    test_ini_parser_wrong_format_1();
    test_ini_parser_wrong_format_2();
    test_ini_parser_wrong_format_3();
    test_ini_parser_compatible_wrong_format_1();
    test_ini_parser_compatible_wrong_format_2();
    test_ini_parser_compatible_wrong_format_3();
    test_ini_parser_compatible_wrong_format_4();
}



TEST_UNIT(test_ini_parser_bug_find_1) {
    std::string path = "../test/test_data/ini/ini_test_data_utf8.txt";

    wcpp::DataStream ds;
    if (!ds.ReadFile(path)) {
        return;
    }
    wcpp::INIParser parser;
    bool parse_ok = parser.Parse(ds.data(), ds.size(), "\r\n", "=");
    H_TEST_ASSERT(parse_ok);
}


TEST_UNIT(test_ini_parser_bug_find_2) {
    {
        std::string d;
        d = "dXJsPWh0dHA6Ly9sei5jbHViLmN1bC5zb2h1LmNvbS9sei9zZXJpYWxpc2VfZGV0YWlscy5waHA/c2VyaWFsaXNlPTYxMDUyJnRwPTIjMjANCnRpdGxlPeWLv+W/mOWbveiAu++8ge+8ge+8gemlseWPl+aXpeWGm+i5gui6j+eahOS4reWbveWmh+Wlsy3mkJzni5DnvZHnq5kNCnVybF9tZDU9ZjliYWE0ZjliYTQ1MGQ4ODdkMzE3YmI1ZGI4MjczYjYNCnJlZmZlcnJlcj1odHRwOi8vbHouY2x1Yi5jdWwuc29odS5jb20vbHovc2VyaWFsaXNlX2RldGFpbHMucGhwP3NlcmlhbGlzZT02MTA1MiZ0cD0yIzE5DQphbmNob3JfdGV4dD0NCnJldF9mb3JtPWpzb24NCnBhZ2VfaW5mbz0wLDAsNQoxLDAsMTAKMiwwLDUKOTksMCw1CjUsMCw1CjYsMCw1CjcsMCw1DQpsb2NhbD0wDQptaWQ9YWYzMTU2YTRjMzYzM2M1ZDUzMTQzMTNiYjEwN2U4MDMNCnByb2R1Y3Q9MzYwc2UNCmNvbWJvPXVsaWtlc2hvcHBpbmcNCnZlcnNpb249MS4wLjAuMTAwNg0KZW5jb2Rpbmc9MQ0KcmV0X3N0YXJ0PTANCnJldF9saW1pdD01DQqJ8PKYzMdCWg==";
        std::string data = wcpp::Base64::Decode(d.data(), d.size());
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\r\n", "=");
        H_TEST_ASSERT(!parse_ok);
    }
    {
        std::string d = "dXJsPWh0dHA6Ly93d3cueWlpa28uY29tL3JpbnNrby9lcnVfMTYyNTIuaHRtbA0KdGl0bGU96YeN5bqG5bu65p2Q5biC5Zy66Ziz5YWJ5p2/6ZSA5ZSuDQp1cmxfbWQ1PTIwNjNiMjQyMWU3ODE2Yzk0YmQxZTNlNTY0YmJmMGRlDQpyZWZmZXJyZXI9DQphbmNob3JfdGV4dD0NCnJldF9mb3JtPWpzb24NCnBhZ2VfaW5mbz0wLDAsNQoxLDAsMTAKMiwwLDUKOTksMCw1CjUsMCw1CjYsMCw1CjcsMCw1DQpsb2NhbD0wDQptaWQ9Y2IxZjAwOGY5YzczN2IxYmJmOGM2MTRjYTkzNjVmMGUNCnByb2R1Y3Q9MzYwc2UNCmNvbWJvPXVsaWtlc2hvcHBpbmcNCnZlcnNpb249MS4wLjAuMTAwNQ0KZW5jb2Rpbmc9MQ0KcmV0X3N0YXJ0PTANCnJldF9saW1pdD01DQq0s0/UqtSWDA==";
        std::string data = wcpp::Base64::Decode(d.data(), d.size());
        wcpp::INIParser parser(false, true);
        bool parse_ok = parser.Parse(data.data(), data.size(), "\r\n", "=");
        H_TEST_ASSERT(parse_ok);
    }
}


TEST_UNIT(testINIParserPerformance) {
    std::string d = "dXJsPWh0dHA6Ly93d3cueWlpa28uY29tL3JpbnNrby9lcnVfMTYyNTIuaHRtbA0KdGl0bGU96YeN5bqG5bu65p2Q5biC5Zy66Ziz5YWJ5p2/6ZSA5ZSuDQp1cmxfbWQ1PTIwNjNiMjQyMWU3ODE2Yzk0YmQxZTNlNTY0YmJmMGRlDQpyZWZmZXJyZXI9DQphbmNob3JfdGV4dD0NCnJldF9mb3JtPWpzb24NCnBhZ2VfaW5mbz0wLDAsNQoxLDAsMTAKMiwwLDUKOTksMCw1CjUsMCw1CjYsMCw1CjcsMCw1DQpsb2NhbD0wDQptaWQ9Y2IxZjAwOGY5YzczN2IxYmJmOGM2MTRjYTkzNjVmMGUNCnByb2R1Y3Q9MzYwc2UNCmNvbWJvPXVsaWtlc2hvcHBpbmcNCnZlcnNpb249MS4wLjAuMTAwNQ0KZW5jb2Rpbmc9MQ0KcmV0X3N0YXJ0PTANCnJldF9saW1pdD01DQq0s0/UqtSWDA==";
    std::string data = wcpp::Base64::Decode(d.data(), d.size());

    int loop = 100;
    wcpp::Timestamp t1 = wcpp::Timestamp::Now();
    for (int i = 0; i < loop; i++) {
        wcpp::INIParser parser(false, true, true);
        bool parse_ok = parser.Parse(data.data(), data.size(), "\r\n", "=");
        H_TEST_ASSERT(parse_ok);
    }
    wcpp::Timestamp t2 = wcpp::Timestamp::Now();
    for (int i = 0; i < loop; i++) {
        wcpp::INIParser parser(false, true, false);
        bool parse_ok = parser.Parse(data.data(), data.size(), "\r\n", "=");
        H_TEST_ASSERT(parse_ok);
    }
    wcpp::Timestamp t3 = wcpp::Timestamp::Now();
    wcpp::Duration keep_sequence_cost = t2 - t1;
    wcpp::Duration no_keep_sequence_cost = t3 - t2;
    std::cout << "keep sequence parsing slow down " << 1 - no_keep_sequence_cost.Seconds() / keep_sequence_cost.Seconds() << "\n";
    //H_TEST_ASSERT(no_keep_sequence_cost.Seconds() < keep_sequence_cost.Seconds());
}

TEST_UNIT(test_ini_parser_more_compatible_test_1) {
    {
        std::string data =
            "k1=a\n"
            "k2=b\n"
            "xxx";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(!parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }

    {
        std::string data =
            "k1=a\n"
            "k2=b\n"
            "xxx\n";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(!parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }

    {
        std::string data =
            "k1=a\n"
            "xxx\n"
            "k2=b\n";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 1);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "");
        H_TEST_ASSERT(!parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }

    {
        std::string data =
            "k1=a\n"
            "xxx\n"
            "k2=b\n"
            "\n";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 1);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "");
        H_TEST_ASSERT(!parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }
    {
        std::string data =
            "k1=a\n"
            "xxx\n"
            "k2=b\n"
            "\n"
            "\n"
            "\n";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 1);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "");
        H_TEST_ASSERT(!parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }

    {
        std::string data =
            "k1=a\n"
            "k2=b\n"
            "xxx\n"
            "\n";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(!parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }

    {
        std::string basedata =
            "k1=a\n"
            "k2=b\n";
        std::string data = basedata +
                           "xxx\n"
                           "\n";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), basedata.size() + 2, "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(!parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), basedata.size() + 2, "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }


    {
        std::string basedata =
            "k1=a\n"
            "k2=b\n";
        std::string data = basedata +
                           "xxx\n"
                           "k3=c\n";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), basedata.size() + 2, "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(!parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), basedata.size() + 2, "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }

    {
        std::string data =
            "k1=a\n"
            "k2=b\n"
            "\n"
            "\n";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }


    {
        std::string data =
            "k1=a\n"
            "k2=b\n";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }

    {
        std::string data =
            "k1=a\n"
            "k2=b";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }

    {
        std::string data =
            "\n\n"
            "k1=a\n\n\n"
            "\n\n"
            "k2=b\n"
            "\n\n";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }

    {
        std::string data =
            "\n\n"
            "k1=a\n"
            "\n\n"
            "k2=b";
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }
}

TEST_UNIT(test_ini_parser_more_compatible_test_2) {
    {
        std::string data =
            "k1=a\n"
            "k2=b\n"
            "xxx\0\0\0";
        data.append(5, '\0');
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(!parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }

    {
        std::string data =
            "k1=a\n"
            "k2=b\n"
            "\0\0\0";
        data.append(5, '\0');
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);

        parser.Reset();
        parser.set_compatible(true);
        parse_ok = parser.Parse(data.data(), data.size(), "\n", "=");
        H_TEST_ASSERT(parser.GetDefaultKeyValueMap().size() == 2);
        H_TEST_ASSERT(parser.Get("k1") == "a");
        H_TEST_ASSERT(parser.Get("k2") == "b");
        H_TEST_ASSERT(parse_ok);
    }
}


TEST_UNIT(test_ini_parser_get_section_map) {
    std::string data =
        "[section]\n"
        "k1=a\n"
        "k2=b\n";
    data.append(5, '\0');
    wcpp::INIParser parser;
    bool parse_ok = parser.Parse(data.data(), data.size());
    H_TEST_ASSERT(parse_ok);
    const wcpp::INIParser::ssmap& s1 = parser.GetKeyValueMap("section");
    H_TEST_ASSERT(s1.size() > 0);
    const wcpp::INIParser::ssmap& s3 = parser.GetKeyValueMap("unknown");
    H_TEST_ASSERT(s3.empty());
}

TEST_UNIT(test_ini_parse_repeat_key) {
    std::string d = "a:1||c:3||c:5||e:6";
    wcpp::INIParser ini(true, false, true);
    bool r = ini.Parse(d.data(), d.size(), "||", ":");
    H_TEST_ASSERT(r);
    std::string output;
    ini.Serialize(output, true);
    H_TEST_ASSERT("a:1||c:5||c:5||e:6||" == output);
    ini.Serialize(output);
    H_TEST_ASSERT("e:6||a:1||c:5||" == output || "a:1||c:5||e:6||" == output);
}


TEST_UNIT(test_ini_parser_get_integer) {
    {
        std::string data =
            "k1=1\n"
            "b2=true\n"
            "[section]\n"
            "k2=2\n"
            "k3=3\n"
            "[section2]\n"
            "f1=1.1\n"
            "b1=false\n"
            "b2=true\n"
            ;
        wcpp::INIParser parser;
        bool parse_ok = parser.Parse(data.data(), data.size());
        bool found = false;
        H_TEST_ASSERT(parse_ok);
        H_TEST_ASSERT(parser.GetInteger("section", "k2") == 2);
        H_TEST_ASSERT(parser.GetInteger("section", "k3") == 3);
        H_TEST_ASSERT(parser.GetInteger("k1") == 1);

        H_TEST_ASSERT(parser.GetDouble("section2", "f1") == 1.1);

        H_TEST_ASSERT(parser.Get("section2", "b1", &found) == "false");

        H_TEST_ASSERT(parser.GetBool("b2", false) == true);
        H_TEST_ASSERT(parser.GetBool("section2", "b2", false));
        H_TEST_ASSERT(!parser.GetBool("section2", "b1", false));
    }
}

TEST_UNIT(test_ini_parse_error_dlog) {
    std::string d = "a:1||||c:3";
    wcpp::INIParser ini;
    bool r = ini.Parse(d.data(), d.size(), "||", ":");
    H_TEST_ASSERT(r);
    bool found;
    std::string v = ini.Get("a", &found);
    H_TEST_ASSERT(found && v == "1");
    v = ini.Get("c", &found);
    H_TEST_ASSERT(found && v == "3");
    v = ini.Get("b", &found);
    H_TEST_ASSERT(!found && v == "");
}



TEST_UNIT(ini_parser_linesep_test) {
    const char* rawdata =
        "mid=ac9219aa5232c4e519ae5fcb4d77ae5b\n"
        "version=4.4\r\n"
        "appext=";
    size_t rawdatalen = strlen(rawdata);
    wcpp::INIParser parser;
    parser.Parse(rawdata, rawdatalen, "\r\n", "=");
    bool found = false;

    {
        std::string key = "mid";
        std::string value = "ac9219aa5232c4e519ae5fcb4d77ae5b";
        H_TEST_ASSERT(value != parser.Get(key));
        H_TEST_ASSERT(value != parser.Get(key, &found));
        H_TEST_ASSERT(found);
    }


    {
        std::string key = "version";
        H_TEST_ASSERT(parser.Get(key) == "");
    }

    {
        std::string key = "appext";
        std::string value = "";
        H_TEST_ASSERT(value == parser.Get(key));
    }

}




TEST_UNIT(INIParser_set_test) {
    wcpp::INIParser ini;
    ini.Set("a", "a");
    ini.Set("", "b", "b");

    bool found = false;
    H_TEST_ASSERT(ini.Get("a", &found) == "a");
    H_TEST_ASSERT(ini.Get("b", &found) == "b");
    ini.SetKVSeparator("=");
    ini.SetLineSeparator("\n");
    std::string s = ini.Serialize();
    H_TEST_ASSERT(s == "a=a\nb=b\n" || s == "b=b\na=a\n");
}




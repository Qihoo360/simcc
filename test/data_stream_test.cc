
#include "test_common.h"
#include "wcpp/data_stream.h"
#include "wcpp/file_util.h"

#include <iostream>

using namespace wcpp;

namespace {

//typedef wcpp::Map<int, double> intdoublemap;
typedef std::map<int, double> intdoublemap;
typedef std::set<int> intset;
typedef std::list<int> intlist;
typedef std::vector<int> intvector;

void serialize_1() {
    char c = 'a';
    short t = 111;
    int   d = 221;
    long  l = 333;
    float f = float(666.66);
    double ff = 7777.777;

    DataStream ds;
    ds << c << t << d << l << f << ff;
    H_TEST_ASSERT(!ds.IsWriteBad());
    H_TEST_ASSERT(ds.size() == sizeof(c) + sizeof(t) + sizeof(d) + sizeof(l) + sizeof(f) + sizeof(ff));

    char c1 = 0;
    short t1 = 0;
    int   d1 = 0;
    long  l1 = 0;
    float f1 = 0.0;
    double ff1 = 0.0;
    ds >> c1 >> t1 >> d1 >> l1 >> f1 >> ff1;
    H_TEST_ASSERT(c == c1);
    H_TEST_ASSERT(t == t1);
    H_TEST_ASSERT(d == d1);
    H_TEST_ASSERT(l == l1);
    H_TEST_ASSERT(f == f1);
    H_TEST_ASSERT(ff == ff1);
    H_TEST_ASSERT(!ds.IsReadBad());
    H_TEST_ASSERT(ds.GetReadableSize() == 0);
}


void serialize_2() {
    std::string a = "aaaaaaaaaaaaaaaaaaaaaaa";
    std::string b = "bbbbbbbbbbbbbbbbbbbb";
    int   d = 221;
    long  l = 333;

    DataStream ds;
    ds << d << a << l << b;
    H_TEST_ASSERT(!ds.IsWriteBad());

    int   d1 = 0;
    long  l1 = 0;
    std::string a1;
    std::string b1;
    ds >> d1 >> a1 >> l1 >> b1;
    H_TEST_ASSERT(d == d1);
    H_TEST_ASSERT(l == l1);
    H_TEST_ASSERT(a == a1);
    H_TEST_ASSERT(b == b1);
    H_TEST_ASSERT(!ds.IsReadBad());
    H_TEST_ASSERT(ds.GetReadableSize() == 0);
}

void serialize_vector_3() {
    intvector iv;
    for (int i = 0; i < 100; ++i) {
        iv.push_back(i);
    }

    std::string a = "aaaaaaaaaaaaaaaaaaaaaaa";
    std::string b = "bbbbbbbbbbbbbbbbbbbb";
    int   d = 221;
    long  l = 333;

    DataStream ds;
    ds << d << a << l << b << iv;
    H_TEST_ASSERT(!ds.IsWriteBad());

    int   d1 = 0;
    long  l1 = 0;
    std::string a1;
    std::string b1;
    intvector iv1;
    ds >> d1 >> a1 >> l1 >> b1 >> iv1;
    H_TEST_ASSERT(d == d1);
    H_TEST_ASSERT(l == l1);
    H_TEST_ASSERT(a == a1);
    H_TEST_ASSERT(b == b1);
    H_TEST_ASSERT(!ds.IsReadBad());
    H_TEST_ASSERT(ds.GetReadableSize() == 0);

    H_TEST_ASSERT(iv.size() == iv1.size());
    intvector::iterator it(iv.begin());
    intvector::iterator ite(iv.end());
    intvector::iterator it1(iv1.begin());
    intvector::iterator ite1(iv1.end());
    for (; it != ite; ++it, ++it1) {
        H_TEST_ASSERT((*it) == (*it1));
    }
}

void serialize_list_4() {
    intlist iv;
    for (int i = 0; i < 100; ++i) {
        iv.push_back(i);
    }

    std::string a = "aaaaaaaaaaaaaaaaaaaaaaa";
    std::string b = "bbbbbbbbbbbbbbbbbbbb";
    int   d = 221;
    long  l = 333;

    DataStream ds;
    ds << d << a << l << b << iv;
    H_TEST_ASSERT(!ds.IsWriteBad());

    int   d1 = 0;
    long  l1 = 0;
    std::string a1;
    std::string b1;
    intlist iv1;
    ds >> d1 >> a1 >> l1 >> b1 >> iv1;
    H_TEST_ASSERT(d == d1);
    H_TEST_ASSERT(l == l1);
    H_TEST_ASSERT(a == a1);
    H_TEST_ASSERT(b == b1);
    H_TEST_ASSERT(!ds.IsReadBad());
    H_TEST_ASSERT(ds.GetReadableSize() == 0);

    H_TEST_ASSERT(iv.size() == iv1.size());
    intlist::iterator it(iv.begin());
    intlist::iterator ite(iv.end());
    intlist::iterator it1(iv1.begin());
    intlist::iterator ite1(iv1.end());
    for (; it != ite; ++it, ++it1) {
        H_TEST_ASSERT((*it) == (*it1));
    }
}

void serialize_set_5() {
    intset iv;
    for (int i = 0; i < 100; ++i) {
        iv.insert(i);
    }

    std::string a = "aaaaaaaaaaaaaaaaaaaaaaa";
    std::string b = "bbbbbbbbbbbbbbbbbbbb";
    int   d = 221;
    long  l = 333;

    DataStream ds;
    ds << d << a << l << b << iv;
    H_TEST_ASSERT(!ds.IsWriteBad());

    int   d1 = 0;
    long  l1 = 0;
    std::string a1;
    std::string b1;
    intset iv1;
    ds >> d1 >> a1 >> l1 >> b1 >> iv1;
    H_TEST_ASSERT(d == d1);
    H_TEST_ASSERT(l == l1);
    H_TEST_ASSERT(a == a1);
    H_TEST_ASSERT(b == b1);
    H_TEST_ASSERT(!ds.IsReadBad());
    H_TEST_ASSERT(ds.GetReadableSize() == 0);

    H_TEST_ASSERT(iv.size() == iv1.size());
    intset::iterator it(iv.begin());
    intset::iterator ite(iv.end());
    intset::iterator it1(iv1.begin());
    intset::iterator ite1(iv1.end());
    for (; it != ite; ++it, ++it1) {
        H_TEST_ASSERT((*it) == (*it1));
    }
}


void serialize_map_6() {
    intset iv;
    intdoublemap idm;
    for (int i = 0; i < 100; ++i) {
        iv.insert(i);
        idm[i] = i;
    }

    std::string a = "aaaaaaaaaaaaaaaaaaaaaaa";
    std::string b = "bbbbbbbbbbbbbbbbbbbb";
    int   d = 221;
    long  l = 333;

    DataStream ds;
    ds << d << a << l << b << iv << idm;
    H_TEST_ASSERT(!ds.IsWriteBad());

    int   d1 = 0;
    long  l1 = 0;
    std::string a1;
    std::string b1;
    intset iv1;
    intdoublemap idm1;
    ds >> d1 >> a1 >> l1 >> b1 >> iv1 >> idm1;
    H_TEST_ASSERT(d == d1);
    H_TEST_ASSERT(l == l1);
    H_TEST_ASSERT(a == a1);
    H_TEST_ASSERT(b == b1);
    H_TEST_ASSERT(!ds.IsReadBad());
    H_TEST_ASSERT(ds.GetReadableSize() == 0);

    H_TEST_ASSERT(iv.size() == iv1.size());
    H_TEST_ASSERT(idm.size() == idm1.size());
    {
        intset::iterator it(iv.begin());
        intset::iterator ite(iv.end());
        intset::iterator it1(iv1.begin());
        intset::iterator ite1(iv1.end());
        for (; it != ite; ++it, ++it1) {
            H_TEST_ASSERT((*it) == (*it1));
        }
    }

    {
        intdoublemap::iterator it(idm.begin());
        intdoublemap::iterator ite(idm.end());
        intdoublemap::iterator it1(idm1.begin());
        intdoublemap::iterator ite1(idm1.end());
        for (; it != ite; ++it, ++it1) {
            H_TEST_ASSERT((it->first) == (it1->first));
            H_TEST_ASSERT((it->second) == (it1->second));
        }
    }
}

void test_append() {
    DataStream ds;
    ds.Write((char)1);
    ds.Write((char)2);
    H_TEST_ASSERT(ds.size() == 2);
    H_TEST_ASSERT(ds.CharAt(0) == 1);
    H_TEST_ASSERT(ds.CharAt(1) == 2);
}

void test_not_selfcreate() {
    char buf[1024];
    char tmp[1024];
    DataStream ds(buf, sizeof(buf), false);
    ds.Write((char)1);
    ds.Write((char)2);
    H_TEST_ASSERT(ds.GetReadableSize() == 2);
    ds.Read(tmp, ds.size());
    H_TEST_ASSERT(ds.size() == 2);
    H_TEST_ASSERT(ds.CharAt(0) == 1);
    H_TEST_ASSERT(ds.CharAt(1) == 2);
    H_TEST_ASSERT(ds.tellp() == 2);
    H_TEST_ASSERT(ds.tellg() == 2);
    H_TEST_ASSERT(ds.GetReadableSize() == 0);
    H_TEST_ASSERT(ds.GetCurrentWriteBuffer() == ds.data() + 2);
    H_TEST_ASSERT(buf[0] == 1);
    H_TEST_ASSERT(buf[1] == 2);

    ds.Reset();
    H_TEST_ASSERT(buf[0] == 1);
    H_TEST_ASSERT(buf[1] == 2);
    ds.Write((char)1);
    ds.Write((char)2);
    H_TEST_ASSERT(ds.GetReadableSize() == 2);
    ds.Read(tmp, ds.size());
    H_TEST_ASSERT(ds.size() == 2);
    H_TEST_ASSERT(ds.CharAt(0) == 1);
    H_TEST_ASSERT(ds.CharAt(1) == 2);
    H_TEST_ASSERT(ds.tellp() == 2);
    H_TEST_ASSERT(ds.tellg() == 2);
    H_TEST_ASSERT(ds.GetReadableSize() == 0);
    H_TEST_ASSERT(ds.GetCurrentWriteBuffer() == ds.data() + 2);
    H_TEST_ASSERT(buf[0] == 1);
    H_TEST_ASSERT(buf[1] == 2);
}

void test_read_write_file() {
    std::string path = "temp_data_stream_write.dat";
    DataStream ds;
    for (int i = 0; i < 30; ++i) {
        ds.Write(path.data(), path.size());
        ds.Write('\n');
    }
    H_TEST_ASSERT(ds.WriteFile(path));
    DataStream file;
    H_TEST_ASSERT(file.ReadFile(path));
    wcpp::FileUtil::Unlink(path);
    H_TEST_ASSERT(wcpp::DataStream::IsContentEquals(file, ds));
}

void test_toText() {
    wcpp::DataStream ds;
    ds.Reserve(512);
    ds.ToText();
    H_TEST_ASSERT(ds.size() == 0);
}

void test_seekg_seekp_resize() {
    DataStream ds(8);
    ds.Write((char)1);
    ds.Write((char)2);
    H_TEST_ASSERT(ds.size() == 2);
    H_TEST_ASSERT(ds.CharAt(0) == 1);
    H_TEST_ASSERT(ds.CharAt(1) == 2);
    H_TEST_ASSERT((const char*)ds.GetCurrentWriteBuffer() - ds.data() == 2);

    ds.seekp(2);
    H_TEST_ASSERT(ds.size() == 4);
    ds.seekp(-2);
    H_TEST_ASSERT(ds.size() == 2);
    ds.seekp(2);
    H_TEST_ASSERT(ds.size() == 4);
    ds.seekp(2);
    H_TEST_ASSERT(ds.size() == 6);
    ds.Write((char)1);
    ds.Write((char)2);
    H_TEST_ASSERT(ds.CharAt(6) == 1);
    H_TEST_ASSERT(ds.CharAt(7) == 2);
    H_TEST_ASSERT((const char*)ds.GetCurrentWriteBuffer() - ds.data() == 8);

    char buf1[2] = {};
    char buf2[2] = {};
    ds.Read(buf1, 2);
    H_TEST_ASSERT(buf1[0] == 1);
    H_TEST_ASSERT(buf1[1] == 2);
    H_TEST_ASSERT((const char*)ds.GetCurrentReadBuffer() - ds.data() == 2);
    ds.seekg(2);
    H_TEST_ASSERT((const char*)ds.GetCurrentReadBuffer() - ds.data() == 4);
    ds.seekg(2);
    H_TEST_ASSERT((const char*)ds.GetCurrentReadBuffer() - ds.data() == 6);
    ds.seekg(-2);
    H_TEST_ASSERT((const char*)ds.GetCurrentReadBuffer() - ds.data() == 4);
    ds.seekg(2);
    H_TEST_ASSERT((const char*)ds.GetCurrentReadBuffer() - ds.data() == 6);
    ds.Read(buf2, 2);
    H_TEST_ASSERT(buf2[0] == 1);
    H_TEST_ASSERT(buf2[1] == 2);
    H_TEST_ASSERT((const char*)ds.GetCurrentReadBuffer() - ds.data() == 8);
    ds.seekg(-10);
    H_TEST_ASSERT(ds.IsReadBad());

    ds.Reset();
    ds.seekp(10);
    H_TEST_ASSERT(!ds.IsWriteBad());
    H_TEST_ASSERT((const char*)ds.GetCurrentWriteBuffer() - ds.data() == 10);
    ds.seekg(10);
    H_TEST_ASSERT((const char*)ds.GetCurrentReadBuffer() - ds.data() == 10);
    ds.seekg(10);
    H_TEST_ASSERT((const char*)ds.GetCurrentReadBuffer() - ds.data() == 10);
    H_TEST_ASSERT(ds.IsReadBad());
    ds.Resize(5);
    H_TEST_ASSERT(ds.size() == 5);
    H_TEST_ASSERT((const char*)ds.GetCurrentWriteBuffer() - ds.data() == 5);
    ds.Write((char)3);
    H_TEST_ASSERT(ds.tellp() == 6);
    H_TEST_ASSERT(ds.size() == 6);

    ds.Reset();
    ds.seekp(10);
    H_TEST_ASSERT(!ds.IsWriteBad());
    H_TEST_ASSERT((const char*)ds.GetCurrentWriteBuffer() - ds.data() == 10);
    ds.seekp(10);
    H_TEST_ASSERT(!ds.IsWriteBad());
    H_TEST_ASSERT((const char*)ds.GetCurrentWriteBuffer() - ds.data() == 20);
    ds.seekg(10);
    H_TEST_ASSERT((const char*)ds.GetCurrentReadBuffer() - ds.data() == 10);
    ds.seekg(10);
    H_TEST_ASSERT((const char*)ds.GetCurrentReadBuffer() - ds.data() == 20);
    H_TEST_ASSERT(!ds.IsReadBad());
    ds.seekg(10);
    H_TEST_ASSERT((const char*)ds.GetCurrentReadBuffer() - ds.data() == 20);
    H_TEST_ASSERT(ds.IsReadBad());
    ds.seekp(-40);
    H_TEST_ASSERT((const char*)ds.GetCurrentWriteBuffer() - ds.data() == 0);
    H_TEST_ASSERT((const char*)ds.GetCurrentWriteBuffer() == ds.data());
    H_TEST_ASSERT(!ds.IsWriteBad());
}

}

TEST_UNIT(memory_data_stream_test_1) {
    serialize_1();
    serialize_2();
    serialize_vector_3();
    serialize_list_4();
    serialize_set_5();
    serialize_map_6();
    test_append();
    test_read_write_file();
    test_toText();
    test_not_selfcreate();
    test_seekg_seekp_resize();
}




namespace {

class TestStruct1 {
public:
    TestStruct1() {}

    int a;
    int b;
    std::string c;
public:
    friend wcpp::DataStream& operator <<(wcpp::DataStream& out, const TestStruct1& val) {
        out << val.a << val.b << val.c;
        return out;
    }

    friend wcpp::DataStream& operator >> (wcpp::DataStream& out, TestStruct1& val) {
        out >> val.a >> val.b >> val.c;
        return out;
    }
};

struct TestStruct2 {
    int a;
    char b[32];
};


void TestStructSerialize() {
    wcpp::DataStream* os = new wcpp::DataStream();
    std::unique_ptr<wcpp::DataStream> p(os);

    std::string str = "hello world";
    wcpp::uint32 i1 = 1;
    wcpp::float32 f1 = 0.5f;
    TestStruct1 stru1a;

    TestStruct2 stru2a = { 111, "test_stru" };
    stru2a.a += 1;

    *os << str
        << i1
        << f1
        << stru1a
        << stru2a;

    wcpp::DataStream out;
    out.Write(os->GetCurrentReadBuffer(), os->GetReadableSize());

    std::string strO;
    wcpp::uint32 u321O = 2;
    wcpp::float32 f321O = 5.0f;
    TestStruct1 stru1b;

    TestStruct2 stru2b;
    out >> strO
        >> u321O
        >> f321O
        >> stru1b
        >> stru2b;

    H_TEST_ASSERT(str == strO);
    H_TEST_ASSERT(i1 == u321O);
    H_TEST_ASSERT(f1 == f321O);
    H_TEST_ASSERT(stru1a.a == stru1b.a);
    H_TEST_ASSERT(stru1a.b == stru1b.b);
    H_TEST_ASSERT(stru1a.c == stru1b.c);
    H_TEST_ASSERT(stru2a.a == stru2b.a);
    H_TEST_ASSERT(strcmp(stru2a.b, stru2b.b) == 0);
}


}


TEST_UNIT(test_memory_data_stream_2) {
    TestStructSerialize();
}


H_IS_POD_TYPE(TestStruct2, true);



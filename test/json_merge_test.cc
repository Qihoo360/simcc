
#include "test_common.h"
#include "simcc/json/json.h"
#include <iostream>

TEST_UNIT(json_merge_test1) {
    simcc::json::JSONObject ja;
    ja.Put("a", "a1-a");
    ja.Put("b", "a2-a");

    simcc::json::JSONObject jb;
    jb.Put("a", "a1-b");
    jb.Put("c", "c1-b");

    ja.Merge(&jb, false);
    H_TEST_ASSERT(ja.size() == 3);
    H_TEST_ASSERT(ja.GetString("a") == "a1-a");
    H_TEST_ASSERT(ja.GetString("b") == "a2-a");
    H_TEST_ASSERT(ja.GetString("c") == "c1-b");

    H_TEST_ASSERT(jb.size() == 2);
    H_TEST_ASSERT(jb.GetString("a") == "a1-b");
    H_TEST_ASSERT(jb.GetString("c") == "c1-b");


    ja.Merge(&jb, true);
    H_TEST_ASSERT(ja.size() == 3);
    H_TEST_ASSERT(ja.GetString("a") == "a1-b");
    H_TEST_ASSERT(ja.GetString("b") == "a2-a");
    H_TEST_ASSERT(ja.GetString("c") == "c1-b");

    H_TEST_ASSERT(jb.size() == 2);
    H_TEST_ASSERT(jb.GetString("a") == "a1-b");
    H_TEST_ASSERT(jb.GetString("c") == "c1-b");
}


TEST_UNIT(json_merge_test2) {
    simcc::json::JSONObject j1;
    simcc::json::JSONObject j2;
    const char* key1 = "apple";
    const char* key2 = "orange";
    const char* value1 = "value1";
    const char* value1_1 = "value1_1";
    const char* value2 = "value2";
    j1.Put(key1, value1);


    j2.Put(key1, value1_1);
    j2.Put(key2, value2);


    j1.Merge(&j2, false);
    H_TEST_ASSERT(j1.GetString(key1) == value1);
    H_TEST_ASSERT(j1.GetString(key2) == value2);

    j1.Merge(&j2, true);
    H_TEST_ASSERT(j1.GetString(key1) == value1_1);
    H_TEST_ASSERT(j1.GetString(key2) == value2);
}





TEST_UNIT(json_merge_test3) {

    simcc::json::JSONObject j1;
    simcc::json::JSONObject j2;
    const char* key1 = "apple";
    const char* key2 = "orange";
    const char* key_obj = "key_obj";
    const char* jkkey3 = "host";
    const char* jkvalue3 = "www.360.com";
    const char* value1 = "value1";
    const char* value2 = "value2";
    const char* value1_1 = "value1_1";
    bool value2_1 = true;
    j1.Put(key1, value2);

    j2.Put(key1, value1);
    j2.Put(key2, value2);

    simcc::json::JSONObject* subj1 = new simcc::json::JSONObject;
    simcc::json::JSONObject* subj2 = new simcc::json::JSONObject;
    subj1->Put(key1, value1_1);
    subj2->Put(key1, value2_1);
    subj2->Put(jkkey3, jkvalue3);
    j1.Put(key_obj, subj1);
    j2.Put(key_obj, subj2);
    j2.Put(key_obj, subj2);

    j1.Merge(&j2, false);
    H_TEST_ASSERT(j1.GetString(key1) == value2);
    H_TEST_ASSERT(j1.GetString(key2) == value2);
    H_TEST_ASSERT(j1.GetJSONObject(key_obj)->GetString(key1) == value1_1);
    H_TEST_ASSERT(j1.GetJSONObject(key_obj)->GetString(jkkey3) == jkvalue3);

    j1.Merge(&j2, true);
    H_TEST_ASSERT(j1.GetString(key1) == value1);
    H_TEST_ASSERT(j1.GetString(key2) == value2);
    H_TEST_ASSERT(j1.GetJSONObject(key_obj)->Equals(*subj2));
}

TEST_UNIT(json_merge_test4) {

    simcc::json::JSONObject j1;
    simcc::json::JSONObject j2;
    const char* key1 = "apple";
    const char* jkkey3 = "host";
    const char* jkvalue3 = "www.360.com";
    const char* value2 = "value2";
    bool value2_1 = true;
    j1.Put(key1, value2);

    simcc::json::JSONObject* subj2 = new simcc::json::JSONObject;

    subj2->Put(key1, value2_1);
    subj2->Put(jkkey3, jkvalue3);
    j2.Put(key1, subj2);

    j1.Merge(&j2, false);
    H_TEST_ASSERT(j1.GetString(key1) == value2);

    j1.Merge(&j2, true);
    H_TEST_ASSERT(j1.GetJSONObject(key1)->Equals(*subj2));
}





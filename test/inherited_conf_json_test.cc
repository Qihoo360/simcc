
#include "test_common.h"
#include "simcc/json/json.h"
#include "simcc/json/inherited_conf_json.h"
#include <iostream>

TEST_UNIT(inherited_conf_json_test1) {

    std::string concrete_conf = "../test/test_data/json/inherited_conf.json";
    simcc::json::JSONObjectPtr jconf = simcc::json::InheritedConfJSONObject::Parse(concrete_conf);
    H_TEST_ASSERT(jconf.get());
    //std::cout << jconf->ToString(true,true);
    H_TEST_ASSERT(jconf->GetString("memcached_hostname") == "hpre3.ccc.xxx.com");
    H_TEST_ASSERT(jconf->GetString("log4cxx_category") == "Cloud.BrowserRelative.Newvalue");
    H_TEST_ASSERT(jconf->GetBool("verbose_log") == true);
    simcc::json::JSONObject* subj1 = new simcc::json::JSONObject;
    subj1->Put("0" , "0");
    subj1->Put("1" , "1");
    subj1->Put("2" , "2");
    subj1->Put("3" , "3");
    subj1->Put("4" , "4");
    subj1->Put("5" , "5");
    subj1->Put("99", "99_newvalue");
    subj1->Put("100", "100_new_key_value");
    subj1->Put("bool", false);
    //std::cout << jconf->getJSONObject("type")->ToString(true,true);
    //std::cout << subj1->ToString(true,true);
    H_TEST_ASSERT(jconf->GetJSONObject("type")->Equals(*subj1));
}

TEST_UNIT(inherited_conf_json_test_failed) {
    std::string concrete_conf = "../test/test_data/json/inherited_conf.json.mockfailed";
    simcc::json::JSONObjectPtr jconf = simcc::json::InheritedConfJSONObject::Parse(concrete_conf);
    H_TEST_ASSERT(!jconf.get());
}



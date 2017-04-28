#include "test_common.h"

#include <simcc/http_data_parser.h>
#include <simcc/string_util.h>
#include <simcc/base64.h>

#include <iostream>

namespace  {
#ifndef CRLF
#define CRLF   "\x0d\x0a"
#endif
    typedef std::map<std::string, std::string> ssmap;
    template<class MAP >
    inline bool ParseHttpMultiPartData(const void* d, size_t dlen, MAP& kv) {
        const char* end = (const char*)d + dlen;
        const char* p = (const char*)d;
        static std::string name_prefix = "Content-Disposition: form-data; name=\"";
        static std::string kvsep       = CRLF CRLF;
        static std::string value_end   = CRLF "-----------------------------";
        while (p < end) {
            const char* found = static_cast<const char*>(memmem(p, end - p, name_prefix.data(), name_prefix.size()));
            if (!found || found >= end) {
                break;
            }

            found += name_prefix.size();
            if (found >= end) {
                break;
            }

            p = found;
            while (p < end && *p != '"') ++p;
            if (p >= end)
            {
                break;
            }

            if (p <= found) continue;

            std::string key(found, p - found);
            if (memcmp(p+1, kvsep.data(), kvsep.size())  != 0) {
                continue;
            }

            const char* value = p + 1 + kvsep.size();
            if (value >= end)
            {
                break;
            }

            p = value;
            found = static_cast<const char*>(memmem(p, end - p, value_end.data(), value_end.size()));

            if (!found || found >= end)
            {
                break;
            }

            kv[key] = std::string(value, found - value);
            p = found + value_end.size();
        }

        return true;
    }
}

TEST_UNIT(http_multi_part_parse) 
{
    std::string b64 = "LS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS03ZDgzZTJkN2ExNDFlDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9InVybHMiDQoNCjVjYzkwOWU1MzRkMGRhMmY0YTg3N2U0OWIxYjBhMjJlCW5VRTBwUWJpWTNxM3FsNWtuSnVpb2w1d28yMGlaRjVicVQxZgoNCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tN2Q4M2UyZDdhMTQxZQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJrd3MiDQoNCgk1cldMNksrVk1RPT0KDQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLTdkODNlMmQ3YTE0MWUNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0icHJvZHVjdCINCg0KaW50ZXJuYWwNCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tN2Q4M2UyZDdhMTQxZQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJjb21ibyINCg0KY2xvdWRxdWVyeV90b29sDQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLTdkODNlMmQ3YTE0MWUNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0idiINCg0KMw0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS03ZDgzZTJkN2ExNDFlDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9Im1pZCINCg0KYTE2Y2YzNjUxNDlhOGFlZDIxZmRkMDRhZTI1NDU4MjQNCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tN2Q4M2UyZDdhMTQxZQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJ2ayINCg0KYmNhYTQzN2YNCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tN2Q4M2UyZDdhMTQxZS0t";
    std::string post = simcc::Base64::Decode(b64.data(), b64.size());
    ssmap kv;
    H_TEST_ASSERT(ParseHttpMultiPartData(post.data(), post.size(), kv));
    H_TEST_ASSERT(kv.size() > 0);
    H_TEST_ASSERT(kv["product"] == "internal");
    H_TEST_ASSERT(kv["combo"] == "cloudquery_tool");
    H_TEST_ASSERT(kv["v"] == "3");
    H_TEST_ASSERT(kv["mid"] == "a16cf365149a8aed21fdd04ae2545824");
    H_TEST_ASSERT(kv["vk"] == "bcaa437f");
}

TEST_UNIT(http_multi_part_parse1) 
{
    std::string b64 = "LS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS03ZDgzZTJkN2ExNDFlDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9InVybHMiDQoNCjVjYzkwOWU1MzRkMGRhMmY0YTg3N2U0OWIxYjBhMjJlCW5VRTBwUWJpWTNxM3FsNWtuSnVpb2w1d28yMGlaRjVicVQxZgoNCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tN2Q4M2UyZDdhMTQxZQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJrd3MiDQoNCgk1cldMNksrVk1RPT0KDQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLTdkODNlMmQ3YTE0MWUNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0icHJvZHVjdCINCg0KaW50ZXJuYWwNCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tN2Q4M2UyZDdhMTQxZQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJjb21ibyINCg0KY2xvdWRxdWVyeV90b29sDQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLTdkODNlMmQ3YTE0MWUNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0idiINCg0KMw0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS03ZDgzZTJkN2ExNDFlDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9Im1pZCINCg0KYTE2Y2YzNjUxNDlhOGFlZDIxZmRkMDRhZTI1NDU4MjQNCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tN2Q4M2UyZDdhMTQxZQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJ2ayINCg0KYmNhYTQzN2YNCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tN2Q4M2UyZDdhMTQxZS0t";
    std::string post = simcc::Base64::Decode(b64.data(), b64.size());
    ssmap kv;
    H_TEST_ASSERT(simcc::HttpMultiDataParser::Parse<std::string>(post.data(), post.size(), kv));
    H_TEST_ASSERT(kv.size() > 0);
    H_TEST_ASSERT(kv["product"] == "internal");
    H_TEST_ASSERT(kv["combo"] == "cloudquery_tool");
    H_TEST_ASSERT(kv["v"] == "3");
    H_TEST_ASSERT(kv["mid"] == "a16cf365149a8aed21fdd04ae2545824");
    H_TEST_ASSERT(kv["vk"] == "bcaa437f");
}

TEST_UNIT(http_multi_part_parse_error1) 
{
    //input data has an error : scombo's value format wrong
    std::string b64 = "LS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJjIg0KDQpnDQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLUF3VlBMcFo1NDhrMlZOUzNKVkdJDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9Im1kNXMiDQoNCp2X1Sc6542kGZNDV0khw2aUEmISgJCgwb6A8aBIqIZ8RvGnk+UBZzuwsMpPobUoF+1MsP2cT5045+pnIisRElHum6WoiJR6yTALdVIQEnlBWlKIaZJlcpzFJjemu7nHBPw7eWDHQXQcCVZhG9kn170VnGnJbKtCZC4N5WH6xT9/6ObOEU+pwE3I+XePrLQAxC5H6u8VCcnrV7YaiSNp+f8w8UPXgGF9cLOpT4w/GX09kX3GZdCMycMFbkoVX6V6HTPYbMZEYW1KcoX/XS4xptyJMtxKn9fa/U+fBKNcWLYBHgAx5EvDuQmND60sDuTdZsTZta+aZKvvBcVS9KQxjUahDfEuDvuH3Ush9BlYNGjm4vDzP6n9MuTbgDMoGe3O+TH6//rLmsZC0kUKKmyt8d5/j9fWdmdiKg2b71xuLDTRT9oUZ0eU7B5e2tPgajO05CmbvIdFolXXm8vkBNsl40sTxvEgQD4qrAvPBGtv8TvsrNpeUAdY7HgnJgPwNDZQRfak5BV4TekLCwLVrB4UkkwAAAAAAAAAAA0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJmb3JtYXQiDQoNClhNTA0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJwcm9kdWN0Ig0KDQpkZWVwc2Nhbg0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJjb21ibyINCg0KcHJpbWFyeQ0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJ2ZXJzaW9uIg0KDQozLjMuOC4yMDE2DQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLUF3VlBMcFo1NDhrMlZOUzNKVkdJDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9InYiDQoNCjINCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tQXdWUExwWjU0OGsyVk5TM0pWR0kNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0ib3N2ZXIiDQoNCjYuMS43NjAwLjI1Ni4xLjANCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tQXdWUExwWjU0OGsyVk5TM0pWR0kNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0ic3Byb2R1Y3QiDQoNCjM2MFNEDQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLUF3VlBMcFo1NDhrMlZOUzNKVkdJDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9InNjb21ibyIN1atmMzYwUlANCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tQXdWUExwWjU0OGsyVk5TM0pWR0kNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0idmsiDQoNCmI4NWQwOGJjDQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLUF3VlBMcFo1NDhrMlZOUzNKVkdJDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9Im1pZCINCg0KYzFlN2IwZDRiODI2MDU5MWQ2OTExYTFmNGRjNWQ2ZjINCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tQXdWUExwWjU0OGsyVk5TM0pWR0kNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0iZXQiDQoNCjENCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tQXdWUExwWjU0OGsyVk5TM0pWR0kNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0ibGFuZ2lkIg0KDQoyMDUyDQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLUF3VlBMcFo1NDhrMlZOUzNKVkdJDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9ImlldmVyIg0KDQo4LjAuNzYwMC4xNzExNQ0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJ3aW42NCINCg0KMA0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJ1aCINCg0KMg0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSS0tDQo=";
    std::string post = simcc::Base64::Decode(b64.data(), b64.size());
    ssmap kv;
    H_TEST_ASSERT(ParseHttpMultiPartData(post.data(), post.size(), kv));
    H_TEST_ASSERT(kv.size() > 0);
    H_TEST_ASSERT(kv["product"] == "deepscan");
    H_TEST_ASSERT(kv["combo"] == "primary");
    H_TEST_ASSERT(kv["sproduct"] == "360SD");
    H_TEST_ASSERT(kv["v"] == "2");
    H_TEST_ASSERT(kv["version"] == "3.3.8.2016");
    H_TEST_ASSERT(kv["osver"] == "6.1.7600.256.1.0");
    H_TEST_ASSERT(kv["mid"] == "c1e7b0d4b8260591d6911a1f4dc5d6f2");
    H_TEST_ASSERT(kv["et"] == "1");
    H_TEST_ASSERT(kv["langid"] == "2052");
    H_TEST_ASSERT(kv["iever"] == "8.0.7600.17115");
    H_TEST_ASSERT(kv["win64"] == "0");
    H_TEST_ASSERT(kv["uh"] == "2");
    //H_TEST_ASSERT(kv["vk"] == "b85d08bc");
}



TEST_UNIT(http_multi_part_parse_error2) 
{
    //input data has an error : scombo's value format wrong
    std::string b64 = "LS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJjIg0KDQpnDQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLUF3VlBMcFo1NDhrMlZOUzNKVkdJDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9Im1kNXMiDQoNCp2X1Sc6542kGZNDV0khw2aUEmISgJCgwb6A8aBIqIZ8RvGnk+UBZzuwsMpPobUoF+1MsP2cT5045+pnIisRElHum6WoiJR6yTALdVIQEnlBWlKIaZJlcpzFJjemu7nHBPw7eWDHQXQcCVZhG9kn170VnGnJbKtCZC4N5WH6xT9/6ObOEU+pwE3I+XePrLQAxC5H6u8VCcnrV7YaiSNp+f8w8UPXgGF9cLOpT4w/GX09kX3GZdCMycMFbkoVX6V6HTPYbMZEYW1KcoX/XS4xptyJMtxKn9fa/U+fBKNcWLYBHgAx5EvDuQmND60sDuTdZsTZta+aZKvvBcVS9KQxjUahDfEuDvuH3Ush9BlYNGjm4vDzP6n9MuTbgDMoGe3O+TH6//rLmsZC0kUKKmyt8d5/j9fWdmdiKg2b71xuLDTRT9oUZ0eU7B5e2tPgajO05CmbvIdFolXXm8vkBNsl40sTxvEgQD4qrAvPBGtv8TvsrNpeUAdY7HgnJgPwNDZQRfak5BV4TekLCwLVrB4UkkwAAAAAAAAAAA0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJmb3JtYXQiDQoNClhNTA0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJwcm9kdWN0Ig0KDQpkZWVwc2Nhbg0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJjb21ibyINCg0KcHJpbWFyeQ0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJ2ZXJzaW9uIg0KDQozLjMuOC4yMDE2DQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLUF3VlBMcFo1NDhrMlZOUzNKVkdJDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9InYiDQoNCjINCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tQXdWUExwWjU0OGsyVk5TM0pWR0kNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0ib3N2ZXIiDQoNCjYuMS43NjAwLjI1Ni4xLjANCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tQXdWUExwWjU0OGsyVk5TM0pWR0kNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0ic3Byb2R1Y3QiDQoNCjM2MFNEDQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLUF3VlBMcFo1NDhrMlZOUzNKVkdJDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9InNjb21ibyIN1atmMzYwUlANCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tQXdWUExwWjU0OGsyVk5TM0pWR0kNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0idmsiDQoNCmI4NWQwOGJjDQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLUF3VlBMcFo1NDhrMlZOUzNKVkdJDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9Im1pZCINCg0KYzFlN2IwZDRiODI2MDU5MWQ2OTExYTFmNGRjNWQ2ZjINCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tQXdWUExwWjU0OGsyVk5TM0pWR0kNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0iZXQiDQoNCjENCi0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tQXdWUExwWjU0OGsyVk5TM0pWR0kNCkNvbnRlbnQtRGlzcG9zaXRpb246IGZvcm0tZGF0YTsgbmFtZT0ibGFuZ2lkIg0KDQoyMDUyDQotLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLUF3VlBMcFo1NDhrMlZOUzNKVkdJDQpDb250ZW50LURpc3Bvc2l0aW9uOiBmb3JtLWRhdGE7IG5hbWU9ImlldmVyIg0KDQo4LjAuNzYwMC4xNzExNQ0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJ3aW42NCINCg0KMA0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSQ0KQ29udGVudC1EaXNwb3NpdGlvbjogZm9ybS1kYXRhOyBuYW1lPSJ1aCINCg0KMg0KLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS1Bd1ZQTHBaNTQ4azJWTlMzSlZHSS0tDQo=";
    std::string post = simcc::Base64::Decode(b64.data(), b64.size());
    std::string string_value;
    simcc::Slice slice_value;
    H_TEST_ASSERT(simcc::HttpMultiDataParser::Extract(post.data(), post.size(), std::string("product"), string_value));
    H_TEST_ASSERT(string_value == "deepscan");
    H_TEST_ASSERT(simcc::HttpMultiDataParser::Extract(post.data(), post.size(), simcc::Slice("product"), slice_value));
    H_TEST_ASSERT(slice_value == "deepscan");


    H_TEST_ASSERT(simcc::HttpMultiDataParser::Extract(post.data(), post.size(), std::string("v"), string_value));
    H_TEST_ASSERT(string_value == "2");
    H_TEST_ASSERT(simcc::HttpMultiDataParser::Extract(post.data(), post.size(), simcc::Slice("v"), slice_value));
    H_TEST_ASSERT(slice_value == "2");

    H_TEST_ASSERT(simcc::HttpMultiDataParser::Extract(post.data(), post.size(), std::string("win64"), string_value));
    H_TEST_ASSERT(string_value == "0");
    H_TEST_ASSERT(simcc::HttpMultiDataParser::Extract(post.data(), post.size(), simcc::Slice("win64"), slice_value));
    H_TEST_ASSERT(slice_value == "0");


    H_TEST_ASSERT(simcc::HttpMultiDataParser::Extract(post.data(), post.size(), std::string("uh"), string_value));
    H_TEST_ASSERT(string_value == "2");
    H_TEST_ASSERT(simcc::HttpMultiDataParser::Extract(post.data(), post.size(), simcc::Slice("uh"), slice_value));
    H_TEST_ASSERT(slice_value == "2");

    H_TEST_ASSERT(simcc::HttpMultiDataParser::Extract(post.data(), post.size(), std::string("mid"), string_value));
    H_TEST_ASSERT(string_value == "c1e7b0d4b8260591d6911a1f4dc5d6f2");
    H_TEST_ASSERT(simcc::HttpMultiDataParser::Extract(post.data(), post.size(), simcc::Slice("mid"), slice_value));
    H_TEST_ASSERT(slice_value == "c1e7b0d4b8260591d6911a1f4dc5d6f2");


    H_TEST_ASSERT(simcc::HttpMultiDataParser::Extract(post.data(), post.size(), std::string("et"), string_value));
    H_TEST_ASSERT(string_value == "1");
    H_TEST_ASSERT(simcc::HttpMultiDataParser::Extract(post.data(), post.size(), simcc::Slice("et"), slice_value));
    H_TEST_ASSERT(slice_value == "1");
}


namespace {

    std::string GetBoundary(const std::string& boundary_line_value) {
        std::string boundary;
        size_t pos = boundary_line_value.find("boundary=");
        if (pos == std::string::npos) {
            static const std::string __s_null;
            return __s_null;
        }
        
        boundary = boundary_line_value.substr(pos + sizeof("boundary=") - 1);
        return boundary;
    }

}

TEST_UNIT(http_multi_part_parse_new_boundary) 
{
    //input data has an error : scombo's value format wrong
    std::string b64 = "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";
    std::string post = simcc::Base64::Decode(b64.data(), b64.size());

    std::vector<std::string> lines;
    simcc::StringUtil::Split(lines, post, "\r\n");
    std::string boundary;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        if (simcc::StringUtil::StartsWith(lines[i], "Content-Type:"))
        {
            boundary = GetBoundary(lines[i]);
            break;
        }
    }
    
    H_TEST_ASSERT(boundary == "------");
    ssmap kv;
    H_TEST_ASSERT(simcc::HttpMultiDataParser::Parse<std::string>(post.data(), post.size(), boundary, kv));

    H_TEST_ASSERT(kv.size() > 0);
    H_TEST_ASSERT(kv["appid"] == "448084bbbf35e286");
    H_TEST_ASSERT(kv["nonce"] == "1366014596992941\n");
    H_TEST_ASSERT(kv["q"] == "dab19e82e1f9a681ee73346d3e7a575e_5fd253259f2d26eebe0472e9347fe9a1_cc4e427b15d6ddd711b304a48f80bdbe");
    H_TEST_ASSERT(kv["timestamp"] == "1366014415000.0");
    H_TEST_ASSERT(kv["v"] == "1.0");
    H_TEST_ASSERT(kv["sign"] == "4ce0ca187c711072");
}





TEST_UNIT(http_url_parser_test1) 
{
    std::string d[][3] = {
        // url                                                                                          key, value,
        {"http://www.microsofttranslator.com/bv.aspx?from=&to=zh-chs&a=http://hnujug.com/", "to", "zh-chs"},
        {"http://www.a.com/bv.aspx?", "to", ""},
        {"http://www.a.com/bv.aspx?to=a", "to", "a"},
        {"http://www.a.com/bv.aspx?to=a&", "to", "a"},
        {"http://www.a.com/bv.aspx?x=y&to=a&", "to", "a"},
        {"http://www.a.com/bv.aspx?x=y&to=a&xxx=yyy", "to", "a"},
        {"http://www.a.com/bv.aspx?to=&x=y", "to", ""}
    };

    for (size_t i = 0; i < H_ARRAYSIZE(d); i++)
    {
        simcc::URIParser::ssmap kv;
        H_TEST_ASSERT(simcc::URIParser::Parse(d[i][0].data(), d[i][0].size(), kv));
        H_TEST_ASSERT( kv[d[i][1] ] == d[i][2]);
    }
}


TEST_UNIT(test_evqing_EmbeddedHTTPServer_ParseURI)
{
    std::string d[][2] = {
        { "/abc", "/abc" },
        { "/abc?aaa", "/abc" },
        { "/abc?aaa=1", "/abc" },
        { "/abc?a", "/abc" },
        { "/abc", "/abc" },
        { "/abc/", "/abc/" },
        { "/abc/", "/abc/" },
        { "/abc/?", "/abc/" },
        { "/abc/?a=c", "/abc/" },
        { "/a?a", "/a" },
        { "/?a", "/" },
        { "/?a=1", "/" },
        { "/?a=2&1", "/" },
        { "/?", "/" }
    };

    for (size_t i = 0; i < H_ARRAYSIZE(d); i++)
    {
        simcc::URIParser::ssmap kv;
        std::string uri;
        H_TEST_ASSERT(simcc::URIParser::Parse(d[i][0].data(), d[i][0].size(), uri, &kv));
        H_TEST_ASSERT(uri == d[i][1]);
    }
}


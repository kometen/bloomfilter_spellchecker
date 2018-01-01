I have put together some useful tools and have created a simple spellchecker.
It uses a word-list I found here on github, a bloomfilter I found and tried
a while back.

The idea is to send a json-formatted list in an array with words which is
parsed using the bloomfilter and can say whether the words exists or not.

It loops the array and add it to a std::map which is then converted to a
json format and returned.

I use the crow webframework and nlohmann json framework.

To compile use

`clang++ -std=c++11 -L/usr/local/lib -lboost_system -o begin begin.cpp BloomFilter.cpp MurmurHash3.cpp`

and run it. It then parses the text file and add it to the bloomfilter and then wait for an input.

Send a curl-request like this

`curl -H "Content-Type: application/json" -X POST -d '{"words": ["jeg æder","carmen","basis"]}' localhost:18080/bloom_json`
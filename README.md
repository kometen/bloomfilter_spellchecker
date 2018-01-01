I have put together some useful tools and have created a simple spellchecker.
It uses a wordlist I found here on github, a bloomfilter I found and tried
a while back.

The article about the bloomfilter can be found at https://blog.michaelschmatz.com/2016/04/11/how-to-write-a-bloom-filter-cpp/.

The idea is to send a json-formatted list in an array with words which is
parsed using the bloomfilter and can say whether the words exists or not.

It loops the array and add it to a std::map which is then converted to a
json format and returned.

I use the crow webframework and nlohmann json framework. And boost.

To compile on macos use

`clang++ -std=c++11 -L/usr/local/lib -lboost_system -o begin begin.cpp BloomFilter.cpp MurmurHash3.cpp`

and on FreeBSD

`clang++ -lthr -std=c++11 -I/usr/local/include -L/usr/local/lib -lboost_system -o begin begin.cpp BloomFilter.cpp MurmurHash3.cpp`

and run it. It then parses the text file and add it to the bloomfilter and then wait for an input.

Send a curl-request like this

`curl -H "Content-Type: application/json" -X POST -d '{"words": ["jeg æder","carmen","basis"]}' trening.gnome.no:18080/bloom_json`

The response may look like

`{"basis":true,"carmen":true,"jeg æder":false,"tarsometatarsal":true}`

The service is running on trening.gnome.no:18080. Currently only an english wordlist is running but if I can get
wordlists in other languages pls. send me a message. One usage can be for wordgames like wordfeud where you just
want to know whether a word is present or not.

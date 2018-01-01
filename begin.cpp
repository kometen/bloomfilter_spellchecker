#include <cmath>
#include <ctime>
#include <crow.h>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

#include "BloomFilter.hpp"

std::ifstream infile("words_alpha.txt");

const auto ELEMENTS = 1370100;
const auto FALSE_POSITIVES = 0.001;

int main(int argc, char const *argv[])
{
	crow::SimpleApp app;
	std::string line;

    auto filter_size = -1 * (ELEMENTS * log(FALSE_POSITIVES)) / (log(2.0) * log(2.0));
    auto hash_functions = filter_size / ELEMENTS * log(2.0);

    BloomFilter::BloomFilter bf(filter_size, hash_functions);

    auto present = false;
    std::vector<std::string> verify_elements = {
        "lygte", "101", "loft", "park", "runway", "umulig"
    };

    while (std::getline(infile, line)) {
    	line.pop_back();
		bf.add(reinterpret_cast<const uint8_t *>(line.c_str()), line.length());
    }

    for (auto i : verify_elements) {
        present = bf.possiblyContains(reinterpret_cast<const uint8_t *>(i.c_str()), i.size());

	    if (!present) {
			std::cout << "Element '" << i << "' is not present." << std::endl;
        } else {
			std::cout << "Element '" << i << "' is (probably) present." << std::endl;
        }
    }

	CROW_ROUTE(app, "/")
	([]() {
		return "Jeg æder blåbærsyltetøj!";
	});

	CROW_ROUTE(app, "/json")
	([] {
		crow::json::wvalue x;
		x["message"] = "Jeg sender json i retur!";
		return x;
	});

	CROW_ROUTE(app, "/hello/<int>")
	([](int count) {
		if (count > 100) {
			return crow::response(400);
		}

		std::ostringstream os;
		os << count << " bottles of beer!";
		return crow::response(os.str());
	});

	CROW_ROUTE(app, "/bloom_json").methods("POST"_method)
	([&bf, &present](const crow::request& req) {
		std::map<std::string, bool> parsed_words = {};
		auto j2 = nlohmann::json::parse(req.body);
		if (j2["words"].is_null()) {
			return crow::response(400);
		}
		std::cout << j2.at("words").dump() << std::endl;
		auto wl = j2.at("words").get<std::vector<std::string>>();

		for (auto itv : wl) {	// vector
		    present = bf.possiblyContains(reinterpret_cast<const uint8_t *>(itv.c_str()), itv.size());
		    if (!present) {
				std::cout << "Element '" << itv << "' is not present." << std::endl;
	        } else {
				std::cout << "Element '" << itv << "' is (probably) present." << std::endl;
	        }
	        parsed_words[itv] = present;
	    }

		nlohmann::json result(parsed_words);
		return crow::response(result.dump());
	});

	app.port(18080).multithreaded().run();
}
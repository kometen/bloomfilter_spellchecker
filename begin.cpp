#include <crow.h>
#include <json.hpp>
#include <map>
#include <string>
#include <vector>

int main(int argc, char const *argv[])
{
	crow::SimpleApp app;

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

	CROW_ROUTE(app, "/text/<string>")
	([](std::string msg) {
		std::cout << "msg: " << msg << std::endl;
		return crow::response(msg);
	});

	CROW_ROUTE(app, "/add_json").methods("POST"_method)
	([](const crow::request& req) {
		auto x = crow::json::load(req.body);
		if (!x) {
			return crow::response(400);
		}
		int sum = x["a"].i() + x["b"].i();
		std::ostringstream os;
		os << sum;
		return crow::response{os.str()};
	});

	CROW_ROUTE(app, "/bloom_json").methods("POST"_method)
	([](const crow::request& req) {
		auto j2 = nlohmann::json::parse(req.body);
		if (j2["words"].is_null()) {
			return crow::response(400);
		}
		std::cout << j2.at("words").dump() << std::endl;
		auto wl = j2.at("words").get<std::vector<std::map<std::string, std::string>>>();
		for (auto it : wl) {
			std::cout << it[0] << std::endl;
		}
		std::string msg = "";
//		msg = j2["a"];
//		msg += std::string(j2["b"].get<std::string>());
		return crow::response(msg);
	});

	app.port(18080).multithreaded().run();
}
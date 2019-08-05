// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <influxdb_raw_db_utf8.h>
#include <influxdb_simple_api.h>
#include <influxdb_line.h>
#include <influxdb_simple_async_api.h>


#include <iostream>
#include <thread>
#include <chrono>
#include <fstream> 

using namespace influxdb::api;
using namespace std;

int main(int argc, char* argv[])
{
    try
    {
        const char* url = "http://localhost:8086";
        influxdb::raw::db_utf8 db(url, "demo");
        influxdb::api::simple_db api(url, "demo");
        api.drop();
       /* api.create();*/

  //      // {"results":[{"series":[{"columns":["name"],"name":"databases","values":[["_internal"],["mydb"]]}]}]}
		//std::cout << "show databases" << std::endl;
  //      std::cout << db.get("show databases") << std::endl;

  //      api.insert(line("test", key_value_pairs(), key_value_pairs("value", 42)));

  //      std::this_thread::sleep_for(std::chrono::milliseconds(101));

  //      // {"results":[{"series":[{"columns":["time","value"],"name":"test","values":[["2016-10-28T22:11:22.8110348Z",42]]}]}]}
  //      std::cout << db.get("select * from demo..test") << std::endl;
		//std::cout << db.get("select * from test..result limit 10") << std::endl;

  //      // or if the async call passes through:
  //      // {"results":[{"series":[{"name":"test","columns":["time","value"],
  //      //             "values":[["2016-12-09T20:24:18.8239801Z",42],["2016-12-09T20:24:18.9026688Z",41]]}]}]}

  //      api.drop();

  //      // multiple lines formatted for one synchronous call:
  //      // multiple,v1=1i
  //      // multiple,v2=2i
  //      /*std::cout << line
  //          ("multiple", key_value_pairs("v1", 1), key_value_pairs())
  //          ("multiple", key_value_pairs("v2", 2), key_value_pairs())
  //      .get() << std::endl;*/

		influxdb::async_api::simple_db async_api(url, "demo");
		async_api.create();
		std::this_thread::sleep_for(std::chrono::milliseconds(101));
		/*boost::posix_time::ptime tbegin_;
		boost::posix_time::ptime tend_;
		tbegin_ = boost::posix_time::microsec_clock::universal_time();*/


		using Clock = std::chrono::high_resolution_clock;
		auto t1 = Clock::now();
		auto count = 50000;
		for (int i = 0; i < count; i++) {
			async_api.insert(
				line
				("test",key_value_pairs("key", i),key_value_pairs("value", i+10))
				("test", key_value_pairs("key", i), key_value_pairs("value1", i + 100))
				/*("test", key_value_pairs("key", i), key_value_pairs("value2", i + 1010))
				("test", key_value_pairs("key", i), key_value_pairs("value3", i + 1010))
				("test", key_value_pairs("key", i), key_value_pairs("value4", i + 1010))*/
			);
		}
		auto t2 = Clock::now();
		auto diff = t2 - t1;
		auto count_per_second = static_cast<double>(count) / (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() / 1000.);
		/*async_api.drop();*/
		auto query = std::string("select count(*) from demo..test");
		while (!(db.get(query).find(std::to_string(count)) != std::string::npos))
		{
			std::cout <<"count:"<< db.get("select count(*) from demo..test").find(std::to_string(count)) << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(101));
		}
		auto new_t2 = Clock::now();

		/*tend_ = boost::posix_time::microsec_clock::universal_time();
		boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse;
		time_elapse = tend_ - tbegin_;*/


		std::cout << "count test:" << db.get("select count(*) from demo..test").find(std::to_string(count)) << std::endl;
		cout << "insert per second: " << count_per_second << std::endl;
		cout << "actual insert per second: " << static_cast<double>(count) / (std::chrono::duration_cast<std::chrono::milliseconds>(new_t2 - t1).count() / 1000.) << std::endl;
		/*async_api.drop();*/
		std::ofstream ofs;
		ofs.open(".//output.txt", ios::out);
		/*ofs.write(" time cost: " + std::string(count_per_second),1);*/
		ofs.close();
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
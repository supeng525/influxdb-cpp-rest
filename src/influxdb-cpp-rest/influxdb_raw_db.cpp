#include "influxdb_raw_db.h"

#include <cpprest/streams.h>

using namespace utility;
using namespace web;
using namespace web::http;

namespace {
    void throw_response(http_response const& response) {
        throw std::runtime_error(conversions::utf16_to_utf8(response.extract_string().get()));
    }
}

influxdb::raw::db::db(string_t const & url) :client(url) {}

void influxdb::raw::db::post(string_t const & query) {
    uri_builder builder(U("/query"));

    builder.append_query(U("q"), query);

    // synchronous for now
    auto response = client.request(methods::POST, builder.to_string()).get();

    if (response.status_code() != status_codes::OK) {
        throw_response(response);
    }
}

string_t influxdb::raw::db::get(string_t const & query) {
    uri_builder builder(U("/query"));

    builder.append_query(U("q"), query);

    // synchronous for now
    auto response = client.request(methods::POST, builder.to_string()).get();
    if (response.status_code() == status_codes::OK)
    {
        return response.extract_string().get();
    }
    else
    {
        throw_response(response);
        return string_t();
    }
}

    uri_builder builder(client.base_uri());
    builder.append(U("/write"));
    builder.append_query(U("db"), db);

    http_request request;

    request.set_request_uri(builder.to_uri());
    request.set_method(methods::POST);
    std::vector<unsigned char> lines_v;
    std::transform(std::begin(lines), std::end(lines), std::back_inserter(lines_v), [](char c) {return (unsigned char)c; });
    request.set_body(lines_v);

    // synchronous for now
    auto response = client.request(request).get();
    if (!(response.status_code() == status_codes::OK || response.status_code() == status_codes::NoContent)) {
        throw_response(response);
    }
}

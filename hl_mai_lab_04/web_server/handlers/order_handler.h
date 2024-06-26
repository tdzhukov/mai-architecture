#pragma once

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <Poco/JSON/Parser.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <boost/stacktrace.hpp>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../database/order.h"
#include "../../helper.h"

class OrderHandler : public HTTPRequestHandler
{

public:
    OrderHandler(const std::string &format) : _format(format)
    {
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());
        try
        {
            std::string scheme;
            std::string info;
            long id {-1};
            std::string login;
            request.getCredentials(scheme, info);
            std::cout << "scheme: " << scheme << " identity: " << info << std::endl;
            if(scheme == "Bearer") {
                if(!extract_payload(info,id,login)) {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_FORBIDDEN);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_authorized");
                    root->set("title", "Internal exception");
                    root->set("status", "403");
                    root->set("detail", "user not authorized");
                    root->set("instance", "/order");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;
                }
            }
            std::cout << "id:" << id << " login:" << login << std::endl;

            if (hasSubstr(request.getURI(), "/orders") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                long id = atol(form.get("user_id").c_str());
                auto results = database::Order::read_by_user_id(id);
                Poco::JSON::Array arr;
                for (auto s : results)
                    arr.add(s.toJSON());
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(arr, ostr);

                return;
            }
            else if (hasSubstr(request.getURI(), "/order") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                long id = atol(form.get("id").c_str());
                std::optional<database::Order> result = database::Order::read_by_id(id);
                if (result)
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(result->toJSON(), ostr);
                    return;
                }
                else
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_found");
                    root->set("title", "Internal exception");
                    root->set("status", "404");
                    root->set("detail", "not found by order id");
                    root->set("instance", "/order");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;
                }
            }
            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
            {
                database::Order order;
                order.id() = atol(form.get("id").c_str());
                order.user_id() = atol(form.get("user_id").c_str());
                order.address() = form.get("address");

                Poco::JSON::Parser parser;
                std::string service_ids_str = form.get("service_ids");
                Poco::JSON::Array::Ptr service_ids = parser.parse(service_ids_str).extract<Poco::JSON::Array::Ptr>();
                for (auto it = service_ids->begin(); it != service_ids->end(); ++it) {
                    order.service_ids().push_back(it->extract<long>());
                }

                order.add();

                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                ostr << order.get_id();
                return;
            }
            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_PUT)
            {
                database::Order order;
                order.id() = atol(form.get("id").c_str());
                order.user_id() = atol(form.get("user_id").c_str());
                order.address() = form.get("address");

                Poco::JSON::Parser parser;
                std::string service_ids_str = form.get("service_ids");
                Poco::JSON::Array::Ptr service_ids = parser.parse(service_ids_str).extract<Poco::JSON::Array::Ptr>();
                for (auto it = service_ids->begin(); it != service_ids->end(); ++it) {
                    order.service_ids().push_back(it->extract<long>());
                }

                order.update();

                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                ostr << order.get_id();
                return;
            }
            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_DELETE)
            {
                long id = atol(form.get("id").c_str());

                std::optional<database::Order> result = database::Order::read_by_id(id);
                if (result)
                {
                    result->del();

                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    ostr << id;
                    return;
                }
                else
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_found");
                    root->set("title", "Internal exception");
                    root->set("status", "404");
                    root->set("detail", "not found by order id");
                    root->set("instance", "/service");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;
                }

            }
        }
        catch (std::exception &exc)
        {
            std::cout << "Caught exception: " << exc.what() << ", trace:\n" << boost::stacktrace::stacktrace() << std::endl;
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("title", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("detail", "request ot found");
        root->set("instance", "/order");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
};

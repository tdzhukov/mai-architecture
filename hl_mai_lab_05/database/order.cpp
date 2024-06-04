#include "order.h"
#include "database.h"

#include <sstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace database
{
    Order Order::fromJSON(const std::string &str)
    {
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        // std::cout << s1 << s2 << std::endl;
        // std::cout << "from json:" << str << std::endl;
        Order order;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(s1+s2);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        order.id() = object->getValue<long>("id");
        order.user_id() = object->getValue<long>("user_id");
        order.address() = object->getValue<std::string>("address");

        std::string service_ids_str = object->getValue<std::string>("service_ids");
        Poco::JSON::Array::Ptr service_ids = parser.parse(service_ids_str).extract<Poco::JSON::Array::Ptr>();
        for (auto it = service_ids->begin(); it != service_ids->end(); ++it) {
            order.service_ids().push_back(it->extract<long>());
        }

        return order;
    }

    long Order::get_id() const
    {
        return _id;
    }

    long Order::get_user_id() const
    {
        return _user_id;
    }
    std::vector<long> Order::get_service_ids() const
    {
        return _service_ids;
    }
    std::string Order::get_address() const
    {
        return _address;
    }
    long &Order::id()
    {
        return _id;
    }
    long &Order::user_id()
    {
        return _user_id;
    }
    std::vector<long> &Order::service_ids()
    {
        return _service_ids;
    }
    std::string &Order::address()
    {
        return _address;
    }


    std::optional<Order> Order::read_by_id(long id)
    {
        std::optional<Order> result;
        std::map<std::string,long> params;
        params["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo("orders",params);

        if(!results.empty()) {
            result = fromJSON(results[0]);
        }

        return result;
    }

    std::vector<Order> Order::read_by_user_id(long user_id)
    {
        std::vector<Order> result;
        std::map<std::string,long> params;
        params["user_id"] = user_id;

        std::vector<std::string> results = database::Database::get().get_from_mongo("orders",params);

        for(std::string& s : results)
            result.push_back(fromJSON(s));


        return result;
    }

    void Order::add()
    {
        database::Database::get().send_to_mongo("orders",toJSON());
    }

    void Order::update()
    {
        std::map<std::string,long> params;
        params["id"] = _id;
        database::Database::get().update_mongo("orders",params,toJSON());
    }

    void Order::del()
    {
        std::map<std::string,long> params;
        params["id"] = _id;
        database::Database::get().delete_from_mongo("services",params);
    }

    Poco::JSON::Object::Ptr Order::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        Poco::JSON::Array::Ptr array = new Poco::JSON::Array();

        root->set("id", _id);
        root->set("user_id", _user_id);
        for (const auto& val: _service_ids) {
            array->add(val);
        }
        root->set("service_ids", array);
        root->set("address", _address);

        return root;
    }
}
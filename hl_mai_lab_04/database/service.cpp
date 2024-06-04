#include "service.h"
#include "database.h"

#include <sstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace database
{
    Service Service::fromJSON(const std::string &str)
    {
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        // std::cout << s1 << s2 << std::endl;
        // std::cout << "from json:" << str << std::endl;
        Service service;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(s1+s2);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        service.id() = object->getValue<long>("id");
        service.user_id() = object->getValue<long>("user_id");
        service.name() = object->getValue<std::string>("name");
        service.description() = object->getValue<std::string>("description");
        service.price() = object->getValue<long>("price");

        return service;
    }

    long Service::get_id() const
    {
        return _id;
    }

    long Service::get_user_id() const
    {
        return _user_id;
    }
    std::string Service::get_name() const
    {
        return _name;
    }
    std::string Service::get_description() const
    {
        return _description;
    }
    long Service::get_price() const
    {
        return _price;
    }
    long &Service::id()
    {
        return _id;
    }
    long &Service::user_id()
    {
        return _user_id;
    }
    std::string &Service::name()
    {
        return _name;
    }
    std::string &Service::description()
    {
        return _description;
    }
    long &Service::price()
    {
        return _price;
    }

    std::optional<Service> Service::read_by_id(long id)
    {
        std::optional<Service> result;
        std::map<std::string,long> params;
        params["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo("services",params);

        if(!results.empty())
            result = fromJSON(results[0]);

        return result;
    }

    std::vector<Service> Service::read_by_user_id(long user_id)
    {
        std::vector<Service> result;
        std::map<std::string,long> params;
        params["user_id"] = user_id;

        std::vector<std::string> results = database::Database::get().get_from_mongo("services",params);

        for(std::string& s : results)
            result.push_back(fromJSON(s));


        return result;
    }

    void Service::add()
    {
        database::Database::get().send_to_mongo("services",toJSON());
    }

    void Service::update()
    {
        std::map<std::string,long> params;
        params["id"] = _id;
        database::Database::get().update_mongo("services",params,toJSON());
    }

    void Service::del()
    {
        std::map<std::string,long> params;
        params["id"] = _id;
        database::Database::get().delete_from_mongo("services",params);
    }

    Poco::JSON::Object::Ptr Service::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("user_id", _user_id);
        root->set("name", _name);
        root->set("description", _description);
        root->set("price", _price);

        return root;
    }
}
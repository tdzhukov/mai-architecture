#pragma once

#include <string>
#include <vector>
#include <optional>

#include <Poco/JSON/Object.h>

namespace database
{
    class Service{
        private:
            long _id;
            long _user_id;
            std::string _name;
            std::string _description;
            long _price;

        public:

            static Service  fromJSON(const std::string & str);

            long               get_id() const;
            long               get_user_id() const;
            std::string        get_name() const;
            std::string        get_description() const;
            long               get_price() const;

            long&               id();
            long&               user_id();
            std::string&        name();
            std::string&        description();
            long&               price();

            static std::optional<Service> read_by_id(long id);
            static std::vector<Service> read_by_user_id(long user_id);
            void   add();
            void   update();
            void   del();
            Poco::JSON::Object::Ptr toJSON() const;

    };
}

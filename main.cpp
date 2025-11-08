#include <iostream>
#include <json/value.h>
#include <string>
#include "reflect.hpp"

#define A

std::string toString(Json::Value json) { 
   Json::StreamWriterBuilder builder;
    builder["indentation"] = " ";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ostringstream os;
    writer->write(json, &os);
    return os.str();
}

Json::Value fromString(const std::string& json) {
    Json::Reader reader;
    Json::Value root;
    reader.parse(json, root);
    return root;
}


template <class T> requires (!reflect_trait<T>::has_members())
Json::Value serialize(T object) {
    return object;
}


template <class T> requires (reflect_trait<T>::has_members())
Json::Value serialize(T const& object) {
    Json::Value root;
    reflect_trait<T>::for_each_members(object, [&](const char* key, auto& value) {
        root[key] = serialize(value);
    });
    return root;
}


template <class T> requires (!reflect_trait<T>::has_members())
T deserialize(Json::Value root) {
    return root.as<T>();
}

template <class T> requires (reflect_trait<T>::has_members())
T deserialize(Json::Value root) {
    T object;
    reflect_trait<T>::for_each_members(object, [&](const char* key, auto& value) {
        using ValueType = std::decay_t<decltype(value)>; 
        if (root.isMember(key))
            value =  deserialize<ValueType>(root[key]);
    });
    return object;
}




struct Address {
    std::string country;
    std::string city;
    // REFLECT(country, city)

};

struct Student {
    std::string name;
    int age;
    int sex;
    Address address;
    // REFLECT(name, age, sex, address);
};


template <class T>
struct Teacher {
    std::string name;
    int age;
    int height;
    int weight;
    T garde;
    // REFLECT(name, age, height, weight, garde);
    // 
};


template <class T, class N>
struct Baby {
    std::string name;
    T month;
    N sex;
    // REFLECT(name, age, sex);
};

template <class T, class N, class M>
struct Subject{
    std::string name;
    T month;
    M day;
    N year;
    // REFLECT(name, age, sex);

};

struct MqttHumity {
    double humity;
    std::string unit;
    REFLECT(humity, unit)
};

struct MqttTempeature {
    double tempeature;
    std::string unit;
    REFLECT(tempeature, unit)
};

struct MqttDataPackage {
    int id;
    std::string timestamp;
    MqttHumity mqtt_humity;
    MqttTempeature mqtt_tempeature;
    REFLECT(id, timestamp, mqtt_humity, mqtt_tempeature);
};





// REFLECT_TYPE_TEMPLATED(((Teacher<T>), class T), name, age, height, weight, garde);
// REFLECT_TYPE_TEMPLATED(((Baby<T,N>), class T, class N), name, month, sex);
// REFLECT_TYPE_TEMPLATED(((Subject<T, N, M>), class T, class N, class M), name, month, day, year);

REFLECT_TYPE(Address, country, city);
REFLECT_TYPE(Student, name, age, sex, address);






int main() {
    MqttDataPackage val = {
        .id = 1,
        .timestamp = "2025-11-08T15:02",
        .mqtt_humity = {
            .humity = 50.10000000000,
            .unit = "%",
        },
        .mqtt_tempeature = {
            .tempeature = 37.10000000000,
            .unit = "℃ ",
        },
    };
    std::string bin = toString(serialize(val));
    std::cout << bin << "\n";
    auto val_1 = deserialize<MqttDataPackage>(fromString(bin));
    std::cout << val_1.id << "\n";
    std::cout << val_1.timestamp << "\n";
    std::cout << val_1.mqtt_tempeature.tempeature << val_1.mqtt_tempeature.unit << "\n";
    std::cout << val_1.mqtt_humity.humity << val_1.mqtt_humity.unit << "\n";
    // Student stu = {
    //     .name = "ma",
    //     .age = 21,
    //     .sex = 1,
    //     .address = {
    //         .country = "china",
    //         .city = "qixian",
    //     }
    // };

    // std::string bin = toString(serialize(stu));
    // std::cout << bin << '\n';
    // auto val = deserialize<Student>(fromString(bin));
    // std::cout << val.age << '\n';
    // std::cout << val.name << '\n';
    // std::cout << val.address.country << '\n';
    // Teacher<int> teacher = {
    //     .name = "yu",
    //     .age = 21,
    //     .height = 179,
    //     .weight = 70,
    //     .garde = 3,
    // };

    // Teacher<float> teacher_2 = {
    //     .name = "yu",
    //     .age = 21,
    //     .height = 179,
    //     .weight = 70,
    //     .garde = 0.3,
    // };
    // Subject<int, float, double> subject = {
    //     .name = "englist",
    //     .month = 10,
    //     .day = 10.000,
    //     .year = 11.0000,
    // };
    // std::string bin_2 = toString(serialize(teacher));
    // std::string bin_3 = toString(serialize(teacher_2));
    // std::string bin_4 = toString(serialize(subject));
    // std::cout << bin_2 << '\n';
    // std::cout << bin_3 << '\n';
    // std::cout << bin_3 << '\n';
    // std::cout << bin_4 << '\n';

    return 0;
}

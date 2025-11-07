#include <iostream>
#include <string>
#include "reflect.hpp"

#define A


// 上面是库

struct Student {
    std::string name;
    int age;
    int sex;
    // REFLECT(name, age, sex);
};


template <class T>
struct Teacher {
    std::string name;
    int age;
    int height;
    int weight;
    T garde;
    // REFLECT(name, age, height, weight, garde);
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

REFLECT_TYPE_TEMPLATED(((Teacher<T>), class T), name, age, height, weight, garde);
REFLECT_TYPE_TEMPLATED(((Baby<T,N>), class T, class N), name, month, sex);
REFLECT_TYPE_TEMPLATED(((Subject<T, N, M>), class T, class N, class M), name, month, day, year);
REFLECT_TYPE(Student, name, age, sex);



template <class T>
std::string serialize(T &object) {
    Json::Value root;
    reflect_trait<T>::for_each_members(object, [&](const char* key, auto& value) {
        root[key] = value;
    });
    #ifdef A
        Json::StreamWriterBuilder builder;
        builder["indentation"] = " ";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        std::ostringstream os;
        writer->write(root, &os);
        return os.str();
    #endif
    return root.toStyledString();
}

template <class T>
T deserialize(const std::string &json) {
    Json::Value root;
    Json::Reader reader;
    reader.parse(json, root);

    T object;
    reflect_trait<T>::for_each_members(object, [&](const char* key, auto& value) {
        using ValueType = std::decay_t<decltype(value)>; 
        if (root.isMember(key))
            value = root[key].as<ValueType>();
    });
    return object;
}


// REFLECT1(Student, name, age, sex);
// REFLECT1(Teacher, name, age, height, weight, garde);

int main() {
    Student stu = {
        .name = "ma",
        .age = 21,
        .sex = 1,
    };
    Teacher<int> teacher = {
        .name = "yu",
        .age = 21,
        .height = 179,
        .weight = 70,
        .garde = 3,
    };

    Teacher<float> teacher_2 = {
        .name = "yu",
        .age = 21,
        .height = 179,
        .weight = 70,
        .garde = 0.3,
    };
    Subject<int, float, double> subject = {
        .name = "englist",
        .month = 10,
        .day = 10.000,
        .year = 11.0000,
    };
    std::string bin = serialize(stu);
    std::string bin_2 = serialize(teacher);
    std::string bin_3 = serialize(teacher_2);
    std::string bin_4 = serialize(subject);
    std::cout << bin << '\n';
    std::cout << bin_2 << '\n';
    std::cout << bin_3 << '\n';
    std::cout << bin_3 << '\n';
    std::cout << bin_4 << '\n';

    auto val = deserialize<Teacher<float>>(bin_2);
    return 0;
}

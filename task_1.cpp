#include <iostream>
#include <string>
#include <memory>


class Any {
    struct BaseHolder {
        virtual ~BaseHolder(){}
        virtual const std::type_info& typeInfo() const = 0;
    };
    
    template<typename T> struct 
    Holder: BaseHolder {
        T content;

        Holder(const T& content) 
            : content(content) {}
        
        const std::type_info& typeInfo() const {
            return typeid(content);
        }
    };

    //std::shared_ptr<BaseHolder> held;
    BaseHolder* held;

public:

    template<typename T>
    Any(const T &content) {
        //held = std::make_shared<T>(content);
        held = new Holder<T>(content);
    }

    ~Any() {
        delete held;
    }

    template<typename U>
    U get() const {
        if(typeid(U) != held->typeInfo()) {
            throw std::runtime_error("Bad any cast");
        } else {
            return static_cast<Holder<U>* >(held)->content;
            //return static_cast< std::shared_ptr<Holder<U>> >(held)->content;
            //return static_cast<Holder<U>* >(held)->content;
        }
    }

};

int main() {
    Any a(5);
    a.get<int>();
    //std::cout << a.get<int>() << std::endl;
    a.get<std::string>(); // error

    return 0;
}

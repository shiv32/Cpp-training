#include "any.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>

// Example 1: Configuration system using any
class ConfigManager {
private:
    std::map<std::string, my_std::any> config;

public:
    template<typename T>
    void set(const std::string& key, T&& value) {
        config[key] = std::forward<T>(value);
    }

    template<typename T>
    T get(const std::string& key) {
        auto it = config.find(key);
        if (it == config.end()) {
            throw std::runtime_error("Key not found: " + key);
        }
        return my_std::any_cast<T>(it->second);
    }

    template<typename T>
    T get_or_default(const std::string& key, const T& default_value) {
        auto it = config.find(key);
        if (it == config.end() || it->second.type() != typeid(T)) {
            return default_value;
        }
        return my_std::any_cast<T>(it->second);
    }

    void print_all() const {
        std::cout << "Configuration:\n";
        for (const auto& [key, value] : config) {
            std::cout << "  " << key << ": " << value.type().name() << "\n";
        }
    }
};

// Example 2: Variant-like behavior with any
class Event {
public:
    std::string name;
    my_std::any data;

    Event(const std::string& n) : name(n) {}
};

// Example 3: Polymorphic container
class PolymorphicContainer {
private:
    std::vector<my_std::any> items;

public:
    void add(const my_std::any& item) {
        items.push_back(item);
    }

    size_t size() const { return items.size(); }

    template<typename T>
    T* get(size_t index) {
        if (index >= items.size()) {
            return nullptr;
        }
        return my_std::any_cast<T>(&items[index]);
    }

    void print_types() const {
        std::cout << "Container types: ";
        for (const auto& item : items) {
            std::cout << item.type().name() << " ";
        }
        std::cout << "\n";
    }
};

int main() {
    std::cout << "=== Advanced std::any Usage Examples ===\n\n";

    // Example 1: Configuration System
    std::cout << "1. Configuration Manager\n";
    std::cout << std::string(40, '-') << "\n";
    {
        ConfigManager cfg;
        cfg.set("app_name", std::string("MyApp"));
        cfg.set("version", 1.5);
        cfg.set("max_connections", 100);
        cfg.set("debug_mode", true);

        cfg.print_all();
        std::cout << "\nRetrieving values:\n";
        std::cout << "  app_name: " << cfg.get<std::string>("app_name") << "\n";
        std::cout << "  version: " << cfg.get<double>("version") << "\n";
        std::cout << "  max_connections: " << cfg.get<int>("max_connections") << "\n";
        std::cout << "  debug_mode: " << std::boolalpha 
                  << cfg.get<bool>("debug_mode") << "\n";
        std::cout << "  timeout (default): " 
                  << cfg.get_or_default<int>("timeout", 30) << "\n";
    }

    // Example 2: Event System
    std::cout << "\n2. Event System\n";
    std::cout << std::string(40, '-') << "\n";
    {
        std::vector<Event> events;
        
        Event click_event("click");
        click_event.data = std::string("Button pressed");
        events.push_back(click_event);
        
        Event key_event("keydown");
        key_event.data = static_cast<int>('A');
        events.push_back(key_event);
        
        Event resize_event("resize");
        resize_event.data = std::pair<int, int>{1920, 1080};
        events.push_back(resize_event);

        for (const auto& event : events) {
            std::cout << "Event: " << event.name 
                      << " (data type: " << event.data.type().name() << ")\n";
        }
    }

    // Example 3: Polymorphic Container
    std::cout << "\n3. Polymorphic Container\n";
    std::cout << std::string(40, '-') << "\n";
    {
        PolymorphicContainer container;
        container.add(42);
        container.add(3.14159);
        container.add(std::string("hello"));
        container.add(std::vector<int>{1, 2, 3});

        container.print_types();

        std::cout << "Retrieving values:\n";
        if (auto* i = container.get<int>(0)) {
            std::cout << "  [0] int: " << *i << "\n";
        }
        if (auto* d = container.get<double>(1)) {
            std::cout << "  [1] double: " << *d << "\n";
        }
        if (auto* s = container.get<std::string>(2)) {
            std::cout << "  [2] string: " << *s << "\n";
        }
        if (auto* v = container.get<std::vector<int>>(3)) {
            std::cout << "  [3] vector: ";
            for (int x : *v) std::cout << x << " ";
            std::cout << "\n";
        }
    }

    // Example 4: Heterogeneous Collection
    std::cout << "\n4. Heterogeneous Collection\n";
    std::cout << std::string(40, '-') << "\n";
    {
        std::vector<my_std::any> mixed_data;
        mixed_data.push_back(123);
        mixed_data.push_back("text");
        mixed_data.push_back(45.67);
        mixed_data.push_back(true);

        std::cout << "Mixed collection:\n";
        for (size_t i = 0; i < mixed_data.size(); ++i) {
            const auto& data = mixed_data[i];
            std::cout << "  [" << i << "] " << data.type().name();
            
            // Runtime type checking
            if (data.type() == typeid(int)) {
                std::cout << " = " << my_std::any_cast<int>(data);
            } else if (data.type() == typeid(const char*)) {
                std::cout << " = " << my_std::any_cast<const char*>(data);
            } else if (data.type() == typeid(double)) {
                std::cout << " = " << my_std::any_cast<double>(data);
            } else if (data.type() == typeid(bool)) {
                std::cout << " = " << std::boolalpha 
                          << my_std::any_cast<bool>(data);
            }
            std::cout << "\n";
        }
    }

    // Example 5: Type-safe visitor pattern
    std::cout << "\n5. Type-Safe Operations\n";
    std::cout << std::string(40, '-') << "\n";
    {
        std::vector<my_std::any> values = {
            10, 
            20.5, 
            std::string("text"),
            true
        };

        std::cout << "Processing heterogeneous data:\n";
        for (const auto& val : values) {
            if (val.type() == typeid(int)) {
                auto x = my_std::any_cast<int>(val);
                std::cout << "  Integer: " << x << " * 2 = " << (x * 2) << "\n";
            } else if (val.type() == typeid(double)) {
                auto x = my_std::any_cast<double>(val);
                std::cout << "  Double: " << x << " * 2 = " << (x * 2) << "\n";
            } else if (val.type() == typeid(std::string)) {
                auto s = my_std::any_cast<std::string>(val);
                std::cout << "  String: " << s << " (length: " 
                          << s.length() << ")\n";
            } else if (val.type() == typeid(bool)) {
                auto b = my_std::any_cast<bool>(val);
                std::cout << "  Boolean: " << std::boolalpha << b << "\n";
            }
        }
    }

    std::cout << "\n=== All Advanced Examples Completed ===\n";
    return 0;
}

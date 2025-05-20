#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <memory>

class Human
{
public:
    Human(const std::string& name)
        : name_(name)
    {
        std::cout << "Constructor Human(" << name_ << ")" << std::endl;
    }

    Human(const Human&) = delete;
    Human& operator=(const Human&) = delete;

    ~Human()
    {
        std::cout << "Destructor ~Human(" << name_ << ")" << std::endl;
    }

    void set_partner(std::weak_ptr<Human> partner)
    {
        partner_ = partner;
    }

    void description() const
    {
        std::cout << "My name is " << name_ << std::endl;

        if (std::shared_ptr<Human> current_partner = partner_.lock(); current_partner)
        {
            std::cout << "My partner is " << current_partner->name_ << std::endl;
        }
    }

private:
    std::weak_ptr<Human> partner_;
    std::string name_;
};

TEST_CASE("shared_ptrs leak - circular dependency")
{
    // RC husband == 1
    auto husband = std::make_shared<Human>("Jan");

    // RC wife == 1
    auto wife = std::make_shared<Human>("Ewa");

    // RC wife == 1
    husband->set_partner(wife);

    // RC husband == 1
    wife->set_partner(husband);

    husband->description();
}

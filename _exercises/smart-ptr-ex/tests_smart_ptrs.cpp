#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <catch2/catch_test_macros.hpp>

using namespace std;

#define UNSAFE 0

class Gadget
{
public:
    Gadget(int id = 0)
        : id_{id}
    {
        std::cout << "Constructing Gadget(" << id_ << ")\n";
    }

    Gadget(const Gadget&) = delete;
    Gadget& operator=(const Gadget&) = delete;

    ~Gadget()
    {
        std::cout << "Destroying ~Gadget(" << id_ << ")\n";
    }

    int id() const
    {
        return id_;
    }

    void set_id(int id)
    {
        id_ = id;
    }

    void use()
    {
        std::cout << "Using a gadget with id: " << id() << '\n';
    }

    void unsafe_might_throw()
    {
        std::cout << "Using a gadget with id: " << id() << " - Ups... It crashed..." << std::endl;
        if  (UNSAFE)
            throw std::runtime_error("ERROR");
    }

private:
    int id_;
};

// This code is part of the legacy code and should not be modified
namespace LegacyCode
{
    Gadget* create_many_gadgets(unsigned int size)
    {
        Gadget* many_gadgets = new Gadget[size];

        for (unsigned int i = 0; i < size; ++i)
            many_gadgets[i].set_id(i);

        return many_gadgets;
    }
}

void reset_value(Gadget& g, int n)
{
    // some logic

    g.set_id(n);
    cout << "New id for Gadget: " << g.id() << endl;
}

//////////////////////////////////////////////
// TODO - modernize the code below

Gadget* create_gadget(int arg)
{
    return new Gadget(arg);
}

class Player
{
    Gadget* gadget_;
    std::ostream* logger_;

    Player(const Player&);
    Player& operator=(const Player&);

public:
    Player(Gadget* g, std::ostream* logger = NULL)
        : gadget_(g)
        , logger_(logger)
    {
        if (g == NULL)
            throw std::invalid_argument("Gadget can not be null");
    }

    ~Player()
    {
        if (logger_)
            *logger_ << "Destroing a gadget: " << gadget_->id() << std::endl;

        delete gadget_;
    }

    void play()
    {
        if (logger_)
            *logger_ << "Player is using a gadget: " << gadget_->id() << std::endl;

        gadget_->use();
    }
};

TEST_CASE("Exercise 1 - Modernize the code using smart pointers")
{
    Gadget* ptr_gadget = create_gadget(4);

    /* kod korzystajacy z ptrX */

    reset_value(*ptr_gadget, 5);

    ptr_gadget->unsafe_might_throw();

    delete ptr_gadget;
}

TEST_CASE("Exercise 2 - Modernize the code using smart pointers")
{
    int size = 10;

    Gadget* buffer = LegacyCode::create_many_gadgets(size);

    for (int i = 0; i < size; ++i)
        buffer[0].unsafe_might_throw();

    delete[] buffer;
}

TEST_CASE("Exercise 3 - Modernize the code using smart pointers")
{
    vector<Gadget*> my_gadgets;

    my_gadgets.push_back(create_gadget(87));
    my_gadgets.push_back(create_gadget(12));
    my_gadgets.push_back(new Gadget(98));

    int value_generator = 0;
    for (vector<Gadget*>::iterator it = my_gadgets.begin(); it != my_gadgets.end(); ++it)
    {
        cout << "Gadget's old id: " << (*it)->id() << endl;
        reset_value(**it, ++value_generator);
    }

    delete my_gadgets[0];
    my_gadgets[0] = NULL;

    Player p(my_gadgets.back());
    my_gadgets.back() = NULL;
    p.play();

    my_gadgets[1]->unsafe_might_throw();

    for (vector<Gadget*>::iterator it = my_gadgets.begin(); it != my_gadgets.end(); ++it)
        delete *it;
}
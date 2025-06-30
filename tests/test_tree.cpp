#include <string>
#include <iostream>
#include <cassert>
#include <type_traits>


#include "algui/TreeNode.hpp"


using namespace algui;


namespace test {


    class Test : public TreeNode<Test> {
    public:
        std::string id;

        Test(const std::string& id = {}) : id(id) {
        }
    };


    template <class T>
    static void print_tree(const std::shared_ptr<T>& node, size_t depth = 0) {
        std::cout << std::string(depth * 4, ' ') << node->id << std::endl;
        Test* parent = dynamic_cast<Test*>(node.get());
        if (parent) {
            for (auto child = parent->getFirstChild(); child; child = child->getNextSibling()) {
                print_tree(child, depth + 1);
            }
        }
    }
}


using namespace test;


void test_tree() {
    std::shared_ptr<Test> node1 = std::make_shared<Test>("node1");
    std::shared_ptr<Test> node11 = std::make_shared<Test>("node11");
    std::shared_ptr<Test> node12 = std::make_shared<Test>("node12");
    std::shared_ptr<Test> node111 = std::make_shared<Test>("node111");
    std::shared_ptr<Test> node112 = std::make_shared<Test>("node112");

    node1->addChild(node11);
    node1->addChild(node12);

    node11->addChild(node111);
    node11->addChild(node112);

    print_tree(node1);

    assert(node1->getRoot() == node1);
    assert(node11->getRoot() == node1);
    assert(node12->getRoot() == node1);
    assert(node111->getRoot() == node1);
    assert(node112->getRoot() == node1);

    assert(node1->contains(node1));
    assert(node1->contains(node11));
    assert(node1->contains(node12));
    assert(node1->contains(node111));
    assert(node1->contains(node112));

    assert(!node11->contains(node1));
    assert(!node11->contains(node12));
    assert(node11->contains(node111));
    assert(node11->contains(node112));

    node11->removeChild(node111);
    node11->removeChild(node112);

    std::cout << '\n';
    print_tree(node1);

    node1->removeChild(node12);
    node1->removeChild(node11);

    std::cout << '\n';
    print_tree(node1);
}

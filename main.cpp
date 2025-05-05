#include <iostream>
#include <fstream>
#include <string>
#include "Tree.h"


int main() {

    std::string mytest = "shuffled_data";

    Tree* tree = new Tree(3.0, "customer100.txt");
    tree->generate_tree();


    tree->write_tree();
    
    
    tree->delete_user(4300000004);
   
   

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    tree->write_tree();

    tree->delete_user(4300000005);


    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    tree->write_tree();

    tree->delete_user(4300000001);


    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    tree->write_tree();

    

    return 0;
}
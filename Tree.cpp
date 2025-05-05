#include "Tree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

void Tree::delete_user(long long x)
{
	cvor* temp = search(x);

	if (!temp) {
		std::cout << "User does not exist" << std::endl;
		return;
	}

	std::stack<cvor*> st;
	st.push(temp);

	while (!st.empty()) {

		temp = st.top();
		st.pop();


		if (is_leaf(temp) || temp->father == root) {

			for (int i = 0; i < temp->keys.size(); i++) {
				if (temp->keys[i]->id == x) {
					delete temp->keys[i];
					temp->keys.erase(temp->keys.begin() + i);

				}
			}


			if (temp->keys.size() >= temp->min_keys) {
				return;
			}
			else
			{
				cvor* brother = can_borrow(temp);

				if (brother) {

					if (brother == temp->right_brother || brother == temp->right_brother && brother == temp->right_brother->right_brother) {
						while (temp != brother) {
							brother = borrow_right(brother);

						}
					}
					else if (brother == temp->left_brother || brother == temp->left_brother && brother == temp->left_brother->left_brother) {
						while (temp != brother) {
							brother = borrow_left(brother);

						}
					}
				}
				else
				{
					cvor * f = minimalizing(temp);
					if (temp->keys.size() >= temp->min_keys) {
						break;
					}
					if(f)st.push(f);

				}
			}
			
		}

		else {

			cvor* p = find_s(temp, x);
			if(p)st.push(p);

		}
		correct_brothers_and_link_everything();
	}
}

Tree::cvor* Tree::minimalizing(cvor* temp)
{
	if (!temp) {
		std::cout << "Ivalid temp" << std::endl;
		return nullptr;
	}
	
	std::vector<info*> vec; 

	cvor* right = nullptr;
	cvor* left = nullptr;

	if (temp->right_brother) {
		right = temp->right_brother;
	}
	if (temp->left_brother) {
		left = temp->left_brother;
	}

	if (temp->father->sons.size() <= 2) {

		temp->father->sons.clear();

		temp->father->keys.insert(temp->father->keys.begin(), temp->keys.begin(), temp->keys.end());

		temp->father->sons.insert(temp->father->sons.begin(), temp->sons.begin(), temp->sons.end());

		if (right) {
			temp->father->keys.insert(temp->father->keys.begin(), right->keys.begin(), right->keys.end());
			temp->father->sons.insert(temp->father->sons.begin(), right->sons.begin(), right->sons.end());
			delete right;
		}
		else if (left) {
			temp->father->keys.insert(temp->father->keys.begin(), left->keys.begin(), left->keys.end());
			temp->father->sons.insert(temp->father->sons.begin(), left->sons.begin(), left->sons.end());
			delete left;
		}
	
		bubbleSort_keys(temp->father->keys);
		if(!temp->father->sons.empty()) bubbleSort_sons(temp->father->sons);
		delete temp;
		
		//// ovo ce mi biti ovberall a ne samo isleaf
		
		return nullptr;
	}

	if (!left ) {
		left = temp;
		
		temp = temp->right_brother;
		
		if(temp->right_brother) right = temp->right_brother;
	}
	else if (!right) {

		right = temp;
		temp = temp->left_brother;
		if(temp->left_brother)left = temp->left_brother;


	}
	cvor* father = temp->father;

	int pos = father->keys.size() - 1;

	
	if (!temp->keys.empty()) {
		for (int i = 0; i < father->keys.size(); i++) {
			if (temp->keys.front()->id < father->keys[i]->id) {
				pos = i;
				break;
			}
		}
	}

	if (right) vec.insert(vec.end(), right->keys.begin(), right->keys.end());
	if (left) vec.insert(vec.end(), left->keys.begin(), left->keys.end());
	vec.insert(vec.end(), temp->keys.begin(), temp->keys.end());
	vec.push_back(father->keys[pos]);
	father->keys.erase(father->keys.begin() + pos);

	
	if (pos > 0) {
		vec.push_back(father->keys[pos-1]);
		father->keys.erase(father->keys.begin() + pos - 1);
	}
	bubbleSort_keys(vec);

	temp->keys.clear();
	if (left) left->keys.clear();
	if (right) right->keys.clear();


	int split = vec.size() / 2;

	
	temp->keys.insert(temp->keys.begin(), vec.begin(), vec.begin() + split);

	
	if (right) {
		right->keys.insert(right->keys.begin(), vec.begin() + split + 1, vec.end());
	}


	father->keys.push_back(vec[split]);
	bubbleSort_keys(father->keys);
	
	for (int i = 0; i < father->sons.size(); i++) {
		if (father->sons[i] == left) {
			father->sons.erase(father->sons.begin() + i);
			break;
		}
	}
	delete left;
	
	bubbleSort_sons(father->sons);
	
	link_everything(father);

	return father;

}

Tree::cvor* Tree::borrow_right(cvor* temp)
{
	
	cvor* brother = temp->left_brother;

	cvor* father = temp->father;

	int pos = father->keys.size() - 1;


	if (!temp->keys.empty()) {
		for (int i = 0; i < father->keys.size(); i++) {

			if (temp->keys.front()->id < father->keys[i]->id) {
				pos = i - 1;
				break;
			}
		}
	}

	brother->keys.push_back(father->keys[pos]);

	father->keys[pos] = temp->keys.front();

	if(!temp->keys.empty())temp->keys.erase(temp->keys.begin());

	temp = brother;

	return temp;

}

Tree::cvor* Tree::borrow_left(cvor* temp)
{

	cvor* brother = temp->right_brother;


	cvor* father = temp->father;

	int pos = father->keys.size() - 1;


	if (!temp->keys.empty()) {
		for (int i = 0; i < father->keys.size(); i++) {

			if (temp->keys.front()->id < father->keys[i]->id) {
				pos = i;
				break;
			}
		}
	}

	brother->keys.insert(brother->keys.begin(), father->keys[pos]);

	father->keys[pos] = temp->keys.back();

	if (!temp->keys.empty()) {
		temp->keys.pop_back();
	}
	// ovdje puca nekad

	temp = brother;

	return temp;

}

void Tree::insert_node(info* inf)
{
	std::stack<cvor*> st;

	if (!root) {
		cvor* cv = new cvor(root_keys_max, root_sons_max, root_keys_min);

		cv->father = nullptr;

		cv->keys.push_back(inf);

		cv->right_brother = nullptr;
		cv->left_brother = nullptr;

		root = cv;

		cv->left_brother = nullptr;
		cv->right_brother = nullptr;

		return;
	}

	cvor* target = search(inf->id);

	insert_key(target, inf);

	st.push(target);

	while (!st.empty()) {

		cvor* temp = st.top();
		st.pop();
		
		if (is_overfull_node(temp)) {

			
			
			if (temp == root) {
				break_root(temp);
				return;
				
			}

			cvor* brother = can_overflow(temp);

			if (!brother) {
				std::cout << "NEMA BRATA ZA OBRADU" << std::endl;
			}

			if (brother && !is_full_node(brother)) {
				overflow(temp, brother);
				//prelivanje
				// <- | ->
			}
			else {
				
				st.push(break_node(temp, brother));
				

			}		
		}
	}
}

Tree::cvor* Tree::break_node(cvor* temp, cvor* brother)
{
	std::vector<info*> vec;
	std::vector<cvor*> vec_son;

	cvor* father = temp->father;
	int split;

	cvor* lower;
	cvor* higher;

	if (temp->left_brother == brother) {

		lower = brother;
		higher = temp;
	}
	else{
		
		lower = temp;
		higher = brother;
	}


	cvor* new_node = new cvor(all_keys_max, all_sons_max, all_keys_min);

	int pos = father->keys.size() - 1; 

	if (!temp->keys.empty()) {
		for (int i = 0; i < father->keys.size(); i++) {

			if (temp->keys.front()->id < father->keys[i]->id) {
				pos = i; 
				break; 
			}
		}
	}
	

	vec.insert(vec.begin(), temp->keys.begin(), temp->keys.end());
	vec.insert(vec.begin(), brother->keys.begin(), brother->keys.end());
	vec.push_back(father->keys[pos]);


	father->keys.erase(father->keys.begin() + pos);
	brother->keys.clear();
	temp->keys.clear();

	bubbleSort_keys(vec);


	lower->keys.insert(lower->keys.begin(), vec.begin(), vec.begin() + f1);
	new_node->keys.insert(new_node->keys.begin(), vec.begin() + f1 + 1, vec.begin() + f1 + f2 + 1 );
	higher->keys.insert(higher->keys.begin(), vec.begin() + f1 + f2 + 2, vec.end());

	father->keys.push_back(vec[f1]);
	father->keys.push_back(vec[f1+f2+1]);

	father->sons.push_back(new_node);

	bubbleSort_keys(father->keys);
	bubbleSort_sons(father->sons);

	link_everything(father);


	return father;

}


void Tree::overflow(cvor* temp, cvor*brother)
{
	std::vector<info*> vec;
	cvor* father = temp->father;

	cvor* lower;
	cvor* higher;

	if (temp->left_brother == brother) {

		lower = brother;
		higher = temp;
	}
	else {

		lower = temp;
		higher = brother;
	}

	int split;
	int pos = father->keys.size() - 1; 

	
	if (!temp->keys.empty()) {
		for (int i = 0; i < father->keys.size(); i++) {
		
			if (temp->keys.front()->id < father->keys[i]->id) {
				pos = i; 
				break;   
			}
		}
	}
	

	vec.insert(vec.begin(), temp->keys.begin(), temp->keys.end());
	vec.insert(vec.begin(), brother->keys.begin(), brother->keys.end());
	vec.push_back(father->keys[pos]);
	
	brother->keys.clear();
	temp->keys.clear();

	bubbleSort_keys(vec);

	split = vec.size() / 2;

	

	for (int i = 0; i < split; i++) {
		lower->keys.push_back(vec[i]);
	}

	for (int i = split + 1; i < vec.size(); i++) {
		higher->keys.push_back(vec[i]);
	}

	father->keys[pos] = vec[split];

	correct_brothers_and_link_everything();
	
}

void Tree::break_root(cvor* temp)
{

	cvor* left = new cvor(all_keys_max,all_sons_max, all_keys_min);
	cvor* right = new cvor(all_keys_max, all_sons_max, all_keys_min);
	int split;

	std::vector<info*> vec;
	std::vector<cvor*> vec_sons;
	std::vector<cvor*>f_sons;

	f_sons.insert(f_sons.begin(), temp->sons.begin(), temp->sons.end());
	temp->sons.clear();

	for (int i = 0; i < temp->keys.size(); i++) {

		vec.push_back(temp->keys[i]);
	}

	bubbleSort_keys(vec);
	temp->keys.clear();

	split = vec.size()/2;

	left->keys.insert(left->keys.begin(), vec.begin(), vec.begin() + split);

	right->keys.insert(right->keys.begin(), vec.begin() + split + 1, vec.end());

	vec_sons.push_back(left);

	vec_sons.push_back(right);

	bubbleSort_sons(vec_sons);

	temp->keys.push_back(vec[split]);

	for (int i = 0; i < temp->keys.size(); i++) {
		temp->sons.push_back(vec_sons[i]);
	}
	temp->sons.push_back(vec_sons.back());

	link_everything(temp);

	if (!f_sons.empty()) {
		for (int i = 0; i < f_sons.size() / 2; i++) {
			left->sons.push_back(f_sons[i]);
		}
		link_everything(left);
		for (int i = f_sons.size()/2; i < f_sons.size(); i++) {
			right->sons.push_back(f_sons[i]);
			
		}
		link_everything(right);
	}

}

void Tree::write_tree() {
	if (root == nullptr) return;  // Provera da li je stablo prazno

	std::queue<cvor*> que;
	que.push(root);

	while (!que.empty()) {
		int nodes_at_current_level = que.size();  // Broj čvorova na trenutnom nivou

		for (int j = 0; j < nodes_at_current_level; j++) {
			cvor* temp = que.front();
			que.pop();

			if (temp != nullptr) {
				// Dodavanje svih sinova trenutnog čvora u red
				for (size_t k = 0; k < temp->sons.size(); k++) {
					que.push(temp->sons[k]);
				}

			
				for (size_t k = 0; k < temp->keys.size(); k++) {
					std::cout << temp->keys[k]->id << " ";
				}
				std::cout << " | ";
			}
		}
		std::cout << std::endl;
		std::cout << "---------------------------------------------" << std::endl;
	}
}


Tree::cvor* Tree::search(long long x)

{

	steps = 1;

	cvor* temp = root;

	while (!is_leaf(temp)) {
		long long min = 0;

		for (size_t i = 0; i < temp->keys.size(); i++) {
			
			if (temp->keys[i] != nullptr && temp->keys[i]->id == x) {
				return temp;
			}

			if (x > temp->keys.back()->id) {
				if (temp->sons.size() > temp->keys.size()) {
					temp = temp->sons[temp->keys.size()];
				}
				else {
					std::cerr << "Neispravan pristup `sons`." << std::endl;
					return nullptr;
				}
				break;
			}

			
			if (min < x && x < temp->keys[i]->id) {
				temp = temp->sons[i];
				break;
			}

			
			if (temp->keys[i] != nullptr) {
				min = temp->keys[i]->id;
			}
			
		}
		steps++;
	}

	
	for (size_t i = 0; i < temp->keys.size(); i++) {
		if (temp->keys[i] != nullptr && temp->keys[i]->id == x) {

			return temp;
		}
	}

	return temp;


}



void Tree::read(std::string file_name)
{


	std::ifstream file(file_name);
	if (!file) {
		std::cout << "File ne postoji";
		return;
	}
	std::string line;

	while (std::getline(file, line)) {

		line.append("|");


		info* temp = new info;

		temp->id = stoll(line.substr(0, line.find('|')));
		line.erase(0, line.find('|') + 1);

		temp->first_name = line.substr(0, line.find('|'));
		line.erase(0, line.find('|') + 1);

		temp->last_name = line.substr(0, line.find('|'));
		line.erase(0, line.find('|') + 1);

		temp->email = line.substr(0, line.find('|'));
		line.erase(0, line.find('|') + 1);

		temp->ad_id = stoll(line.substr(0, line.find('|')));


		//std::cout << temp->id << " | " << temp->first_name << " | " << temp->last_name << " | " << temp->email << " | " << temp->ad_id << std::endl;

		data.push_back(temp);


	}

	file.close();

}

void Tree::add_new_user()
{

	std::cout << "Unesite podatke: ";

	std::string line;

	std::getline(std::cin, line);

	info* temp = new info;

		temp->id = stoll(line.substr(0, line.find('|')));
		line.erase(0, line.find('|') + 1);

		temp->first_name = line.substr(0, line.find('|'));
		line.erase(0, line.find('|') + 1);

		temp->last_name = line.substr(0, line.find('|'));
		line.erase(0, line.find('|') + 1);

		temp->email = line.substr(0, line.find('|'));
		line.erase(0, line.find('|') + 1);

		temp->ad_id = stoll(line.substr(0, line.find('|')));

	
		std::cout << temp->id << " | " << temp->first_name << " | " << temp->last_name << " | " << temp->email << " | " << temp->ad_id << std::endl;

		insert_node(temp);

}

void Tree::search_by_name()
{

	if (root == nullptr) return;


	steps = 1;

	std::cout << "Unesite ime: ";

	std::string target;

	std::getline(std::cin, target);


	info* temp;



	std::queue<cvor*> que;
	que.push(root);

	while (!que.empty()) {

		cvor* processing = que.front();
		que.pop();
		for (int i = 0; i < processing->sons.size(); i++) {

			que.push(processing->sons[i]);
		}

		for (int i = 0; i < processing->keys.size(); i++) {

			if (processing->keys[i]->first_name == target) {

				temp = processing->keys[i];
				std::cout << "Steps: " << steps << std::endl;
				std::cout << std::endl;
				std::cout << temp->id << " | " << temp->first_name << " | " << temp->last_name << " | " << temp->email << " | " << temp->ad_id << std::endl;
				std::cout << std::endl;
				return;
			}

		}
		steps++;



	}

	std::cout << "User does not exist" << std::endl;



}

Tree::info* Tree::search_by_id(long long x)
{
	

	if (x == 0) {

		std::cout << "Unesite id: ";

		std::cin >> x;

		std::cout << std::endl;
	}



	info* temp = nullptr;

	cvor* target_node = search(x);

	

	for (int i = 0; i < target_node->keys.size(); i++) {

		if (target_node->keys[i]->id == x) {

			temp = target_node->keys[i];

		}
	}

	if (!temp) {
		std::cout << "Steps: " << steps << std::endl;
		std::cout << std::endl;
		return nullptr;
	}
	std::cout << "Steps: " << steps << std::endl;
	std::cout << std::endl;
	

	return temp;


}

void Tree::output_id() {

	info* temp = search_by_id();

	if (!temp) {
		std::cout << "User does not exist" << std::endl;
		std::cout << std::endl;
	}
	else {
		std::cout << temp->id << " | " << temp->first_name << " | " << temp->last_name << " | " << temp->email << " | " << temp->ad_id << std::endl;
		std::cout << std::endl;


	}

}

void Tree::search_multiple_keys(int k)
{

	int sum = 0;


	std::vector<info*> output_arr;

	std::vector<long long> arr;

	long long x;

	while (1) {

		std::cout << "Unesite id: ";
		std::cin >> x;
		std::cout << std::endl;

		if (x == 0) {
			break;
		}
		arr.push_back(x);


	}

	for (int i = 0; i < arr.size(); i++) {
		info* temp = search_by_id(arr[i]);
		if (temp) {
			output_arr.push_back(temp);
			
		}
		sum += steps;


	}

	std::cout << sum << std::endl;
	write_file(output_arr);



}


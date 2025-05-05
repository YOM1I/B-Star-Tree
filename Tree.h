#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
#include <stack>
#include <fstream>
#include <iostream>


class Tree{
	public:

		int m;

		int all_sons_min;
		int all_sons_max;
		int all_keys_min;
		int all_keys_max;

		int root_sons_min;
		int root_sons_max;
		int root_keys_min;
		int root_keys_max;

		int f1;
		int f2;
		int f3;

		int test;

		int steps;

		struct info
		{
			long long id = 0;
			std::string first_name;
			std::string last_name;
			std::string email;
			long long ad_id = 0;

		};

		struct cvor {
			cvor* father = nullptr;
			cvor* left_brother = nullptr;
			cvor* right_brother = nullptr;
			std::vector<info*> keys;
			std::vector<cvor*> sons;
			int max_keys;
			int max_sons;

			int min_keys;

			cvor(double max_k, double max_s, double min_keys)
				: father(nullptr), left_brother(nullptr), right_brother(nullptr),
				max_keys(static_cast<int>(max_k)), max_sons(static_cast<int>(max_s)) , min_keys(static_cast<int>(min_keys)) {}
		};

		std::vector<info*> data;

		cvor* root = nullptr;

	public:

		Tree(int par, std::string inf) {
			m = par;

			root_sons_min = 2;
			root_sons_max = 2 * static_cast<int>(std::floor((2 * m - 2) / 3)) + 1;
			root_keys_min = root_sons_min - 1;
			root_keys_max = root_sons_max - 1;

			all_sons_min = static_cast<int>(std::ceil((2 * m - 1) / 3));
			all_sons_max = static_cast<int>(m);
			all_keys_min = static_cast<int>(std::ceil((2 * m - 1) / 3));
			all_keys_max = static_cast<int>(m - 1);

			f1 = static_cast<int>(std::floor((2 * m - 2) / 3));
			f2 = static_cast<int>(std::floor((2 * m - 1) / 3));
			f3 = static_cast<int>(std::floor((2 * m) / 3));

			read(inf);


		}

		//glavne funckije

		void delete_user(long long x);

		cvor* minimalizing(cvor*temp);

		cvor* borrow_right(cvor* temp);

		cvor* borrow_left(cvor* temp);

		void insert_node(info* inf);

		cvor* break_node(cvor* temp, cvor* brother);

		void overflow(cvor* temp, cvor* brother);

		void break_root(cvor* temp);

		void write_tree();

		cvor* search(long long x);

		void read(std::string file_name);

		

		/// kratke funkcije


		void generate_tree();
		
		bool is_leaf(cvor* temp);

		bool is_full_node(cvor* temp);

		bool is_overfull_node(cvor* temp);

		cvor* can_overflow(cvor* temp);

		void insert_key(cvor* temp, info* x);

		void bubbleSort_keys(std::vector<info*>& vec);

		void bubbleSort_sons(std::vector<cvor*>& vec);

		void correct_brothers_and_link_everything();

		void link_everything(cvor* temp);

		cvor* can_borrow(cvor* temp);

		cvor* find_s(cvor*temp, long long x);


		//ostale tacke zadatka


		void add_new_user();

		info* search_by_id(long long x = 0);

		void search_by_name();

		void output_id();

		void search_multiple_keys(int k);

		void write_file(std::vector<info*> output_txt);

	};

	inline void Tree::generate_tree()
	{
		test = 5; 
		for (int i = 0; i < test; i++) {

			info* temp = data[i];

			insert_node(temp);

			correct_brothers_and_link_everything();
		}
	}


	inline bool Tree::is_leaf(cvor* temp)
	{
		for (int i = 0; i < temp->max_sons; i++) {
			if (temp->sons.size() != 0) return false;
		}

		return true;
	}

	inline bool Tree::is_full_node(cvor* temp)
	{
		return temp->keys.size() == temp->max_keys;
	}
	inline bool Tree::is_overfull_node(cvor* temp)
	{
		return temp->keys.size() > temp->max_keys;
	}

	inline Tree::cvor* Tree::can_overflow(cvor* temp)
	{
		if (temp && temp->right_brother != nullptr) {
	
			return temp->right_brother;
		}
		else if (temp && temp->left_brother != nullptr) {
			
			return temp->left_brother;
		}
		return nullptr;
	}


	inline void Tree::insert_key(cvor* temp, info* x) {
		int pos = temp->keys.size();
		temp->keys.resize(temp->keys.size() + 1, nullptr);

		for (int i = 0; i < temp->keys.size() - 1; i++) {
			if (temp->keys[i] != nullptr && x->id < temp->keys[i]->id) {
				pos = i;
				break;
			}
		}

		for (size_t i = temp->keys.size() - 1; i > static_cast<size_t>(pos); i--) {
			temp->keys[i] = temp->keys[i - 1];
		}

		temp->keys[pos] = x;
	}

	inline void Tree::bubbleSort_keys(std::vector<info*>& vec) {
		for (size_t i = 0; i < vec.size() - 1; i++) {
			bool swapped = false;
			for (size_t j = 0; j < vec.size() - i - 1; j++) {
			
				if (vec[j]->id > vec[j + 1]->id) {
					std::swap(vec[j], vec[j + 1]);
					swapped = true;
				}
			}
			if (!swapped) break;
		}
	}


	inline void Tree::bubbleSort_sons(std::vector<cvor*>& vec) {
		
		for (size_t i = 0; i < vec.size() - 1; i++) {
			bool swapped = false;
			for (size_t j = 0; j < vec.size() - i - 1; j++) {
				if (vec[j]->keys.back()->id > vec[j + 1]->keys.back()->id) {
					std::swap(vec[j], vec[j + 1]);
					swapped = true;
				}
			}
			if (!swapped) break;
		}
	}


	inline void Tree::link_everything(cvor* temp)
	{
		for (int i = 0; i < temp->sons.size(); i++) {
			if (i == temp->sons.size() - 1) {
				temp->sons[i]->right_brother = nullptr;
			}
			else {
				temp->sons[i]->right_brother = temp->sons[i + 1];
			}
		}

		for (int i = 0; i < temp->sons.size(); i++) {
			if (i == 0) {
				temp->sons[i]->left_brother = nullptr;
			}
			else {
				temp->sons[i]->left_brother = temp->sons[i - 1];
			}
		}
		for (int i = 0; i < temp->sons.size(); i++) {
			temp->sons[i]->father = temp;
		}
	}

	inline Tree::cvor* Tree::can_borrow(cvor* temp)
	{
		if (temp->right_brother && temp->right_brother->keys.size() > temp->right_brother->min_keys) return temp->right_brother;
		else if (temp->left_brother && temp->left_brother->keys.size() > temp->left_brother->min_keys) return temp->left_brother;
		else if(temp->right_brother && temp->right_brother->right_brother && temp->right_brother->right_brother->keys.size() > temp->right_brother->right_brother->min_keys)
			return temp->right_brother->right_brother;
		else if (temp->left_brother && temp->left_brother->left_brother && temp->left_brother->left_brother->keys.size() > temp->left_brother->left_brother->min_keys) 
			return temp->left_brother->left_brother;


		return nullptr;
	}

	inline Tree::cvor* Tree::find_s(cvor* temp, long long x)

	{
		
		cvor* p = nullptr;

		int pos = temp->keys.size() - 1;

		for (int  i = 0; i < temp->keys.size(); i++) {

			if (temp->keys[i]->id == x) {

				pos = i;
				break;
			}

		}
		p = temp->sons[pos+1];

		while (!is_leaf(p)) {
			p = p->sons.front();
		}
		temp->keys[pos] = p->keys.front(); // pos

		p->keys.erase(p->keys.begin());




		return p;
	}

	inline void Tree::correct_brothers_and_link_everything()
	{
		
		std::queue<cvor*> st;

		cvor* temp = root;

		st.push(temp);

		while (!st.empty()) {

			temp = st.front();
			st.pop();
			link_everything(temp);

			while (!temp->sons.empty() && !temp->keys.empty() && (temp->sons.size() > temp->keys.size() + 1 || temp->sons.size() > temp->max_sons) && temp != root) {
				if (temp->right_brother != nullptr) {

					temp->right_brother->sons.insert(temp->right_brother->sons.begin(), temp->sons.back());
					temp->sons.pop_back(); 
					link_everything(temp->right_brother);

				}
				else if (temp->left_brother != nullptr) {
					
					temp->left_brother->sons.push_back(temp->sons.front());
					temp->sons.erase(temp->sons.begin());
					link_everything(temp->left_brother);
				}
			}

			link_everything(temp);

			for (int i = 0; i < temp->sons.size(); i++) {
				st.push(temp->sons[i]);
			}

		}
	}

	inline void Tree::write_file(std::vector<info*> output_txt) {

		std::ofstream file("output.txt");

		for (int i = 0; i < output_txt.size(); i++) {
			info* temp = output_txt[i];

			file << temp->id << " | " << temp->first_name << " | " << temp->last_name << " | " << temp->email << " | " << temp->ad_id << std::endl;

		}
		file.close();

	}





#include <iostream>
#include <string>
#include "Tree.h"

void show_meni() {
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "              Meni                     " << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "1. Ispis indeksa na standardni izlaz" << std::endl;
	std::cout << "2. Dodavanje novog zapisa u tabelu" << std::endl;
	std::cout << "3. Brisanje zapisa u tabeli po zadatom primarnom kljucu" << std::endl;
	std::cout << "4. Pretraga jednog podatka po primarnom kljucu" << std::endl;
	std::cout << "5. Pretraga svih podataka na osnovu imena korisnika" << std::endl;
	std::cout << "6. Pretraga k podataka po primarnim kljucima" << std::endl;
	std::cout << "7. Prekid programa" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Izaberite opciju: ";
}

int main() {
	int choice;
	double m;
	std::string file_name = "customer100.txt";

	std::cout << "Unesite m:";
	std::cin >> m;
	std::cout << std::endl;

	Tree* tree = new Tree(m, file_name);

	do {
		show_meni();
		std::cin >> choice;

		switch (choice) {
		case 1:

			tree->write_tree();
			break;
		case 2:

			tree->add_new_user();

			break;
		case 3:
			long long x;
			std::cout << "Unesite ID:";
			std::cin >> x;
			std::cout << std::endl;

			tree->delete_user(x);
			break;
		case 4:

			long long y;
			std::cout << "Unesite ID:";
			std::cin >> y;


			tree->output_id(y);
			break;
		case 5:
			std::cout << std::endl;
			tree->search_by_name();

			break;
		case 6:
			int k;
			std::cout << "Unesite k:";
			std::cin >> k;
			tree->search_multiple_keys(k);
			std::cout << std::endl;
			break;
		case 7:
			std::cout << std::endl;
			delete tree;
			break;
		default:
			std::cout << "Neispravan unos" << std::endl;
		}

	} while (choice != 7);

	return 0;
}

#include"VariableRecord.h"


int main() {
	auto FR = VariableRecord("datos1.txt");
	FR.load();
	int searchTarget = 3;
	std::cout << "\nRegistro [" << searchTarget << "]:" << std::endl;
	FR.readRecord(searchTarget).print();
	return 0;
}
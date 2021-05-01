#include<iostream>
#include<vector>
#include<fstream>

struct Alumno {
	char codigo[5];
	char nombre[11];
	char apellidos[20];
	char carrera[15];

	void print() {
		printCodigo();
		printNombre();
		printApellidos();
		printCarrera();
		std::cout << std::endl;
	}

	void printCodigo() {
		std::cout << "Codigo: ";
		for (int i = 0; i < sizeof(codigo); i++) {
			std::cout << codigo[i];
		}
		std::cout << std::endl;
	}

	void printNombre() {
		std::cout << "Nombre: ";
		for (int i = 0; i < sizeof(nombre); i++) {
			std::cout << nombre[i];
		}
		std::cout << std::endl;
	}

	void printApellidos() {
		std::cout << "Apellidos: ";
		for (int i = 0; i < sizeof(apellidos); i++) {
			std::cout << apellidos[i];
		}
		std::cout << std::endl;
	}

	void printCarrera() {
		std::cout << "Carrera: ";
		for (int i = 0; i < sizeof(carrera); i++) {
			std::cout << carrera[i];
		}
		std::cout << std::endl;
	}
};

class FixedRecord {
private:
	std::vector<Alumno> recordVec;
	std::string fileName;

public:
	FixedRecord(std::string nombreArchivo) :fileName(nombreArchivo) {
		std::ifstream file;
		std::string line;
		Alumno* alumn;
		int i;
		file.open(nombreArchivo);
		while (file) {
			alumn = new Alumno;
			file.read(alumn->codigo, 5);
			file.read(alumn->nombre, 11);
			file.read(alumn->apellidos, 20);
			file.read(alumn->carrera, 15);
			file.get();
			file.get();
			add(*alumn);
		}
		file.close();
	}

	std::vector<Alumno> load() {
		for (auto it = recordVec.begin(); it != recordVec.end(); it++) {
			it->print();
		}
		return recordVec;
	}

	void add(Alumno record) {
		recordVec.push_back(record);
	}

	Alumno readRecord(int pos) {
		return recordVec[pos];
	}
};
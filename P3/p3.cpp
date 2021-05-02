#include<iostream>
#include<vector>
#include<fstream>

struct Alumno {
	std::string nombre;
	std::string apellidos;
	std::string carrera;
	float mensualidad;

	void print() {
		std::cout << "Nombre: " << nombre << "\nApellidos: " << apellidos << "\nCarrera: "
			<< carrera << "\nMensualidad: " << mensualidad << "\n\n";
	}
};

class VariableRecord {
private:
	std::vector<Alumno> recordVec;
	std::string fileName;

public:
	VariableRecord(std::string nombreArchivo) :fileName(nombreArchivo) {
		std::ifstream file;
		std::string word;
		Alumno* alumn = new Alumno;
		char c;
		int attribute = 0;
		file.open(nombreArchivo);
		while (file.get(c)) {
			if (c == '\n') break;
		}
		while (file.get(c)) {
			if (c == '|') {
				if (attribute == 0) {
					alumn->nombre = word;
				}
				else if (attribute == 1) {
					alumn->apellidos = word;
				}
				else if (attribute == 2) {
					alumn->carrera = word;
				}
				attribute++;
				word.clear();
			}
			else if (c == '\n') {
				alumn->mensualidad = std::stof(word);
				word.clear();
				attribute = 0;
				add(*alumn);
				alumn = new Alumno;
			}
			else {
				word.push_back(c);
			}
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


int main() {
	auto FR = VariableRecord("datos1.txt");
	FR.load();
	int searchTarget = 3;
	std::cout << "\nRegistro [" << searchTarget << "]:" << std::endl;
	FR.readRecord(searchTarget).print();
	return 0;
}
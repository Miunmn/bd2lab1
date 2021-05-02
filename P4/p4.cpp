#include <iostream>
#include<fstream>
#include<cstdio>
#include<vector>
#include <string>
#include <cstring>
#include <map>

using namespace std;

class Matricula
{
public:
	std::string codigo;
	int ciclo;
	float mensualidad;
	std::string observaciones;

public:

	Matricula() {}

	Matricula(string co, int ci, float me, string obs) :codigo(co), ciclo(ci), mensualidad(me), observaciones(obs) {}

	void setData() {
		cout << "Codigo: \n";
		cin >> codigo;

		cout << "Ciclo: \n";
		cin >> ciclo;

		cout << "Mensualidad: \n";
		cin >> mensualidad;

		cout << "Observaciones \n";
		cin >> observaciones;
	}

	void showData() {
		cout << endl;
		cout << "Codigo: " << codigo << endl;
		cout << "Ciclo: " << ciclo << endl;
		cout << "Mensualidad: " << mensualidad << endl;
		cout << "Observaciones: " << observaciones << endl;
	}
};

class metaRegistro {
public:
	int pos;
	int size;
	int firstStringSize;

	metaRegistro() :pos(0), size(0), firstStringSize(0) {}

	metaRegistro(int _pos, int _size, int _firstStringSize) :pos(_pos), size(_size), firstStringSize(_firstStringSize) {}
};

class FixedRecordFile
{
private:
	string fileName;
	string fileBINName;
	string metadataFile;
	std::vector<Matricula> regVec;
	std::vector<metaRegistro> metaRegVec;
	int last_pos = 0;
	int last_size = 0;
public:
	FixedRecordFile(string _fileName, string _fileBINName) {
		this->fileName = _fileName;
		this->fileBINName = _fileBINName;
		this->metadataFile = "metadata.txt";
		std::ifstream file;
		file.open(fileName);
		if (file.is_open())
		{
			std::string word;
			char c;
			int lenght, state = 0;
			auto matric = new Matricula;
			//auto regVec = new std::vector<Matricula>;
			while (file.get(c)) {
				if (c == ':') {
					lenght = std::stoi(word);
					word.clear();
					for (int i = 0; i < lenght; i++) {
						file.get(c);
						word.push_back(c);
					}
					if (state == 0) {
						matric->codigo = word;
						//cout<<"state: "<<state<<" matric->codigo: "<<matric->codigo<<endl;
						state++;
					}
					else if (state == 1) {
						matric->ciclo = std::stoi(word);
						//cout<<"state: "<<state<<" matric->ciclo: "<<matric->ciclo<<endl;
						state++;
					}
					else if (state == 2) {
						matric->mensualidad = std::stof(word);
						//cout<<"state: "<<state<<" matric->mensualidad: "<<matric->mensualidad<<endl;
						state++;
					}
					else if (state == 3) {
						matric->observaciones = word;
						//cout<<"state: "<<state<<" matric->observaciones: "<<matric->observaciones<<endl;
						state = 0;
						regVec.push_back(*matric);
						matric = new Matricula;
					}
					word.clear();
				}
				else
				{
					word.push_back(c);
				}
			}
		}
		create_bin_file();
	}

	void create_bin_file()
	{
		ofstream outFile;

		outFile.open(fileBINName, ios::binary | ios::app);

		for (int i = 0; i < regVec.size(); i++)
		{
			writeRecord(regVec[i]);
		}
	}

	std::vector<Matricula> load()
	{
		return regVec;
	}

	void print_vector()
	{
		for (auto &matricula : regVec)
		{
			matricula.showData();
		}
	}

	/*
	* function to write in a binary file.
	*/
	void writeRecord(Matricula obj) {
		ofstream outFile, metaFile;
		outFile.open(fileBINName, ios::binary | ios::app);
		for (int i = 0; i < obj.codigo.size(); i++) {
			outFile.write(&obj.codigo[i], sizeof(char));
		}
		outFile.write((char *) &(obj.ciclo), sizeof(obj.ciclo));
		outFile.write((char *) &(obj.mensualidad), sizeof(obj.mensualidad));
		for (int i = 0; i < obj.observaciones.size(); i++) {
			outFile.write(&obj.observaciones[i], sizeof(char));
		}
		writeMetaDataFile(obj);

		outFile.close();
	}

	void writeMetaReg(Matricula obj) {
		metaRegistro* metareg;
		if (!metaRegVec.empty()) {
			metareg = new metaRegistro(metaRegVec.back().pos + metaRegVec.back().size, obj.codigo.size() + sizeof(obj.ciclo) + sizeof(obj.mensualidad) + obj.observaciones.size(), obj.codigo.size());
		}
		else {
			metareg = new metaRegistro(0, obj.codigo.size() + sizeof(obj.ciclo) + sizeof(obj.mensualidad) + obj.observaciones.size(), obj.codigo.size());
		}
		metaRegVec.push_back(*metareg);
	}

	void stackprinter(Matricula obj) {
		std::cout << "Tamano: " << sizeof(obj.observaciones) << std::endl;
		for (int i = 0; i < sizeof(obj.observaciones); i++) {
			std::cout << i << ". " << obj.observaciones[i] << std::endl;
		}
		std::cout << std::endl;
	}

	/*
	* function to display records of file
	*/
	void scanAll() {

		readMetaFile();

		ifstream inFile;
		inFile.open(fileBINName, ios::binary);
		//read the records
		Matricula obj;
		char *buffer, *buffer2;
		for (auto it : metaRegVec) {
			buffer = new char[it.firstStringSize];
			buffer2 = new char[it.size - (it.firstStringSize + sizeof(int) + sizeof(int))];
			inFile.read(buffer, it.firstStringSize);
			inFile.read((char *)&(obj.ciclo), sizeof(int));
			inFile.read((char *)&(obj.mensualidad), sizeof(int));
			inFile.read(buffer2, it.size - (it.firstStringSize + sizeof(int) + sizeof(int)));
			obj.codigo.assign(buffer, it.firstStringSize);
			obj.observaciones.assign(buffer2, it.size - (it.firstStringSize + sizeof(int) + sizeof(int)));
			obj.showData();
		}
		/*while (inFile.read((char *) &obj, sizeof(obj)))
		{
			obj.showData();
		}*/
		inFile.close();
	}

	/*
	* function to insert a record
	*/
	void insertRecord(Matricula  obj, int pos)
	{
		fstream outFile;
		cout << "size(): " << size() << endl;
		outFile.open(this->fileBINName, ios::binary | ios::out | ios::in);
		if (outFile.is_open()) {
			outFile.seekp(pos * sizeof(obj), ios::beg);
			outFile.write((char *)&obj, sizeof(obj));
			outFile.close();
		}
		else cout << "Could not open the file.\n";
		cout << "size(): " << size();
	}

	int size() {
		int numRecords = 0;
		fstream inFile;
		inFile.open(this->fileBINName, ios::in | ios::binary);
		if (inFile.is_open()) {
			inFile.seekg(0, ios::end);
			long bytes = inFile.tellg();
			numRecords = bytes / sizeof(Matricula);
			inFile.close();
		}
		else cout << "Could not open the file.\n";
		return numRecords;
	}

	void change_next_del_bin(int pos_obj, int pos_del)
	{
		fstream inFile;
		inFile.open(this->fileBINName, ios::in | ios::binary);
		if (inFile.is_open())
		{
			inFile.seekp(pos_obj * sizeof(pos_obj) + 23, ios::beg);
			cout << inFile.peek();
		}

	}
	void writeMetaDataFile(Matricula obj)
	{
		int pos = last_pos + last_size;
		int size = obj.codigo.size() + sizeof(obj.ciclo) + sizeof(obj.mensualidad) + obj.observaciones.size();
		int fss = obj.codigo.size();
		//Escribir pos+ | + size + | + fss + \n
		last_pos = pos;
		last_size = size;
		string line;
		ofstream Fileobj;
		Fileobj.open(metadataFile, ios::app);
		if (Fileobj.is_open())
		{
			line = to_string(pos) + '|' + to_string(size) + '|' + to_string(fss) + '\n';
			Fileobj << line;
		}
		Fileobj.close();

	}
	Matricula readRecord(int pos) {

		readMetaFile();

		fstream inFile;
		inFile.open(this->fileBINName, ios::in | ios::binary);
		Matricula obj;
		auto it = metaRegVec[pos];

		if (inFile.is_open())
		{
			//moverse hasta la posicion del alumno
			inFile.seekg(it.pos, ios::beg);
			//leer el alumno
			char *buffer, *buffer2;
			buffer = new char[it.firstStringSize];
			buffer2 = new char[it.size - (it.firstStringSize + sizeof(int) + sizeof(int))];
			inFile.read(buffer, it.firstStringSize);
			inFile.read((char *)&(obj.ciclo), sizeof(int));
			inFile.read((char *)&(obj.mensualidad), sizeof(int));
			inFile.read(buffer2, it.size - (it.firstStringSize + sizeof(int) + sizeof(int)));
			obj.codigo.assign(buffer, it.firstStringSize);
			obj.observaciones.assign(buffer2);
			obj.showData();
			//obj.showData();
			inFile.close();
		}
		return obj;
	}

	void readMetaFile() {

		metaRegVec.clear();

		ifstream file;
		file.open(metadataFile);
		char c;
		std::string word;
		int state = 0;
		auto aux = new metaRegistro;
		while (file.get(c)) {
			if (c == '|') {
				if (state == 0) {
					aux->pos = std::stoi(word);
					state++;
				}
				else if (state == 1) {
					aux->size = std::stoi(word);
					state++;
				}
				word.clear();
			}
			else if (c == '\n') {
				aux->firstStringSize = std::stoi(word);
				state = 0;
				metaRegVec.push_back(*aux);
				aux = new metaRegistro;
				word.clear();
			}
			else {
				word.push_back(c);
			}
		}

	}
};


int main() {
	FixedRecordFile FR("datos4.txt", "datos4.bin");
	//FR.stackprinter(FR.load().front());
	//FR.load().front().showData();
	//vector<Matricula> matriculas = FR.load();
	//FR.print_vector();
	FR.scanAll();
	cout << "FR.size();: " << FR.size();
	cout << "\n\n----------------readRecord(3)----------------\n\n";
	FR.readRecord(3);
	cout << "\n\n----------------add()----------------\n\n";
	FR.writeRecord(Matricula("99999", 3, 345.67, "Falta regresar 5 libros de la biblioteca."));
	FR.scanAll();

	return 0;
}

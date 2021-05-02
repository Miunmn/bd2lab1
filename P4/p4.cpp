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

	metaRegistro(int _pos, int _size, int _firstStringSize) :pos(_pos), size(_size), firstStringSize(_firstStringSize) {}
};

class FixedRecordFile
{
private:
	string fileName;
	string fileBINName;
	std::vector<Matricula> regVec;
	std::vector<metaRegistro> metaRegVec;
public:
	FixedRecordFile(string _fileName, string _fileBINName) {
		this->fileName = _fileName;
		this->fileBINName = _fileBINName;
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

		//print_vector();

		for (int i = 0; i < regVec.size(); i++)
		{
			writeRecord(regVec[i]);
		}

		/*
		for(auto &matricula : regVec)
		{
			writeRecord(matricula);
		}*/
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
		writeMetaReg(obj);
		/*
		cout<<"sizeof(obj): "<< sizeof(obj)<<endl;
		cout<<"sizeof(obj.mensualidad): "<< sizeof(obj.mensualidad)<<endl;
		cout<<"sizeof(obj.ciclo): "<< sizeof(obj.ciclo)<<endl;*/
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
			obj.observaciones.assign(buffer2);
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

	Matricula readRecord(int pos)
	{
		fstream inFile;
		inFile.open(this->fileBINName, ios::in | ios::binary);
		Matricula obj;

		if (inFile.is_open())
		{
			//moverse hasta la posicion del alumno
			inFile.seekg(pos * sizeof(Matricula), ios::beg);
			//leer el alumno
			inFile.read((char *)&obj, sizeof(obj));
			//obj.showData();
			inFile.close();
		}
		return obj;
	}
};


int main() {
	FixedRecordFile FR("datos1.txt", "datos4.bin");
	//FR.stackprinter(FR.load().front());
	//FR.load().front().showData();
	//vector<Matricula> matriculas = FR.load();
	//FR.print_vector();
	FR.scanAll();
	//cout<<"FR.size();: "<<FR.size();
	return 0;
}

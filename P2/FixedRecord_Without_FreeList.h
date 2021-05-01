#include <iostream>
#include<fstream>
#include<cstdio>

using namespace std;

class Record {
private:
    char nombre[30];    
    char carrera[20];//string -> tamanio variable
    int ciclo;

public:
    void setData() {
        cout << "Alumno:";
        cin >> nombre;
        cout << "Carrera: ";
        cin >> carrera;
        cout << "Ciclo: ";
        cin >> ciclo;
    }

    void showData() {
        cout << "\nNombre: " << nombre;
        cout << "\nCarrera: " << carrera;
        cout << "\nCiclo : " << ciclo;
    }
};

class FixedRecordFile {
private:
    string fileName;    

public:
    FixedRecordFile(string _fileName) {
        this->fileName = _fileName;
    }

    /*
    * function to write in a binary file.
    */
    void writeRecord(Record obj) {
        ofstream outFile;
        outFile.open(fileName, ios::binary | ios::app);        
        outFile.write((char *) &obj, sizeof(obj));
        outFile.close();
    }

    /*
    * function to display records of file
    */
    void scanAll() {
        ifstream inFile;
        inFile.open(fileName, ios::binary);
        //read the records
        Record obj;
        while (inFile.read((char *) &obj, sizeof(obj))) {
            obj.showData();        	
        }
        inFile.close();
    }

    /*
    * function to insert a record
    */
    void insertRecord(Record  obj, int pos) {
        fstream outFile;
        outFile.open(this->fileName, ios::out | ios::binary);
        if (outFile.is_open()) {
            outFile.seekg(pos * sizeof(obj), ios::beg);
            outFile.write((char *) &obj, sizeof(obj));
            outFile.close();
        }else cout << "Could not open the file.\n";
    }

    int size() {
        int numRecords = 0;
        fstream inFile;
        inFile.open(this->fileName, ios::in | ios::binary);
        if (inFile.is_open()) {
            inFile.seekg(0, ios::end);
            long bytes = inFile.tellg();
            numRecords = bytes / sizeof(Record);
            inFile.close();
        } else cout << "Could not open the file.\n";
        return numRecords;
    }

};
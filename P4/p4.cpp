#include <iostream>
#include<fstream>
#include<cstdio>
#include<vector>
#include <string> 
#include <cstring>

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
        cout<< "Codigo: \n";
        cin>> codigo;

        cout << "Ciclo: \n";
        cin >> ciclo;

        cout<< "Mensualidad: \n";
        cin>>mensualidad;
        
        cout<< "Observaciones \n";
        cin>>observaciones;
    }

    void showData() {
        cout<<endl;
        cout<<"Codigo: "<<codigo<<endl;
        cout << "Ciclo: " << ciclo<<endl;
        cout<< "Mensualidad: "<< mensualidad<<endl;
        cout << "Observaciones: " << observaciones<<endl;
    }
};

class FixedRecordFile
{
private:
    string fileName;    
    string fileBINName;
    std::vector<Matricula> regVec;
public:
    FixedRecordFile(string _fileName,string _fileBINName) {
        this->fileName = _fileName;
        this->fileBINName = _fileBINName;
        std::ifstream file;
        file.open(fileName);
        if(file.is_open())
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
                    if (state == 0){
                        matric->codigo = word;
                        state++;
                    }else if (state == 1){
                        matric->ciclo = std::stoi(word);
                        state++;
                    }else if (state == 2){
                        matric->ciclo = std::stof(word);
                        state++;
                    }else if (state == 3){
                        matric->observaciones = word;
                        state = 0;
                        regVec.push_back(*matric);
                        matric = new Matricula;
                    }
                    word.clear();
                }
                else {
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

        for(auto &matricula : regVec)
        {
            writeRecord(matricula);
        }
    }

    std::vector<Matricula> load()
    {
        return regVec;
    }

    void print_vector()
    {
        for(auto &matricula: regVec)
        {
            matricula.showData();
        }
    }

    /*
    * function to write in a binary file.
    */
    void writeRecord(Matricula obj) {
        ofstream outFile;
        outFile.open(fileBINName, ios::binary | ios::app);        
        outFile.write((char *) &obj, sizeof(obj));
        outFile.close();
    }

    /*
    * function to display records of file
    */
    void scanAll() {
        ifstream inFile;
        inFile.open(fileBINName, ios::binary);
        //read the records
        Matricula obj;
        while (inFile.read((char *) &obj, sizeof(obj)))
        {
                obj.showData(); 
        }
        inFile.close();
    }

    /*
    * function to insert a record
    */
    void insertRecord(Matricula  obj, int pos)
    {
        fstream outFile;
        cout<<"size(): "<<size()<<endl;
        outFile.open(this->fileBINName,ios::binary|ios::out|ios::in);
        if (outFile.is_open()) {
            outFile.seekp(pos * sizeof(obj), ios::beg);
            outFile.write((char *) &obj, sizeof(obj));
            outFile.close();
        }else cout << "Could not open the file.\n";
        cout<<"size(): "<<size();
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
        } else cout << "Could not open the file.\n";
        return numRecords;
    }

    void change_next_del_bin(int pos_obj, int pos_del)
    {
        fstream inFile;
        inFile.open(this->fileBINName, ios::in | ios::binary);
        if (inFile.is_open())
        {
            inFile.seekp(pos_obj * sizeof(pos_obj)+23, ios::beg);
            cout<<inFile.peek();
        }
    
    }

    Matricula readRecord(int pos)
    {
        fstream inFile;
        inFile.open(this->fileBINName, ios::in | ios::binary);
        Matricula obj;

        if(inFile.is_open())
        {
            //moverse hasta la posicion del alumno
            inFile.seekg(pos * sizeof(Matricula), ios::beg);
            //leer el alumno
            inFile.read((char *) &obj, sizeof(obj));
            //obj.showData();
            inFile.close();
        }
        return obj;
    }

    bool delete_(int pos)
    {

    }
};


int main() {
    FixedRecordFile FR("datos4.txt","datos4.bin");
    vector<Matricula> matriculas = FR.load();
    FR.print_vector();
    //FR.scanAll();

    return 0;
}

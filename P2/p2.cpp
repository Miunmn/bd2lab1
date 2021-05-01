#include <iostream>
#include<fstream>
#include<cstdio>

using namespace std;

class Alumno
{
private:
    //char codigo[5];
    int codigo;
    char nombre[11];
    char apellidos[20];    
    char carrera[15];
    
    int ciclo;
    float mensualidad;
    int nextDel;

public:
    void setData() {
        cout<< "Codigo: \n";
        cin>> codigo;

        cout << "Nombre:\n";
        cin >> nombre;

        cout<< "Apellidos: \n";
        cin>>apellidos;

        cout << "Carrera: \n";
        cin >> carrera;

        cout << "Ciclo: \n";
        cin >> ciclo;

        cout<< "Mensualidad: \n";
        cin>>mensualidad;
    }

    void showData() {
        cout<<"\nCodigo: "<<codigo;
        cout << "\nNombre: " << nombre;
        cout<< "\nApellidos: "<< apellidos;
        cout << "\nCarrera: " << carrera;
        cout << "\nCiclo : " << ciclo;
        cout<< "\nMensualidad: "<< mensualidad;
    }
};

class FixedRecordFile
{
private:
    string fileName;    

public:
    FixedRecordFile(string _fileName) {
        this->fileName = _fileName;
    }

    /*
    * function to write in a binary file.
    */
    void writeRecord(Alumno obj) {
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
        Alumno obj;
        while (inFile.read((char *) &obj, sizeof(obj))) {
            obj.showData();        	
        }
        inFile.close();
    }

    /*
    * function to insert a record
    */
    void insertRecord(Alumno  obj, int pos) {
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
            numRecords = bytes / sizeof(Alumno);
            inFile.close();
        } else cout << "Could not open the file.\n";
        return numRecords;
    }

    Alumno readRecord(int pos)
    {
        fstream inFile;
        inFile.open(this->fileName, ios::in | ios::binary);
        Alumno obj;

        if(inFile.is_open())
        {
            //moverse hasta la posicion del alumno
            inFile.seekg(pos * sizeof(Alumno), ios::beg);
            //leer el alumno
            inFile.read((char *) &obj, sizeof(obj));
            //obj.showData();
            inFile.close();
        }
        return obj;
    }
};

int main() {
    auto FR = FixedRecordFile("datos2.muerelab");
    for (int i = 0 ;i<7 ; i++)
    {
        Alumno alumno1;
        alumno1.setData();
        FR.writeRecord(alumno1);
    }
    std::cout<<"\n\n\n";
    Alumno obj = FR.readRecord(0); 
    obj.showData();
    return 0;
}

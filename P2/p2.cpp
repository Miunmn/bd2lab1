#include <iostream>
#include<fstream>
#include<cstdio>
#include<vector>
#include <string> 
#include <cstring>

using namespace std;

class Alumno
{
public:
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
        
        cout<< "nextDel \n";
        cin>>nextDel;
    }

    void showData() {
        cout<<"\nCodigo: "<<codigo;
        
        /*for(int i = 0 ; i < 5 ;i++)
        {
        cout <<codigo[i];
        }*/        
        cout << "\nNombre: " << nombre;
        cout<< "\nApellidos: "<< apellidos;
        cout << "\nCarrera: " << carrera;
        cout << "\nCiclo : " << ciclo;
        cout<< "\nMensualidad: "<< mensualidad;
        cout << "\nNextdel : " << nextDel;
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
        int i = 0;
        while (inFile.read((char *) &obj, sizeof(obj)))
        {
            cout<<"\n\nAlumno: "<<i;i++;
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
            obj.showData();
            inFile.close();
        }
        return obj;
    }
};

std::vector<Alumno> alumnos(string nombreArchivo)
{  
    /*
    char codigo[5];
    char nombre[11];
    char apellidos[20];    
    char carrera[15];
    
    int ciclo;
    float mensualidad;
    int nextDel;
    */
    
    std::vector<Alumno> retorno;
    std::ifstream file;
    std::string line;
    Alumno* alumn;
    int i;
    file.open(nombreArchivo);
    while(getline(file,line)){
        alumn = new Alumno;
        alumn->codigo = stoi(line.substr(0,4));
        //Nombre
        std::string aux_string = (line.substr(5,11));
        std::memcpy(alumn->nombre,aux_string.c_str(),aux_string.size()+1);
        //Apellidos
        aux_string = (line.substr(16,20));
        std::memcpy(alumn->apellidos,aux_string.c_str(),aux_string.size()+1);
        //Carrera
        aux_string = (line.substr(36,15));
        std::memcpy(alumn->carrera,aux_string.c_str(),aux_string.size()+1);
        //Ciclo
        alumn->ciclo = stoi(line.substr(51,1));
        //Mensualidad
        alumn->mensualidad = stof(line.substr(52,7));
        //NextDel;
        alumn->nextDel = stoi(line.substr(59,2));
        retorno.push_back(*alumn);
    }
    file.close(); 
    return retorno;
}


int main() {
    std::string file_name = "datos2.txt";
    auto FR = FixedRecordFile("datos2.muerelab");
    std::ifstream file;
    std::string line;
    //Alumno* alumn;
    std::vector<Alumno> alumnos_=alumnos(file_name);

    for(auto &alumno_ : alumnos_)
    {
        FR.writeRecord(alumno_);
    }
    FR.scanAll();


    /*for (int i = 0 ;i<8 ; i++)
    {
        Alumno alumno1;
        alumno1.setData();
        FR.writeRecord(alumno1);
        std::cout<<"\n\n\n\n";
    }*/
    //FR.scanAll();
    /*
    Alumno obj = FR.readRecord(0); 
    std::cout<<"\n\n\n\n";
    obj.showData();
    */
    return 0;
}

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
    char codigo[5];
    //int codigo;
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
        cout<<endl;
        cout<<"Codigo: "<<codigo<<endl;
        cout << "Nombre: " << nombre<<endl;
        cout<< "Apellidos: "<< apellidos<<endl;
        cout << "Carrera: " << carrera<<endl;
        cout << "Ciclo : " << ciclo<<endl;
        cout<< "Mensualidad: "<< mensualidad<<endl;
        cout << "Nextdel : " << nextDel<<endl;
    }
};

class FixedRecordFile
{
private:
    string fileName;    
    int lastDeleted = 0;

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
        int i = 1;
        //auto alumno = readRecord(0);
        //int iterator_delete = alumno.nextDel;
        while (inFile.read((char *) &obj, sizeof(obj)))
        {
            obj.showData(); 
            /*
            if( i== -1 || i!=iterator_delete)
            {
                obj.showData(); 
            }
            else
            {
                iterator_delete = obj.nextDel;
            }
            i++;*/
        }
        inFile.close();
    }

    /*
    * function to insert a record
    */
    void insertRecord(Alumno  obj, int pos)
    {
        fstream outFile;
        cout<<"size(): "<<size()<<endl;
        outFile.open(this->fileName,ios::binary|ios::out|ios::in);
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
        inFile.open(this->fileName, ios::in | ios::binary);
        if (inFile.is_open()) {
            inFile.seekg(0, ios::end);
            long bytes = inFile.tellg();
            numRecords = bytes / sizeof(Alumno);
            inFile.close();
        } else cout << "Could not open the file.\n";
        return numRecords;
    }

    void change_next_del_bin(int pos_obj, int pos_del)
    {
        fstream inFile;
        inFile.open(this->fileName, ios::in | ios::binary);
        if (inFile.is_open())
        {
            inFile.seekp(pos_obj * sizeof(pos_obj)+23, ios::beg);
            cout<<inFile.peek();
        }
    
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

    bool delete_(int pos)
    {
        auto alumno = readRecord(0);
        if(alumno.nextDel == -2)
        {
            alumno.nextDel = pos;
            insertRecord(alumno,0);
        }
        else
        {
            int i = 0;
            int delete_iterator = alumno.nextDel;
            while(delete_iterator != -2)
            {
                alumno = readRecord(delete_iterator);
                i = delete_iterator;
                delete_iterator = alumno.nextDel;
            }
            alumno.nextDel = pos;
            insertRecord(alumno,i);
        }
        auto alumno_eliminado = readRecord(pos);
        alumno_eliminado.nextDel = -2;
        insertRecord(alumno_eliminado,pos);
    }
};

std::vector<Alumno> alumnos(string nombreArchivo)
{     
    std::vector<Alumno> retorno;
    std::ifstream file;
    std::string line;
    Alumno* alumn;
    int i;
    file.open(nombreArchivo);
    while(getline(file,line)){
        alumn = new Alumno;

        //Codigo
        std::string aux_string = (line.substr(0,5));
        std::memcpy(alumn->codigo,aux_string.c_str(),aux_string.size()+1);
        //Nombre
        aux_string = (line.substr(5,11));
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
        //alumn->showData();

        retorno.push_back(*alumn);
    }
    file.close(); 
    return retorno;
}


int main() {
    std::string file_name = "datos2.txt";
    auto FR = FixedRecordFile("datos2.bin");
    std::ifstream file;
    std::string line;
    //Alumno* alumn;
    std::vector<Alumno> alumnos_ = alumnos(file_name);

    for(auto &alumno_ : alumnos_)
    {
        FR.writeRecord(alumno_);
    }
    cout<<"===================FR.scanAll()==============="<<endl<<endl<<endl;
    FR.scanAll();
    cout<<endl<<endl<<endl;
    cout<<"==================FR.delete_(5)==============="<<endl<<endl<<endl;
    FR.delete_(5);
    cout<<endl<<endl<<endl;
    cout<<"==================FR.delete_(4)==============="<<endl<<endl<<endl;
    FR.delete_(4);
    cout<<endl<<endl<<endl;
    cout<<"==================FR.delete_(2)==============="<<endl<<endl<<endl;
    FR.delete_(2);
    cout<<endl<<endl<<endl;
    cout<<"===================FR.scanAll()==============="<<endl<<endl<<endl;
    cout<<"FR.size(): "<<FR.size();
    FR.scanAll();
    cout<<endl<<endl<<endl;
 /* 
    for (int i = 0 ;i<8 ; i++)
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

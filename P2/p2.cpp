#include<iostream>
#include<vector>
#include<fstream>

struct Alumno {
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];

    int ciclo;
    float mensualidad;
    int nextDel;

    void setNextdel(int next_del_){ nextDel = next_del_;}

    void print(){
        printCodigo();
        printNombre();
        printApellidos();
        printCarrera();
        std::cout<<std::endl;
    }

    void printCodigo(){
        for(int i = 0; i < 5;i++){
            std::cout<<codigo[i];
        }
    }

    void printNombre(){
        for(int i = 0; i < 11;i++){
            std::cout<<nombre[i];
        }
    }

    void printApellidos(){
        for(int i = 0; i < 20;i++){
            std::cout<<apellidos[i];
        }
    }

    void printCarrera(){
        for(int i = 0; i < 15;i++){
            std::cout<<carrera[i];
        }
    }
};

class Record {
private:
    std::vector<Alumno> recordVec;
    std::string fileName;
    std::fstream file_obj;


public:
    //Cumple con el punto a)
    Record(std::string nombreArchivo):fileName(nombreArchivo){
    }  

    std::vector<Alumno> load(){
        /*
        for(auto it = recordVec.begin(); it != recordVec.end(); it++){
            it->print();
        }
        return recordVec;
        */


    }

    void add(Alumno record){
        recordVec.push_back(record);
    }

    Alumno readRecord(int pos){
        return recordVec[pos];
    }

    bool delete_(int entry_number)
    {
        Alumno alumno;
        file_obj.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
        if (file_obj.is_open())
        {   
            //leer el encabezado
            int pointer;
            file_obj.read((char *) &pointer, sizeof(int));

            //leer el registro
            file_obj.seekg(entry_number*sizeof(Alumno)+sizeof(int));
            file_obj.read((char *) &alumno, sizeof(Alumno) );

            //cambiar el pointer de eliminacion
            alumno.setNextdel(pointer);

            //escribir el registro
            file_obj.seekg(entry_number * sizeof(Alumno)+ sizeof(int));
            file_obj.write((char *) &alumno, sizeof(Alumno));   

            //escribir el pointer de eliminacion
            file_obj.seekg(0,std::ios::beg);
            file_obj.write((char *) &entry_number, sizeof(int));

            
            file_obj.close();
            return true;
        }
        else
        {
            return false;
        }
    }
};

int main() {
    auto FR = Record("datos2.bin");
    FR.load();
    std::cout<<"\nRegistro [2]:"<<std::endl;
    FR.readRecord(2).print();
    return 0;
}

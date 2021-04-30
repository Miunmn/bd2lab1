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
    

public:
    Record(std::string nombreArchivo):fileName(nombreArchivo){
    }
    void delete_(int entry_number)
    {
        std::fstream file_obj;
        Alumno alumno;
        file_obj.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
        if (file_obj.is_open())
        {   
            int pointer;
            file_obj.read((char *) &pointer, sizeof(int));

            file_obj.seekg(entry_number*sizeof(Alumno)+sizeof(int));
            file_obj.read((char *) &alumno, sizeof(Alumno) );

            alumno.setNextdel(pointer);

            file_obj.seekg(entry_number * sizeof(Alumno)+ sizeof(int));
            file_obj.write((char *) &alumno, sizeof(Alumno));   

            file_obj.seekg(0,std::ios::beg);
            file_obj.write((char *) &entry_number, sizeof(int));

            file_obj.close();
        }
    }
    

    std::vector<Alumno> load(){
        for(auto it = recordVec.begin(); it != recordVec.end(); it++){
            it->print();
        }
        return recordVec;
    }

    void add(Alumno record){
        recordVec.push_back(record);
    }

    Alumno readRecord(int pos){
        return recordVec[pos];
    }
};

int main() {
    auto FR = Record("datos2.txt");
    FR.load();
    std::cout<<"\nRegistro [9]:"<<std::endl;
    FR.readRecord(2).print();
    return 0;
}
